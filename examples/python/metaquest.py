#!/usr/bin/env python3

from __future__ import annotations

import argparse
import signal
import sys
import threading
import time
from dataclasses import dataclass, field

import igris_c_sdk as igc_sdk

DEFAULT_DOMAIN_ID = 0
DEFAULT_TOPIC = "igris_c/metaquest"
FINGER_NAMES = ("thumb", "index", "middle", "ring", "pinky")


def format_array(values: tuple[float, ...], precision: int = 3) -> str:
    return "[" + ", ".join(f"{value:.{precision}f}" for value in values) + "]"


def format_bool(value: bool) -> str:
    return "Y" if value else "N"


def format_bool_tuple(values: tuple[bool, ...]) -> str:
    return "[" + ", ".join(format_bool(value) for value in values) + "]"


def uses_controller_tracking(tracking_mode: int) -> bool:
    return tracking_mode == 0


def uses_hand_tracking(tracking_mode: int) -> bool:
    return tracking_mode == 1


def print_rule(width: int = 108) -> None:
    sys.stdout.write("=" * width + "\n")


def print_kv(key: str, value: str) -> None:
    sys.stdout.write(f"{key:<20} | {value}\n")


def vec3_tuple(vec: object) -> tuple[float, float, float]:
    data = vec.data()
    return (float(data[0]), float(data[1]), float(data[2]))


def vec4_tuple(vec: object) -> tuple[float, float, float, float]:
    data = vec.data()
    return (float(data[0]), float(data[1]), float(data[2]), float(data[3]))


def float_tuple(values: object) -> tuple[float, ...]:
    return tuple(float(value) for value in values)


def bool_tuple(values: object) -> tuple[bool, ...]:
    return tuple(bool(value) for value in values)


def as_sequence(value: object) -> list[object]:
    try:
        return list(value)
    except TypeError:
        return [value]


@dataclass
class HeaderSnapshot:
    seq: int
    sec: int
    nanosec: int
    frame_id: str


@dataclass
class ControllerSnapshot:
    position: tuple[float, float, float]
    quaternion: tuple[float, float, float, float]
    euler: tuple[float, float, float]
    linear_velocity: tuple[float, float, float]
    angular_velocity: tuple[float, float, float]
    joystick: tuple[float, float]
    touchpad: tuple[float, float]
    index_trigger: float
    hand_trigger: float
    button_one: bool
    button_two: bool
    button_three: bool
    button_four: bool
    joystick_click: bool
    index_trigger_touch: bool
    hand_trigger_touch: bool

    def buttons_text(self) -> str:
        return (
            f"1:{format_bool(self.button_one)} "
            f"2:{format_bool(self.button_two)} "
            f"3:{format_bool(self.button_three)} "
            f"4:{format_bool(self.button_four)} "
            f"JC:{format_bool(self.joystick_click)} "
            f"IT:{format_bool(self.index_trigger_touch)} "
            f"HT:{format_bool(self.hand_trigger_touch)}"
        )


@dataclass
class FingerSnapshot:
    name: str
    pinch: bool
    pinch_strength: float
    finger_confidence: float
    tip_valid: bool
    tip_position: tuple[float, float, float]
    tip_quaternion: tuple[float, float, float, float]
    tip_to_thumb_distance: float


@dataclass
class HandSnapshot:
    position: tuple[float, float, float]
    quaternion: tuple[float, float, float, float]
    euler: tuple[float, float, float]
    confidence: float
    pinch: tuple[bool, bool, bool, bool, bool]
    tip_valid: tuple[bool, bool, bool, bool, bool]
    fingers: list[FingerSnapshot]


@dataclass
class QuestSnapshot:
    header: HeaderSnapshot
    tracking_mode: int
    head_position: tuple[float, float, float]
    head_quaternion: tuple[float, float, float, float]
    head_euler: tuple[float, float, float]
    controllers: list[ControllerSnapshot]
    hands: list[HandSnapshot]
    ip: str
    battery: float


