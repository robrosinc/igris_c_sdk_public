#!/usr/bin/env python3
"""Interactive menu-driven service-API + LowCmd publishing example.

Demonstrates how to drive the IGRIS-C SDK from Python:
- ChannelFactory init and teardown
- LowState subscription with thread-safe callback state
- LowCmd one-shot publishing
- Service API (PDU, torque, control mode, Mujoco sim) via IgrisC_Client

Run with:
    python client.py --domain-id 0 --namespace robot1
"""

from __future__ import annotations

import argparse
import signal
import sys
import threading
import time
from dataclasses import dataclass, field

from common import REDRAW, print_banner

import igris_c_sdk as igc_sdk

DEFAULT_DOMAIN_ID = 0
SERVICE_TIMEOUT_MS = 5000
LOWSTATE_TOPIC = "lowstate"
LOWCMD_TOPIC = "lowcmd"
NUM_MOTORS = 31


@dataclass
class LowStateMonitor:
    lock: threading.Lock = field(default_factory=threading.Lock)
    count: int = 0

    def callback(self, msg: object) -> None:
        # We just track that the subscription is alive; no per-message logging
        # so the menu prompt isn't drowned out.
        with self.lock:
            self.count += 1
        del msg


def call_init_pdu(client: igc_sdk.IgrisC_Client, init_type: int, label: str) -> None:
    print(f"Calling InitPdu({label})...")
    res = client.InitPdu(init_type, SERVICE_TIMEOUT_MS)
    _print_result(f"InitPdu({label})", res)


def call_set_torque(client: igc_sdk.IgrisC_Client, torque_type: int, label: str) -> None:
    print(f"Calling SetTorque({label})...")
    res = client.SetTorque(torque_type, SERVICE_TIMEOUT_MS)
    _print_result(f"SetTorque({label})", res)


def call_control_mode(
    client: igc_sdk.IgrisC_Client,
    cmd_type: int,
    label: str,
    preset_id: str = "",
    is_cyclic: bool = False,
) -> None:
    print(f"Calling SendControlModeCommand({label})...")
    res = client.SendControlModeCommand(cmd_type, preset_id, is_cyclic, SERVICE_TIMEOUT_MS)
    _print_result(f"ControlModeCommand({label})", res)


def call_init_hand(client: igc_sdk.IgrisC_Client) -> None:
    print("Calling InitHand()...")
    res = client.InitHand(SERVICE_TIMEOUT_MS)
    _print_result("InitHand", res)


def call_mujoco_sim(client: igc_sdk.IgrisC_Client, cmd_type: int, label: str) -> None:
    print(f"Calling SendMujocoSimCmd({label})...")
    res = client.SendMujocoSimCmd(cmd_type, SERVICE_TIMEOUT_MS)
    _print_result(f"MujocoSimCmd({label})", res)


def _print_result(label: str, response: object) -> None:
    if response.success():
        print(f"  -> {label}: SUCCESS")
    else:
        print(f"  -> {label}: FAILED ({response.message()})")


def publish_zero_lowcmd(publisher: igc_sdk.LowCmdPublisher) -> None:
    cmd = igc_sdk.LowCmd_()
    motors = cmd.motors()
    for i in range(NUM_MOTORS):
        motor = motors[i]
        motor.id(i)
        motor.q(0.0)
        motor.dq(0.0)
        motor.tau(0.0)
        motor.kp(0.0)
        motor.kd(0.0)
    publisher.write(cmd)
    print("Published LowCmd (all zeros, kp=kd=0).")


