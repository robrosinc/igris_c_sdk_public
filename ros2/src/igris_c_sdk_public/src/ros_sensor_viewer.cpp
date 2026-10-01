/**
 * @file ros_sensor_viewer.cpp
 * @brief Camera stream viewer for the ROS 2 lane, mirroring
 *        cyclonedds/cyclonedds_sensor_viewer.cpp: ONE window, images on the left and the
 *        subscription controls on the right.
 *
 * This example demonstrates:
 * - Subscribing to the camera's standard sensor_msgs on both wire forms
 *   (Image on "<topic>", CompressedImage on "<topic>/compressed")
 * - Creating and dropping rclcpp subscriptions at run time, so an unwatched stream stops
 *   crossing the network instead of being received and discarded
 * - Receiving on a dedicated executor thread, which is what keeps a 200 Hz topic at 200 Hz
 *   while the render loop runs at its own pace
 *
 * GUI Layout:
 * - Left panel: image tiles, three per row, a wide stereo pair spanning two
 * - Right panel: per-stream enable + "compressed" lane toggles, aggregate rate, IMU readout
 *
 * Usage: ros2 run igris_c_sdk ros_sensor_viewer --ros-args -p robot_namespace:=<ns>
 */
// Why the panel is not decoration: this used to open a separate OpenCV window per stream -
// eight windows to arrange by hand every run - and it subscribed to all of them whether you
// were looking or not. Unticking a stream drops the rclcpp subscription and the data stops
// crossing the network: measured on the link, 22.0 MB/s with every stream on against 0.3 MB/s
// with only the IMU.
//
// The "compressed" toggle picks which ONE of the two lanes is subscribed, so the panel also
// answers "is this stream raw or encoded right now" without reading camera.yaml.
//
// Kept separate from ros_sensor_listener on purpose: this target needs a display and the GUI
// stack, while the dependency-light text probe keeps building everywhere. It no longer needs
// OpenCV highgui though - ImGui draws the frames, so core/imgproc/imgcodecs is enough and the
// target builds in the igris-c-core env, which grafts exactly those three.

#include "common.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using igris_c_sdk_examples::g_running;
using igris_c_sdk_examples::signalHandler;

namespace {

// UI scale, same rule and same floor as ros_gui_client: ImGui's default font is sized in raw
// pixels, so on a 4K panel an unscaled window renders text too small to read from a desk. The
// floor of 1.15 is what makes it legible at 1080p too; the cap keeps a very wide monitor from
// turning the panel into a billboard.
constexpr float kBaseWindowWidth  = 1800.0f;
constexpr float kBaseWindowHeight = 900.0f;

float clamp_ui_scale(float scale) { return std::clamp(scale, 1.15f, 2.5f); }

float compute_ui_scale(float width, float height) { return clamp_ui_scale(std::min(width / kBaseWindowWidth, height / kBaseWindowHeight)); }

// Rate over a fixed window: count messages, divide by elapsed. NOT an exponentially smoothed
// 1/dt -- that is convex, so by Jensen's inequality E[1/dt] > 1/E[dt] and the short gaps
// dominate the average. Measured against the robot's d435 IMU while it published a steady
// 200 Hz, an EMA showed 15028, 3580 and 691 Hz in consecutive windows.
struct StreamStats {
    std::chrono::steady_clock::time_point window_start = std::chrono::steady_clock::now();
    std::uint64_t frames                               = 0;  // in the current window
    std::uint64_t bytes                                = 0;
    double fps                                         = 0.0;  // last completed window
    double bytes_per_sec                               = 0.0;
    // Until the first window closes there is no rate to show. Printing the 0.0 it starts
    // at reads as "stalled", which is exactly what the colour below means - and a stream
    // whose discovery completed a second later than its neighbours would sit there in red
    // while its frames were arriving fine. Observed on d435_color.
    bool measured = false;
};

void update_stats(StreamStats &stats, std::size_t bytes) {
    stats.frames += 1;
    stats.bytes += bytes;
    const auto now     = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - stats.window_start).count();
    if (elapsed >= 1000) {
        const double seconds = static_cast<double>(elapsed) / 1000.0;
        stats.fps            = static_cast<double>(stats.frames) / seconds;
        stats.bytes_per_sec  = static_cast<double>(stats.bytes) / seconds;
        stats.frames         = 0;
        stats.bytes          = 0;
        stats.window_start   = now;
        stats.measured       = true;
    }
}

