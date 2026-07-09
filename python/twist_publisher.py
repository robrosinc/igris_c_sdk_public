#!/usr/bin/env python3
"""Interactive walk-velocity (Twist) publisher example.

Publishes igris_c::msg::dds::WalkTwist on rt/cmd_vel at 20 Hz to drive the
robot's walk controller. Python port of
examples/cyclonedds/cyclonedds_twist_publisher.cpp (and the ROS2
examples/ros2/src/ros_twist_publisher.cpp).

Switch the robot into walkmode first (e.g. client.py option 12 "Walkmode"),
then steer with the keyboard.

Run with:
    python twist_publisher.py --domain-id 0 --namespace robot1

Controls: w/s = linear.x +/- , a/d = linear.y +/- , q/e = angular.z +/- ,
          space = zero, x = exit
"""

from __future__ import annotations

import argparse
import select
import sys
import termios
import time
import tty

import igris_c_sdk as igc_sdk

DEFAULT_DOMAIN_ID = 0
CMD_VEL_TOPIC = "rt/cmd_vel"
PUBLISH_PERIOD_S = 1.0 / 20.0  # 20 Hz
STEP_LIN = 0.05  # m/s per keypress
STEP_ANG = 0.1  # rad/s per keypress


class RawTerminal:
    """Put stdin in cbreak mode so single keystrokes steer without Enter."""

    def __enter__(self) -> "RawTerminal":
        self._fd = sys.stdin.fileno()
        self._old = termios.tcgetattr(self._fd)
        tty.setcbreak(self._fd)
        return self

    def __exit__(self, *_exc: object) -> None:
        termios.tcsetattr(self._fd, termios.TCSADRAIN, self._old)


def read_key() -> str | None:
    """Non-blocking single-character read; returns None if no key is ready."""
    if select.select([sys.stdin], [], [], 0)[0]:
        return sys.stdin.read(1)
    return None


def stamp_header(header: object, seq: int) -> None:
    # Every IGRIS message carries a common Header: incrementing seq + wall clock.
    now = time.time()
    sec = int(now)
    header.seq(seq)
    header.sec(sec)
    header.nanosec(int((now - sec) * 1e9))


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="IGRIS-C SDK walk-velocity (Twist) publisher")
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


def print_help() -> None:
    print("Publishing WalkTwist on rt/cmd_vel at 20 Hz.")
    print("Switch the robot into walkmode first (e.g. client.py option 12 'Walkmode').")
    print(
        "Controls: w/s = linear.x +/- , a/d = linear.y +/- , q/e = angular.z +/- , space = zero, x = exit"
    )


def main() -> int:
    args = build_arg_parser().parse_args()
    print(f"Domain ID: {args.domain_id}")
    print(f'Namespace: "{args.namespace}"')

    channel = igc_sdk.ChannelFactory.Instance()
    channel.Init(args.domain_id, args.namespace)
    if not channel.IsInitialized():
        print("Failed to initialize ChannelFactory", file=sys.stderr)
        return 1

    # Default QoS (~rclcpp::QoS(10)), matching the bridge's rt/cmd_vel subscriber.
    twist_pub = igc_sdk.WalkTwistPublisher(CMD_VEL_TOPIC, igc_sdk.QosProfile.Default())
    if not twist_pub.init():
        print("Failed to initialize WalkTwist publisher", file=sys.stderr)
        channel.Release()
        return 1

    print_help()

    vx = 0.0
    vy = 0.0
    wz = 0.0
    seq = 0
    running = True

    try:
        with RawTerminal():
            while running:
                loop_start = time.monotonic()

                key = read_key()
                while key is not None:
                    if key == "w":
                        vx += STEP_LIN
                    elif key == "s":
                        vx -= STEP_LIN
                    elif key == "a":
                        vy += STEP_LIN
                    elif key == "d":
                        vy -= STEP_LIN
                    elif key == "q":
                        wz += STEP_ANG
                    elif key == "e":
                        wz -= STEP_ANG
                    elif key == " ":
                        vx = vy = wz = 0.0
                    elif key == "x":
                        running = False
                    key = read_key()

                msg = igc_sdk.WalkTwist()
                stamp_header(msg.header(), seq)
                seq += 1
                twist = msg.twist()
                twist.linear().x(vx)
                twist.linear().y(vy)
                twist.angular().z(wz)
                twist_pub.write(msg)

                print(
                    f"\r\033[Klinear.x={vx:.2f} linear.y={vy:.2f} angular.z={wz:.2f}  ",
                    end="",
                    flush=True,
                )

                elapsed = time.monotonic() - loop_start
                if elapsed < PUBLISH_PERIOD_S:
                    time.sleep(PUBLISH_PERIOD_S - elapsed)
    except KeyboardInterrupt:
        pass
    finally:
        print()
        twist_pub.stop()
        channel.Release()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
