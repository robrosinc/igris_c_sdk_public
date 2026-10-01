#!/usr/bin/env python3
"""Camera stream viewer: ONE window, images on the left and the subscription controls on the right.

The Python counterpart of cyclonedds_sensor_viewer, deliberately the same window. It used to
draw one OpenCV window per stream and reprint a terminal block, and it subscribed to every
stream whether you were looking or not - about 21 MB/s over the link. The control panel is
therefore not decoration: unticking a stream calls Subscriber.stop() and the data stops crossing
the network (measured: 22.0 MB/s with everything on, 0.3 MB/s with only the IMU).

Each stream exists on the wire in two forms and the camera publishes whichever its config says:
sensor_msgs/Image on "<topic>" (raw) and sensor_msgs/CompressedImage on "<topic>/compressed".
The per-row "compressed" toggle picks which ONE is subscribed, so the panel also answers "is
this stream raw or encoded right now" without reading camera.yaml.
"""

from __future__ import annotations

import argparse
import signal
import sys
import threading
import time
from dataclasses import dataclass, field

import cv2
import dearpygui.dearpygui as dpg
import numpy as np
from common import print_banner

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

# Three tiles per row. Two made each tile large enough that seven streams did not fit on screen
# and had to be scrolled past; three keeps the whole set visible.
COLUMNS = 3

# UI scale, same rule and floor as sdk_gui_client: the default font is sized in raw pixels, so
# on a 4K panel an unscaled window renders text too small to read from a desk.
BASE_WINDOW_WIDTH = 1800.0
BASE_WINDOW_HEIGHT = 900.0
MIN_UI_SCALE = 1.15
MAX_UI_SCALE = 2.5

PANEL_WIDTH = 340

try:
    import tkinter as _tk
except Exception:  # noqa: BLE001 - tkinter is optional; only used to size the window
    _tk = None


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
    except Exception:  # noqa: BLE001 - any tk failure (no display etc.) -> fallback
        pass
    return 1920, 1080


def compute_ui_scale(screen_w: int, screen_h: int) -> float:
    raw = min(screen_w / BASE_WINDOW_WIDTH, screen_h / BASE_WINDOW_HEIGHT)
    return max(MIN_UI_SCALE, min(MAX_UI_SCALE, raw))


def default_compressed(topic: str) -> bool:
    """Which lane a stream starts on, matching what igris_c_camera publishes by default.

    camera.yaml ships `depth_format: raw` ("geometry consumers need raw") and publishes both IR
    imagers as raw mono8, while colour, eyes_stereo and the hand cameras are `format: jpeg`.
    Starting every stream on /compressed would leave depth and IR blank on launch - subscribed to
    a topic the camera does not write. Matched on the topic NAME so a custom topic list gets the
    same treatment.
    """
    return "depth" not in topic and "_ir_" not in topic


def format_rate(bytes_per_sec: float) -> str:
    if bytes_per_sec > 1024 * 1024:
        return f"{bytes_per_sec / (1024 * 1024):.1f} MB/s"
    if bytes_per_sec > 1024:
        return f"{bytes_per_sec / 1024:.1f} KB/s"
    return f"{bytes_per_sec:.0f} B/s"


@dataclass
class Stats:
    """Rate over a fixed window: count messages, divide by elapsed.

    NOT an exponentially smoothed 1/dt, which is what this used to do and which reads HIGH: 1/dt
    is convex, so by Jensen's inequality E[1/dt] > 1/E[dt] and the short gaps dominate. Measured
    against the robot's d435 IMU at a steady 200 Hz, the EMA showed 15028, 3580 and 691 Hz in
    consecutive windows.
    """

    window_start: float = field(default_factory=time.monotonic)
    frames: int = 0
    byte_count: int = 0
    fps: float = 0.0
    bytes_per_sec: float = 0.0
    # Until the first window closes there is no rate to show. Printing the 0.0 it starts at reads
    # as "stalled", and a stream whose discovery completed a second later than its neighbours
    # would sit there in red while its frames were arriving fine.
    measured: bool = False

    def update(self, n_bytes: int) -> None:
        self.frames += 1
        self.byte_count += n_bytes
        now = time.monotonic()
        elapsed = now - self.window_start
        if elapsed >= 1.0:
            self.fps = self.frames / elapsed
            self.bytes_per_sec = self.byte_count / elapsed
            self.frames = 0
            self.byte_count = 0
            self.window_start = now
            self.measured = True