std::string format_rate(double bytes_per_sec) {
    std::ostringstream oss;
    if (bytes_per_sec > 1024.0 * 1024.0) {
        oss << std::fixed << std::setprecision(1) << (bytes_per_sec / (1024.0 * 1024.0)) << " MB/s";
    } else if (bytes_per_sec > 1024.0) {
        oss << std::fixed << std::setprecision(1) << (bytes_per_sec / 1024.0) << " KB/s";
    } else {
        oss << std::fixed << std::setprecision(0) << bytes_per_sec << " B/s";
    }
    return oss.str();
}

// Map a sensor_msgs/Image encoding onto an OpenCV type. Only the encodings igris_c_camera
// actually emits on the raw lane are handled; anything else is reported as unsupported instead
// of being reinterpreted as garbage rows.
bool cv_type_for(const std::string &encoding, int &cv_type, std::size_t &elem_size) {
    if (encoding == "16UC1") {
        cv_type   = CV_16UC1;
        elem_size = 2;
        return true;
    }
    if (encoding == "mono8" || encoding == "8UC1") {
        cv_type   = CV_8UC1;
        elem_size = 1;
        return true;
    }
    if (encoding == "bgr8" || encoding == "rgb8" || encoding == "8UC3") {
        cv_type   = CV_8UC3;
        elem_size = 3;
        return true;
    }
    return false;
}

// Produce something a GL texture can take: 8-bit, 3 channels, BGR order. 16UC1 depth is
// min/max normalised and colour-mapped; single-channel is expanded so one upload path serves
// every stream; rgb8 is byte-swapped because the upload below declares GL_BGR.
cv::Mat to_display(const cv::Mat &raw, int cv_type, bool swap_rb) {
    if (cv_type == CV_16UC1) {
        double min_val = 0.0;
        double max_val = 0.0;
        cv::minMaxLoc(raw, &min_val, &max_val);
        if (max_val <= min_val) {
            max_val = min_val + 1.0;
        }
        cv::Mat depth_u8;
        raw.convertTo(depth_u8, CV_8UC1, 255.0 / (max_val - min_val), -min_val * 255.0 / (max_val - min_val));
        cv::Mat depth_vis;
        cv::applyColorMap(depth_u8, depth_vis, cv::COLORMAP_JET);
        return depth_vis;
    }
    if (cv_type == CV_8UC1) {
        cv::Mat bgr;
        cv::cvtColor(raw, bgr, cv::COLOR_GRAY2BGR);
        return bgr;
    }
    if (swap_rb) {
        cv::Mat bgr;
        cv::cvtColor(raw, bgr, cv::COLOR_RGB2BGR);
        return bgr;
    }
    return raw.clone();
}

// Which lane a stream starts on, matching what igris_c_camera publishes by default.
//
// camera.yaml ships `depth_format: raw` ("geometry consumers need raw") and publishes both IR
// imagers as raw mono8, while colour, eyes_stereo and the hand cameras are `format: jpeg`.
// Starting every stream on /compressed would therefore leave depth and IR blank on launch -
// subscribed to a topic the camera does not write. Matched on the topic NAME so a custom topic
// list gets the same treatment.
bool default_compressed(const std::string &topic) {
    return topic.find("depth") == std::string::npos && topic.find("_ir_") == std::string::npos;
}

// One stream: the two possible subscriptions, the newest frame, and its GL texture.
struct Stream {
    explicit Stream(std::string topic, std::string resolved)
        : name(std::move(topic)), resolved_name(std::move(resolved)), compressed(default_compressed(name)) {}

    std::string name;
    std::string resolved_name;  // what actually goes on the wire, namespace applied
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr raw_sub;
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr compressed_sub;

