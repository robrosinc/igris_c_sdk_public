#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <igris_sdk/channel_factory.hpp>
#include <igris_sdk/igris_c_client.hpp>
#include <igris_sdk/igris_c_msgs.hpp>
#include <igris_sdk/subscriber.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <numeric>
#include <opencv2/opencv.hpp>
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

using namespace igris_sdk;
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

    explicit Stream(const std::string &topic_name) : name(topic_name), subscriber(topic_name) {}
};
}  // namespace

int main(int argc, char **argv) {
    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    if (argc > 1) {
        domain_id = std::atoi(argv[1]);
    } else {
        domain_id = 10;  // Default domain ID
    }

    std::vector<std::string> topics;
    for (int i = 2; i < argc; ++i) {
        topics.emplace_back(argv[i]);
    }
    if (topics.empty()) {
        topics = {
            "igris_c/sensor/d435_color", "igris_c/sensor/d435_depth", "igris_c/sensor/eyes_stereo",
            "igris_c/sensor/left_hand",  "igris_c/sensor/right_hand",
        };
    }

    ChannelFactory::Instance()->Init(domain_id);
    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    IgrisC_Client client;
    client.Init();
    client.SetTimeout(10.0f);

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

                cv::Mat raw(1, static_cast<int>(bytes.size()), CV_8UC1, const_cast<uint8_t *>(bytes.data()));
                cv::Mat img = cv::imdecode(raw, cv::IMREAD_UNCHANGED);
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