def to_display(img: np.ndarray) -> np.ndarray:
    """Return an 8-bit 3-channel BGR image, whatever arrived.

    16UC1 depth is min/max normalised and colour-mapped; single channel is expanded so one upload
    path serves every stream.
    """
    if img.dtype == np.uint16 or (img.ndim == 2 and img.dtype != np.uint8):
        lo = float(img.min())
        hi = float(img.max())
        if hi <= lo:
            hi = lo + 1.0
        scaled = ((img.astype(np.float32) - lo) * (255.0 / (hi - lo))).astype(np.uint8)
        return cv2.applyColorMap(scaled, cv2.COLORMAP_JET)
    if img.ndim == 2:
        return cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
    if img.shape[2] == 4:
        return cv2.cvtColor(img, cv2.COLOR_BGRA2BGR)
    return img


@dataclass
class Stream:
    name: str
    enabled: bool = True
    compressed: bool = False  # replaced in __post_init__ from the topic name
    raw_sub: object | None = None
    compressed_sub: object | None = None
    stats: Stats = field(default_factory=Stats)
    detail: str = ""
    frame: np.ndarray | None = None  # BGR uint8, newest
    lock: threading.Lock = field(default_factory=threading.Lock)
    # GUI-thread only.
    texture_tag: str = ""
    tex_w: int = 0
    tex_h: int = 0

    def __post_init__(self) -> None:
        self.compressed = default_compressed(self.name)

    def snapshot(self) -> tuple[np.ndarray | None, str, float, float, bool]:
        with self.lock:
            return (
                self.frame,
                self.detail,
                self.stats.fps,
                self.stats.bytes_per_sec,
                self.stats.measured,
            )

    def record(self, frame: np.ndarray | None, detail: str, n_bytes: int) -> None:
        with self.lock:
            if frame is not None:
                self.frame = frame
            self.detail = detail
            self.stats.update(n_bytes)

    def clear(self) -> None:
        with self.lock:
            self.frame = None
            self.detail = ""
            self.stats = Stats()


@dataclass
class ImuState:
    name: str = IMU_TOPIC
    enabled: bool = True
    subscriber: object | None = None
    stats: Stats = field(default_factory=Stats)
    accel: tuple[float, float, float] = (0.0, 0.0, 0.0)
    gyro: tuple[float, float, float] = (0.0, 0.0, 0.0)
    stamp_ns: int = 0
    latency_ms: float = 0.0
    frame_id: str = ""
    has_data: bool = False
    lock: threading.Lock = field(default_factory=threading.Lock)


def on_raw(stream: Stream, msg) -> None:  # noqa: ANN001 - SDK binding type
    data = np.frombuffer(bytes(msg.data()), dtype=np.uint8)
    encoding = msg.encoding()
    width, height = int(msg.width()), int(msg.height())
    detail = f"{encoding} {width}x{height}"
    if width == 0 or height == 0:
        stream.record(None, detail + " (zero dimension)", data.size)
        return
    channels = {"16UC1": 1, "mono8": 1, "8UC1": 1, "bgr8": 3, "rgb8": 3, "8UC3": 3}.get(encoding)
    if channels is None:
        stream.record(None, detail + " (unsupported encoding)", data.size)
        return
    dtype = np.uint16 if encoding == "16UC1" else np.uint8
    expected = width * height * channels * np.dtype(dtype).itemsize
    if data.size < expected:
        # Showing it would paint garbage rows.
        stream.record(None, detail + " (short buffer)", data.size)
        return
    img = data[:expected].view(dtype).reshape(height, width, channels).squeeze()
    shown = to_display(img)
    if encoding == "rgb8":
        shown = cv2.cvtColor(shown, cv2.COLOR_RGB2BGR)
    stream.record(shown, detail, data.size)


