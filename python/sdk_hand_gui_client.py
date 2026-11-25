#!/usr/bin/env python3
"""DearPyGui port of cyclonedds_hand_gui_client.cpp.

Mirrors the two-panel ImGui hand GUI: 12 hand motor sliders (auto-published
at ~20 Hz) on the left, and a live HandState table + Hand Init button +
response log on the right.

Uses the same igris_c_sdk Python API as hand_example.py:
  - igc_sdk.HandCmdPublisher / igc_sdk.HandStateSubscriber
  - igc_sdk.HandInitRequest_RequestPublisher / igc_sdk.HandInitRequest_ResponseSubscriber

Run with:
    python sdk_hand_gui_client.py --domain-id 0 --namespace robot1
"""

from __future__ import annotations

import argparse
import signal
import sys
import threading
import time
from collections import deque
from dataclasses import dataclass, field
from datetime import datetime
from typing import Callable

import dearpygui.dearpygui as dpg
from common import REDRAW, print_banner

import igris_c_sdk as igc_sdk

NUM_MOTORS = 12
HANDCMD_TOPIC = "handcmd"
HANDSTATE_TOPIC = "handstate"
HAND_INIT_REQUEST_TOPIC = "service/hand_init/request"
HAND_INIT_RESPONSE_TOPIC = "service/hand_init/response"

HANDCMD_PERIOD_S = 1.0 / 20.0  # 20 Hz auto-publish loop
MAX_LOG_LINES = 50

# Hand motor ids: right 11~16, left 21~26 (matches dxl_hand_controller order).
MOTOR_IDS: tuple[int, ...] = (11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 25, 26)
MOTOR_NAMES: tuple[str, ...] = (
    "R_Thumb",
    "R_Index",
    "R_Middle",
    "R_Ring",
    "R_Pinky",
    "R_Spread",
    "L_Thumb",
    "L_Index",
    "L_Middle",
    "L_Ring",
    "L_Pinky",
    "L_Spread",
)

COLOR_GREEN = (90, 220, 110)
COLOR_YELLOW = (240, 220, 80)
COLOR_HEADER = (155, 220, 255)
COLOR_RIGHT = (255, 205, 130)
COLOR_LEFT = (130, 205, 255)
COLOR_ORANGE = (255, 128, 0)
COLOR_DEFAULT = (255, 255, 255)


@dataclass
class AppState:
    state_lock: threading.Lock = field(default_factory=threading.Lock)
    log_lock: threading.Lock = field(default_factory=threading.Lock)
    log_lines: deque[str] = field(default_factory=lambda: deque(maxlen=MAX_LOG_LINES))
    log_dirty: bool = True

    handstate_count: int = 0
    first_state_received: bool = False
    motor_q: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    motor_dq: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    motor_tau: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    motor_temp: list[int] = field(default_factory=lambda: [0] * NUM_MOTORS)

    target_positions: list[float] = field(default_factory=lambda: [0.0] * NUM_MOTORS)
    auto_publish: bool = True
    handcmd_publish_count: int = 0
    initializing: bool = False

    def add_log(self, message: str) -> None:
        stamp = datetime.now().strftime("%H:%M:%S")
        with self.log_lock:
            self.log_lines.append(f"[{stamp}] {message}")
            self.log_dirty = True


def make_handstate_callback(state: AppState) -> Callable[[object], None]:
    def callback(msg: object) -> None:
        motor_state = msg.motor_state()
        q = [float(motor_state[i].q()) for i in range(NUM_MOTORS)]
        dq = [float(motor_state[i].dq()) for i in range(NUM_MOTORS)]
        tau = [float(motor_state[i].tau_est()) for i in range(NUM_MOTORS)]
        temp = [int(motor_state[i].temperature()) for i in range(NUM_MOTORS)]

        first_seen = False
        with state.state_lock:
            state.handstate_count += 1
            state.motor_q = q
            state.motor_dq = dq
            state.motor_tau = tau
            state.motor_temp = temp
            if not state.first_state_received:
                state.first_state_received = True
                first_seen = True

        if first_seen:
            state.add_log("First HandState received")

    return callback


