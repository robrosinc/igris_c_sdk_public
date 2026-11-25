#!/usr/bin/env python3
"""Headless hand control example using the IGRIS SDK (igris_c_sdk).

Demonstrates the end-effector (hand) control flow over igris_c_sdk DDS channels:
  - Initialization via the service/hand_init request/response service
  - HandCmd publishing (normalized 0~1 joint targets) on handcmd
  - HandState subscription on handstate

Works for any end effector type (dexterous hand / 1-dof / magnet); only the
driven motor ids differ (hand: 11~16/21~26, 1-dof: 11/21, magnet: 11/12/21/22).

Robot namespace: every topic resolves under the robot-unit namespace passed to
ChannelFactory.Init(). e.g. namespace "igris_c_IG01" turns the bare topic "handcmd" into "igris_c_IG01/handcmd".

Usage: python hand_example.py --domain-id 0 --namespace igris_c_IG01
"""

from __future__ import annotations

import argparse
import sys
import time

from common import REDRAW, print_banner

import igris_c_sdk as igc_sdk

# Hand motor ids: right 11~16, left 21~26. (1-dof: {11,21}, magnet: {11,12,21,22})
MOTOR_IDS = [11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 25, 26]


def make_handcmd(targets: list[float]) -> "igc_sdk.HandCmd":
    """Build a HandCmd from 12 normalized [0,1] targets (one per motor id slot)."""
    motors = []
    for mid, q in zip(MOTOR_IDS, targets):
        m = igc_sdk.MotorCmd_()
        m.id(mid)
        m.q(max(0.0, min(1.0, q)))
        motors.append(m)
    cmd = igc_sdk.HandCmd_()
    cmd.motor_cmd(motors)  # set the variable-length motor command list
    return cmd


def main() -> int:
    parser = argparse.ArgumentParser(description="IGRIS SDK hand example")
    parser.add_argument("--domain-id", type=int, default=0)
    parser.add_argument(
        "--namespace", default="igris_c_IG01", help="robot-unit namespace, e.g. igris_c_IG01"
    )
    parser.add_argument(
        "--dds-topic-naming",
        choices=igc_sdk.topic_naming_names(),
        default="native",
        help="Wire layout for topic names; must match the robot's igris_c.network.transport "
        "(native for cyclonedds_native, ros_compatible for cyclonedds_ros_compatible).",
    )
    args = parser.parse_args()

    print_banner("Hand Example", args.domain_id, args.namespace, args.dds_topic_naming)

    # Bare topic names are laid out on the wire by the SDK using the namespace set here; see resolve_topic_name().
    channel = igc_sdk.ChannelFactory.Instance()
    channel.Init(
        args.domain_id, args.namespace, "", igc_sdk.parse_topic_naming(args.dds_topic_naming)
    )
    if not channel.IsInitialized():
        print("Failed to initialize ChannelFactory", file=sys.stderr)
        return 1

    cmd_pub = igc_sdk.HandCmdPublisher("handcmd", igc_sdk.QosProfile.SensorData())
    if not cmd_pub.init():
        print("Failed to initialize HandCmdPublisher", file=sys.stderr)
        channel.Release()
        return 1

    # Initialization is a request/response service (no id=99 in handcmd anymore).
    init_req_pub = igc_sdk.HandInitRequest_RequestPublisher(
        "service/hand_init/request", igc_sdk.QosProfile.Services()
    )
    init_req_pub.init()

    def on_state(state: object) -> None:
        ms = state.motor_state()
        if len(ms) >= 3:
            print(f"q[0..2]={ms[0].q():.3f} {ms[1].q():.3f} {ms[2].q():.3f}")

    state_sub = igc_sdk.HandStateSubscriber("handstate", igc_sdk.QosProfile.SensorData())
    state_sub.init(on_state)

    def on_init_res(res: object) -> None:
        print(f"[hand_init] success={res.success()} code={res.error_code()} msg={res.message()}")

    init_res_sub = igc_sdk.HandInitRequest_ResponseSubscriber(
        "service/hand_init/response", igc_sdk.QosProfile.Services()
    )
    init_res_sub.init(on_init_res)

    # 1) Trigger initialization via the hand_init service.
    req = igc_sdk.HandInitRequest_Request_()
    req.request_id("hand_init")
    init_req_pub.write(req)
    time.sleep(1.5)

    open_pose = [0.05] * 12
    close_pose = [0.85] * 12
    pinch_pose = [0.0] * 12
    pinch_pose[0], pinch_pose[1], pinch_pose[2] = (
        0.80,
        0.75,
        0.60,
    )  # right thumb / index / middle (ids 11,12,13)

    cmd_pub.write(make_handcmd(open_pose))
    time.sleep(2.0)
    cmd_pub.write(make_handcmd(close_pose))
    time.sleep(2.5)
    cmd_pub.write(make_handcmd(pinch_pose))
    time.sleep(2.0)
    cmd_pub.write(make_handcmd(open_pose))
    time.sleep(1.5)

    state_sub.stop()
    init_res_sub.stop()
    channel.Release()
    print("Done.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