    bool enabled    = true;  // panel checkbox: is anything subscribed at all
    bool compressed = true;  // panel checkbox: which of the two lanes; set from the topic name

    StreamStats stats;
    cv::Mat latest_image;  // 8UC3 BGR, ready to upload
    std::string detail;    // encoding/format + dimensions, from the last message
    bool has_frame = false;
    mutable std::mutex mutex;

    // Touched only by the render thread, so no lock.
    GLuint texture = 0;
    int tex_width  = 0;
    int tex_height = 0;
};

void record_frame(Stream &stream, cv::Mat image, std::string detail, std::size_t bytes) {
    const std::lock_guard<std::mutex> lock(stream.mutex);
    stream.latest_image = std::move(image);
    stream.has_frame    = true;
    stream.detail       = std::move(detail);
    update_stats(stream.stats, bytes);
}

// A message that arrived but cannot be shown still counts: a stream publishing something this
// example does not understand must read as alive, with the reason, not as absent.
void record_reject(Stream &stream, std::string detail, std::size_t bytes) {
    const std::lock_guard<std::mutex> lock(stream.mutex);
    stream.detail = std::move(detail);
    update_stats(stream.stats, bytes);
}

void on_raw_image(Stream &stream, const sensor_msgs::msg::Image &msg) {
    int cv_type           = -1;
    std::size_t elem_size = 0;
    std::ostringstream detail;
    detail << msg.encoding << " " << msg.width << "x" << msg.height;

    if (!cv_type_for(msg.encoding, cv_type, elem_size)) {
        detail << " (unsupported encoding)";
        record_reject(stream, detail.str(), msg.data.size());
        return;
    }
    // Zero dimensions would pass the size check below and then divide by zero when deriving the
    // row stride.
    if (msg.width == 0 || msg.height == 0) {
        detail << " (zero dimension)";
        record_reject(stream, detail.str(), msg.data.size());
        return;
    }
    const std::size_t expected = static_cast<std::size_t>(msg.width) * static_cast<std::size_t>(msg.height) * elem_size;
    if (msg.data.size() < expected) {
        detail << " (short buffer)";  // showing it would paint garbage rows
        record_reject(stream, detail.str(), msg.data.size());
        return;
    }

    const cv::Mat raw(static_cast<int>(msg.height), static_cast<int>(msg.width), cv_type, const_cast<std::uint8_t *>(msg.data.data()),
                      msg.step ? msg.step : expected / msg.height);
    record_frame(stream, to_display(raw, cv_type, msg.encoding == "rgb8"), detail.str(), msg.data.size());
}

void on_compressed_image(Stream &stream, const sensor_msgs::msg::CompressedImage &msg) {
    if (msg.data.empty()) {
        return;
    }
    std::ostringstream detail;
    detail << msg.format;

    const cv::Mat encoded(1, static_cast<int>(msg.data.size()), CV_8UC1, const_cast<std::uint8_t *>(msg.data.data()));
    cv::Mat img = cv::imdecode(encoded, cv::IMREAD_UNCHANGED);
    if (img.empty()) {
        detail << " (decode failed)";
        record_reject(stream, detail.str(), msg.data.size());
        return;
    }
    detail << " " << img.cols << "x" << img.rows;
    // png16 depth decodes to 16UC1 and mono jpeg to 8UC1, so both take the same normalise /
    // expand pass as the raw lane rather than being uploaded directly.
    cv::Mat shown = (img.channels() == 1) ? to_display(img, img.type() == CV_16UC1 ? CV_16UC1 : CV_8UC1, false) : img;
    record_frame(stream, std::move(shown), detail.str(), msg.data.size());
}

// Drop what the previous lane left behind, so a toggled stream does not keep showing a frame
// that is no longer arriving, and does not average two lanes into one rate.
void clear_stream_data(Stream &stream) {
    const std::lock_guard<std::mutex> lock(stream.mutex);
    stream.has_frame = false;
    stream.latest_image.release();
    stream.detail.clear();
    stream.stats = StreamStats{};
}