def make_handcmd(state: AppState) -> "igc_sdk.HandCmd":
    with state.state_lock:
        targets = list(state.target_positions)

    motors = []
    for mid, q in zip(MOTOR_IDS, targets):
        m = igc_sdk.MotorCmd_()
        m.id(mid)
        m.q(max(0.0, min(1.0, q)))
        m.dq(0.0)
        m.tau(0.0)
        m.kp(0.0)
        m.kd(0.0)
        motors.append(m)

    cmd = igc_sdk.HandCmd_()
    cmd.motor_cmd(motors)
    return cmd


def handcmd_publish_loop(
    state: AppState,
    publisher: "igc_sdk.HandCmdPublisher",
    stop_event: threading.Event,
) -> None:
    next_time = time.monotonic()
    while not stop_event.is_set():
        next_time += HANDCMD_PERIOD_S
        sleep_s = next_time - time.monotonic()
        if sleep_s > 0:
            stop_event.wait(timeout=sleep_s)
            if stop_event.is_set():
                break
        else:
            next_time = time.monotonic()

        with state.state_lock:
            do_publish = state.auto_publish
        if not do_publish:
            continue

        publisher.write(make_handcmd(state))
        with state.state_lock:
            state.handcmd_publish_count += 1


def set_home_pose(state: AppState) -> None:
    with state.state_lock:
        state.target_positions = [0.0] * NUM_MOTORS
    state.add_log("Home pose set (all 0.0)")


def set_closed_pose(state: AppState) -> None:
    with state.state_lock:
        state.target_positions = [0.6] * NUM_MOTORS
    state.add_log("Closed pose set (all 0.6)")


def set_hand_only(state: AppState, start: int, end: int, value: float, label: str) -> None:
    with state.state_lock:
        for i in range(start, end):
            state.target_positions[i] = value
    state.add_log(f"{label} hand set to {value}")


def make_hand_init_response_callback(state: AppState) -> Callable[[object], None]:
    def callback(res: object) -> None:
        result = "OK" if res.success() else "FAIL"
        state.add_log(f"HandInit response [{res.request_id()}] {result}: {res.message()}")

    return callback


def send_hand_init_request(
    state: AppState, publisher: "igc_sdk.HandInitRequest_RequestPublisher"
) -> None:
    with state.state_lock:
        if state.initializing:
            state.add_log("HandInit already in progress")
            return
        state.auto_publish = False
        state.initializing = True

    state.add_log("Stopped publishing for init...")
    time.sleep(0.1)  # ensure no commands in flight

    req = igc_sdk.HandInitRequest_Request_()
    req.request_id("gui_hand_init")

    if publisher.write(req):
        state.add_log("HandInit request sent")
        state.add_log("Wait for init to complete, then click 'Resume Publishing'")
    else:
        state.add_log("Failed to send HandInit request")
        with state.state_lock:
            state.initializing = False
            state.auto_publish = True


def resume_publishing(state: AppState) -> None:
    with state.state_lock:
        state.initializing = False
        state.auto_publish = True
    state.add_log("Publishing resumed")


class Tags:
    main_window = "main_window"
    left_panel = "left_panel"
    right_panel = "right_panel"
    log_text = "log_text"

    handstate_count = "handstate_count"
    connection_status = "connection_status"
    publish_status = "publish_status"
    init_button = "init_button"
    resume_button = "resume_button"
    pause_init_label = "pause_init_label"

    @staticmethod
    def slider(i: int) -> str:
        return f"slider_{i}"

    @staticmethod
    def state_cell(i: int, col: str) -> str:
        return f"state_{col}_{i}"


