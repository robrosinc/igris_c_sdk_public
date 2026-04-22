#include "igris_sdk/igris_c_msgs.hpp"

#include <atomic>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <dds/dds.hpp>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

using StereoConfigRequest    = igris_c::msg::dds::StereoConfigRequest;
using RealsenseConfigRequest = igris_c::msg::dds::RealsenseConfigRequest;
using MonoConfigRequest      = igris_c::msg::dds::MonoConfigRequest;
using ServiceResponse        = igris_c::msg::dds::ServiceResponse;

namespace {
constexpr int kDefaultDomainId  = 10;
constexpr int kDefaultTimeoutMs = 60000;  // 60 seconds

constexpr const char *kStereoConfigRequestTopic    = "igris_c/sensor/stereo_config_request";
constexpr const char *kRealsenseConfigRequestTopic = "igris_c/sensor/realsense_config_request";
constexpr const char *kMonoConfigRequestTopic      = "igris_c/sensor/mono_config_request";
constexpr const char *kCameraConfigResponseTopic   = "igris_c/sensor/camera_config_response";

std::atomic<uint64_t> g_request_seq{0};

std::string trim_copy(const std::string &s) {
    std::size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])) != 0) {
        ++start;
    }

    std::size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])) != 0) {
        --end;
    }

    return s.substr(start, end - start);
}

std::string to_lower_copy(std::string s) {
    for (char &c : s) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return s;
}

bool read_line(std::string &out) {
    if (!std::getline(std::cin, out)) {
        return false;
    }
    out = trim_copy(out);
    return true;
}

std::optional<uint32_t> parse_u32(const std::string &text) {
    if (text.empty()) {
        return std::nullopt;
    }

    char *end           = nullptr;
    unsigned long value = std::strtoul(text.c_str(), &end, 10);
    if (end == text.c_str() || *end != '\0') {
        return std::nullopt;
    }

    if (value > static_cast<unsigned long>(UINT32_MAX)) {
        return std::nullopt;
    }

    return static_cast<uint32_t>(value);
}

std::optional<bool> parse_bool(const std::string &text) {
    std::string v = to_lower_copy(trim_copy(text));
    if (v == "1" || v == "true" || v == "t" || v == "y" || v == "yes" || v == "on") {
        return true;
    }
    if (v == "0" || v == "false" || v == "f" || v == "n" || v == "no" || v == "off") {
        return false;
    }
    return std::nullopt;
}

uint32_t ask_u32(const std::string &label, uint32_t default_value) {
    while (true) {
        std::cout << label << " [default=" << default_value << "]: ";
        std::string line;
        if (!read_line(line)) {
            return default_value;
        }

        if (line.empty()) {
            return default_value;
        }

        auto parsed = parse_u32(line);
        if (parsed.has_value()) {
            return *parsed;
        }

        std::cout << "Please enter a valid uint32 number.\n";
    }
}

bool ask_bool(const std::string &label, bool default_value) {
    while (true) {
        std::cout << label << " [default=" << (default_value ? "true" : "false") << ", input: y/n or true/false or 1/0]: ";
        std::string line;
        if (!read_line(line)) {
            return default_value;
        }

        if (line.empty()) {
            return default_value;
        }

        auto parsed = parse_bool(line);
        if (parsed.has_value()) {
            return *parsed;
        }

        std::cout << "Please enter a valid boolean value.\n";
    }
}

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

std::optional<int> parse_int_arg(const char *value) {
    if (!value || value[0] == '\0') {
        return std::nullopt;
    }

    char *end   = nullptr;
    long parsed = std::strtol(value, &end, 10);
    if (end == value || *end != '\0') {
        return std::nullopt;
    }

    return static_cast<int>(parsed);
}

std::vector<std::string> collect_positional_args(int argc, char **argv) {
    std::vector<std::string> positional_args;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--domain" || arg == "--domain-id" || arg == "--timeout-ms") {
            ++i;
            continue;
        }
        if (!arg.empty() && arg[0] == '-') {
            continue;
        }
        positional_args.push_back(arg);
    }

    return positional_args;
}

int resolve_domain_id(int argc, char **argv) {
    for (int i = 1; i + 1 < argc; ++i) {
        if ((std::strcmp(argv[i], "--domain") == 0 || std::strcmp(argv[i], "--domain-id") == 0)) {
            auto parsed = parse_int_arg(argv[i + 1]);
            if (parsed.has_value()) {
                return *parsed;
            }
        }
    }

    const auto positional_args = collect_positional_args(argc, argv);
    if (!positional_args.empty()) {
        auto parsed = parse_int_arg(positional_args[0].c_str());
        if (parsed.has_value()) {
            return *parsed;
        }
    }

    return getenv_int("DDS_DOMAIN_ID", kDefaultDomainId);
}

int resolve_timeout_ms(int argc, char **argv) {
    for (int i = 1; i + 1 < argc; ++i) {
        if (std::strcmp(argv[i], "--timeout-ms") == 0) {
            auto parsed = parse_int_arg(argv[i + 1]);
            if (parsed.has_value() && *parsed > 0) {
                return *parsed;
            }
        }
    }

    const auto positional_args = collect_positional_args(argc, argv);
    if (positional_args.size() >= 2) {
        auto parsed = parse_int_arg(positional_args[1].c_str());
        if (parsed.has_value() && *parsed > 0) {
            return *parsed;
        }
    }

    return kDefaultTimeoutMs;
}

