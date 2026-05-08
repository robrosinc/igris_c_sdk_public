#!/usr/bin/env python3
"""DearPyGui port of sdk_gui_client.cpp.

Mirrors the three-panel ImGui example: 31 motor sliders + Reset/Home,
DDS/IMU/LowCmd status in the centre, and Service API buttons on the right
with a response log strip on the bottom.

Run with:
    python sdk_gui_client.py --domain-id 0 --namespace robot1
"""

from __future__ import annotations

import argparse
import math
import signal
import sys
import threading
import time
from collections import deque
from dataclasses import dataclass, field
from datetime import datetime
from typing import Callable

import dearpygui.dearpygui as dpg

import igris_c_sdk as igc_sdk

try:
    import tkinter as _tk
except ImportError:  # tkinter is optional — only used to probe the screen size
    _tk = None

# -- Static configuration ------------------------------------------------------

NUM_MOTORS = 31
LOWSTATE_TOPIC = "rt/lowstate"
LOWCMD_TOPIC = "rt/lowcmd"
ROBOTSTATE_TOPIC = "rt/robotstate"

LOWCMD_PERIOD_S = 1.0 / 300.0  # 300 Hz publish loop
SERVICE_TIMEOUT_MS = 60_000
MUJOCO_TIMEOUT_MS = 5_000
ROBOTSTATE_TIMEOUT_S = 2.0
RESET_DURATION_S = 5.0
DEFAULT_CONVERGENCE_S = 1.5
MIN_DELTA = 1e-4
MAX_LOG_LINES = 200
DIFF_TOL_RAD = 0.05  # |cmd - state| > 0.05 rad highlights row

# UI scaling — computed at runtime from screen size. All hardcoded widget
# pixel sizes go through `S()` so a 4K monitor gets ~2x bigger buttons / fonts
# without us touching every literal. Mirrors ComputeUiScale in the C++ port.
BASE_WINDOW_WIDTH = 1800.0
BASE_WINDOW_HEIGHT = 900.0
MIN_UI_SCALE = 1.0
MAX_UI_SCALE = 2.5


class _UI:
    """Holds runtime-tuned UI parameters.

    Wrapping the active UI scale in a class attribute (instead of a module
    global) lets us mutate it from main() without a `global` statement —
    which Pylint flags via PLW0603.
    """

    scale: float = 1.0


# Panel proportions (sum < 1.0 — the rest goes to the left slider panel).
# Match the C++ baseline of 720/320 against 1800: center 40%, right 18%.
# Using proportions instead of `BASE * UI_SCALE` keeps the slider columns
# from getting starved at high scales, where 720*2.5 alone would eat half
# of a 4K window.
CENTER_PANEL_RATIO = 0.40
RIGHT_PANEL_RATIO = 0.18

# Hard floor for the layout. Below this the LowCmd slider Position column
# collapses to zero — keep it just above the point where it starts pinching.
LAYOUT_MIN_W_BASE = 1300.0
LAYOUT_MIN_H_BASE = 750.0


def S(value: float) -> int:
    """Scale a pixel size by the active UI scale and round to int."""
    return int(round(value * _UI.scale))


def detect_screen_size() -> tuple[int, int]:
    """Best-effort screen-resolution probe. Falls back to 1920x1080 on failure."""
    if _tk is None:
        return 1920, 1080
    try:
        root = _tk.Tk()
        root.withdraw()
        width = root.winfo_screenwidth()
        height = root.winfo_screenheight()
        root.destroy()
        if width > 0 and height > 0:
            return int(width), int(height)
    except Exception:  # noqa: BLE001 - any tk failure (no display etc.) → fallback
        pass
    return 1920, 1080


def compute_ui_scale(screen_w: int, screen_h: int) -> float:
    raw = min(screen_w / BASE_WINDOW_WIDTH, screen_h / BASE_WINDOW_HEIGHT)
    return max(MIN_UI_SCALE, min(MAX_UI_SCALE, raw))


MOTOR_NAMES: tuple[str, ...] = (
    "Waist_Yaw",
    "Waist_L",
    "Waist_R",
    "Hip_Pitch_L",
    "Hip_Roll_L",
    "Hip_Yaw_L",
    "Knee_Pitch_L",
    "Ankle_Out_L",
    "Ankle_In_L",
    "Hip_Pitch_R",
    "Hip_Roll_R",
    "Hip_Yaw_R",
    "Knee_Pitch_R",
    "Ankle_Out_R",
    "Ankle_In_R",
    "Shoulder_Pitch_L",
    "Shoulder_Roll_L",
    "Shoulder_Yaw_L",
    "Elbow_Pitch_L",
    "Wrist_Yaw_L",
    "Wrist_Front_L",
    "Wrist_Back_L",
    "Shoulder_Pitch_R",
    "Shoulder_Roll_R",
    "Shoulder_Yaw_R",
    "Elbow_Pitch_R",
    "Wrist_Yaw_R",
    "Wrist_Front_R",
    "Wrist_Back_R",
    "Neck_Yaw",
    "Neck_Pitch",
)

JOINT_NAMES: tuple[str, ...] = (
    "Waist_Yaw",
    "Waist_Roll",
    "Waist_Pitch",
    "Hip_Pitch_L",
    "Hip_Roll_L",
    "Hip_Yaw_L",
    "Knee_Pitch_L",
    "Ankle_Pitch_L",
    "Ankle_Roll_L",
    "Hip_Pitch_R",
    "Hip_Roll_R",
    "Hip_Yaw_R",
    "Knee_Pitch_R",
    "Ankle_Pitch_R",
    "Ankle_Roll_R",
    "Shoulder_Pitch_L",
    "Shoulder_Roll_L",
    "Shoulder_Yaw_L",
    "Elbow_Pitch_L",
    "Wrist_Yaw_L",
    "Wrist_Roll_L",
    "Wrist_Pitch_L",
    "Shoulder_Pitch_R",
    "Shoulder_Roll_R",
    "Shoulder_Yaw_R",
    "Elbow_Pitch_R",
    "Wrist_Yaw_R",
    "Wrist_Roll_R",
    "Wrist_Pitch_R",
    "Neck_Yaw",
    "Neck_Pitch",
)

JOINT_POS_MAX: tuple[float, ...] = (
    1.57,
    0.310,
    0.28,
    0.480,
    2.300,
    1.570,
    2.280,
    0.698,
    0.349,
    0.480,
    0.330,
    1.570,
    2.280,
    0.698,
    0.349,
    1.047,
    3.140,
    1.570,
    0.0,
    1.570,
    0.870,
    0.650,
    1.047,
    0.170,
    1.570,
    0.0,
    1.570,
    1.221,
    0.650,
    1.221,
    0.520,
)
JOINT_POS_MIN: tuple[float, ...] = (
    -1.57,
    -0.310,
    -0.87,
    -2.000,
    -0.330,
    -1.570,
    0.0,
    -0.70,
    -0.350,
    -2.000,
    -2.300,
    -1.570,
    0.0,
    -0.70,
    -0.350,
    -3.141,
    -0.170,
    -1.570,
    -2.0,
    -1.570,
    -1.221,
    -0.650,
    -3.141,
    -3.140,
    -1.570,
    -2.0,
    -1.570,
    -0.870,
    -0.650,
    -1.221,
    -0.520,
)
MOTOR_POS_MAX: tuple[float, ...] = (
    1.57,
    0.87,
    0.87,
    0.480,
    2.300,
    1.570,
    2.280,
    0.609,
    0.523,
    0.480,
    0.330,
    1.570,
    2.280,
    0.609,
    0.523,
    1.047,
    3.140,
    1.570,
    0.0,
    1.570,
    0.75,
    0.75,
    1.047,
    0.170,
    1.570,
    0.0,
    1.570,
    0.98,
    0.98,
    1.221,
    0.520,
)
MOTOR_POS_MIN: tuple[float, ...] = (
    -1.57,
    -0.34,
    -0.34,
    -2.000,
    -0.330,
    -1.570,
    0.0,
    -0.630,
    -0.617,
    -2.000,
    -2.300,
    -1.570,
    0.0,
    -0.630,
    -0.617,
    -3.141,
    -0.170,
    -1.570,
    -2.0,
    -1.570,
    -0.98,
    -0.98,
    -3.141,
    -3.140,
    -1.570,
    -2.0,
    -1.570,
    -0.75,
    -0.75,
    -1.221,
    -0.520,
)

