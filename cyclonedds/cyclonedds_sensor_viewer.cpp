#include <atomic>
#include <cctype>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <igris_c_sdk/channel_factory.hpp>
#include <igris_c_sdk/igris_c_client.hpp>
#include <igris_c_sdk/igris_c_msgs.hpp>
#include <igris_c_sdk/subscriber.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

struct status {
    std::string name;
    float fps           = 0.0f;
    float bytes_per_sec = 0.0f;
};

namespace {
int domain_id = 10;

using namespace igris_c_sdk;
using CompressedMessage = igris_c::msg::dds::CompressedMessage;

std::atomic<bool> g_stop{false};

void handle_signal(int) { g_stop.store(true); }

int getenv_int(const char *name, int fallback) {
    const char *value = std::getenv(name);
    if (!value || value[0] == '\0') {
        return fallback;
    }
    char *end   = nullptr;
    long parsed = std::strtol(value, &end, 10);
    if (end == value) {
        return fallback;
    }
    return static_cast<int>(parsed);
}

struct StreamStats {
    std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();
    double fps                                      = 0.0;
    double bytes_per_sec                            = 0.0;
};

void update_stats(StreamStats &stats, size_t bytes) {
    auto now     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - stats.last_time).count();
    if (elapsed > 0) {
        double seconds  = static_cast<double>(elapsed) / 1'000'000.0;
        double inst_fps = 1.0 / seconds;
        double inst_bps = static_cast<double>(bytes) / seconds;
        double alpha    = 0.15;
        if (stats.fps <= 0.0) {
            stats.fps           = inst_fps;
            stats.bytes_per_sec = inst_bps;
        } else {
            stats.fps           = (1.0 - alpha) * stats.fps + alpha * inst_fps;
            stats.bytes_per_sec = (1.0 - alpha) * stats.bytes_per_sec + alpha * inst_bps;
        }
    }
    stats.last_time = now;
}

std::optional<cv::Mat> decode_raw_frame(const CompressedMessage &msg) {
    const std::string &format = msg.format();
    const auto &bytes         = msg.image_data();
    if (bytes.empty()) {
        return std::nullopt;
    }

    const size_t first_sep  = format.find(';');
    const size_t second_sep = (first_sep == std::string::npos) ? std::string::npos : format.find(';', first_sep + 1);
    if (first_sep == std::string::npos || second_sep == std::string::npos) {
        return std::nullopt;
    }

    const std::string encoding = format.substr(0, first_sep);
    const std::string mode     = format.substr(first_sep + 1, second_sep - first_sep - 1);
    const std::string size     = format.substr(second_sep + 1);
    if (mode != "raw") {
        return std::nullopt;
    }

    int cv_type      = -1;
    size_t elem_size = 0;
    if (encoding == "16UC1") {
        cv_type   = CV_16UC1;
        elem_size = 2;
    } else if (encoding == "8UC1") {
        cv_type   = CV_8UC1;
        elem_size = 1;
    } else if (encoding == "8UC3") {
        cv_type   = CV_8UC3;
        elem_size = 3;
    } else {
        return std::nullopt;
    }

    const size_t x_pos = size.find('x');
    if (x_pos == std::string::npos || x_pos == 0 || x_pos + 1 >= size.size()) {
        return std::nullopt;
    }

    for (char ch : size.substr(0, x_pos)) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            return std::nullopt;
        }
    }
    for (char ch : size.substr(x_pos + 1)) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            return std::nullopt;
        }
    }

    const int width  = std::stoi(size.substr(0, x_pos));
    const int height = std::stoi(size.substr(x_pos + 1));
    if (width <= 0 || height <= 0) {
        return std::nullopt;
    }

    const size_t expected_size = static_cast<size_t>(width) * static_cast<size_t>(height) * elem_size;
    if (bytes.size() != expected_size) {
        return std::nullopt;
    }

    cv::Mat raw(height, width, cv_type, const_cast<uint8_t *>(bytes.data()));
    if (cv_type == CV_16UC1) {
        cv::Mat depth_u8;
        cv::Mat depth_vis;
        double min_val = 0.0;
        double max_val = 0.0;
        cv::minMaxLoc(raw, &min_val, &max_val);
        if (max_val <= min_val) {
            max_val = min_val + 1.0;
        }
        raw.convertTo(depth_u8, CV_8UC1, 255.0 / (max_val - min_val), -min_val * 255.0 / (max_val - min_val));
        cv::applyColorMap(depth_u8, depth_vis, cv::COLORMAP_JET);
        return depth_vis;
    }
    if (cv_type == CV_8UC3) {
        cv::Mat bgr;
        cv::cvtColor(raw, bgr, cv::COLOR_RGB2BGR);
        return bgr.clone();
    }
    return raw.clone();
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

struct Stream {
    std::string name;
    Subscriber<CompressedMessage> subscriber;
    StreamStats stats;
    cv::Mat latest_image;
    mutable std::mutex mutex;
    bool has_frame = false;