def snapshot_from_message(message: object) -> QuestSnapshot:
    header = message.header()
    controllers = [
        ControllerSnapshot(
            position=vec3_tuple(controller.position()),
            quaternion=vec4_tuple(controller.quaternion()),
            euler=vec3_tuple(controller.euler()),
            linear_velocity=float_tuple(controller.linear_velocity()),
            angular_velocity=float_tuple(controller.angular_velocity()),
            joystick=float_tuple(controller.joystick()),
            touchpad=float_tuple(controller.touchpad()),
            index_trigger=float(controller.index_trigger()),
            hand_trigger=float(controller.hand_trigger()),
            button_one=bool(controller.button_one()),
            button_two=bool(controller.button_two()),
            button_three=bool(controller.button_three()),
            button_four=bool(controller.button_four()),
            joystick_click=bool(controller.joystick_click()),
            index_trigger_touch=bool(controller.index_trigger_touch()),
            hand_trigger_touch=bool(controller.hand_trigger_touch()),
        )
        for controller in as_sequence(message.controller())
    ]
    hands: list[HandSnapshot] = []
    for hand in as_sequence(message.hand()):
        tip_positions = hand.tip_position()
        tip_quaternions = hand.tip_quaternion()
        fingers = [
            FingerSnapshot(
                name=FINGER_NAMES[index],
                pinch=bool(hand.pinch()[index]),
                pinch_strength=float(hand.pinch_strength()[index]),
                finger_confidence=float(hand.finger_confidence()[index]),
                tip_valid=bool(hand.tip_valid()[index]),
                tip_position=vec3_tuple(tip_positions[index]),
                tip_quaternion=vec4_tuple(tip_quaternions[index]),
                tip_to_thumb_distance=float(hand.tip_to_thumb_distance()[index]),
            )
            for index in range(5)
        ]
        hands.append(
            HandSnapshot(
                position=vec3_tuple(hand.position()),
                quaternion=vec4_tuple(hand.quaternion()),
                euler=vec3_tuple(hand.euler()),
                confidence=float(hand.confidence()),
                pinch=bool_tuple(hand.pinch()),
                tip_valid=bool_tuple(hand.tip_valid()),
                fingers=fingers,
            )
        )

    return QuestSnapshot(
        header=HeaderSnapshot(
            seq=int(header.seq()),
            sec=int(header.sec()),
            nanosec=int(header.nanosec()),
            frame_id=str(header.frame_id()),
        ),
        tracking_mode=int(message.tracking_mode()),
        head_position=vec3_tuple(message.head_position()),
        head_quaternion=vec4_tuple(message.head_quaternion()),
        head_euler=vec3_tuple(message.head_euler()),
        controllers=controllers,
        hands=hands,
        ip=str(message.ip()),
        battery=float(message.battery()),
    )


@dataclass
class SubscriberState:
    topic: str
    lock: threading.Lock = field(default_factory=threading.Lock)
    message_count: int = 0
    last_receive: float = 0.0
    snapshot: QuestSnapshot | None = None

    def on_message(self, message: object) -> None:
        now = time.perf_counter()
        snapshot = snapshot_from_message(message)

        with self.lock:
            self.last_receive = now
            self.message_count += 1
            self.snapshot = snapshot

    def view(self) -> tuple[int, float, float, QuestSnapshot | None]:
        with self.lock:
            return self.message_count, self.last_receive, self.snapshot