DEFAULT_KP: tuple[float, ...] = (
    50.0,
    25.0,
    25.0,
    500.0,
    200.0,
    50.0,
    500.0,
    50.0,
    50.0,
    500.0,
    200.0,
    50.0,
    500.0,
    50.0,
    50.0,
    50.0,
    50.0,
    30.0,
    30.0,
    15.0,
    15.0,
    15.0,
    50.0,
    50.0,
    30.0,
    30.0,
    15.0,
    15.0,
    15.0,
    2.0,
    5.0,
)
DEFAULT_KD: tuple[float, ...] = (
    0.8,
    0.8,
    0.8,
    3.0,
    0.5,
    0.5,
    3.0,
    3.0,
    3.0,
    3.0,
    0.5,
    0.5,
    3.0,
    3.0,
    3.0,
    0.5,
    0.5,
    0.15,
    0.15,
    0.1,
    0.1,
    0.1,
    0.5,
    0.5,
    0.15,
    0.15,
    0.1,
    0.1,
    0.1,
    0.05,
    0.1,
)

# 5 parallel-link groups: waist, left ankle, right ankle, left wrist, right wrist
PARALLEL_LINK_NAMES: tuple[str, ...] = (
    "Waist",
    "Left Ankle",
    "Right Ankle",
    "Left Wrist",
    "Right Wrist",
)
PARALLEL_LINK_JOINTS: tuple[tuple[int, int], ...] = ((1, 2), (7, 8), (13, 14), (20, 21), (27, 28))
NUM_PARALLEL_LINKS = len(PARALLEL_LINK_JOINTS)
SECTION_BREAKS: tuple[int, ...] = (3, 9, 15, 22, 29)

SPACE_MOTOR = 0
SPACE_JOINT = 1


def parallel_link_for_joint(idx: int) -> int:
    for link_idx, pair in enumerate(PARALLEL_LINK_JOINTS):
        if idx in pair:
            return link_idx
    return -1


# Robot control state name lookup (mirrors RobotControlStateToString in C++).
ROBOT_STATE_LABEL: dict[int, str] = {}


def _build_robot_state_labels() -> None:
    enum_cls = igc_sdk.RobotControlState
    mapping = {
        "ROBOT_STATE_NOT_READY": "NOT_READY",
        "ROBOT_STATE_IDLE": "IDLE",
        "ROBOT_STATE_STOP": "STOP",
        "ROBOT_STATE_LOW": "LOW",
        "ROBOT_STATE_HIGH_MOTION_ACTIVE": "HIGH_MOTION_ACTIVE",
        "ROBOT_STATE_HIGH_MOTION_STANDBY": "HIGH_MOTION_STANDBY",
        "ROBOT_STATE_WALK_LOW": "WALK_LOW",
        "ROBOT_STATE_WALK_HIGH_MOTION_ACTIVE": "WALK_HIGH_MOTION_ACTIVE",
        "ROBOT_STATE_WALK_HIGH_MOTION_STANDBY": "WALK_HIGH_MOTION_STANDBY",
        "ROBOT_STATE_WHOLE_BODY_CONTROL": "WHOLE_BODY_CONTROL",
    }
    for name, label in mapping.items():
        member = getattr(enum_cls, name, None)
        if member is not None:
            ROBOT_STATE_LABEL[int(member)] = label


# -- Shared state --------------------------------------------------------------


@dataclass
class AppState:
    state_lock: threading.Lock = field(default_factory=threading.Lock)
    log_lock: threading.Lock = field(default_factory=threading.Lock)
    log_lines: deque[str] = field(default_factory=lambda: deque(maxlen=MAX_LOG_LINES))
    log_dirty: bool = True

    first_state_received: bool = False
    lowstate_count: int = 0
    current_motor_pos: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    current_joint_pos: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    imu_quaternion: list[float] = field(default_factory=lambda: [1.0, 0.0, 0.0, 0.0])
    imu_gyroscope: list[float] = field(default_factory=lambda: [0.0, 0.0, 0.0])
    imu_accelerometer: list[float] = field(default_factory=lambda: [0.0, 0.0, 0.0])
    imu_rpy: list[float] = field(default_factory=lambda: [0.0, 0.0, 0.0])

    robotstate_connected: bool = False
    last_robotstate_time: float = 0.0
    robot_state_label: str = "--"
    is_sim_env: bool = False

    lowlevel_active: bool = False
    target_motor_pos: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    target_joint_pos: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    cmd_motor_pos: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    cmd_joint_pos: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    parallel_space: list[int] = field(default_factory=lambda: [SPACE_MOTOR] * NUM_PARALLEL_LINKS)
    kp: list[float] = field(default_factory=lambda: list(DEFAULT_KP))
    kd: list[float] = field(default_factory=lambda: list(DEFAULT_KD))
    convergence_time: float = DEFAULT_CONVERGENCE_S
    reset_mode_active: bool = False
    lowcmd_publish_count: int = 0
    last_cmd_q: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    last_cmd_tau: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    last_cmd_kp: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    last_cmd_kd: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)

    service_call_in_progress: bool = False

    pending_service_label: str = ""
    pending_service_action: Callable[[], None] | None = None

    def use_motor_space_for_joint(self, idx: int) -> bool:
        link_idx = parallel_link_for_joint(idx)
        if link_idx < 0:
            return False
        return self.parallel_space[link_idx] == SPACE_MOTOR

    def add_log(self, message: str) -> None:
        stamp = datetime.now().strftime("%H:%M:%S")
        with self.log_lock:
            self.log_lines.append(f"[{stamp}] {message}")
            self.log_dirty = True

    def sync_parallel_link_to_state(self, link_idx: int) -> None:
        if not (0 <= link_idx < NUM_PARALLEL_LINKS):
            return
        for joint_idx in PARALLEL_LINK_JOINTS[link_idx]:
            self.target_motor_pos[joint_idx] = self.current_motor_pos[joint_idx]
            self.cmd_motor_pos[joint_idx] = self.current_motor_pos[joint_idx]
            self.target_joint_pos[joint_idx] = self.current_joint_pos[joint_idx]
            self.cmd_joint_pos[joint_idx] = self.current_joint_pos[joint_idx]


# -- Subscribers ---------------------------------------------------------------


def make_lowstate_callback(state: AppState) -> Callable[[object], None]:
    def callback(msg: object) -> None:
        motor_state = msg.motor_state()
        joint_state = msg.joint_state()
        imu = msg.imu_state()
        quaternion = list(imu.quaternion())
        gyroscope = list(imu.gyroscope())
        accelerometer = list(imu.accelerometer())
        rpy = list(imu.rpy())
        motor_q = [float(motor_state[i].q()) for i in range(NUM_MOTORS)]
        joint_q = [float(joint_state[i].q()) for i in range(NUM_MOTORS)]

        first_seen = False
        with state.state_lock:
            state.lowstate_count += 1
            state.current_motor_pos = motor_q
            state.current_joint_pos = joint_q
            state.imu_quaternion = [float(v) for v in quaternion]
            state.imu_gyroscope = [float(v) for v in gyroscope]
            state.imu_accelerometer = [float(v) for v in accelerometer]
            state.imu_rpy = [float(v) for v in rpy]
            if not state.first_state_received:
                state.first_state_received = True
                state.target_motor_pos = list(motor_q)
                state.target_joint_pos = list(joint_q)
                state.cmd_motor_pos = list(motor_q)
                state.cmd_joint_pos = list(joint_q)
                first_seen = True

        if first_seen:
            state.add_log("First LowState received - reference values initialized")

    return callback


def make_robotstate_callback(state: AppState) -> Callable[[object], None]:
    sim_env_value = int(getattr(igc_sdk.RobotEnvironment, "ROBOT_ENV_SIM"))

    def callback(msg: object) -> None:
        env = int(msg.environment())
        robot_state = int(msg.state())
        with state.state_lock:
            state.robotstate_connected = True
            state.last_robotstate_time = time.monotonic()
            state.robot_state_label = ROBOT_STATE_LABEL.get(robot_state, "UNKNOWN")
            state.is_sim_env = env == sim_env_value

    return callback


# -- Service-call helpers ------------------------------------------------------