    explicit Stream(const std::string &topic_name) : name(topic_name), subscriber(topic_name, QosProfile::SensorData()) {}
};
}  // namespace

int main(int argc, char **argv) {
    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <domain_id> <namespace> [topics...]" << std::endl;
        std::cerr << "       " << argv[0] << " 10 \"\" (no namespace)" << std::endl;
        return 1;
    }
    domain_id            = std::atoi(argv[1]);
    const std::string ns = argv[2];

    std::vector<std::string> topics;
    for (int i = 3; i < argc; ++i) {
        topics.emplace_back(argv[i]);
    }
    if (topics.empty()) {
        topics = {
            "sensor/d435_color", "sensor/d435_depth", "sensor/eyes_stereo", "sensor/left_hand", "sensor/right_hand",
        };
    }

    ChannelFactory::Instance()->Init(domain_id, ns);
    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    std::vector<std::unique_ptr<Stream>> streams;
    streams.reserve(topics.size());
    for (const auto &name : topics) {
        streams.emplace_back(std::make_unique<Stream>(name));
        cv::namedWindow(name, cv::WINDOW_NORMAL);
    }

    std::vector<status> stream_status(streams.size());
    for (size_t i = 0; i < streams.size(); ++i) {
        stream_status[i].name = streams[i]->name;

        if (!streams[i]->subscriber.init([stream = streams[i].get()](const CompressedMessage &msg) {
                const auto &bytes = msg.image_data();
                if (bytes.empty()) {
                    return;
                }

                cv::Mat img;
                cv::Mat encoded(1, static_cast<int>(bytes.size()), CV_8UC1, const_cast<uint8_t *>(bytes.data()));
                img = cv::imdecode(encoded, cv::IMREAD_UNCHANGED);
                if (img.empty()) {
                    auto raw_frame = decode_raw_frame(msg);
                    if (raw_frame.has_value()) {
                        img = std::move(*raw_frame);
                    }
                }
                if (img.empty()) {
                    return;
                }

                std::lock_guard<std::mutex> lock(stream->mutex);
                update_stats(stream->stats, bytes.size());
                stream->latest_image = img;
                stream->has_frame    = true;
            })) {
            std::cerr << "Failed to initialize subscriber for topic: " << streams[i]->name << std::endl;
            return 1;
        }
    }

    while (!g_stop.load()) {
        bool got_data = false;

        for (size_t i = 0; i < streams.size(); ++i) {
            auto &stream = *streams[i];
            cv::Mat frame;
            double fps           = 0.0;
            double bytes_per_sec = 0.0;
            {
                std::lock_guard<std::mutex> lock(stream.mutex);
                if (stream.has_frame) {
                    frame = stream.latest_image.clone();
                }
                fps           = stream.stats.fps;
                bytes_per_sec = stream.stats.bytes_per_sec;
            }

            if (!frame.empty()) {
                got_data = true;

                std::ostringstream oss;
                oss << std::fixed << std::setprecision(1) << fps << " FPS, " << format_rate(bytes_per_sec);
                cv::Scalar text_color = (fps <= 20.0 ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0));
                cv::putText(frame, oss.str(), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, text_color, 2);
                stream_status[i].fps           = static_cast<float>(fps);
                stream_status[i].bytes_per_sec = static_cast<float>(bytes_per_sec);
                cv::imshow(stream.name, frame);
            }
        }

        std::cout << "\x1B[2J\x1B[H";
        std::cout << "==============================\n\n";
        std::cout << "Domain ID: " << domain_id << "\n";
        std::cout << "Namespace: " << ns << "\n";
        std::cout << "Stream Status:\n";
        for (const auto &s : stream_status) {
            bool low_fps            = (s.fps <= 20.0f);
            const char *color_start = low_fps ? "\x1B[31m" : "\x1B[0m";
            const char *color_end   = "\x1B[0m";
            std::cout << color_start << s.name << ": " << std::fixed << std::setprecision(1) << s.fps << " FPS, "
                      << format_rate(s.bytes_per_sec) << color_end << "\n";
        }
        std::cout << std::endl;
        std::cout << "Total bytes/sec: "
                  << format_rate(std::accumulate(stream_status.begin(), stream_status.end(), 0.0f,
                                                 [](float sum, const status &s) { return sum + s.bytes_per_sec; }))
                  << "\n";
        std::cout << "Avg FPS: " << std::fixed << std::setprecision(1)
                  << (stream_status.empty() ? 0.0f
                                            : std::accumulate(stream_status.begin(), stream_status.end(), 0.0f,
                                                              [](float sum, const status &s) { return sum + s.fps; }) /
                                                  stream_status.size())
                  << " FPS\n";
        std::cout << "Press 'q' or 'ESC' to quit.\n";
        std::cout << "==============================\n";
        std::cout << std::flush;

        int key = cv::waitKey(1);
        if (key == 27 || key == 'q' || key == 'Q') {
            break;
        }

        if (!got_data) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }

    for (const auto &stream : streams) {
        cv::destroyWindow(stream->name);
    }
    return 0;
}