std::string make_request_id() {
    using namespace std::chrono;
    const auto now_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    const auto seq    = g_request_seq.fetch_add(1, std::memory_order_relaxed);
    return "jetson_cfg_req_" + std::to_string(now_ms) + "_" + std::to_string(seq);
}

std::optional<ServiceResponse> wait_for_response(dds::sub::DataReader<ServiceResponse> &reader, const std::string &request_id,
                                                 std::chrono::milliseconds timeout) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;

    while (std::chrono::steady_clock::now() < deadline) {
        auto samples = reader.take();
        for (const auto &sample : samples) {
            if (!sample.info().valid()) {
                continue;
            }

            const ServiceResponse &res = sample.data();
            if (res.request_id() == request_id) {
                return res;
            }

            std::cout << "[info] Received response for another request_id=" << res.request_id() << ", waiting...\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return std::nullopt;
}

void print_menu() {
    std::cout << "\n=== Camera Config Request CLI ===\n";
    std::cout << "1) stereo config publish\n";
    std::cout << "2) realsense config publish\n";
    std::cout << "3) mono config publish\n";
    std::cout << "q) quit\n";
    std::cout << "select: ";
}

void print_response(const ServiceResponse &res) {
    std::cout << "\n=== ServiceResponse ===\n";
    std::cout << "request_id: " << res.request_id() << "\n";
    std::cout << "success   : " << (res.success() ? "true" : "false") << "\n";
    std::cout << "message   : " << res.message() << "\n";
    std::cout << "error_code: " << res.error_code() << "\n";
}

}  // namespace

int main(int argc, char **argv) {
    const int domain_id  = resolve_domain_id(argc, argv);
    const int timeout_ms = resolve_timeout_ms(argc, argv);

    std::cout << "DDS domain_id=" << domain_id << ", timeout_ms=" << timeout_ms << "\n";

    dds::domain::DomainParticipant participant(domain_id);
    dds::pub::Publisher publisher(participant);
    dds::sub::Subscriber subscriber(participant);

    dds::topic::Topic<StereoConfigRequest> stereo_req_topic(participant, kStereoConfigRequestTopic);
    dds::topic::Topic<RealsenseConfigRequest> realsense_req_topic(participant, kRealsenseConfigRequestTopic);
    dds::topic::Topic<MonoConfigRequest> mono_req_topic(participant, kMonoConfigRequestTopic);
    dds::topic::Topic<ServiceResponse> res_topic(participant, kCameraConfigResponseTopic);

    dds::pub::DataWriter<StereoConfigRequest> stereo_writer(publisher, stereo_req_topic);
    dds::pub::DataWriter<RealsenseConfigRequest> realsense_writer(publisher, realsense_req_topic);
    dds::pub::DataWriter<MonoConfigRequest> mono_writer(publisher, mono_req_topic);
    dds::sub::DataReader<ServiceResponse> res_reader(subscriber, res_topic);

    while (true) {
        print_menu();

        std::string selection;
        if (!read_line(selection)) {
            std::cout << "\nInput stream closed. Exiting.\n";
            break;
        }

        selection = to_lower_copy(selection);
        if (selection == "q" || selection == "quit" || selection == "exit") {
            std::cout << "Exiting.\n";
            break;
        }

        const std::string request_id = make_request_id();

        if (selection == "1" || selection == "stereo") {
            StereoConfigRequest req;
            req.request_id(request_id);
            req.stereo_width(ask_u32("stereo_width", 1280));
            req.stereo_height(ask_u32("stereo_height", 480));
            req.stereo_fps(ask_u32("stereo_fps", 30));
            req.stereo_jpeg_quality(ask_u32("stereo_jpeg_quality", 90));
            req.enable_stereo(ask_bool("enable_stereo", true));

            stereo_writer.write(req);
            std::cout << "\n[publish] StereoConfigRequest sent. request_id=" << request_id << "\n";
        } else if (selection == "2" || selection == "realsense" || selection == "d435") {
            RealsenseConfigRequest req;
            req.request_id(request_id);
            req.d_435_width(ask_u32("d_435_width", 640));
            req.d_435_height(ask_u32("d_435_height", 480));
            req.d_435_fps(ask_u32("d_435_fps", 30));
            req.d_435_jpeg_quality(ask_u32("d_435_jpeg_quality", 90));
            req.enable_color(ask_bool("enable_color", true));
            req.enable_depth(ask_bool("enable_depth", true));
            req.enable_ir(ask_bool("enable_ir", false));

            realsense_writer.write(req);
            std::cout << "\n[publish] RealsenseConfigRequest sent. request_id=" << request_id << "\n";
        } else if (selection == "3" || selection == "mono") {
            MonoConfigRequest req;
            req.request_id(request_id);
            req.mono_width(ask_u32("mono_width", 640));
            req.mono_height(ask_u32("mono_height", 480));
            req.mono_fps(ask_u32("mono_fps", 30));
            req.mono_jpeg_quality(ask_u32("mono_jpeg_quality", 90));
            req.enable_mono(ask_bool("enable_mono", true));

            mono_writer.write(req);
            std::cout << "\n[publish] MonoConfigRequest sent. request_id=" << request_id << "\n";
        } else {
            std::cout << "Unsupported selection: " << selection << "\n";
            continue;
        }

        std::cout << "[wait] Waiting for ServiceResponse...\n";
        auto response = wait_for_response(res_reader, request_id, std::chrono::milliseconds(timeout_ms));
        if (!response.has_value()) {
            std::cout << "[timeout] Did not receive response for request_id=" << request_id << " within " << timeout_ms << "ms.\n";
            continue;
        }

        print_response(*response);
    }

    return 0;
}
