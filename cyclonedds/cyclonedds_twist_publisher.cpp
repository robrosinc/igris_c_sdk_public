/**
 * @file cyclonedds_twist_publisher.cpp
 * @brief Walk velocity (Twist) publisher using the IGRIS SDK over CycloneDDS.
 *
 * Publishes igris_c::msg::dds::WalkTwist on rt/cmd_vel at 20 Hz to drive the
 * robot's walk controller. Mirrors examples/ros2/src/ros_twist_publisher.cpp.
 *
 * Switch the robot into walkmode first (e.g. cyclonedds_service control-mode
 * command), then steer with the keyboard.
 *
 * Usage: ./cyclonedds_twist_publisher <domain_id> <namespace>
 *        ./cyclonedds_twist_publisher 0 ""   (domain 0, no namespace)
 *
 * Controls: w/s = linear.x +/- , a/d = linear.y +/- , q/e = angular.z +/- ,
 *           space = zero, x = exit
 */
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <igris_c_sdk/channel_factory.hpp>
#include <igris_c_sdk/igris_c_msgs.hpp>
#include <igris_c_sdk/publisher.hpp>
#include <iostream>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>

using namespace igris_c_sdk;
using namespace igris_c::msg::dds;

namespace {
std::atomic<bool> g_running(true);
void SignalHandler(int) { g_running = false; }

// Put the terminal in raw, non-blocking mode so single keystrokes steer the
// robot without waiting for Enter; restored on destruction.
struct TerminalRawMode {
    termios old_term{};
    bool active{false};

    TerminalRawMode() {
        if (tcgetattr(STDIN_FILENO, &old_term) == 0) {
            termios raw = old_term;
            raw.c_lflag &= static_cast<unsigned>(~(ICANON | ECHO));
            raw.c_cc[VMIN]  = 0;
            raw.c_cc[VTIME] = 0;
            active          = (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0);
        }
    }
    ~TerminalRawMode() {
        if (active)
            tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    }
};

bool ReadKey(char &key) {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeval timeout{};
    timeout.tv_sec  = 0;
    timeout.tv_usec = 0;
    if (select(STDIN_FILENO + 1, &set, nullptr, nullptr, &timeout) <= 0)
        return false;
    return ::read(STDIN_FILENO, &key, 1) == 1;
}

// Stamp the common Header (every IGRIS message carries one): incrementing seq +
// wall-clock sec/nanosec + a frame_id label.
// `Header` is exported by both igris_c_sdk and igris_c::msg::dds (the former is
// an alias of the latter), so qualify it to avoid an ambiguous-name error.
void StampHeader(igris_c::msg::dds::Header &h, uint32_t seq, const char *frame_id) {
    const auto now = std::chrono::system_clock::now().time_since_epoch();
    const auto sec = std::chrono::duration_cast<std::chrono::seconds>(now);
    h.seq(seq);
    h.sec(static_cast<uint32_t>(sec.count()));
    h.nanosec(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - sec).count()));
    std::strncpy(h.frame_id().data(), frame_id, h.frame_id().size() - 1);
}

void PrintHelp() {
    std::cout << "Publishing WalkTwist on rt/cmd_vel at 20 Hz." << std::endl;
    std::cout << "Switch the robot into walkmode first (e.g. cyclonedds_service control-mode command)." << std::endl;
    std::cout << "Controls: w/s = linear.x +/- , a/d = linear.y +/- , q/e = angular.z +/- , space = zero, x = exit" << std::endl;
}
}  // namespace

int main(int argc, char **argv) {
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    const int domain_id  = (argc > 1) ? std::atoi(argv[1]) : 0;
    const std::string ns = (argc > 2) ? argv[2] : "";
    std::cout << "Domain ID: " << domain_id << " | Namespace: \"" << ns << "\"" << std::endl;

    ChannelFactory::Instance()->Init(domain_id, ns);
    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    // Default QoS (~rclcpp::QoS(10)), matching the ROS twist publisher.
    Publisher<WalkTwist> twist_pub("rt/cmd_vel", QosProfile::Default());
    if (!twist_pub.init()) {
        std::cerr << "Failed to initialize WalkTwist publisher" << std::endl;
        return 1;
    }

    TerminalRawMode raw_mode;
    PrintHelp();

    double vx             = 0.0;
    double vy             = 0.0;
    double wz             = 0.0;
    const double step_lin = 0.05;
    const double step_ang = 0.1;
    uint32_t seq          = 0;

    const auto period = std::chrono::milliseconds(50);  // 20 Hz
    while (g_running.load(std::memory_order_relaxed)) {
        const auto loop_start = std::chrono::steady_clock::now();

        char key = 0;
        while (ReadKey(key)) {
            switch (key) {
            case 'w':
                vx += step_lin;
                break;
            case 's':
                vx -= step_lin;
                break;
            case 'a':
                vy += step_lin;
                break;
            case 'd':
                vy -= step_lin;
                break;
            case 'q':
                wz += step_ang;
                break;
            case 'e':
                wz -= step_ang;
                break;
            case ' ':
                vx = vy = wz = 0.0;
                break;
            case 'x':
                g_running.store(false, std::memory_order_relaxed);
                break;
            default:
                break;
            }
        }

        WalkTwist msg;
        StampHeader(msg.header(), seq++, "cmd_vel");
        msg.twist().linear().x(vx);
        msg.twist().linear().y(vy);
        msg.twist().angular().z(wz);
        twist_pub.write(msg);

        std::cout << "\r\033[K" << "linear.x=" << vx << " linear.y=" << vy << " angular.z=" << wz << "  " << std::flush;
        std::this_thread::sleep_until(loop_start + period);
    }

    std::cout << std::endl;
    return 0;
}