def on_compressed(stream: Stream, msg) -> None:  # noqa: ANN001 - SDK binding type
    data = np.frombuffer(bytes(msg.data()), dtype=np.uint8)
    if data.size == 0:
        return
    detail = msg.format()
    img = cv2.imdecode(data, cv2.IMREAD_UNCHANGED)
    if img is None:
        stream.record(None, detail + " (decode failed)", data.size)
        return
    # png16 depth decodes to 16-bit and mono jpeg to one channel, so both take the same
    # normalise / expand pass as the raw lane rather than being uploaded directly.
    shown = to_display(img)
    stream.record(shown, f"{detail} {img.shape[1]}x{img.shape[0]}", data.size)


def apply_subscription(stream: Stream) -> None:
    """Create or tear down the subscription the checkboxes currently ask for.

    Called from the GUI thread only, and never while holding stream.lock: Subscriber.stop()
    blocks until any in-flight callback has returned, so holding the lock would make it wait for
    a callback that is waiting for the lock. Tearing down first keeps the two lanes mutually
    exclusive, which is the entire point of the compressed toggle: not paying for both.
    """
    want_raw = stream.enabled and not stream.compressed
    want_compressed = stream.enabled and stream.compressed

    if not want_raw and stream.raw_sub is not None:
        stream.raw_sub.stop()
        stream.raw_sub = None
    if not want_compressed and stream.compressed_sub is not None:
        stream.compressed_sub.stop()
        stream.compressed_sub = None

    if want_raw and stream.raw_sub is None:
        sub = igc_sdk.ImageSubscriber(stream.name, igc_sdk.QosProfile.SensorData())
        if sub.init(lambda msg, s=stream: on_raw(s, msg)):
            stream.raw_sub = sub
        else:
            print(f"Failed to subscribe: {stream.name}", file=sys.stderr)
            stream.enabled = False
    if want_compressed and stream.compressed_sub is None:
        sub = igc_sdk.CompressedImageSubscriber(
            stream.name + "/compressed", igc_sdk.QosProfile.SensorData()
        )
        if sub.init(lambda msg, s=stream: on_compressed(s, msg)):
            stream.compressed_sub = sub
        else:
            print(f"Failed to subscribe: {stream.name}/compressed", file=sys.stderr)
            stream.enabled = False


def apply_imu_subscription(imu: ImuState) -> None:
    if not imu.enabled and imu.subscriber is not None:
        imu.subscriber.stop()
        imu.subscriber = None
        with imu.lock:
            imu.has_data = False
            imu.stats = Stats()
        return
    if imu.enabled and imu.subscriber is None:
        sub = igc_sdk.ImuSubscriber(imu.name, igc_sdk.QosProfile.SensorData())

        def cb(msg, state=imu):  # noqa: ANN001 - SDK binding type
            rx_ns = time.time_ns()
            with state.lock:
                accel = msg.linear_acceleration()
                gyro = msg.angular_velocity()
                state.accel = (accel.x(), accel.y(), accel.z())
                state.gyro = (gyro.x(), gyro.y(), gyro.z())
                stamp = msg.header().stamp()
                state.stamp_ns = stamp.sec() * 1_000_000_000 + stamp.nanosec()
                # Measured on arrival, not at draw time, so the refresh rate does not leak in.
                state.latency_ms = 0.0 if state.stamp_ns == 0 else (rx_ns - state.stamp_ns) / 1e6
                state.frame_id = msg.header().frame_id()
                state.has_data = True
                state.stats.update(320)  # approx. serialized sensor_msgs/Imu size

        if sub.init(cb):
            imu.subscriber = sub
        else:
            print(f"Failed to subscribe: {imu.name}", file=sys.stderr)
            imu.enabled = False


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "topics",
        nargs="*",
        default=None,
        help="Image topics to subscribe to (default: the camera's seven)",
    )
    parser.add_argument(
        "--domain-id", type=int, default=0, help="Cyclone DDS domain id (default: 0)"
    )
    parser.add_argument(
        "--namespace", default="", help="Topic namespace prefix (e.g. igris_c_IG01)"
    )
    parser.add_argument(
        "--dds-topic-naming",
        default="native",
        choices=igc_sdk.topic_naming_names(),
        help="Wire layout for topic names; must match the robot's igris_c.network.transport "
        "(native for cyclonedds_native, ros_compatible for cyclonedds_ros_compatible).",
    )
    return parser