def print_snapshot(domain_id: int, state: SubscriberState) -> None:
    message_count, last_receive, snapshot = state.view()
    sys.stdout.write("\x1b[2J\x1b[H")
    print_rule()
    sys.stdout.write("MetaQuest Raw Viewer\n")
    print_rule()
    print_kv("Domain", str(domain_id))
    print_kv("Topic", state.topic)

    if snapshot is None:
        print_kv("Status", "Waiting for QuestMessage samples...")
        print_kv("Quit", "Ctrl+C")
        print_rule()
        sys.stdout.flush()
        return

    age_ms = int((time.perf_counter() - last_receive) * 1000.0) if last_receive > 0.0 else 0
    print_kv("Message Count", str(message_count))
    print_kv("Age", f"{age_ms} ms")
    print_kv("Seq", str(snapshot.header.seq))
    print_kv("Stamp", f"{snapshot.header.sec}.{snapshot.header.nanosec:09d}")
    print_kv("Frame ID", snapshot.header.frame_id)
    print_kv("IP", snapshot.ip)
    print_kv("Battery", f"{snapshot.battery:.1f}")
    print_kv("Tracking Mode", str(snapshot.tracking_mode))
    if uses_controller_tracking(snapshot.tracking_mode):
        print_kv("Active Input", "controller")
    elif uses_hand_tracking(snapshot.tracking_mode):
        print_kv("Active Input", "hand")
    else:
        print_kv("Active Input", "unknown")
    print_rule()
    sys.stdout.write("Pose Summary\n")
    print_rule()
    print_kv("Head Pos", format_array(snapshot.head_position))
    print_kv("Head Euler", format_array(snapshot.head_euler))
    print_kv("Head Quat", format_array(snapshot.head_quaternion))
    if uses_controller_tracking(snapshot.tracking_mode):
        for index, controller in enumerate(snapshot.controllers):
            prefix = f"Ctrl[{index}]"
            print_kv(f"{prefix} Pos", format_array(controller.position))
            print_kv(f"{prefix} Euler", format_array(controller.euler))
            print_kv(f"{prefix} Quat", format_array(controller.quaternion))
            print_kv(f"{prefix} Lin Vel", format_array(controller.linear_velocity))
            print_kv(f"{prefix} Ang Vel", format_array(controller.angular_velocity))
            print_kv(f"{prefix} Joystick", format_array(controller.joystick))
            print_kv(f"{prefix} Touchpad", format_array(controller.touchpad))
            print_kv(f"{prefix} Index Trg", f"{controller.index_trigger:.3f}")
            print_kv(f"{prefix} Hand Trg", f"{controller.hand_trigger:.3f}")
            print_kv(f"{prefix} Buttons", controller.buttons_text())
    elif uses_hand_tracking(snapshot.tracking_mode):
        for index, hand in enumerate(snapshot.hands):
            prefix = f"Hand[{index}]"
            print_kv(f"{prefix} Pos", format_array(hand.position))
            print_kv(f"{prefix} Euler", format_array(hand.euler))
            print_kv(f"{prefix} Quat", format_array(hand.quaternion))
            print_kv(f"{prefix} Conf", f"{hand.confidence:.3f}")
            print_kv(f"{prefix} Pinch", format_bool_tuple(hand.pinch))
            print_kv(f"{prefix} Tip Valid", format_bool_tuple(hand.tip_valid))
            print_kv(f"{prefix} Fingers", str(list(FINGER_NAMES)))
            print_rule()
            sys.stdout.write(f"{prefix} Finger Table\n")
            print_rule()
            sys.stdout.write(
                f"{'Finger':<8} | {'Pinch':<5} | {'Strength':<8} | {'Conf':<8} | "
                f"{'Valid':<8} | {'Dist':<10} | {'Tip Pos':<22} | Tip Quat\n"
            )
            print_rule()
            for finger in hand.fingers:
                sys.stdout.write(
                    f"{finger.name:<8} | {format_bool(finger.pinch):<5} | {finger.pinch_strength:<8.3f} | "
                    f"{finger.finger_confidence:<8.3f} | {format_bool(finger.tip_valid):<8} | "
                    f"{finger.tip_to_thumb_distance:<10.3f} | {format_array(finger.tip_position):<22} | "
                    f"{format_array(finger.tip_quaternion)}\n"
                )
            print_rule()
    else:
        print_kv("Tracking Data", "Unsupported tracking_mode")
    print_kv("Quit", "Ctrl+C")
    print_rule()
    sys.stdout.flush()


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="MetaQuest raw QuestMessage viewer")
    parser.add_argument(
        "--domain-id",
        type=int,
        default=DEFAULT_DOMAIN_ID,
        help=f"Cyclone DDS domain id (default: {DEFAULT_DOMAIN_ID})",
    )
    parser.add_argument(
        "topic",
        nargs="?",
        default=DEFAULT_TOPIC,
        help=f"QuestMessage topic name (default: {DEFAULT_TOPIC})",
    )
    return parser


def main() -> int:
    args = build_arg_parser().parse_args()
    state = SubscriberState(topic=args.topic)
    stop_event = threading.Event()

    def handle_signal(_signum: int, _frame: object) -> None:
        stop_event.set()

    signal.signal(signal.SIGINT, handle_signal)
    signal.signal(signal.SIGTERM, handle_signal)

    channel = igc_sdk.ChannelFactory.Instance()
    channel.Init(args.domain_id)

    subscriber = igc_sdk.QuestMessageSubscriber(args.topic)
    if not subscriber.init(state.on_message):
        print("Failed to initialize QuestMessageSubscriber", file=sys.stderr)
        channel.Release()
        return 1

    try:
        while not stop_event.is_set():
            print_snapshot(args.domain_id, state)
            time.sleep(0.1)
    finally:
        subscriber.stop()
        channel.Release()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
