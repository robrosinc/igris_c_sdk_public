#!/usr/bin/env python3
"""Read-only MasterArmState monitor for the TOW3 master arm using the IGRIS SDK (igris_c_sdk).

Subscribes to masterarmstate, the telemetry the TOW3 master arm publishes every tick, and
redraws the latest sample in place:
  - state (MASTERARM_NOT_INITIALIZED / INITIALIZED / PUBLISHING), receive rate and sample age
  - arm joints: left 15..21 and right 22..28 (joint_state q/dq, motor_state q)
  - waist joints 0..2, which the master arm fills from the backpack IMU pose
  - imu_state (backpack IMU) and joy_state (hand controllers)

Nothing is published. The example only reads, so it can run next to a live teleop session.

joint_state is in the robot's joint convention, not raw leader-arm angles: the master arm
applies axis_dir * scale and clamps to the robot joint limits before publishing. motor_state
equals joint_state except for the parallel pairs (wrist roll/pitch, waist roll/pitch), which
carry the converted motor angles.

Robot namespace: every topic resolves under the namespace passed to ChannelFactory.Init(). Use
the same domain id, namespace and topic naming as the master arm; e.g. namespace "igris_c_IG01"
turns the bare topic "masterarmstate" into "igris_c_IG01/masterarmstate".

Usage: python tow3_state.py --domain-id 0 --namespace igris_c_IG01
"""

from __future__ import annotations

import argparse
import math
import sys
import threading
import time
from dataclasses import dataclass

from common import REDRAW, alt_screen, print_banner

import igris_c_sdk as igc_sdk

# joint_state / motor_state are indexed by robot motor id (N_JOINTS = 31). The TOW3 master arm
# fills the arms (15..28) and the waist (0..2); every other slot stays zero.
LEFT_ARM_BASE = 15  # 15..21 = left arm
RIGHT_ARM_BASE = 22  # 22..28 = right arm
ARM_JOINT_NAMES = [
    "Shoulder_Pitch",
    "Shoulder_Roll",
    "Shoulder_Yaw",
    "Elbow_Pitch",
    "Wrist_Yaw",
    "Wrist_Roll",
    "Wrist_Pitch",
]
WAIST_JOINT_NAMES = ["Waist_Yaw", "Waist_Roll", "Waist_Pitch"]  # ids 0..2
SHOWN_IDS = list(range(3)) + list(range(LEFT_ARM_BASE, RIGHT_ARM_BASE + len(ARM_JOINT_NAMES)))

STATE_NAMES = {
    igc_sdk.MASTERARM_NOT_INITIALIZED: "NOT_INITIALIZED",
    igc_sdk.MASTERARM_INITIALIZED: "INITIALIZED",
    igc_sdk.MASTERARM_PUBLISHING: "PUBLISHING",
}

# Shown as STALE when the newest sample is older than this. Display threshold only.
STALE_AFTER_S = 0.5


@dataclass
class Sample:
    """Plain-Python copy of one MasterArmState.

    The message handed to the callback is a view of the SDK's buffer and is only valid during
    the callback, so the values are copied out there instead of keeping the object.
    """

    state: int
    stamp: tuple[int, int]
    q: dict[int, float]
    dq: dict[int, float]
    motor_q: dict[int, float]
    imu_quat: tuple[float, float, float, float]
    imu_gyro: tuple[float, float, float]
    imu_accel: tuple[float, float, float]
    joy_axes: list[float]
    joy_buttons: list[int]
    received_at: float


class Latest:
    """Newest sample plus a receive counter, shared between the DDS thread and the redraw loop."""

    def __init__(self) -> None:
        self.lock = threading.Lock()
        self.sample: Sample | None = None
        self.count = 0

    def on_state(self, msg: object) -> None:
        js = msg.joint_state()
        ms = msg.motor_state()
        imu = msg.imu_state()
        joy = msg.joy_state()
        o, w, a = imu.orientation(), imu.angular_velocity(), imu.linear_acceleration()
        sample = Sample(
            state=int(msg.state()),
            stamp=(msg.header().stamp().sec(), msg.header().stamp().nanosec()),
            q={i: js[i].q() for i in SHOWN_IDS},
            dq={i: js[i].dq() for i in SHOWN_IDS},
            motor_q={i: ms[i].q() for i in SHOWN_IDS},
            imu_quat=(o.x(), o.y(), o.z(), o.w()),
            imu_gyro=(w.x(), w.y(), w.z()),
            imu_accel=(a.x(), a.y(), a.z()),
            joy_axes=list(joy.axes()),
            joy_buttons=list(joy.buttons()),
            received_at=time.monotonic(),
        )
        with self.lock:
            self.sample = sample
            self.count += 1

    def snapshot(self) -> tuple[Sample | None, int]:
        with self.lock:
            return self.sample, self.count


def render_waiting() -> str:
    return (
        "Waiting for masterarmstate...\n\n"
        "No sample yet. Check that:\n"
        "  - domain id, namespace and --dds-topic-naming match the master arm's settings\n"
        "  - this host is on the network the master arm uses for DDS\n"
    )