def _spawn_service(state: AppState, label: str, fn: Callable[[], object]) -> None:
    with state.state_lock:
        if state.service_call_in_progress:
            state.add_log("Service call already in progress, please wait...")
            return
        state.service_call_in_progress = True

    state.add_log(f"Calling {label}...")

    def runner() -> None:
        try:
            response = fn()
            ok = bool(response.success())
            message = response.message() or ""
            verdict = "SUCCESS" if ok else "FAILED"
            state.add_log(f"{label}: {verdict} - {message}".rstrip(" -"))
        except Exception as exc:  # noqa: BLE001
            state.add_log(f"{label}: EXCEPTION - {exc}")
        finally:
            with state.state_lock:
                state.service_call_in_progress = False

    threading.Thread(target=runner, name=f"svc-{label}", daemon=True).start()


def call_init_bms(
    state: AppState, client: igc_sdk.IgrisC_Client, init_type: igc_sdk.BmsInitType, label: str
) -> None:
    _spawn_service(
        state, f"InitBms({label})", lambda: client.InitBms(init_type, SERVICE_TIMEOUT_MS)
    )


def call_set_torque(
    state: AppState, client: igc_sdk.IgrisC_Client, torque_type: igc_sdk.TorqueType, label: str
) -> None:
    _spawn_service(
        state, f"SetTorque({label})", lambda: client.SetTorque(torque_type, SERVICE_TIMEOUT_MS)
    )


def call_control_mode(
    state: AppState,
    client: igc_sdk.IgrisC_Client,
    cmd_type: igc_sdk.ControlModeCommandType,
    label: str,
    preset_id: str = "",
    is_cyclic: bool = False,
) -> None:
    _spawn_service(
        state,
        f"ControlModeCommand({label})",
        lambda: client.SendControlModeCommand(cmd_type, preset_id, is_cyclic, SERVICE_TIMEOUT_MS),
    )


def call_mujoco_sim(
    state: AppState,
    client: igc_sdk.IgrisC_Client,
    cmd_type: igc_sdk.MujocoSimCommandType,
    label: str,
) -> None:
    _spawn_service(
        state, f"MujocoSim({label})", lambda: client.SendMujocoSimCmd(cmd_type, MUJOCO_TIMEOUT_MS)
    )


# -- LowCmd publishing thread --------------------------------------------------


def lowcmd_publish_loop(
    state: AppState,
    publisher: igc_sdk.LowCmdPublisher,
    stop_event: threading.Event,
) -> None:
    next_time = time.monotonic()
    cmd = igc_sdk.LowCmd()
    motors = cmd.motors()

    while not stop_event.is_set():
        next_time += LOWCMD_PERIOD_S
        sleep_s = next_time - time.monotonic()
        if sleep_s > 0:
            stop_event.wait(timeout=sleep_s)
            if stop_event.is_set():
                break
        else:
            next_time = time.monotonic()

        with state.state_lock:
            if not state.lowlevel_active:
                continue

            duration = RESET_DURATION_S if state.reset_mode_active else state.convergence_time
            if duration <= 0.01:
                alpha = 1.0
            else:
                alpha = 1.0 - math.pow(0.05, 1.0 / (duration * 300.0))

            # pybind11 returns kinematic_modes() as a converted list of enum
            # values, so writing to it via `[i] = ...` is silently dropped.
            # Build the full list and feed it through the setter overload.
            kinematic_pjs = igc_sdk.KinematicMode.PJS
            kinematic_ms = igc_sdk.KinematicMode.MS
            cmd.kinematic_modes(
                [
                    kinematic_pjs if state.parallel_space[link_idx] == SPACE_JOINT else kinematic_ms
                    for link_idx in range(NUM_PARALLEL_LINKS)
                ]
            )

            all_reached = True
            for i in range(NUM_MOTORS):
                use_motor_space = state.use_motor_space_for_joint(i)
                if use_motor_space:
                    target = state.target_motor_pos[i]
                    cmd_pos = state.cmd_motor_pos[i]
                else:
                    target = state.target_joint_pos[i]
                    cmd_pos = state.cmd_joint_pos[i]

                delta = target - cmd_pos
                if abs(delta) > MIN_DELTA:
                    cmd_pos += delta * alpha
                    all_reached = False
                else:
                    cmd_pos = target

                if use_motor_space:
                    state.cmd_motor_pos[i] = cmd_pos
                else:
                    state.cmd_joint_pos[i] = cmd_pos

                motor = motors[i]
                motor.id(i)
                motor.q(cmd_pos)
                motor.dq(0.0)
                motor.tau(0.0)
                motor.kp(state.kp[i])
                motor.kd(state.kd[i])

                state.last_cmd_q[i] = cmd_pos
                state.last_cmd_tau[i] = 0.0
                state.last_cmd_kp[i] = state.kp[i]
                state.last_cmd_kd[i] = state.kd[i]

            if all_reached and state.reset_mode_active:
                state.reset_mode_active = False
            state.lowcmd_publish_count += 1

        publisher.write(cmd)


def start_lowcmd_publishing(state: AppState) -> None:
    with state.state_lock:
        if state.lowlevel_active:
            state.add_log("LowCmd publishing already active")
            return
        if not state.first_state_received:
            state.add_log("Cannot start LowCmd: waiting for first LowState")
            return
        for i in range(NUM_MOTORS):
            state.target_motor_pos[i] = state.current_motor_pos[i]
            state.target_joint_pos[i] = state.current_joint_pos[i]
            state.cmd_motor_pos[i] = state.current_motor_pos[i]
            state.cmd_joint_pos[i] = state.current_joint_pos[i]
        state.lowlevel_active = True
    state.add_log("LowCmd publishing started - initialized to current positions")


def stop_lowcmd_publishing(state: AppState) -> None:
    with state.state_lock:
        if not state.lowlevel_active:
            state.add_log("LowCmd publishing already stopped")
            return
        state.lowlevel_active = False
    state.add_log("LowCmd publishing stopped")


# -- DPG widget tags -----------------------------------------------------------


class Tags:
    main_window = "main_window"
    log_text = "log_text"

    left_panel = "left_panel"
    center_panel = "center_panel"
    right_panel = "right_panel"

    lowstate_count = "lowstate_count"
    duration_slider = "duration_slider"
    duration_text = "duration_text"
    reference_status = "reference_status"
    publish_button = "publish_button"

    dds_domain = "dds_domain"
    dds_namespace = "dds_namespace"
    dds_status = "dds_status"
    dds_robot_state = "dds_robot_state"
    dds_environment = "dds_environment"
    mujoco_group = "mujoco_group"

    imu_quaternion = "imu_quaternion"
    imu_gyroscope = "imu_gyroscope"
    imu_accelerometer = "imu_accelerometer"
    imu_rpy = "imu_rpy"

    lowcmd_status = "lowcmd_status"
    lowcmd_count = "lowcmd_count"
    lowcmd_inactive_hint = "lowcmd_inactive_hint"
    lowcmd_tabbar = "lowcmd_tabbar"

    confirm_modal = "confirm_modal"
    confirm_label = "confirm_label"

    @staticmethod
    def slider_row(i: int) -> str:
        return f"slider_row_{i}"

    @staticmethod
    def slider(i: int) -> str:
        return f"slider_{i}"

    @staticmethod
    def slider_name(i: int) -> str:
        return f"slider_name_{i}"

    @staticmethod
    def kp_input(i: int) -> str:
        return f"kp_input_{i}"

    @staticmethod
    def kd_input(i: int) -> str:
        return f"kd_input_{i}"

    @staticmethod
    def space_radio(i: int) -> str:
        return f"space_radio_{i}"

    @staticmethod
    def cmd_cell(i: int, col: str) -> str:
        return f"cmd_{col}_{i}"

    @staticmethod
    def diff_cell(i: int, col: str) -> str:
        return f"diff_{col}_{i}"


# -- UI build ------------------------------------------------------------------


COLOR_GREEN = (90, 220, 110)
COLOR_RED = (235, 90, 90)
COLOR_YELLOW = (240, 220, 80)
COLOR_GREY = (140, 140, 140)
COLOR_LABEL = (130, 230, 160)
COLOR_SIM = (130, 200, 255)
COLOR_REAL = (255, 140, 80)
COLOR_HEADER = (155, 220, 255)


