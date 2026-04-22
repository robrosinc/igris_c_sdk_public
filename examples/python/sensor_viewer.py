#!/usr/bin/env python3

from __future__ import annotations

import argparse
import signal
import sys
import threading
import time
from dataclasses import dataclass, field

import cv2
import igris_c_sdk as igc_sdk
import numpy as np

DEFAULT_TOPICS = [
    "igris_c/sensor/d435_color",
    "igris_c/sensor/d435_depth",
    "igris_c/sensor/eyes_stereo",
    "igris_c/sensor/left_hand",
    "igris_c/sensor/right_hand",
]


def format_rate(bytes_per_sec: float) -> str:
    if bytes_per_sec > 1024.0 * 1024.0:
        return f"{bytes_per_sec / (1024.0 * 1024.0):.1f} MB/s"
    if bytes_per_sec > 1024.0:
        return f"{bytes_per_sec / 1024.0:.1f} KB/s"
    return f"{bytes_per_sec:.0f} B/s"


@dataclass
class StreamState:
    name: str
    fps: float = 0.0
    bytes_per_sec: float = 0.0
    last_time: float = 0.0
    last_payload: bytes | None = None
    sequence: int = 0
    decoded_sequence: int = -1
    frame: np.ndarray | None = None
    lock: threading.Lock = field(default_factory=threading.Lock)

    def update(self, payload: bytes) -> None:
        now = time.perf_counter()
        with self.lock:
            if self.last_time > 0.0:
                elapsed = now - self.last_time
                if elapsed > 0.0:
                    inst_fps = 1.0 / elapsed
                    inst_bps = len(payload) / elapsed
                    alpha = 0.15
                    if self.fps <= 0.0:
                        self.fps = inst_fps
                        self.bytes_per_sec = inst_bps
                    else:
                        self.fps = (1.0 - alpha) * self.fps + alpha * inst_fps
                        self.bytes_per_sec = (1.0 - alpha) * self.bytes_per_sec + alpha * inst_bps
            self.last_time = now
            self.last_payload = payload
            self.sequence += 1

    def decode_latest(self) -> np.ndarray | None:
        with self.lock:
            if self.last_payload is None or self.sequence == self.decoded_sequence:
                return self.frame
            payload = self.last_payload
            sequence = self.sequence

        raw = np.frombuffer(payload, dtype=np.uint8)
        frame = cv2.imdecode(raw, cv2.IMREAD_UNCHANGED)
        if frame is None or frame.size == 0:
            return None

        with self.lock:
            if self.sequence == sequence:
                self.frame = frame
                self.decoded_sequence = sequence
            return self.frame

    def snapshot(self) -> tuple[float, float]:
        with self.lock:
            return self.fps, self.bytes_per_sec


def print_status(streams: list[StreamState]) -> None:
    sys.stdout.write("\x1b[2J\x1b[H")
    sys.stdout.write("==============================\n\n")
    sys.stdout.write("Stream Status:\n")

    total_bps = 0.0
    total_fps = 0.0
    for stream in streams:
        fps, bps = stream.snapshot()
        total_bps += bps
        total_fps += fps
        color_start = "\x1b[31m" if fps <= 20.0 else "\x1b[0m"
        sys.stdout.write(f"{color_start}{stream.name}: {fps:.1f} FPS, {format_rate(bps)}\x1b[0m\n")

    avg_fps = total_fps / len(streams) if streams else 0.0
    sys.stdout.write("\n")
    sys.stdout.write(f"Total bytes/sec: {format_rate(total_bps)}\n")
    sys.stdout.write(f"Avg FPS: {avg_fps:.1f} FPS\n")
    sys.stdout.write("Press 'q' or 'ESC' to quit.\n")
    sys.stdout.write("==============================\n")
    sys.stdout.flush()


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="IGRIS compressed sensor stream viewer")
    parser.add_argument(
        "--domain-id",
        type=int,
        default=10,
        help="Cyclone DDS domain id (default: 10)",
    )
    parser.add_argument(
        "topics",
        nargs="*",
        help="Compressed image topics to subscribe to",
    )
    return parser


def create_subscribers(domain_id: int, streams: list[StreamState]) -> list[object]:
    channel = igc_sdk.ChannelFactory.Instance()
    channel.Init(domain_id)

    subscribers: list[object] = []
    for stream in streams:
        subscriber = igc_sdk.CompressedMessageSubscriber(stream.name)
        subscriber.init(lambda msg, stream=stream: stream.update(bytes(msg.image_data())))
        subscribers.append(subscriber)
    return subscribers


def main() -> int:
    args = build_arg_parser().parse_args()
    topics = args.topics or DEFAULT_TOPICS
    streams = [StreamState(name=topic) for topic in topics]

    stop_event = threading.Event()

    def handle_signal(_signum: int, _frame: object) -> None:
        stop_event.set()

    signal.signal(signal.SIGINT, handle_signal)
    signal.signal(signal.SIGTERM, handle_signal)

    subscribers = create_subscribers(args.domain_id, streams)

    for stream in streams:
        cv2.namedWindow(stream.name, cv2.WINDOW_NORMAL)

    last_status_print = 0.0
    try:
        while not stop_event.is_set():
            got_data = False

            for stream in streams:
                frame = stream.decode_latest()
                if frame is None:
                    continue

                got_data = True
                display = frame.copy()
                fps, bps = stream.snapshot()
                text = f"{fps:.1f} FPS, {format_rate(bps)}"
                color = (0, 0, 255) if fps <= 20.0 else (0, 255, 0)
                cv2.putText(
                    display,
                    text,
                    (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.8,
                    color,
                    2,
                )
                cv2.imshow(stream.name, display)

            now = time.perf_counter()
            if now - last_status_print >= 0.5:
                print_status(streams)
                last_status_print = now

            key = cv2.waitKey(1) & 0xFF
            if key in (27, ord("q"), ord("Q")):
                break

            if not got_data:
                time.sleep(0.002)
    finally:
        for subscriber in subscribers:
            subscriber.stop()
        for stream in streams:
            cv2.destroyWindow(stream.name)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