@dataclass
class GuiContext:
    state: AppState
    handcmd_pub: "igc_sdk.HandCmdPublisher"
    hand_init_req_pub: "igc_sdk.HandInitRequest_RequestPublisher"

    def make_slider_callback(self, idx: int) -> Callable[[object, object, object], None]:
        def callback(_sender: object, value: float, _user_data: object) -> None:
            with self.state.state_lock:
                self.state.target_positions[idx] = float(value)

        return callback

    def on_reset_all(self) -> None:
        set_home_pose(self.state)
        for i in range(NUM_MOTORS):
            dpg.set_value(Tags.slider(i), 0.0)

    def on_closed_pose(self) -> None:
        set_closed_pose(self.state)
        for i in range(NUM_MOTORS):
            dpg.set_value(Tags.slider(i), 0.6)

    def on_left_open(self) -> None:
        set_hand_only(self.state, 6, 12, 0.0, "Left")
        for i in range(6, 12):
            dpg.set_value(Tags.slider(i), 0.0)

    def on_left_close(self) -> None:
        set_hand_only(self.state, 6, 12, 0.6, "Left")
        for i in range(6, 12):
            dpg.set_value(Tags.slider(i), 0.6)

    def on_right_open(self) -> None:
        set_hand_only(self.state, 0, 6, 0.0, "Right")
        for i in range(6):
            dpg.set_value(Tags.slider(i), 0.0)

    def on_right_close(self) -> None:
        set_hand_only(self.state, 0, 6, 0.6, "Right")
        for i in range(6):
            dpg.set_value(Tags.slider(i), 0.6)

    def on_hand_init(self) -> None:
        threading.Thread(
            target=send_hand_init_request,
            args=(self.state, self.hand_init_req_pub),
            name="hand-init",
            daemon=True,
        ).start()

    def on_resume(self) -> None:
        resume_publishing(self.state)


def build_left_panel(ctx: GuiContext) -> None:
    with dpg.child_window(tag=Tags.left_panel, border=True, width=420, height=-1):
        dpg.add_text("Hand Motor Targets (Normalized 0.0 ~ 1.0)", color=COLOR_HEADER)
        dpg.add_separator()
        dpg.add_text("HandState messages: 0", tag=Tags.handstate_count)
        dpg.add_text("Waiting for state...", tag=Tags.connection_status, color=COLOR_YELLOW)
        dpg.add_separator()

        dpg.add_text("Right Hand (ID 11-16)", color=COLOR_RIGHT)
        for i in range(6):
            dpg.add_slider_float(
                label=f"ID {MOTOR_IDS[i]}: {MOTOR_NAMES[i]}",
                tag=Tags.slider(i),
                default_value=0.0,
                min_value=0.0,
                max_value=1.0,
                format="%.2f",
                callback=ctx.make_slider_callback(i),
            )

        dpg.add_separator()
        dpg.add_text("Left Hand (ID 21-26)", color=COLOR_LEFT)
        for i in range(6, 12):
            dpg.add_slider_float(
                label=f"ID {MOTOR_IDS[i]}: {MOTOR_NAMES[i]}",
                tag=Tags.slider(i),
                default_value=0.0,
                min_value=0.0,
                max_value=1.0,
                format="%.2f",
                callback=ctx.make_slider_callback(i),
            )


def build_right_panel(ctx: GuiContext) -> None:
    with dpg.child_window(tag=Tags.right_panel, border=True, width=-1, height=-1):
        dpg.add_text("Commands", color=COLOR_HEADER)
        dpg.add_separator()

        dpg.add_button(label="Reset All (0.0)", width=-1, height=32, callback=ctx.on_reset_all)
        dpg.add_button(label="Closed Pose (0.6)", width=-1, height=32, callback=ctx.on_closed_pose)
        dpg.add_separator()

        dpg.add_button(
            label="Initialize Hand (Calibrate)",
            tag=Tags.init_button,
            width=-1,
            height=36,
            callback=ctx.on_hand_init,
        )
        dpg.add_text(
            "Publishing PAUSED for init", tag=Tags.pause_init_label, color=COLOR_YELLOW, show=False
        )
        dpg.add_button(
            label="Resume Publishing",
            tag=Tags.resume_button,
            width=-1,
            height=32,
            show=False,
            callback=ctx.on_resume,
        )
        dpg.add_separator()

        dpg.add_text("Left Hand Only:")
        with dpg.group(horizontal=True):
            dpg.add_button(label="L: Open", width=100, callback=ctx.on_left_open)
            dpg.add_button(label="L: Close", width=100, callback=ctx.on_left_close)

        dpg.add_text("Right Hand Only:")
        with dpg.group(horizontal=True):
            dpg.add_button(label="R: Open", width=100, callback=ctx.on_right_open)
            dpg.add_button(label="R: Close", width=100, callback=ctx.on_right_close)
        dpg.add_separator()

        dpg.add_text("Publishing at 20Hz | Commands: 0", tag=Tags.publish_status)
        dpg.add_separator()

        dpg.add_text("Motor State (from HandState)", color=COLOR_HEADER)
        dpg.add_separator()
        with dpg.table(
            header_row=True,
            policy=dpg.mvTable_SizingStretchProp,
            resizable=False,
            scrollY=True,
            height=220,
            borders_innerH=True,
            borders_outerH=True,
            borders_innerV=True,
            borders_outerV=True,
        ):
            dpg.add_table_column(label="#", width_fixed=True, init_width_or_weight=30)
            dpg.add_table_column(label="q")
            dpg.add_table_column(label="dq")
            dpg.add_table_column(label="tau")
            dpg.add_table_column(label="temp")
            for i in range(NUM_MOTORS):
                with dpg.table_row():
                    dpg.add_text(str(i))
                    dpg.add_text("0.000", tag=Tags.state_cell(i, "q"))
                    dpg.add_text("0.000", tag=Tags.state_cell(i, "dq"))
                    dpg.add_text("0.00", tag=Tags.state_cell(i, "tau"))
                    dpg.add_text("0", tag=Tags.state_cell(i, "temp"))

        dpg.add_separator()
        dpg.add_text("Log", color=COLOR_HEADER)
        dpg.add_separator()
        with dpg.child_window(border=False, width=-1, height=-1):
            dpg.add_text("", tag=Tags.log_text)