def build_left_panel(ctx: "GuiContext") -> None:
    with dpg.child_window(border=False, width=-1, height=-1):
        dpg.add_text("LowCmd Sliders", color=COLOR_HEADER)
        dpg.add_separator()
        with dpg.group(horizontal=True):
            dpg.add_button(
                label="Reset Pos", width=S(-300), height=S(32), callback=ctx.on_reset_pos
            )
            dpg.add_button(
                label="Reset Gains", width=S(-150), height=S(32), callback=ctx.on_reset_gains
            )
            dpg.add_button(label="Home", width=-1, height=S(32), callback=ctx.on_home)
        dpg.add_button(
            tag=Tags.publish_button,
            label="LowCmd Publish Start",
            width=-1,
            height=S(32),
            callback=ctx.on_toggle_publish,
        )
        dpg.add_separator()
        with dpg.group(horizontal=True):
            dpg.add_text("LowState: 0", tag=Tags.lowstate_count)
            dpg.add_slider_float(
                tag=Tags.duration_slider,
                default_value=DEFAULT_CONVERGENCE_S,
                min_value=0.0,
                max_value=3.0,
                width=S(-130),
                format="",
                callback=ctx.on_duration_changed,
            )
            dpg.add_text(f"Duration: {DEFAULT_CONVERGENCE_S:.1f}s", tag=Tags.duration_text)
        dpg.add_text("Waiting for first state...", tag=Tags.reference_status, color=COLOR_YELLOW)
        dpg.add_separator()

        with dpg.child_window(border=False, width=-1, height=-1):
            with dpg.table(
                header_row=True,
                policy=dpg.mvTable_SizingStretchProp,
                resizable=False,
                borders_innerH=False,
                borders_outerH=False,
                borders_innerV=False,
                borders_outerV=False,
            ):
                dpg.add_table_column(label="#", width_fixed=True, init_width_or_weight=S(30))
                dpg.add_table_column(label="Name", width_fixed=True, init_width_or_weight=S(150))
                dpg.add_table_column(label="Space", width_fixed=True, init_width_or_weight=S(110))
                dpg.add_table_column(label="Position", width_stretch=True, init_width_or_weight=1.0)
                dpg.add_table_column(label="Kp", width_fixed=True, init_width_or_weight=S(70))
                dpg.add_table_column(label="Kd", width_fixed=True, init_width_or_weight=S(60))

                for i in range(NUM_MOTORS):
                    if i in SECTION_BREAKS:
                        with dpg.table_row():
                            for _ in range(6):
                                dpg.add_separator()
                    with dpg.table_row(tag=Tags.slider_row(i)):
                        dpg.add_text(f"{i}.")
                        dpg.add_text(MOTOR_NAMES[i], tag=Tags.slider_name(i))
                        link_idx = parallel_link_for_joint(i)
                        if link_idx >= 0:
                            dpg.add_radio_button(
                                tag=Tags.space_radio(i),
                                items=("MS", "PJS"),
                                default_value="MS",
                                horizontal=True,
                                callback=ctx.make_space_callback(i),
                            )
                        else:
                            dpg.add_text("-", color=COLOR_GREY)
                        dpg.add_slider_float(
                            tag=Tags.slider(i),
                            default_value=0.0,
                            min_value=MOTOR_POS_MIN[i],
                            max_value=MOTOR_POS_MAX[i],
                            format="%.3f",
                            width=-1,
                            callback=ctx.make_slider_callback(i),
                        )
                        dpg.add_input_float(
                            tag=Tags.kp_input(i),
                            default_value=DEFAULT_KP[i],
                            format="%.1f",
                            step=0,
                            on_enter=True,
                            width=-1,
                            callback=ctx.make_kp_callback(i),
                        )
                        dpg.add_input_float(
                            tag=Tags.kd_input(i),
                            default_value=DEFAULT_KD[i],
                            format="%.2f",
                            step=0,
                            on_enter=True,
                            width=-1,
                            callback=ctx.make_kd_callback(i),
                        )


def build_center_panel(ctx: "GuiContext") -> None:
    with dpg.child_window(border=False, width=-1, height=-1):
        dpg.add_text("DDS Connection Status", color=COLOR_HEADER)
        dpg.add_separator()
        dpg.add_text("Domain:    -", tag=Tags.dds_domain)
        dpg.add_text("Namespace: -", tag=Tags.dds_namespace)
        dpg.add_text("Status: --", tag=Tags.dds_status, color=COLOR_GREY)
        dpg.add_text("Robot State: --", tag=Tags.dds_robot_state, color=COLOR_GREY)
        dpg.add_text("Environment: --", tag=Tags.dds_environment, color=COLOR_GREY)

        with dpg.group(tag=Tags.mujoco_group, show=False):
            dpg.add_separator()
            dpg.add_text("Mujoco Sim", color=COLOR_HEADER)
            with dpg.group(horizontal=True):
                dpg.add_button(
                    label="Pause", width=S(140), height=S(32), callback=ctx.on_mujoco_pause
                )
                dpg.add_button(
                    label="Resume", width=S(140), height=S(32), callback=ctx.on_mujoco_resume
                )
            with dpg.group(horizontal=True):
                dpg.add_button(
                    label="Reload Model", width=S(140), height=S(32), callback=ctx.on_mujoco_reload
                )
                dpg.add_button(
                    label="Reset State", width=S(140), height=S(32), callback=ctx.on_mujoco_reset
                )

        dpg.add_separator()
        dpg.add_text("IMU State", color=COLOR_HEADER)
        dpg.add_separator()
        dpg.add_text("Quaternion:", color=COLOR_LABEL)
        dpg.add_text("  w: 0.0000   x: 0.0000   y: 0.0000   z: 0.0000", tag=Tags.imu_quaternion)
        dpg.add_text("Gyroscope (rad/s):", color=COLOR_LABEL)
        dpg.add_text("  x: 0.0000   y: 0.0000   z: 0.0000", tag=Tags.imu_gyroscope)
        dpg.add_text("Accelerometer (m/s^2):", color=COLOR_LABEL)
        dpg.add_text("  x: 0.0000   y: 0.0000   z: 0.0000", tag=Tags.imu_accelerometer)
        dpg.add_text("Roll-Pitch-Yaw (rad):", color=COLOR_LABEL)
        dpg.add_text("  R: 0.0000   P: 0.0000   Y: 0.0000", tag=Tags.imu_rpy)

        dpg.add_separator()
        dpg.add_text("LowCmd Publishing", color=COLOR_HEADER)
        dpg.add_separator()
        dpg.add_text("Status: INACTIVE", tag=Tags.lowcmd_status, color=COLOR_RED)
        dpg.add_text("Published: 0 msgs", tag=Tags.lowcmd_count)
        dpg.add_text(
            "Press LowCmd Publish Start to begin",
            tag=Tags.lowcmd_inactive_hint,
            color=COLOR_GREY,
        )

        with dpg.tab_bar(tag=Tags.lowcmd_tabbar, show=False):
            with dpg.tab(label="LowCmd"):
                with dpg.table(
                    header_row=True,
                    policy=dpg.mvTable_SizingStretchProp,
                    resizable=False,
                    scrollY=True,
                    height=S(300),
                    borders_innerH=True,
                    borders_outerH=True,
                    borders_innerV=True,
                    borders_outerV=True,
                ):
                    dpg.add_table_column(label="#", width_fixed=True, init_width_or_weight=S(30))
                    dpg.add_table_column(label="Joint")
                    dpg.add_table_column(label="q (rad)")
                    dpg.add_table_column(label="tau (Nm)")
                    dpg.add_table_column(label="kp")
                    dpg.add_table_column(label="kd")
                    for i in range(NUM_MOTORS):
                        with dpg.table_row():
                            dpg.add_text(str(i))
                            dpg.add_text(JOINT_NAMES[i])
                            dpg.add_text("0.000", tag=Tags.cmd_cell(i, "q"))
                            dpg.add_text("0.00", tag=Tags.cmd_cell(i, "tau"))
                            dpg.add_text("0.0", tag=Tags.cmd_cell(i, "kp"))
                            dpg.add_text("0.00", tag=Tags.cmd_cell(i, "kd"))

            with dpg.tab(label="Cmd vs State"):
                with dpg.table(
                    header_row=True,
                    policy=dpg.mvTable_SizingStretchProp,
                    resizable=False,
                    scrollY=True,
                    height=S(300),
                    borders_innerH=True,
                    borders_outerH=True,
                    borders_innerV=True,
                    borders_outerV=True,
                ):
                    dpg.add_table_column(label="#", width_fixed=True, init_width_or_weight=S(30))
                    dpg.add_table_column(label="Joint")
                    dpg.add_table_column(label="Space")
                    dpg.add_table_column(label="cmd (rad)")
                    dpg.add_table_column(label="state (rad)")
                    dpg.add_table_column(label="diff")
                    for i in range(NUM_MOTORS):
                        with dpg.table_row():
                            dpg.add_text(str(i))
                            dpg.add_text(JOINT_NAMES[i], tag=Tags.diff_cell(i, "name"))
                            dpg.add_text("MS/PJS", tag=Tags.diff_cell(i, "space"))
                            dpg.add_text("0.000", tag=Tags.diff_cell(i, "cmd"))
                            dpg.add_text("0.000", tag=Tags.diff_cell(i, "state"))
                            dpg.add_text("+0.000", tag=Tags.diff_cell(i, "diff"))