def main() -> int:
    args = build_arg_parser().parse_args()
    topics = args.topics or DEFAULT_TOPICS

    print_banner("Sensor Viewer", args.domain_id, args.namespace, args.dds_topic_naming)

    channel = igc_sdk.ChannelFactory.Instance()
    channel.Init(
        args.domain_id, args.namespace, "", igc_sdk.parse_topic_naming(args.dds_topic_naming)
    )
    if not channel.IsInitialized():
        print("Failed to initialize ChannelFactory", file=sys.stderr)
        return 1

    streams = [Stream(name=t) for t in topics]
    imu = ImuState()
    stop_event = threading.Event()

    def handle_signal(_signum, _frame) -> None:
        stop_event.set()

    signal.signal(signal.SIGINT, handle_signal)
    signal.signal(signal.SIGTERM, handle_signal)

    for stream in streams:
        apply_subscription(stream)
    apply_imu_subscription(imu)

    screen_w, screen_h = detect_screen_size()
    ui_scale = compute_ui_scale(screen_w, screen_h)

    dpg.create_context()
    dpg.create_viewport(
        title="IGRIS-C SDK Sensor Viewer (Python)",
        width=max(1280, int(screen_w * 0.9)),
        height=max(800, int(screen_h * 0.9)),
        min_width=900,
        min_height=600,
    )
    # Bitmap-scale fonts to match the global UI scale. Crisp would need a TTF at the target size;
    # this is good enough for an example tool, and matches sdk_gui_client.
    dpg.set_global_font_scale(ui_scale)

    # Textures are added lazily, once a stream's frame size is known, so the registry just has to
    # exist and be addressable by tag.
    dpg.add_texture_registry(tag="tex_registry")

    with dpg.window(tag="main_window"):
        with dpg.group(horizontal=True):
            dpg.add_child_window(tag="images", width=-int(PANEL_WIDTH * ui_scale), autosize_y=True)
            with dpg.child_window(tag="panel", width=-1, autosize_y=True):
                dpg.add_text("Streams")
                dpg.add_separator()
                dpg.add_text("Unticking stops the subscription,", color=(150, 150, 150))
                dpg.add_text("so the data stops crossing the link.", color=(150, 150, 150))
                dpg.add_spacer(height=4)
                for stream in streams:
                    dpg.add_checkbox(
                        label=stream.name,
                        tag=f"en::{stream.name}",
                        default_value=stream.enabled,
                        callback=lambda s, v, u: _toggle_enabled(u, v),
                        user_data=stream,
                    )
                    with dpg.group(indent=20):
                        dpg.add_checkbox(
                            label="compressed",
                            tag=f"cmp::{stream.name}",
                            default_value=stream.compressed,
                            callback=lambda s, v, u: _toggle_compressed(u, v),
                            user_data=stream,
                        )
                dpg.add_spacer(height=4)
                dpg.add_separator()
                dpg.add_text("Total: -", tag="total_rate")
                dpg.add_spacer(height=4)
                dpg.add_text("IMU")
                dpg.add_separator()
                dpg.add_checkbox(
                    label=IMU_TOPIC,
                    tag="en::imu",
                    default_value=imu.enabled,
                    callback=lambda s, v, u: _toggle_imu(u, v),
                    user_data=imu,
                )
                dpg.add_text("no data", tag="imu_text")
                dpg.add_spacer(height=4)
                dpg.add_separator()
                dpg.add_text(
                    f"domain {args.domain_id} / ns {args.namespace or '(none)'}",
                    color=(130, 130, 130),
                )
                dpg.add_text(f"naming: {args.dds_topic_naming}", color=(130, 130, 130))

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.set_primary_window("main_window", True)

    layout_signature: tuple = ()

    try:
        while dpg.is_dearpygui_running() and not stop_event.is_set():
            _pump_textures(streams)
            layout_signature = _rebuild_grid_if_needed(streams, layout_signature, ui_scale)
            _refresh_labels(streams, imu)
            dpg.render_dearpygui_frame()
    finally:
        # Subscribers first: stop() must run before the process tears down, and it blocks until
        # any in-flight callback has returned, so nothing can still be writing into a Stream.
        for stream in streams:
            stream.enabled = False
            apply_subscription(stream)
        imu.enabled = False
        apply_imu_subscription(imu)
        dpg.destroy_context()

    return 0