// Create or drop the subscription the checkboxes currently ask for. Dropping first keeps the
// two lanes mutually exclusive, which is the entire point of the compressed toggle: not paying
// for both.
//
// SensorDataQoS matches the publisher; a reliable subscriber would not match the camera's
// best-effort writer and would receive nothing at all.
void apply_subscription(const rclcpp::Node::SharedPtr &node, Stream &stream) {
    const bool want_raw        = stream.enabled && !stream.compressed;
    const bool want_compressed = stream.enabled && stream.compressed;

    if (!want_raw && stream.raw_sub) {
        stream.raw_sub.reset();
    }
    if (!want_compressed && stream.compressed_sub) {
        stream.compressed_sub.reset();
    }

    if (want_raw && !stream.raw_sub) {
        stream.raw_sub = node->create_subscription<sensor_msgs::msg::Image>(
            stream.resolved_name, rclcpp::SensorDataQoS(), [s = &stream](const sensor_msgs::msg::Image &msg) { on_raw_image(*s, msg); });
    }
    if (want_compressed && !stream.compressed_sub) {
        stream.compressed_sub = node->create_subscription<sensor_msgs::msg::CompressedImage>(
            stream.resolved_name + "/compressed", rclcpp::SensorDataQoS(),
            [s = &stream](const sensor_msgs::msg::CompressedImage &msg) { on_compressed_image(*s, msg); });
    }
}

struct ImuStream {
    explicit ImuStream(std::string topic, std::string resolved) : name(std::move(topic)), resolved_name(std::move(resolved)) {}

    std::string name;
    std::string resolved_name;
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr subscriber;
    bool enabled = true;

    StreamStats stats;
    std::array<double, 3> accel = {};
    std::array<double, 3> gyro  = {};
    std::uint64_t stamp_ns      = 0;    // 0 => publisher left the header stamp unset
    double latency_ms           = 0.0;  // stamp -> arrival, measured in the callback
    std::string frame_id;
    bool has_data = false;
    mutable std::mutex mutex;
};