def build_right_panel(ctx: "GuiContext") -> None:
    with dpg.child_window(border=False, width=-1, height=-1):
        dpg.add_text("Service API Commands", color=COLOR_HEADER)
        dpg.add_separator()
        dpg.add_text("Power & Torque", color=COLOR_HEADER)
        dpg.add_button(label="Init BMS", width=-1, height=S(32), callback=ctx.on_init_bms)
        dpg.add_button(label="Init Motor", width=-1, height=S(32), callback=ctx.on_init_motor)
        dpg.add_button(label="Init BMS + Motor", width=-1, height=S(32), callback=ctx.on_init_both)
        dpg.add_button(label="BMS OFF", width=-1, height=S(32), callback=ctx.on_bms_off)
        dpg.add_button(label="Torque ON", width=-1, height=S(32), callback=ctx.on_torque_on)
        dpg.add_button(label="Torque OFF", width=-1, height=S(32), callback=ctx.on_torque_off)

        dpg.add_separator()
        dpg.add_text("Control Mode", color=COLOR_HEADER)
        dpg.add_button(label="High-Level", width=-1, height=S(32), callback=ctx.on_high_level)
        dpg.add_button(label="Low-Level", width=-1, height=S(32), callback=ctx.on_low_level)
        dpg.add_button(
            label="Joint Position Hold", width=-1, height=S(32), callback=ctx.on_joint_hold
        )
        dpg.add_button(label="Motion Stop", width=-1, height=S(32), callback=ctx.on_motion_stop)
        dpg.add_button(label="Walkmode", width=-1, height=S(32), callback=ctx.on_walkmode)
        dpg.add_button(
            label="Low-Level Walkmode", width=-1, height=S(32), callback=ctx.on_low_walkmode
        )
        dpg.add_button(
            label="High-Level Walkmode", width=-1, height=S(32), callback=ctx.on_high_walkmode
        )

        dpg.add_separator()
        dpg.add_text("Custom Modes", color=COLOR_HEADER)
        dpg.add_button(label="Custom Mode 1", width=-1, height=S(32), callback=ctx.on_custom1)
        dpg.add_button(label="Custom Mode 2", width=-1, height=S(32), callback=ctx.on_custom2)
        dpg.add_button(label="Custom Mode 3", width=-1, height=S(32), callback=ctx.on_custom3)

        dpg.add_separator()
        dpg.add_text("Motion Presets", color=COLOR_HEADER)
        dpg.add_button(label="HOME", width=-1, height=S(32), callback=ctx.on_motion_home)


def build_log_panel() -> None:
    with dpg.child_window(border=True, width=-1, height=-1):
        with dpg.group(horizontal=True):
            dpg.add_text("Response Log", color=COLOR_HEADER)
        dpg.add_separator()
        with dpg.child_window(border=False, width=-1, height=-1):
            dpg.add_text("", tag=Tags.log_text)


def build_confirm_popup(state: AppState) -> None:
    with dpg.window(
        label="Confirm Service Command",
        modal=True,
        show=False,
        tag=Tags.confirm_modal,
        no_resize=True,
        no_move=True,
        no_collapse=True,
        width=S(420),
        height=S(180),
        no_close=True,
    ):
        dpg.add_text("Execute the following command?")
        dpg.add_text("--", tag=Tags.confirm_label, color=(255, 215, 100))
        dpg.add_separator()
        dpg.add_text("This affects the live robot.", color=(255, 130, 130))
        dpg.add_separator()
        with dpg.group(horizontal=True):
            dpg.add_button(
                label="Confirm",
                width=S(180),
                height=S(32),
                callback=lambda: _confirm_modal_resolve(state, True),
            )
            dpg.add_button(
                label="Cancel",
                width=S(180),
                height=S(32),
                callback=lambda: _confirm_modal_resolve(state, False),
            )


def _confirm_modal_resolve(state: AppState, accepted: bool) -> None:
    with state.state_lock:
        action = state.pending_service_action
        label = state.pending_service_label
        state.pending_service_action = None
        state.pending_service_label = ""
    dpg.configure_item(Tags.confirm_modal, show=False)
    if accepted and action is not None:
        action()
    elif not accepted and label:
        state.add_log(f"Cancelled: {label}")


def install_modal_key_handlers(state: AppState) -> None:
    """Wire Enter/Escape to the confirm-modal Confirm/Cancel buttons.

    DPG's modal windows have no built-in default-button binding, so we install
    a global key-press handler that no-ops unless a service action is staged.
    """

    def on_key(accepted: bool) -> None:
        with state.state_lock:
            has_pending = state.pending_service_action is not None
        if has_pending:
            _confirm_modal_resolve(state, accepted)

    with dpg.handler_registry():
        dpg.add_key_press_handler(key=dpg.mvKey_Return, callback=lambda: on_key(True))
        dpg.add_key_press_handler(key=dpg.mvKey_NumPadEnter, callback=lambda: on_key(True))
        dpg.add_key_press_handler(key=dpg.mvKey_Escape, callback=lambda: on_key(False))


# -- GUI context (callbacks) ---------------------------------------------------


