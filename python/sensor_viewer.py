#!/usr/bin/env python3

from __future__ import annotations

import argparse
import signal
import sys
import threading
import time
from dataclasses import dataclass, field

import cv2
import numpy as np

import igris_c_sdk as igc_sdk

DEFAULT_TOPICS = [
    "sensor/d435_color",
    "sensor/d435_depth",
    "sensor/d435_ir_left",
    "sensor/d435_ir_right",
    "sensor/eyes_stereo",
    "sensor/left_hand",
    "sensor/right_hand",
]

IMU_TOPIC = "sensor/d435_imu"


def decode_raw_frame(payload: bytes, fmt: str) -> np.ndarray | None:
    parts = fmt.split(";")
    if len(parts) != 3 or parts[1] != "raw":
        return None
    encoding, _, size = parts

    layouts = {
        "mono8": (1, np.uint8),
        "8UC1": (1, np.uint8),
        "8UC3": (3, np.uint8),
        "16UC1": (1, np.uint16),
    }
    layout = layouts.get(encoding)
    x_pos = size.find("x")
    w_str, h_str = (size[:x_pos], size[x_pos + 1 :]) if x_pos > 0 else ("", "")
    if layout is None or not (w_str.isdigit() and h_str.isdigit()):
        return None

    channels, dtype = layout
    width, height = int(w_str), int(h_str)
    expected_size = width * height * channels * np.dtype(dtype).itemsize
    if width <= 0 or height <= 0 or len(payload) != expected_size:
        return None

    shape = (height, width, channels) if channels == 3 else (height, width)
    raw = np.frombuffer(payload, dtype=dtype).reshape(shape)

    if dtype == np.uint16:
        min_val = float(raw.min())
        max_val = max(float(raw.max()), min_val + 1.0)
        scale = 255.0 / (max_val - min_val)
        depth_u8 = cv2.convertScaleAbs(raw, alpha=scale, beta=-min_val * scale)
        return cv2.applyColorMap(depth_u8, cv2.COLORMAP_JET)
    if channels == 3:
        return cv2.cvtColor(raw, cv2.COLOR_RGB2BGR)
    return raw.copy()


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
    last_format: str = ""
    sequence: int = 0
    decoded_sequence: int = -1
    frame: np.ndarray | None = None
    lock: threading.Lock = field(default_factory=threading.Lock)

    def update(self, payload: bytes, fmt: str) -> None:
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
            self.last_format = fmt
            self.sequence += 1

    def decode_latest(self) -> np.ndarray | None:
        with self.lock:
            if self.last_payload is None or self.sequence == self.decoded_sequence:
                return self.frame
            payload = self.last_payload
            fmt = self.last_format
            sequence = self.sequence

        raw = np.frombuffer(payload, dtype=np.uint8)
        frame = cv2.imdecode(raw, cv2.IMREAD_UNCHANGED)
        if frame is None or frame.size == 0:
            frame = decode_raw_frame(payload, fmt)
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


@dataclass
class ImuState:
    name: str
    hz: float = 0.0
    last_time: float = 0.0
    accel: tuple[float, float, float] = (0.0, 0.0, 0.0)
    gyro: tuple[float, float, float] = (0.0, 0.0, 0.0)
    rpy: tuple[float, float, float] = (0.0, 0.0, 0.0)
    has_data: bool = False
    lock: threading.Lock = field(default_factory=threading.Lock)

    def update(
        self,
        accel: tuple[float, float, float],
        gyro: tuple[float, float, float],
        rpy: tuple[float, float, float],
    ) -> None:
        now = time.perf_counter()
        with self.lock:
            if self.last_time > 0.0:
                elapsed = now - self.last_time
                if elapsed > 0.0:
                    inst_hz = 1.0 / elapsed
                    alpha = 0.15
                    self.hz = (
                        inst_hz if self.hz <= 0.0 else (1.0 - alpha) * self.hz + alpha * inst_hz
                    )
            self.last_time = now
            self.accel = accel
            self.gyro = gyro
            self.rpy = rpy
            self.has_data = True

    def snapshot(
        self,
    ) -> tuple[
        bool,
        float,
        tuple[float, float, float],
        tuple[float, float, float],
        tuple[float, float, float],
    ]:
        with self.lock:
            return self.has_data, self.hz, self.accel, self.gyro, self.rpy


def print_status(streams: list[StreamState], imu: ImuState) -> None:
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

    has_data, hz, accel, gyro, rpy = imu.snapshot()
    sys.stdout.write(f"\n{imu.name}: ")
    if has_data:
        sys.stdout.write(f"{hz:.1f} Hz\n")
        sys.stdout.write(f"  accel [m/s^2]: {accel[0]:.3f}, {accel[1]:.3f}, {accel[2]:.3f}\n")
        sys.stdout.write(f"  gyro  [rad/s]: {gyro[0]:.3f}, {gyro[1]:.3f}, {gyro[2]:.3f}\n")
        sys.stdout.write(f"  rpy   [rad]:   {rpy[0]:.3f}, {rpy[1]:.3f}, {rpy[2]:.3f}\n")
    else:
        sys.stdout.write("no data\n")

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
        "--namespace",
        type=str,
        required=True,
        help="Topic namespace prefix (e.g. robot1)",
    )
    parser.add_argument(
        "topics",
        nargs="*",
        help="Compressed image topics to subscribe to",
    )
    return parser


def create_subscribers(
    domain_id: int, namespace: str, streams: list[StreamState], imu: ImuState
) -> list[object]:
    channel = igc_sdk.ChannelFactory.Instance()
    channel.Init(domain_id, namespace)

    subscribers: list[object] = []
    for stream in streams:
        subscriber = igc_sdk.CompressedMessageSubscriber(
            stream.name, igc_sdk.QosProfile.SensorData()
        )
        subscriber.init(
            lambda msg, stream=stream: stream.update(bytes(msg.image_data()), msg.format())
        )
        subscribers.append(subscriber)

    imu_subscriber = igc_sdk.IMUStateSubscriber(imu.name, igc_sdk.QosProfile.SensorData())
    imu_subscriber.init(
        lambda msg: imu.update(tuple(msg.accelerometer()), tuple(msg.gyroscope()), tuple(msg.rpy()))
    )
    subscribers.append(imu_subscriber)
    return subscribers


def main() -> int:
    args = build_arg_parser().parse_args()
    topics = args.topics or DEFAULT_TOPICS
    streams = [StreamState(name=topic) for topic in topics]
    imu = ImuState(name=IMU_TOPIC)

    stop_event = threading.Event()

    def handle_signal(_signum: int, _frame: object) -> None:
        stop_event.set()

    signal.signal(signal.SIGINT, handle_signal)
    signal.signal(signal.SIGTERM, handle_signal)

    subscribers = create_subscribers(args.domain_id, args.namespace, streams, imu)

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
                print_status(streams, imu)
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