def _toggle_enabled(stream: Stream, value: bool) -> None:
    stream.enabled = value
    apply_subscription(stream)
    stream.clear()


def _toggle_compressed(stream: Stream, value: bool) -> None:
    stream.compressed = value
    apply_subscription(stream)
    stream.clear()


def _toggle_imu(imu: ImuState, value: bool) -> None:
    imu.enabled = value
    apply_imu_subscription(imu)


def _pump_textures(streams: list[Stream]) -> None:
    """Push the newest frame into each stream's texture, creating it on first use.

    DearPyGui textures are fixed-size, so a size change means a new texture - which is why the
    grid below keys its rebuild on the texture size as well as on the visible set.
    """
    for stream in streams:
        frame, _, _, _, _ = stream.snapshot()
        if frame is None:
            continue
        height, width = frame.shape[:2]
        if stream.texture_tag and (width != stream.tex_w or height != stream.tex_h):
            dpg.delete_item(stream.texture_tag)
            stream.texture_tag = ""
        if not stream.texture_tag:
            tag = f"tex::{stream.name}"
            rgba = np.zeros((height, width, 4), dtype=np.float32)
            rgba[:, :, 3] = 1.0
            dpg.add_raw_texture(
                width,
                height,
                rgba.ravel(),
                format=dpg.mvFormat_Float_rgba,
                tag=tag,
                parent="tex_registry",
            )
            stream.texture_tag = tag
            stream.tex_w, stream.tex_h = width, height
        rgba = np.empty((height, width, 4), dtype=np.float32)
        rgba[:, :, 0] = frame[:, :, 2] / 255.0  # BGR -> RGB
        rgba[:, :, 1] = frame[:, :, 1] / 255.0
        rgba[:, :, 2] = frame[:, :, 0] / 255.0
        rgba[:, :, 3] = 1.0
        dpg.set_value(stream.texture_tag, rgba.ravel())