@dataclass
class GuiContext:
    state: AppState
    client: igc_sdk.IgrisC_Client

    def request_confirm(self, label: str, action: Callable[[], None]) -> None:
        with self.state.state_lock:
            self.state.pending_service_action = action
            self.state.pending_service_label = label
        dpg.set_value(Tags.confirm_label, label)
        dpg.configure_item(Tags.confirm_modal, show=True)

    def make_slider_callback(self, idx: int) -> Callable[[object, object, object], None]:
        def callback(_sender: object, app_value: float, _user: object) -> None:
            with self.state.state_lock:
                if not self.state.lowlevel_active:
                    use_motor = self.state.use_motor_space_for_joint(idx)
                    cur = (
                        self.state.current_motor_pos[idx]
                        if use_motor
                        else self.state.current_joint_pos[idx]
                    )
                    dpg.set_value(Tags.slider(idx), cur)
                    return
                self.state.reset_mode_active = False
                if self.state.use_motor_space_for_joint(idx):
                    self.state.target_motor_pos[idx] = float(app_value)
                else:
                    self.state.target_joint_pos[idx] = float(app_value)

        return callback

    def make_kp_callback(self, idx: int) -> Callable[[object, object, object], None]:
        def callback(_sender: object, app_value: float, _user: object) -> None:
            value = max(0.0, float(app_value))
            with self.state.state_lock:
                old = self.state.kp[idx]
                self.state.kp[idx] = value
            self.state.add_log(f"Joint {idx} ({JOINT_NAMES[idx]}) kp: {old:.2f} -> {value:.2f}")

        return callback

    def make_kd_callback(self, idx: int) -> Callable[[object, object, object], None]:
        def callback(_sender: object, app_value: float, _user: object) -> None:
            value = max(0.0, float(app_value))
            with self.state.state_lock:
                old = self.state.kd[idx]
                self.state.kd[idx] = value
            self.state.add_log(f"Joint {idx} ({JOINT_NAMES[idx]}) kd: {old:.3f} -> {value:.3f}")

        return callback

    def make_space_callback(self, joint_idx: int) -> Callable[[object, object, object], None]:
        link_idx = parallel_link_for_joint(joint_idx)

        def callback(_sender: object, app_value: object, _user: object) -> None:
            if link_idx < 0:
                return
            tag = str(app_value)
            space = SPACE_MOTOR if tag == "MS" else SPACE_JOINT
            with self.state.state_lock:
                if self.state.parallel_space[link_idx] == space:
                    return
                self.state.parallel_space[link_idx] = space
                self.state.sync_parallel_link_to_state(link_idx)
                # Mirror the change onto the partner joint(s) of this link so
                # both rows stay in sync (and update each row's display name +
                # slider range to match the active space).
                for member in PARALLEL_LINK_JOINTS[link_idx]:
                    dpg.set_value(Tags.space_radio(member), tag)
                    if space == SPACE_MOTOR:
                        dpg.set_value(Tags.slider_name(member), MOTOR_NAMES[member])
                        dpg.configure_item(
                            Tags.slider(member),
                            min_value=MOTOR_POS_MIN[member],
                            max_value=MOTOR_POS_MAX[member],
                        )
                    else:
                        dpg.set_value(Tags.slider_name(member), JOINT_NAMES[member])
                        dpg.configure_item(
                            Tags.slider(member),
                            min_value=JOINT_POS_MIN[member],
                            max_value=JOINT_POS_MAX[member],
                        )
            self.state.add_log(f"{PARALLEL_LINK_NAMES[link_idx]} space changed to {tag}")

        return callback

    def on_reset_pos(self) -> None:
        with self.state.state_lock:
            if not self.state.lowlevel_active:
                self.state.add_log("Enable LOW_LEVEL publishing first to use Reset Pos")
                return
            for i in range(NUM_MOTORS):
                self.state.target_motor_pos[i] = 0.0
                self.state.target_joint_pos[i] = 0.0
            self.state.reset_mode_active = True
        self.state.add_log("All target positions reset to zero (5s transition)")

    def on_reset_gains(self) -> None:
        with self.state.state_lock:
            self.state.kp = list(DEFAULT_KP)
            self.state.kd = list(DEFAULT_KD)
        for i in range(NUM_MOTORS):
            dpg.set_value(Tags.kp_input(i), DEFAULT_KP[i])
            dpg.set_value(Tags.kd_input(i), DEFAULT_KD[i])
        self.state.add_log("All Kp/Kd gains reset to defaults")

    def on_home(self) -> None:
        with self.state.state_lock:
            if not self.state.lowlevel_active:
                self.state.add_log("Enable LOW_LEVEL publishing first to use Home")
                return
            for i in range(NUM_MOTORS):
                self.state.target_motor_pos[i] = 0.0
                self.state.target_joint_pos[i] = 0.0
            self.state.target_motor_pos[16] = 0.4
            self.state.target_motor_pos[23] = -0.4
            self.state.target_joint_pos[16] = 0.4
            self.state.target_joint_pos[23] = -0.4
            self.state.reset_mode_active = True
        self.state.add_log("Home position (5s transition)")

    def on_toggle_publish(self) -> None:
        with self.state.state_lock:
            active = self.state.lowlevel_active
        if active:
            stop_lowcmd_publishing(self.state)
        else:
            start_lowcmd_publishing(self.state)

    def on_duration_changed(self, _sender: object, app_value: float, _user: object) -> None:
        with self.state.state_lock:
            self.state.convergence_time = float(app_value)
        dpg.set_value(Tags.duration_text, f"Duration: {float(app_value):.1f}s")

    def on_init_bms(self) -> None:
        self.request_confirm(
            "Init BMS",
            lambda: call_init_bms(
                self.state, self.client, igc_sdk.BmsInitType.BMS_INIT, "BMS_INIT"
            ),
        )

    def on_init_motor(self) -> None:
        self.request_confirm(
            "Init Motor",
            lambda: call_init_bms(
                self.state, self.client, igc_sdk.BmsInitType.MOTOR_INIT, "MOTOR_INIT"
            ),
        )

    def on_init_both(self) -> None:
        self.request_confirm(
            "Init BMS + Motor",
            lambda: call_init_bms(
                self.state,
                self.client,
                igc_sdk.BmsInitType.BMS_AND_MOTOR_INIT,
                "BMS_AND_MOTOR_INIT",
            ),
        )

    def on_bms_off(self) -> None:
        self.request_confirm(
            "BMS OFF",
            lambda: call_init_bms(
                self.state, self.client, igc_sdk.BmsInitType.BMS_INIT_NONE, "BMS_INIT_NONE"
            ),
        )

    def on_torque_on(self) -> None:
        self.request_confirm(
            "Torque ON",
            lambda: call_set_torque(
                self.state, self.client, igc_sdk.TorqueType.TORQUE_ON, "TORQUE_ON"
            ),
        )

    def on_torque_off(self) -> None:
        self.request_confirm(
            "Torque OFF",
            lambda: call_set_torque(
                self.state, self.client, igc_sdk.TorqueType.TORQUE_OFF, "TORQUE_OFF"
            ),
        )

    def on_high_level(self) -> None:
        self.request_confirm(
            "Control Mode: High-Level",
            lambda: call_control_mode(
                self.state,
                self.client,
                igc_sdk.ControlModeCommandType.CONTROL_MODE_CMD_HIGH_LEVEL_JOINT_CONTROL,
                "HIGH_LEVEL_JOINT_CONTROL",
            ),
        )

    def on_low_level(self) -> None:
        self.request_confirm(
            "Control Mode: Low-Level",
            lambda: call_control_mode(
                self.state,
                self.client,
                igc_sdk.ControlModeCommandType.CONTROL_MODE_CMD_LOW_LEVEL_JOINT_CONTROL,
                "LOW_LEVEL_JOINT_CONTROL",
            ),
        )

    def on_joint_hold(self) -> None:
        self.request_confirm(
            "Joint Position Hold",
            lambda: call_control_mode(
                self.state,
                self.client,
                igc_sdk.ControlModeCommandType.CONTROL_MODE_CMD_JOINT_POSITION_HOLD,
                "JOINT_POSITION_HOLD",
            ),
        )

    def on_motion_stop(self) -> None:
        self.request_confirm(
            "Motion Stop",
            lambda: call_control_mode(
                self.state,
                self.client,
                igc_sdk.ControlModeCommandType.CONTROL_MODE_CMD_MOTION_STOP,
                "MOTION_STOP",
            ),
        )

    def on_walkmode(self) -> None:
        self.request_confirm(
            "Walkmode",
            lambda: call_control_mode(
                self.state,
                self.client,
                igc_sdk.ControlModeCommandType.CONTROL_MODE_CMD_WALKMODE_ON,
                "WALKMODE_ON",
            ),
        )

    def on_low_walkmode(self) -> None:
        self.request_confirm(
            "Low-Level Walkmode",
            lambda: call_control_mode(
                self.state,
                self.client,
                igc_sdk.ControlModeCommandType.CONTROL_MODE_CMD_LOW_LEVEL_WALKMODE_ON,
                "LOW_LEVEL_WALKMODE_ON",
            ),
        )

    def on_high_walkmode(self) -> None:
        self.request_confirm(
            "High-Level Walkmode",
            lambda: call_control_mode(
                self.state,
                self.client,
                igc_sdk.ControlModeCommandType.CONTROL_MODE_CMD_HIGH_LEVEL_WALKMODE_ON,
                "HIGH_LEVEL_WALKMODE_ON",
            ),
        )

    def on_custom1(self) -> None:
        self.request_confirm(
            "Custom Mode 1",
            lambda: call_control_mode(
                self.state,
                self.client,
                igc_sdk.ControlModeCommandType.CONTROL_MODE_CMD_CUSTOM_MODE_1,
                "CUSTOM_MODE_1",
            ),
        )

    def on_custom2(self) -> None:
        self.request_confirm(
            "Custom Mode 2",
            lambda: call_control_mode(
                self.state,
                self.client,
                igc_sdk.ControlModeCommandType.CONTROL_MODE_CMD_CUSTOM_MODE_2,
                "CUSTOM_MODE_2",
            ),
        )

    def on_custom3(self) -> None:
        self.request_confirm(
            "Custom Mode 3",
            lambda: call_control_mode(
                self.state,
                self.client,
                igc_sdk.ControlModeCommandType.CONTROL_MODE_CMD_CUSTOM_MODE_3,
                "CUSTOM_MODE_3",
            ),
        )

    def on_motion_home(self) -> None:
        self.request_confirm(
            "Motion Preset: HOME",
            lambda: call_control_mode(
                self.state,
                self.client,
                igc_sdk.ControlModeCommandType.CONTROL_MODE_CMD_MOTION_PRESET,
                "MOTION_PRESET(HOME)",
                "HOME",
                False,
            ),
        )

    def on_mujoco_pause(self) -> None:
        self.request_confirm(
            "Mujoco Sim: Pause",
            lambda: call_mujoco_sim(
                self.state, self.client, igc_sdk.MujocoSimCommandType.MUJOCO_SIM_PAUSE, "PAUSE"
            ),
        )

    def on_mujoco_resume(self) -> None:
        self.request_confirm(
            "Mujoco Sim: Resume",
            lambda: call_mujoco_sim(
                self.state, self.client, igc_sdk.MujocoSimCommandType.MUJOCO_SIM_RESUME, "RESUME"
            ),
        )

    def on_mujoco_reload(self) -> None:
        self.request_confirm(
            "Mujoco Sim: Reload Model",
            lambda: call_mujoco_sim(
                self.state, self.client, igc_sdk.MujocoSimCommandType.MUJOCO_SIM_RELOAD, "RELOAD"
            ),
        )

    def on_mujoco_reset(self) -> None:
        self.request_confirm(
            "Mujoco Sim: Reset State",
            lambda: call_mujoco_sim(
                self.state, self.client, igc_sdk.MujocoSimCommandType.MUJOCO_SIM_RESET, "RESET"
            ),
        )


