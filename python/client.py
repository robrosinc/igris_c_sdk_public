#!/usr/bin/env python3
"""Interactive menu-driven service-API + LowCmd publishing example.

Demonstrates how to drive the IGRIS-C SDK from Python:
- ChannelFactory init and teardown
- LowState subscription with thread-safe callback state
- LowCmd one-shot publishing
- Service API (BMS, torque, control mode, Mujoco sim) via IgrisC_Client

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

import igris_c_sdk as igc_sdk

DEFAULT_DOMAIN_ID = 0
SERVICE_TIMEOUT_MS = 5000
LOWSTATE_TOPIC = "rt/lowstate"
LOWCMD_TOPIC = "rt/lowcmd"
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


def call_init_bms(
    client: igc_sdk.IgrisC_Client, init_type: igc_sdk.BmsInitType, label: str
) -> None:
    print(f"Calling InitBms({label})...")
    res = client.InitBms(init_type, SERVICE_TIMEOUT_MS)
    _print_result(f"InitBms({label})", res)


def call_set_torque(
    client: igc_sdk.IgrisC_Client, torque_type: igc_sdk.TorqueType, label: str
) -> None:
    print(f"Calling SetTorque({label})...")
    res = client.SetTorque(torque_type, SERVICE_TIMEOUT_MS)
    _print_result(f"SetTorque({label})", res)


def call_control_mode(
    client: igc_sdk.IgrisC_Client,
    cmd_type: igc_sdk.ControlModeCommandType,
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


def _print_result(label: str, response: object) -> None:
    if response.success():
        print(f"  -> {label}: SUCCESS")
    else:
        print(f"  -> {label}: FAILED ({response.message()})")


def publish_zero_lowcmd(publisher: igc_sdk.LowCmdPublisher) -> None:
    cmd = igc_sdk.LowCmd()
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
   1. Init BMS
   2. Init Motor
   3. Init BMS + Motor
   4. BMS OFF
   5. Init Hand
   6. Torque ON
   7. Torque OFF
  Control Mode
   8. High-Level
   9. Low-Level
  10. Joint Position Hold
  11. Motion Stop
  12. Walkmode
  13. Low-Level Walkmode
  14. High-Level Walkmode
  Custom Modes
  15. Custom Mode 1
  16. Custom Mode 2
  17. Custom Mode 3
  Motion Presets
  18. HOME
  Publishing
  19. Publish a single LowCmd (all-zero, gains=0)
   q. Quit
"""

# Map menu option -> (label, kwargs for call_control_mode).
_CONTROL_MODE_DISPATCH: dict[str, tuple[str, dict]] = {
    "8": (
        "HIGH_LEVEL_JOINT_CONTROL",
        {"cmd_type_name": "CONTROL_MODE_CMD_HIGH_LEVEL_JOINT_CONTROL"},
    ),
    "9": (
        "LOW_LEVEL_JOINT_CONTROL",
        {"cmd_type_name": "CONTROL_MODE_CMD_LOW_LEVEL_JOINT_CONTROL"},
    ),
    "10": (
        "JOINT_POSITION_HOLD",
        {"cmd_type_name": "CONTROL_MODE_CMD_JOINT_POSITION_HOLD"},
    ),
    "11": ("MOTION_STOP", {"cmd_type_name": "CONTROL_MODE_CMD_MOTION_STOP"}),
    "12": ("WALKMODE_ON", {"cmd_type_name": "CONTROL_MODE_CMD_WALKMODE_ON"}),
    "13": (
        "LOW_LEVEL_WALKMODE_ON",
        {"cmd_type_name": "CONTROL_MODE_CMD_LOW_LEVEL_WALKMODE_ON"},
    ),
    "14": (
        "HIGH_LEVEL_WALKMODE_ON",
        {"cmd_type_name": "CONTROL_MODE_CMD_HIGH_LEVEL_WALKMODE_ON"},
    ),
    "15": ("CUSTOM_MODE_1", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_1"}),
    "16": ("CUSTOM_MODE_2", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_2"}),
    "17": ("CUSTOM_MODE_3", {"cmd_type_name": "CONTROL_MODE_CMD_CUSTOM_MODE_3"}),
    "18": (
        "MOTION_PRESET(HOME)",
        {"cmd_type_name": "CONTROL_MODE_CMD_MOTION_PRESET", "preset_id": "HOME"},
    ),
}


def _dispatch_control_mode(
    client: igc_sdk.IgrisC_Client, label: str, cmd_type_name: str, preset_id: str = ""
) -> None:
    cmd_type = getattr(igc_sdk.ControlModeCommandType, cmd_type_name)
    call_control_mode(client, cmd_type, label, preset_id=preset_id)


def dispatch(
    choice: str, client: igc_sdk.IgrisC_Client, lowcmd_pub: igc_sdk.LowCmdPublisher
) -> bool:
    if choice == "1":
        call_init_bms(client, igc_sdk.BmsInitType.BMS_INIT, "BMS_INIT")
    elif choice == "2":
        call_init_bms(client, igc_sdk.BmsInitType.MOTOR_INIT, "MOTOR_INIT")
    elif choice == "3":
        call_init_bms(client, igc_sdk.BmsInitType.BMS_AND_MOTOR_INIT, "BMS_AND_MOTOR_INIT")
    elif choice == "4":
        call_init_bms(client, igc_sdk.BmsInitType.BMS_INIT_NONE, "BMS_OFF")
    elif choice == "5":
        call_init_hand(client)
    elif choice == "6":
        call_set_torque(client, igc_sdk.TorqueType.TORQUE_ON, "TORQUE_ON")
    elif choice == "7":
        call_set_torque(client, igc_sdk.TorqueType.TORQUE_OFF, "TORQUE_OFF")
    elif choice in _CONTROL_MODE_DISPATCH:
        label, kwargs = _CONTROL_MODE_DISPATCH[choice]
        _dispatch_control_mode(client, label, **kwargs)
    elif choice == "19":
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

    channel = igc_sdk.ChannelFactory.Instance()
    channel.Init(args.domain_id, args.namespace)
    if not channel.IsInitialized():
        print("Failed to initialize ChannelFactory", file=sys.stderr)
        return 1

    client = igc_sdk.IgrisC_Client()
    client.Init()
    client.SetTimeout(5.0)

    monitor = LowStateMonitor()
    lowstate_sub = igc_sdk.LowStateSubscriber(LOWSTATE_TOPIC)
    if not lowstate_sub.init(monitor.callback):
        print("Failed to initialize LowStateSubscriber", file=sys.stderr)
        channel.Release()
        return 1

    lowcmd_pub = igc_sdk.LowCmdPublisher(LOWCMD_TOPIC)
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