def _rebuild_grid_if_needed(streams: list[Stream], signature: tuple, ui_scale: float) -> tuple:
    """Lay the visible tiles out in rows of three, giving wide frames two columns.

    DearPyGui is retained-mode, so the grid is rebuilt only when its shape changes - a stream
    toggled, or a frame size that flips a tile between one and two columns. A frame twice as wide
    as it is tall (the stereo pair is 1280x480) is unreadable squeezed into one column. Decided by
    shape rather than by topic name, so any other side-by-side stream gets the same treatment.
    """
    spans = {}
    for stream in streams:
        wide = stream.tex_w > 0 and stream.tex_h > 0 and (stream.tex_w / stream.tex_h) >= 2.0
        spans[stream.name] = 2 if wide else 1

    # Tile size is baked into the widgets at build time, so the available width has to be part of
    # the signature - otherwise a rebuild that happened before the child window had been laid out
    # (its rect reads 0 on the first frames) leaves thumbnail-sized tiles for the rest of the run,
    # and resizing the window never corrects them. Bucketed so a drag does not rebuild every frame.
    avail = dpg.get_item_rect_size("images")[0] - int(20 * ui_scale)
    if avail < 200:
        # Not laid out yet: fall back to what the viewport implies.
        avail = max(
            400, dpg.get_viewport_client_width() - int(PANEL_WIDTH * ui_scale) - int(40 * ui_scale)
        )
    new_signature = tuple((s.name, s.enabled, spans[s.name]) for s in streams) + (int(avail) // 40,)
    if new_signature == signature:
        return signature

    dpg.delete_item("images", children_only=True)

    unit = max(120, (int(avail) - 8 * (COLUMNS - 1)) // COLUMNS)

    column = 0
    row_tag = None
    any_shown = False
    for stream in streams:
        if not stream.enabled:
            continue
        any_shown = True
        span = spans[stream.name]
        if column + span > COLUMNS:
            column = 0
            row_tag = None
        if row_tag is None:
            row_tag = dpg.add_group(horizontal=True, parent="images")
        tile_w = unit * span + 8 * (span - 1)
        with dpg.group(parent=row_tag):
            dpg.add_text(stream.name, tag=f"name::{stream.name}")
            dpg.add_text("measuring...", tag=f"rate::{stream.name}", color=(150, 150, 150))
            if stream.texture_tag:
                aspect = stream.tex_h / stream.tex_w
                dpg.add_image(stream.texture_tag, width=tile_w, height=int(tile_w * aspect))
            else:
                # A subscribed stream with no frame yet is the interesting case (wrong domain,
                # wrong naming, camera off), so say so instead of leaving a gap.
                dpg.add_text("waiting for data...", color=(150, 150, 150))
            dpg.add_text("-", tag=f"detail::{stream.name}", color=(130, 130, 130))
        column += span
        if column >= COLUMNS:
            column = 0
            row_tag = None

    if not any_shown:
        dpg.add_text(
            "No streams enabled. Tick one in the panel on the right.",
            parent="images",
            color=(150, 150, 150),
        )
    return new_signature


def _refresh_labels(streams: list[Stream], imu: ImuState) -> None:
    total_bps = 0.0
    for stream in streams:
        _, detail, fps, bps, measured = stream.snapshot()
        total_bps += bps
        if not dpg.does_item_exist(f"rate::{stream.name}"):
            continue
        if not measured:
            dpg.set_value(f"rate::{stream.name}", "measuring...")
            dpg.configure_item(f"rate::{stream.name}", color=(150, 150, 150))
        else:
            dpg.set_value(f"rate::{stream.name}", f"{fps:.1f} FPS, {format_rate(bps)}")
            # Red means STALLED, not slow: a viewer cannot know the configured rate, and
            # camera.yaml runs most streams at 15 fps, so a "below 20" threshold painted healthy
            # streams red permanently.
            dpg.configure_item(
                f"rate::{stream.name}", color=(255, 90, 90) if fps <= 1.0 else (150, 255, 150)
            )
        dpg.set_value(f"detail::{stream.name}", detail or "-")

    dpg.set_value("total_rate", f"Total: {format_rate(total_bps)}")

    with imu.lock:
        if not imu.enabled:
            text = "disabled"
        elif not imu.has_data:
            text = "no data"
        elif not imu.stats.measured:
            text = "measuring..."
        else:
            latency = "stamp unset" if imu.stamp_ns == 0 else f"latency: {imu.latency_ms:.1f} ms"
            text = (
                f"{imu.stats.fps:.1f} Hz\n"
                f"accel [m/s^2]: {imu.accel[0]:.3f} {imu.accel[1]:.3f} {imu.accel[2]:.3f}\n"
                f"gyro  [rad/s]: {imu.gyro[0]:.3f} {imu.gyro[1]:.3f} {imu.gyro[2]:.3f}\n"
                f"{latency}\n"
                f"frame_id: {imu.frame_id or '(none)'}"
            )
    dpg.set_value("imu_text", text)


if __name__ == "__main__":
    sys.exit(main())