# -- Per-frame UI update -------------------------------------------------------


def _set_text_color(tag: str, color: tuple[int, int, int]) -> None:
    dpg.configure_item(tag, color=color)


def update_ui(state: AppState, channel: igc_sdk.ChannelFactory) -> None:
    with state.state_lock:
        snapshot = {
            "first": state.first_state_received,
            "lowstate_count": state.lowstate_count,
            "current_motor_pos": list(state.current_motor_pos),
            "current_joint_pos": list(state.current_joint_pos),
            "imu_q": list(state.imu_quaternion),
            "imu_g": list(state.imu_gyroscope),
            "imu_a": list(state.imu_accelerometer),
            "imu_rpy": list(state.imu_rpy),
            "robotstate_connected": state.robotstate_connected,
            "last_robotstate_time": state.last_robotstate_time,
            "robot_state_label": state.robot_state_label,
            "is_sim_env": state.is_sim_env,
            "lowlevel_active": state.lowlevel_active,
            "publish_count": state.lowcmd_publish_count,
            "target_motor_pos": list(state.target_motor_pos),
            "target_joint_pos": list(state.target_joint_pos),
            "parallel_space": list(state.parallel_space),
            "last_cmd_q": list(state.last_cmd_q),
            "last_cmd_tau": list(state.last_cmd_tau),
            "last_cmd_kp": list(state.last_cmd_kp),
            "last_cmd_kd": list(state.last_cmd_kd),
        }

    dpg.set_value(Tags.lowstate_count, f"LowState: {snapshot['lowstate_count']}")
    if snapshot["first"]:
        dpg.set_value(Tags.reference_status, "Reference initialized")
        _set_text_color(Tags.reference_status, COLOR_GREEN)
    else:
        dpg.set_value(Tags.reference_status, "Waiting for first state...")
        _set_text_color(Tags.reference_status, COLOR_YELLOW)

    dpg.set_value(Tags.dds_domain, f"Domain:    {channel.GetDomainId()}")
    ns = channel.GetNamespace() or "(none)"
    dpg.set_value(Tags.dds_namespace, f"Namespace: {ns}")
    now = time.monotonic()
    is_connected = (
        snapshot["robotstate_connected"]
        and (now - snapshot["last_robotstate_time"]) < ROBOTSTATE_TIMEOUT_S
    )
    if is_connected:
        dpg.set_value(Tags.dds_status, "Status: Connected")
        _set_text_color(Tags.dds_status, COLOR_GREEN)
        dpg.set_value(Tags.dds_robot_state, f"Robot State: {snapshot['robot_state_label']}")
        _set_text_color(Tags.dds_robot_state, COLOR_GREEN)
        env_label = "SIM" if snapshot["is_sim_env"] else "REAL"
        env_color = COLOR_SIM if snapshot["is_sim_env"] else COLOR_REAL
        dpg.set_value(Tags.dds_environment, f"Environment: {env_label}")
        _set_text_color(Tags.dds_environment, env_color)
        dpg.configure_item(Tags.mujoco_group, show=snapshot["is_sim_env"])
    else:
        dpg.set_value(Tags.dds_status, "Status: Disconnected")
        _set_text_color(Tags.dds_status, COLOR_RED)
        dpg.set_value(Tags.dds_robot_state, "Robot State: --")
        _set_text_color(Tags.dds_robot_state, COLOR_GREY)
        dpg.set_value(Tags.dds_environment, "Environment: --")
        _set_text_color(Tags.dds_environment, COLOR_GREY)
        dpg.configure_item(Tags.mujoco_group, show=False)

    if snapshot["first"]:
        q = snapshot["imu_q"]
        g = snapshot["imu_g"]
        a = snapshot["imu_a"]
        rpy = snapshot["imu_rpy"]
        dpg.set_value(
            Tags.imu_quaternion,
            f"  w: {q[0]:8.4f}   x: {q[1]:8.4f}   y: {q[2]:8.4f}   z: {q[3]:8.4f}",
        )
        dpg.set_value(Tags.imu_gyroscope, f"  x: {g[0]:8.4f}   y: {g[1]:8.4f}   z: {g[2]:8.4f}")
        dpg.set_value(Tags.imu_accelerometer, f"  x: {a[0]:8.4f}   y: {a[1]:8.4f}   z: {a[2]:8.4f}")
        dpg.set_value(Tags.imu_rpy, f"  R: {rpy[0]:8.4f}   P: {rpy[1]:8.4f}   Y: {rpy[2]:8.4f}")

    if snapshot["lowlevel_active"]:
        dpg.set_value(Tags.lowcmd_status, "Status: ACTIVE")
        _set_text_color(Tags.lowcmd_status, COLOR_GREEN)
        dpg.configure_item(Tags.publish_button, label="LowCmd Publish Stop")
        dpg.configure_item(Tags.lowcmd_inactive_hint, show=False)
        dpg.configure_item(Tags.lowcmd_tabbar, show=True)
    else:
        dpg.set_value(Tags.lowcmd_status, "Status: INACTIVE")
        _set_text_color(Tags.lowcmd_status, COLOR_RED)
        dpg.configure_item(Tags.publish_button, label="LowCmd Publish Start")
        dpg.configure_item(Tags.lowcmd_inactive_hint, show=True)
        dpg.configure_item(Tags.lowcmd_tabbar, show=False)
    dpg.set_value(Tags.lowcmd_count, f"Published: {snapshot['publish_count']} msgs")

    for i in range(NUM_MOTORS):
        link_idx = parallel_link_for_joint(i)
        use_motor = link_idx < 0 or snapshot["parallel_space"][link_idx] == SPACE_MOTOR
        if snapshot["lowlevel_active"]:
            value = (
                snapshot["target_motor_pos"][i] if use_motor else snapshot["target_joint_pos"][i]
            )
        else:
            value = (
                snapshot["current_motor_pos"][i] if use_motor else snapshot["current_joint_pos"][i]
            )
        if not dpg.is_item_active(Tags.slider(i)):
            dpg.set_value(Tags.slider(i), float(value))

        dpg.set_value(Tags.cmd_cell(i, "q"), f"{snapshot['last_cmd_q'][i]:7.3f}")
        dpg.set_value(Tags.cmd_cell(i, "tau"), f"{snapshot['last_cmd_tau'][i]:6.2f}")
        dpg.set_value(Tags.cmd_cell(i, "kp"), f"{snapshot['last_cmd_kp'][i]:5.1f}")
        dpg.set_value(Tags.cmd_cell(i, "kd"), f"{snapshot['last_cmd_kd'][i]:4.2f}")

        has_parallel = link_idx >= 0
        if has_parallel:
            space_tag = "MS" if snapshot["parallel_space"][link_idx] == SPACE_MOTOR else "PJS"
            display_name = (
                MOTOR_NAMES[i]
                if snapshot["parallel_space"][link_idx] == SPACE_MOTOR
                else JOINT_NAMES[i]
            )
        else:
            space_tag = "MS/PJS"
            display_name = JOINT_NAMES[i]
        cmd_q = snapshot["last_cmd_q"][i]
        state_q = (
            snapshot["current_motor_pos"][i] if use_motor else snapshot["current_joint_pos"][i]
        )
        diff = cmd_q - state_q
        dpg.set_value(Tags.diff_cell(i, "name"), display_name)
        dpg.set_value(Tags.diff_cell(i, "space"), space_tag)
        dpg.set_value(Tags.diff_cell(i, "cmd"), f"{cmd_q:7.3f}")
        dpg.set_value(Tags.diff_cell(i, "state"), f"{state_q:7.3f}")
        dpg.set_value(Tags.diff_cell(i, "diff"), f"{diff:+7.3f}")
        out_of_tol = abs(diff) > DIFF_TOL_RAD
        _set_text_color(Tags.diff_cell(i, "diff"), COLOR_RED if out_of_tol else COLOR_GREY)

    with state.log_lock:
        if state.log_dirty:
            dpg.set_value(Tags.log_text, "\n".join(state.log_lines))
            state.log_dirty = False