def update_ui(state: AppState) -> None:
    with state.state_lock:
        count = state.handstate_count
        first_seen = state.first_state_received
        q = list(state.motor_q)
        dq = list(state.motor_dq)
        tau = list(state.motor_tau)
        temp = list(state.motor_temp)
        auto_publish = state.auto_publish
        publish_count = state.handcmd_publish_count
        initializing = state.initializing

    dpg.set_value(Tags.handstate_count, f"HandState messages: {count}")
    if first_seen:
        dpg.set_value(Tags.connection_status, "Connected")
        dpg.configure_item(Tags.connection_status, color=COLOR_GREEN)
        for i in range(NUM_MOTORS):
            dpg.set_value(Tags.state_cell(i, "q"), f"{q[i]:.3f}")
            dpg.set_value(Tags.state_cell(i, "dq"), f"{dq[i]:.3f}")
            dpg.set_value(Tags.state_cell(i, "tau"), f"{tau[i]:.2f}")
            dpg.set_value(Tags.state_cell(i, "temp"), str(temp[i]))

    dpg.configure_item(Tags.init_button, enabled=not initializing)
    dpg.configure_item(Tags.resume_button, show=initializing)
    dpg.configure_item(Tags.pause_init_label, show=initializing)
    if initializing:
        dpg.set_value(Tags.publish_status, f"Publishing PAUSED | Commands: {publish_count}")
        dpg.configure_item(Tags.publish_status, color=COLOR_ORANGE)
    else:
        status = "20Hz" if auto_publish else "PAUSED"
        dpg.set_value(Tags.publish_status, f"Publishing at {status} | Commands: {publish_count}")
        dpg.configure_item(Tags.publish_status, color=COLOR_DEFAULT)

    with state.log_lock:
        if state.log_dirty:
            dpg.set_value(Tags.log_text, "\n".join(state.log_lines))
            state.log_dirty = False


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="IGRIS-C SDK hand GUI client (Python port of cyclonedds_hand_gui_client.cpp)"
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
        "--dds-topic-naming",
        choices=igc_sdk.topic_naming_names(),
        default="native",
        help="Wire layout for topic names; must match the robot's igris_c.network.transport "
        "(native for cyclonedds_native, ros_compatible for cyclonedds_ros_compatible).",
    )
    return parser