MENU = """
Select an action:
  Power & Torque
   1. Init PDU
   2. Init Motor
   3. Init PDU and Motor
   4. PDU OFF
   5. Torque ON
   6. Torque OFF
   7. Init Hand
  Motion Presets
  10. Motion Preset: HOME
  11. Motion Preset: HI_WAVE
  Control Mode
  12. Joint Position Hold
  13. Motion Stop
  14. Walkmode ON
  15. Low Level Joint Control
  16. Low Level Walkmode ON
  17. High Level Joint Control
  18. High Level Walkmode ON
  Custom Modes
  19. Custom Mode 1
  20. Custom Mode 2
  21. Custom Mode 3
  22. Custom Mode 4
  23. Custom Mode 5
  24. Custom Mode 6
  25. Custom Mode 7
  26. Custom Mode 8
  27. Custom Mode 9
  28. Custom Mode 10
  Mujoco Sim
  30. Mujoco Sim Pause
  31. Mujoco Sim Resume
  32. Mujoco Sim Reload
  33. Mujoco Sim Reset
  Publishing
  40. Publish a single LowCmd (all-zero, gains=0)
   0. Exit
"""


# Map menu option -> (label, kwargs for call_control_mode).
_CONTROL_MODE_DISPATCH: dict[str, tuple[str, dict]] = {
    "10": (
        "MOTION_PRESET(HOME)",
        {"cmd_type_name": "CONTROL_MODE_CMD_MOTION_PRESET", "preset_id": "HOME"},
    ),
    "11": (
        "MOTION_PRESET(HI_WAVE)",
        {"cmd_type_name": "CONTROL_MODE_CMD_MOTION_PRESET", "preset_id": "HI_WAVE"},
    ),
    "12": (
        "JOINT_POSITION_HOLD",
        {"cmd_type_name": "CONTROL_MODE_CMD_JOINT_POSITION_HOLD"},
    ),
    "13": ("MOTION_STOP", {"cmd_type_name": "CONTROL_MODE_CMD_MOTION_STOP"}),
    "14": ("WALKMODE_ON", {"cmd_type_name": "CONTROL_MODE_CMD_WALKMODE_ON"}),
    "15": (
        "LOW_LEVEL_JOINT_CONTROL",
        {"cmd_type_name": "CONTROL_MODE_CMD_LOW_LEVEL_JOINT_CONTROL"},
    ),
    "16": (
        "LOW_LEVEL_WALKMODE_ON",
        {"cmd_type_name": "CONTROL_MODE_CMD_LOW_LEVEL_WALKMODE_ON"},
    ),
    "17": (
        "HIGH_LEVEL_JOINT_CONTROL",
        {"cmd_type_name": "CONTROL_MODE_CMD_HIGH_LEVEL_JOINT_CONTROL"},
    ),
    "18": (
        "HIGH_LEVEL_WALKMODE_ON",
        {"cmd_type_name": "CONTROL_MODE_CMD_HIGH_LEVEL_WALKMODE_ON"},
    ),
    "19": ("CUSTOM_MODE_1", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_1"}),
    "20": ("CUSTOM_MODE_2", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_2"}),
    "21": ("CUSTOM_MODE_3", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_3"}),
    "22": ("CUSTOM_MODE_4", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_4"}),
    "23": ("CUSTOM_MODE_5", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_5"}),
    "24": ("CUSTOM_MODE_6", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_6"}),
    "25": ("CUSTOM_MODE_7", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_7"}),
    "26": ("CUSTOM_MODE_8", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_8"}),
    "27": ("CUSTOM_MODE_9", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_9"}),
    "28": ("CUSTOM_MODE_10", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_10"}),
}

# Mujoco sim option -> (label, command constant). The constants are referenced directly rather
# than by name through getattr(): test/check_python_example_symbols.py only sees literal
# igc_sdk.<NAME> access, so a name held as a string would escape the symbol check and surface as
# an AttributeError on a live robot instead.
_MUJOCO_SIM_DISPATCH: dict[str, tuple[str, int]] = {
    "30": ("MUJOCO_SIM_PAUSE", igc_sdk.MUJOCO_SIM_PAUSE),
    "31": ("MUJOCO_SIM_RESUME", igc_sdk.MUJOCO_SIM_RESUME),
    "32": ("MUJOCO_SIM_RELOAD", igc_sdk.MUJOCO_SIM_RELOAD),
    "33": ("MUJOCO_SIM_RESET", igc_sdk.MUJOCO_SIM_RESET),
}


def _dispatch_control_mode(
    client: igc_sdk.IgrisC_Client, label: str, cmd_type_name: str, preset_id: str = ""
) -> None:
    cmd_type = getattr(igc_sdk, cmd_type_name)
    call_control_mode(client, cmd_type, label, preset_id=preset_id)


def dispatch(
    choice: str, client: igc_sdk.IgrisC_Client, lowcmd_pub: igc_sdk.LowCmdPublisher
) -> bool:
    if choice == "1":
        call_init_pdu(client, igc_sdk.PDU_INIT, "PDU_INIT")
    elif choice == "2":
        call_init_pdu(client, igc_sdk.MOTOR_INIT, "MOTOR_INIT")
    elif choice == "3":
        call_init_pdu(client, igc_sdk.PDU_AND_MOTOR_INIT, "PDU_AND_MOTOR_INIT")
    elif choice == "4":
        call_init_pdu(client, igc_sdk.PDU_OFF, "PDU_OFF")
    elif choice == "5":
        call_set_torque(client, igc_sdk.TORQUE_ON, "TORQUE_ON")
    elif choice == "6":
        call_set_torque(client, igc_sdk.TORQUE_OFF, "TORQUE_OFF")
    elif choice == "7":
        call_init_hand(client)
    elif choice in _CONTROL_MODE_DISPATCH:
        label, kwargs = _CONTROL_MODE_DISPATCH[choice]
        _dispatch_control_mode(client, label, **kwargs)
    elif choice in _MUJOCO_SIM_DISPATCH:
        label, cmd_type = _MUJOCO_SIM_DISPATCH[choice]
        call_mujoco_sim(client, cmd_type, label)
    elif choice == "40":
        publish_zero_lowcmd(lowcmd_pub)
    elif choice in ("q", "Q", "0", "exit", "quit"):
        return False
    else:
        print(f"Unknown choice: {choice!r}")
    return True


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="IGRIS-C SDK service-API menu client")
    parser.add_argument(
        "--domain-id",
        type=int,
        default=DEFAULT_DOMAIN_ID,
        help=f"Cyclone DDS domain id (default: {DEFAULT_DOMAIN_ID})",
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
    print(f"Domain ID: {args.domain_id}")
    print(f'Namespace: "{args.namespace}"')

    stop_event = threading.Event()

    def handle_signal(_signum: int, _frame: object) -> None:
        stop_event.set()

    signal.signal(signal.SIGINT, handle_signal)
    signal.signal(signal.SIGTERM, handle_signal)

    print_banner("Service + LowCmd Client", args.domain_id, args.namespace, args.dds_topic_naming)

    channel = igc_sdk.ChannelFactory.Instance()
    channel.Init(
        args.domain_id, args.namespace, "", igc_sdk.parse_topic_naming(args.dds_topic_naming)
    )
    if not channel.IsInitialized():
        print("Failed to initialize ChannelFactory", file=sys.stderr)
        return 1

    client = igc_sdk.IgrisC_Client()
    client.Init()
    client.SetTimeout(5.0)

    monitor = LowStateMonitor()
    lowstate_sub = igc_sdk.LowStateSubscriber(LOWSTATE_TOPIC, igc_sdk.QosProfile.SensorData())
    if not lowstate_sub.init(monitor.callback):
        print("Failed to initialize LowStateSubscriber", file=sys.stderr)
        channel.Release()
        return 1

    lowcmd_pub = igc_sdk.LowCmdPublisher(LOWCMD_TOPIC, igc_sdk.QosProfile.SensorData())
    if not lowcmd_pub.init():
        print("Failed to initialize LowCmdPublisher", file=sys.stderr)
        lowstate_sub.stop()
        channel.Release()
        return 1

    # Give the subscriber a moment to connect before the first menu.
    time.sleep(0.3)

    try:
        while not stop_event.is_set():
            print(MENU)
            try:
                choice = input("Enter choice: ").strip()
            except EOFError:
                break

            if not dispatch(choice, client, lowcmd_pub):
                break
    finally:
        print("Shutting down...")
        lowstate_sub.stop()
        lowcmd_pub.stop()
        channel.Release()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