# -- Main ----------------------------------------------------------------------


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="IGRIS-C SDK GUI client (Python port of sdk_gui_client.cpp)"
    )
    parser.add_argument(
        "--domain-id", type=int, default=0, help="Cyclone DDS domain id (default: 0)"
    )
    parser.add_argument(
        "--namespace",
        type=str,
        default="",
        help='Topic namespace prefix (e.g. "robot1"). Empty string = no prefix.',
    )
    parser.add_argument(
        "--ui-scale",
        type=float,
        default=None,
        help=f"Override UI scale (clamped to [{MIN_UI_SCALE}, {MAX_UI_SCALE}]). Auto-detected from screen size by default.",
    )
    return parser


def _initialize_sdk(
    args: argparse.Namespace, state: AppState
) -> (
    tuple[
        igc_sdk.ChannelFactory,
        igc_sdk.IgrisC_Client,
        igc_sdk.LowStateSubscriber,
        igc_sdk.RobotStateSubscriber,
        igc_sdk.LowCmdPublisher,
    ]
    | None
):
    """Bring up DDS channel, service client, subscribers, and the LowCmd publisher.

    Returns the wired-up tuple, or None after printing/logging the failure
    cause. On a partial failure we tear down whatever we already initialized
    so the caller can just `return 1`.
    """
    channel = igc_sdk.ChannelFactory.Instance()
    channel.Init(args.domain_id, args.namespace)
    if not channel.IsInitialized():
        print("Failed to initialize ChannelFactory", file=sys.stderr)
        return None

    client = igc_sdk.IgrisC_Client()
    client.Init()
    client.SetTimeout(5.0)

    lowstate_sub = igc_sdk.LowStateSubscriber(LOWSTATE_TOPIC)
    if not lowstate_sub.init(make_lowstate_callback(state)):
        print("Failed to initialize LowState subscriber", file=sys.stderr)
        channel.Release()
        return None

    robotstate_sub = igc_sdk.RobotStateSubscriber(ROBOTSTATE_TOPIC)
    if not robotstate_sub.init(make_robotstate_callback(state)):
        state.add_log("Failed to subscribe to RobotState")

    lowcmd_pub = igc_sdk.LowCmdPublisher(LOWCMD_TOPIC)
    if not lowcmd_pub.init():
        print("Failed to initialize LowCmd publisher", file=sys.stderr)
        lowstate_sub.stop()
        robotstate_sub.stop()
        channel.Release()
        return None

    return channel, client, lowstate_sub, robotstate_sub, lowcmd_pub


def main() -> int:
    args = build_arg_parser().parse_args()

    print("IGRIS-SDK Dev Tool (Python)")
    print(f"  Domain ID: {args.domain_id}")
    print(f'  Namespace: "{args.namespace}"')

    screen_w, screen_h = detect_screen_size()
    if args.ui_scale is not None:
        _UI.scale = max(MIN_UI_SCALE, min(MAX_UI_SCALE, args.ui_scale))
    else:
        _UI.scale = compute_ui_scale(screen_w, screen_h)
    print(f"  Screen: {screen_w}x{screen_h}  UI scale: {_UI.scale:.2f}")

    # Resize floor — sized so the LowCmd slider Position column never
    # collapses to zero. Tied to _UI.scale so the floor grows with the fonts.
    viewport_min_w = int(LAYOUT_MIN_W_BASE * _UI.scale)
    viewport_min_h = int(LAYOUT_MIN_H_BASE * _UI.scale)
    # Open small. Width starts ~15% above the layout floor (taller for
    # vertical headroom) and never wider than 55% of the (often combined)
    # screen — opening at near-fullscreen on multi-monitor setups was the
    # main complaint.
    target_w = int(viewport_min_w * 1.15)
    target_h = int(viewport_min_h * 1.30)
    viewport_w = max(viewport_min_w, min(int(screen_w * 0.55), target_w))
    viewport_h = max(viewport_min_h, min(int(screen_h * 0.65), target_h))

    _build_robot_state_labels()

    state = AppState()
    stop_event = threading.Event()

    def handle_signal(_signum: int, _frame: object) -> None:
        stop_event.set()

    signal.signal(signal.SIGINT, handle_signal)
    signal.signal(signal.SIGTERM, handle_signal)

    sdk = _initialize_sdk(args, state)
    if sdk is None:
        return 1
    channel, client, lowstate_sub, robotstate_sub, lowcmd_pub = sdk

    publish_thread = threading.Thread(
        target=lowcmd_publish_loop,
        args=(state, lowcmd_pub, stop_event),
        name="lowcmd-pub",
        daemon=True,
    )
    publish_thread.start()

    state.add_log("GUI Client initialized")

    dpg.create_context()
    dpg.create_viewport(
        title="IGRIS-SDK Dev Tool  ·  AI-Powered",
        width=viewport_w,
        height=viewport_h,
        min_width=viewport_min_w,
        min_height=viewport_min_h,
    )
    # Bitmap-scale fonts to match the global UI scale. Crisp would need a TTF
    # at the target size; this is good enough for an example tool.
    dpg.set_global_font_scale(_UI.scale)

    ctx = GuiContext(state=state, client=client)

    # Panel widths are proportional to the *current* viewport so left:center
    # stays close to 1:1 even after the user resizes. Re-applied on every
    # viewport-resize event below.
    #
    # GROUP_GAP_W = ItemSpacing.x (default 8 px, not affected by UI_SCALE) ×
    # 2 gaps between 3 panels. Using S(20) here was the bug — at scale 2.5 it
    # left ~50 px slack while DPG actually drew only 16 px of inter-panel gap,
    # so the right panel ended ~34 px short of the Response Log strip.
    GROUP_GAP_W = 16

    def apply_panel_widths(vw: int) -> None:
        cw = int(vw * CENTER_PANEL_RATIO)
        rw = int(vw * RIGHT_PANEL_RATIO)
        lw_neg = -(cw + rw + GROUP_GAP_W)
        if dpg.does_item_exist(Tags.left_panel):
            dpg.configure_item(Tags.left_panel, width=lw_neg)
            dpg.configure_item(Tags.center_panel, width=cw)
            dpg.configure_item(Tags.right_panel, width=rw)

    initial_center_w = int(viewport_w * CENTER_PANEL_RATIO)
    initial_right_w = int(viewport_w * RIGHT_PANEL_RATIO)
    initial_left_neg_w = -(initial_center_w + initial_right_w + GROUP_GAP_W)

    with dpg.window(tag=Tags.main_window):
        with dpg.group(horizontal=True):
            with dpg.child_window(
                tag=Tags.left_panel, width=initial_left_neg_w, height=S(-180), border=True
            ):
                build_left_panel(ctx)
            with dpg.child_window(
                tag=Tags.center_panel, width=initial_center_w, height=S(-180), border=True
            ):
                build_center_panel(ctx)
            with dpg.child_window(
                tag=Tags.right_panel, width=initial_right_w, height=S(-180), border=True
            ):
                build_right_panel(ctx)
        build_log_panel()

    build_confirm_popup(state)
    install_modal_key_handlers(state)

    def on_viewport_resize(_sender: object = None, _data: object = None) -> None:
        apply_panel_widths(dpg.get_viewport_client_width())

    dpg.set_viewport_resize_callback(on_viewport_resize)

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.set_primary_window(Tags.main_window, True)

    try:
        while dpg.is_dearpygui_running() and not stop_event.is_set():
            update_ui(state, channel)
            dpg.render_dearpygui_frame()
    finally:
        stop_event.set()
        if publish_thread.is_alive():
            publish_thread.join(timeout=1.0)
        try:
            lowstate_sub.stop()
        except Exception:  # noqa: BLE001
            pass
        try:
            robotstate_sub.stop()
        except Exception:  # noqa: BLE001
            pass
        try:
            lowcmd_pub.stop()
        except Exception:  # noqa: BLE001
            pass
        channel.Release()
        dpg.destroy_context()
        print("GUI Client terminated")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