def render_sample(s: Sample, rate_hz: float, age_s: float) -> str:
    lines = [
        f"state          : {STATE_NAMES.get(s.state, 'UNKNOWN')} ({s.state})",
        f"rx rate        : {rate_hz:.1f} Hz",
        f"sample age     : {age_s:.3f} s" + ("   STALE" if age_s > STALE_AFTER_S else ""),
        f"header stamp   : {s.stamp[0]}.{s.stamp[1]:09d}",
        "",
        f"{'ARM':<14} | {'LEFT':<41} | {'RIGHT':<41}",
        f"{'joint':<14} | {'id':>2} {'q[rad]':>8} {'q[deg]':>8} {'dq[rad/s]':>9} {'motor_q':>9}"
        f" | {'id':>2} {'q[rad]':>8} {'q[deg]':>8} {'dq[rad/s]':>9} {'motor_q':>9}",
    ]

    def cols(i: int) -> str:
        return f"{i:>2} {s.q[i]:+8.3f} {math.degrees(s.q[i]):+8.2f} {s.dq[i]:+9.3f} {s.motor_q[i]:+9.3f}"

    for k, name in enumerate(ARM_JOINT_NAMES):
        lines.append(f"{name:<14} | {cols(LEFT_ARM_BASE + k)} | {cols(RIGHT_ARM_BASE + k)}")

    lines += ["", f"{'WAIST (IMU)':<14} | {'id':>2} {'q[rad]':>8} {'q[deg]':>8} {'motor_q':>9}"]
    for i, name in enumerate(WAIST_JOINT_NAMES):
        lines.append(
            f"{name:<14} | {i:>2} {s.q[i]:+8.3f} {math.degrees(s.q[i]):+8.2f} {s.motor_q[i]:+9.3f}"
        )

    qx, qy, qz, qw = s.imu_quat
    lines += [
        "",
        f"IMU quat (x y z w)   : {qx:+7.4f} {qy:+7.4f} {qz:+7.4f} {qw:+7.4f}",
        "IMU gyro [rad/s]     : " + " ".join(f"{v:+8.3f}" for v in s.imu_gyro),
        "IMU accel [m/s^2]    : " + " ".join(f"{v:+8.3f}" for v in s.imu_accel),
        "",
        # joy_state layout as written by the master arm:
        #   axes[6]    = [L.x, L.y, L.trigger, R.x, R.y, R.trigger]
        #   buttons[8] = [L.sw1, L.sw2, L.sw3, L.sel, R.sw1, R.sw2, R.sw3, R.sel]
        #   (0/1; an offline controller reports zeros)
        "JOY axes    [L.x L.y L.trig R.x R.y R.trig]          : "
        + " ".join(f"{v:+5.2f}" for v in s.joy_axes),
        "JOY buttons [L.sw1 sw2 sw3 sel  R.sw1 sw2 sw3 sel]   : "
        + " ".join(str(b) for b in s.joy_buttons),
    ]
    return "\n".join(lines) + "\n"


def main() -> int:
    parser = argparse.ArgumentParser(description="IGRIS SDK TOW3 MasterArmState monitor")
    parser.add_argument("--domain-id", type=int, default=0)
    parser.add_argument(
        "--namespace",
        default="igris_c_IG01",
        help="namespace of the master arm, e.g. igris_c_IG01",
    )
    parser.add_argument(
        "--dds-topic-naming",
        choices=igc_sdk.topic_naming_names(),
        default="native",
        help="Wire layout for topic names; must match the master arm's topic naming setting.",
    )
    args = parser.parse_args()

    print_banner("TOW3 State", args.domain_id, args.namespace, args.dds_topic_naming)

    # Bare topic names are laid out on the wire by the SDK using the namespace set here; see resolve_topic_name().
    channel = igc_sdk.ChannelFactory.Instance()
    channel.Init(
        args.domain_id, args.namespace, "", igc_sdk.parse_topic_naming(args.dds_topic_naming)
    )
    if not channel.IsInitialized():
        print("Failed to initialize ChannelFactory", file=sys.stderr)
        return 1

    latest = Latest()
    # Best-effort reader: it matches the master arm's writer whatever its reliability, and sends
    # no acknowledgements back to the master arm, so a monitor on a slow link cannot add traffic
    # to it. Dropped samples only cost a frame.
    state_sub = igc_sdk.MasterArmStateSubscriber("masterarmstate", igc_sdk.QosProfile.SensorData())
    if not state_sub.init(latest.on_state):
        print("Failed to initialize MasterArmState subscriber", file=sys.stderr)
        channel.Release()
        return 1
    print("MasterArmState subscriber initialized. Press Ctrl+C to stop.")

    rate_since = time.monotonic()
    rate_from = 0
    rate_hz = 0.0
    try:
        # Redrawn in place on the alternate screen: the wiring printed above stays in the real
        # terminal as a record, and Ctrl+C restores it untouched.
        with alt_screen():
            while True:
                now = time.monotonic()
                sample, count = latest.snapshot()
                if now - rate_since >= 1.0:
                    rate_hz = (count - rate_from) / (now - rate_since)
                    rate_from, rate_since = count, now

                sys.stdout.write(REDRAW)
                print_banner("TOW3 State", args.domain_id, args.namespace, args.dds_topic_naming)
                if sample is None:
                    sys.stdout.write(render_waiting())
                else:
                    sys.stdout.write(render_sample(sample, rate_hz, now - sample.received_at))
                sys.stdout.write("\nPress Ctrl+C to stop.\n")
                sys.stdout.flush()
                time.sleep(0.1)
    except KeyboardInterrupt:
        pass
    finally:
        state_sub.stop()
        channel.Release()

    print("Stopped.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