void apply_imu_subscription(const rclcpp::Node::SharedPtr &node, ImuStream &imu) {
    if (!imu.enabled && imu.subscriber) {
        imu.subscriber.reset();
        const std::lock_guard<std::mutex> lock(imu.mutex);
        imu.has_data = false;
        imu.stats    = StreamStats{};
        return;
    }
    if (imu.enabled && !imu.subscriber) {
        imu.subscriber = node->create_subscription<sensor_msgs::msg::Imu>(
            imu.resolved_name, rclcpp::SensorDataQoS(), [p = &imu](const sensor_msgs::msg::Imu &msg) {
                const auto rx_ns = static_cast<std::uint64_t>(
                    std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
                const std::lock_guard<std::mutex> lock(p->mutex);
                p->accel    = {msg.linear_acceleration.x, msg.linear_acceleration.y, msg.linear_acceleration.z};
                p->gyro     = {msg.angular_velocity.x, msg.angular_velocity.y, msg.angular_velocity.z};
                p->stamp_ns = static_cast<std::uint64_t>(msg.header.stamp.sec) * 1000000000ULL + msg.header.stamp.nanosec;
                // Measured on arrival, not at draw time, so the refresh rate does not leak in.
                p->latency_ms = (p->stamp_ns == 0) ? 0.0 : (static_cast<double>(rx_ns) - static_cast<double>(p->stamp_ns)) / 1e6;
                p->frame_id   = msg.header.frame_id;
                p->has_data   = true;
                update_stats(p->stats, 320);  // approx. serialized sensor_msgs/Imu size
            });
    }
}

// Upload the newest frame into the stream's texture. Allocates on first use and whenever the
// frame size changes; otherwise it is a sub-image update, which is the cheap path.
void upload_texture(Stream &stream) {
    cv::Mat frame;
    {
        const std::lock_guard<std::mutex> lock(stream.mutex);
        if (!stream.has_frame || stream.latest_image.empty()) {
            return;
        }
        frame = stream.latest_image.clone();
    }
    if (!frame.isContinuous()) {
        frame = frame.clone();
    }

    if (stream.texture == 0) {
        glGenTextures(1, &stream.texture);
        glBindTexture(GL_TEXTURE_2D, stream.texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        glBindTexture(GL_TEXTURE_2D, stream.texture);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (frame.cols != stream.tex_width || frame.rows != stream.tex_height) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
        stream.tex_width  = frame.cols;
        stream.tex_height = frame.rows;
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
    }
}

}  // namespace

int main(int argc, char **argv) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    igris_c_sdk_examples::initRclcpp(argc, argv);
    auto node                         = rclcpp::Node::make_shared("igris_c_ros_sensor_viewer");
    const std::string robot_namespace = igris_c_sdk_examples::resolveRobotNamespace(node);

    const std::vector<std::string> image_topics = {
        "sensor/d435_color",  "sensor/d435_depth", "sensor/d435_ir_left", "sensor/d435_ir_right",
        "sensor/eyes_stereo", "sensor/left_hand",  "sensor/right_hand",
    };
    const std::string imu_topic = "sensor/d435_imu";

    // Echo the resolved wiring before subscribing: an empty namespace or a mismatched
    // ROS_DOMAIN_ID leaves this viewer silent while nothing looks broken.
    igris_c_sdk_examples::printBanner("Sensor Viewer", robot_namespace);

    std::vector<std::unique_ptr<Stream>> streams;
    streams.reserve(image_topics.size());
    for (const std::string &topic : image_topics) {
        streams.push_back(std::make_unique<Stream>(topic, igris_c_sdk_examples::resolveRosTopic(robot_namespace, topic)));
    }
    ImuStream imu(imu_topic, igris_c_sdk_examples::resolveRosTopic(robot_namespace, imu_topic));

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        rclcpp::shutdown();
        return 1;
    }
    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Sized off the monitor rather than a fixed pixel count: seven camera tiles plus the panel
    // is unusable in a small default window on a high-resolution display.
    int initial_width  = 1280;
    int initial_height = 800;
    if (GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor()) {
        int work_x = 0, work_y = 0, work_width = 0, work_height = 0;
        glfwGetMonitorWorkarea(primary_monitor, &work_x, &work_y, &work_width, &work_height);
        if (work_width > 0 && work_height > 0) {
            initial_width  = std::max(initial_width, static_cast<int>(static_cast<float>(work_width) * 0.9f));
            initial_height = std::max(initial_height, static_cast<int>(static_cast<float>(work_height) * 0.9f));
        }
    }

    GLFWwindow *window = glfwCreateWindow(initial_width, initial_height, "IGRIS-C SDK Sensor Viewer (ROS 2)", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        rclcpp::shutdown();
        return 1;
    }
    glfwSetWindowSizeLimits(window, 900, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO &io = ImGui::GetIO();
    // Re-applied from this pristine copy on every change: ScaleAllSizes compounds, so scaling an
    // already-scaled style grows the padding without bound as the window is resized.
    const ImGuiStyle base_style = ImGui::GetStyle();
    float applied_ui_scale      = 0.0f;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    for (auto &stream : streams) {
        apply_subscription(node, *stream);
    }
    apply_imu_subscription(node, imu);

    // Receive runs on its own thread, NOT inside the render loop.
    //
    // rclcpp::spin_some() executes the work that was ready when it collected, which for a single
    // subscription is ONE message per call. Calling it once per render iteration therefore caps
    // every topic at the iteration rate. Measured against the robot's d435 IMU (publishing
    // 204 Hz) with the old cv::imshow loop: 82 Hz at best, and 33 Hz at a 30 ms loop - exactly
    // 1/period. With this thread it reads ~200 Hz and barely moves when the loop slows down.
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    std::thread spin_thread([&executor]() { executor.spin(); });

    constexpr float kBasePanelWidth = 340.0f;  // scaled by ui_scale below, like every other size

    while (!glfwWindowShouldClose(window) && rclcpp::ok() && g_running.load(std::memory_order_relaxed)) {
        glfwPollEvents();

        for (auto &stream : streams) {
            upload_texture(*stream);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const float ui_scale = compute_ui_scale(io.DisplaySize.x, io.DisplaySize.y);
        if (std::abs(ui_scale - applied_ui_scale) > 0.001f) {
            ImGui::GetStyle() = base_style;
            ImGui::GetStyle().ScaleAllSizes(ui_scale);
            io.FontGlobalScale = ui_scale;
            applied_ui_scale   = ui_scale;
        }

        int display_w = 0;
        int display_h = 0;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        // One full-window ImGui window holding both halves, so there is nothing for the user to
        // drag around or lose behind another panel.
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(display_w), static_cast<float>(display_h)));
        ImGui::Begin("##sensor_viewer", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBringToFrontOnFocus);

        const float avail_w  = ImGui::GetContentRegionAvail().x;
        const float images_w = std::max(200.0f, avail_w - kBasePanelWidth * ui_scale);

        // --------------------------------------------------------------------- images (left)
        ImGui::BeginChild("##images", ImVec2(images_w, 0), true);
        {
            // Three tiles per row. Two made each tile large enough that seven streams did not fit
            // on screen and had to be scrolled past; three keeps the whole set visible.
            constexpr int kColumns = 3;
            const float gap        = ImGui::GetStyle().ItemSpacing.x;
            const float tile_unit  = std::max(120.0f, (ImGui::GetContentRegionAvail().x - gap * static_cast<float>(kColumns - 1)) /
                                                          static_cast<float>(kColumns));
            int column             = 0;
            bool any_shown         = false;

            for (auto &stream_ptr : streams) {
                Stream &stream = *stream_ptr;
                if (!stream.enabled) {
                    continue;
                }
                any_shown = true;

                double fps           = 0.0;
                double bytes_per_sec = 0.0;
                std::string detail;
                bool has_frame = false;
                bool measured  = false;
                {
                    const std::lock_guard<std::mutex> lock(stream.mutex);
                    fps           = stream.stats.fps;
                    bytes_per_sec = stream.stats.bytes_per_sec;
                    measured      = stream.stats.measured;
                    detail        = stream.detail;
                    has_frame     = stream.has_frame;
                }

                // A frame twice as wide as it is tall - the stereo pair is 1280x480 - is
                // unreadable squeezed into one column, so give it two. Decided by shape rather
                // than by topic name so any other side-by-side stream gets the same treatment;
                // before the first frame arrives nothing is known, so it starts at one column.
                const bool wide = (stream.tex_width > 0 && stream.tex_height > 0) &&
                                  (static_cast<float>(stream.tex_width) / static_cast<float>(stream.tex_height) >= 2.0f);
                const int span     = wide ? 2 : 1;
                const float tile_w = tile_unit * static_cast<float>(span) + gap * static_cast<float>(span - 1);

                // Wrap early rather than letting a 2-wide tile hang off the row.
                if (column + span > kColumns) {
                    column = 0;
                }
                if (column != 0) {
                    ImGui::SameLine();
                }
                ImGui::BeginGroup();
                ImGui::PushID(stream.name.c_str());
                ImGui::TextUnformatted(stream.name.c_str());
                // Red means STALLED, not slow. A viewer cannot know the configured rate -
                // camera.yaml runs most streams at 15 fps and the hands at 20 - so a "below 20"
                // threshold painted healthy streams red permanently. Flag only what can be
                // recognised: nothing arriving.
                if (!measured) {
                    ImGui::TextDisabled("measuring...");
                } else {
                    ImGui::TextColored(fps <= 1.0 ? ImVec4(1.0f, 0.35f, 0.35f, 1.0f) : ImVec4(0.6f, 1.0f, 0.6f, 1.0f), "%.1f FPS, %s", fps,
                                       format_rate(bytes_per_sec).c_str());
                }
                if (has_frame && stream.texture != 0 && stream.tex_width > 0) {
                    const float aspect = static_cast<float>(stream.tex_height) / static_cast<float>(stream.tex_width);
                    ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(stream.texture)), ImVec2(tile_w, tile_w * aspect));
                } else {
                    // A subscribed stream with no frame yet is the interesting case (wrong
                    // domain, wrong namespace, camera off), so say so instead of leaving a gap.
                    ImGui::TextDisabled("waiting for data...");
                    ImGui::Dummy(ImVec2(tile_w, tile_w * 0.4f));
                }
                ImGui::TextDisabled("%s", detail.empty() ? "-" : detail.c_str());
                ImGui::PopID();
                ImGui::EndGroup();

                column += span;
                if (column >= kColumns) {
                    column = 0;
                }
            }

            if (!any_shown) {
                ImGui::TextDisabled("No streams enabled. Tick one in the panel on the right.");
            }
        }
        ImGui::EndChild();

        // ------------------------------------------------------------------- controls (right)
        ImGui::SameLine();
        ImGui::BeginChild("##panel", ImVec2(0, 0), true);
        {
            ImGui::TextUnformatted("Streams");
            ImGui::Separator();
            ImGui::TextDisabled("Unticking stops the subscription,");
            ImGui::TextDisabled("so the data stops crossing the link.");
            ImGui::Spacing();

            double total_bps = 0.0;
            for (auto &stream_ptr : streams) {
                Stream &stream = *stream_ptr;
                ImGui::PushID(stream.name.c_str());

                bool enabled = stream.enabled;
                if (ImGui::Checkbox(stream.name.c_str(), &enabled)) {
                    stream.enabled = enabled;
                    apply_subscription(node, stream);
                    clear_stream_data(stream);
                }

                ImGui::Indent();
                bool compressed = stream.compressed;
                if (ImGui::Checkbox("compressed", &compressed)) {
                    stream.compressed = compressed;
                    apply_subscription(node, stream);
                    clear_stream_data(stream);
                }
                ImGui::Unindent();

                {
                    const std::lock_guard<std::mutex> lock(stream.mutex);
                    total_bps += stream.stats.bytes_per_sec;
                }
                ImGui::PopID();
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Text("Total: %s", format_rate(total_bps).c_str());

            ImGui::Spacing();
            ImGui::TextUnformatted("IMU");
            ImGui::Separator();
            bool imu_enabled = imu.enabled;
            if (ImGui::Checkbox(imu.name.c_str(), &imu_enabled)) {
                imu.enabled = imu_enabled;
                apply_imu_subscription(node, imu);
            }
            {
                const std::lock_guard<std::mutex> lock(imu.mutex);
                if (!imu.enabled) {
                    ImGui::TextDisabled("disabled");
                } else if (!imu.has_data) {
                    ImGui::TextDisabled("no data");
                } else if (!imu.stats.measured) {
                    ImGui::TextDisabled("measuring...");
                } else {
                    ImGui::Text("%.1f Hz", imu.stats.fps);
                    ImGui::Text("accel [m/s^2]: %.3f %.3f %.3f", imu.accel[0], imu.accel[1], imu.accel[2]);
                    ImGui::Text("gyro  [rad/s]: %.3f %.3f %.3f", imu.gyro[0], imu.gyro[1], imu.gyro[2]);
                    if (imu.stamp_ns == 0) {
                        ImGui::TextDisabled("stamp unset (publisher did not stamp)");
                    } else {
                        ImGui::Text("latency: %.1f ms", imu.latency_ms);
                    }
                    ImGui::TextDisabled("frame_id: %s", imu.frame_id.empty() ? "(none)" : imu.frame_id.c_str());
                }
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::TextDisabled("domain %zu / ns %s", igris_c_sdk_examples::resolveRosDomainId(),
                                robot_namespace.empty() ? "(none)" : robot_namespace.c_str());
        }
        ImGui::EndChild();

        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.10f, 0.10f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Stop the executor before dropping the subscriptions so spin() returns on its own; joining
    // after shutdown would race the context teardown the callbacks still reference.
    executor.cancel();
    if (spin_thread.joinable()) {
        spin_thread.join();
    }
    for (auto &stream : streams) {
        stream->enabled = false;
        apply_subscription(node, *stream);
        if (stream->texture != 0) {
            glDeleteTextures(1, &stream->texture);
            stream->texture = 0;
        }
    }
    imu.enabled = false;
    apply_imu_subscription(node, imu);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    rclcpp::shutdown();
    return 0;
}