def main() -> int:
    args = build_arg_parser().parse_args()

    print_banner("Hand GUI Client", args.domain_id, args.namespace, args.dds_topic_naming)
    print("Make sure the hand controller node is running!\n")

    state = AppState()
    stop_event = threading.Event()

    def handle_signal(_signum: int, _frame: object) -> None:
        stop_event.set()

    signal.signal(signal.SIGINT, handle_signal)
    signal.signal(signal.SIGTERM, handle_signal)

    channel = igc_sdk.ChannelFactory.Instance()
    channel.Init(
        args.domain_id, args.namespace, "", igc_sdk.parse_topic_naming(args.dds_topic_naming)
    )
    if not channel.IsInitialized():
        print("Failed to initialize ChannelFactory", file=sys.stderr)
        return 1

    handstate_sub = igc_sdk.HandStateSubscriber(HANDSTATE_TOPIC, igc_sdk.QosProfile.SensorData())
    if not handstate_sub.init(make_handstate_callback(state)):
        print("Failed to initialize HandState subscriber", file=sys.stderr)
        channel.Release()
        return 1

    handcmd_pub = igc_sdk.HandCmdPublisher(HANDCMD_TOPIC, igc_sdk.QosProfile.SensorData())
    if not handcmd_pub.init():
        print("Failed to initialize HandCmd publisher", file=sys.stderr)
        handstate_sub.stop()
        channel.Release()
        return 1

    hand_init_req_pub = igc_sdk.HandInitRequest_RequestPublisher(
        HAND_INIT_REQUEST_TOPIC, igc_sdk.QosProfile.Services()
    )
    if not hand_init_req_pub.init():
        print("Failed to initialize HandInit request publisher", file=sys.stderr)
        handcmd_pub.stop()
        handstate_sub.stop()
        channel.Release()
        return 1

    hand_init_res_sub = igc_sdk.HandInitRequest_ResponseSubscriber(
        HAND_INIT_RESPONSE_TOPIC, igc_sdk.QosProfile.Services()
    )
    if not hand_init_res_sub.init(make_hand_init_response_callback(state)):
        print("Failed to initialize HandInit response subscriber", file=sys.stderr)
        hand_init_req_pub.stop()
        handcmd_pub.stop()
        handstate_sub.stop()
        channel.Release()
        return 1

    publish_thread = threading.Thread(
        target=handcmd_publish_loop,
        args=(state, handcmd_pub, stop_event),
        name="handcmd-pub",
        daemon=True,
    )
    publish_thread.start()

    state.add_log("Hand GUI Client initialized successfully")

    dpg.create_context()
    dpg.create_viewport(
        title="IGRIS-SDK Hand GUI Client", width=1200, height=700, min_width=900, min_height=550
    )
    ctx = GuiContext(state=state, handcmd_pub=handcmd_pub, hand_init_req_pub=hand_init_req_pub)

    with dpg.window(tag=Tags.main_window):
        with dpg.group(horizontal=True):
            build_left_panel(ctx)
            build_right_panel(ctx)

    # Disabled-state theme so the init button visibly greys out while a hand init is
    # in progress. DearPyGui does not dim enabled=False items on its own (unlike the
    # ImGui BeginDisabled path used by the cyclonedds GUI), so bind an explicit theme.
    with dpg.theme() as init_disabled_theme:
        with dpg.theme_component(dpg.mvButton, enabled_state=False):
            dpg.add_theme_color(dpg.mvThemeCol_Button, (45, 45, 45))
            dpg.add_theme_color(dpg.mvThemeCol_ButtonHovered, (45, 45, 45))
            dpg.add_theme_color(dpg.mvThemeCol_ButtonActive, (45, 45, 45))
            dpg.add_theme_color(dpg.mvThemeCol_Text, (110, 110, 110))
    dpg.bind_item_theme(Tags.init_button, init_disabled_theme)

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.set_primary_window(Tags.main_window, True)

    try:
        while dpg.is_dearpygui_running() and not stop_event.is_set():
            update_ui(state)
            dpg.render_dearpygui_frame()
    finally:
        stop_event.set()
        if publish_thread.is_alive():
            publish_thread.join(timeout=1.0)
        try:
            hand_init_res_sub.stop()
        except Exception:  # noqa: BLE001
            pass
        try:
            hand_init_req_pub.stop()
        except Exception:  # noqa: BLE001
            pass
        try:
            handcmd_pub.stop()
        except Exception:  # noqa: BLE001
            pass
        try:
            handstate_sub.stop()
        except Exception:  # noqa: BLE001
            pass
        channel.Release()
        dpg.destroy_context()
        print("Hand GUI Client terminated")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
