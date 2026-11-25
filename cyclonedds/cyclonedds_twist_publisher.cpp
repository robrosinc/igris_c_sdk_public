/**
 * @file cyclonedds_twist_publisher.cpp
 * @brief Walk velocity (Twist) publisher using the IGRIS SDK over CycloneDDS.
 *
 * Publishes geometry_msgs::msg::dds_::TwistStamped_ on cmd_vel at 20 Hz to drive
 * the robot's walk controller. Mirrors examples/ros2/src/ros_twist_publisher.cpp.
 *
 * Switch the robot into walkmode first (e.g. cyclonedds_service control-mode
 * command), then steer with the keyboard.
 *
 * Usage: ./cyclonedds_twist_publisher <domain_id> <namespace> [--dds-topic-naming native|ros_compatible]
 *        Both positionals are optional; the naming flag may appear anywhere and must match the robot's
 *        igris_c.network.transport (native pairs with cyclonedds_native,
 *        ros_compatible with cyclonedds_ros_compatible).
 *        ./cyclonedds_twist_publisher 0 ""   (domain 0, no namespace)
 *
 * Controls: w/s = linear.x +/- , a/d = linear.y +/- , q/e = angular.z +/- ,
 *           space = zero, x = exit
 */
#include "common.hpp"
#include "igris_c_sdk/namespace_resolver.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <igris_c_sdk/channel_factory.hpp>
#include <igris_c_sdk/publisher.hpp>
#include <igris_c_sdk/types.hpp>
#include <iostream>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace igris_c_sdk;

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

// Stamp the std_msgs Header: wall-clock stamp + a frame_id label.
void StampHeader(std_msgs::msg::dds_::Header_ &h, const char *frame_id) {
    const auto now = std::chrono::system_clock::now().time_since_epoch();
    const auto sec = std::chrono::duration_cast<std::chrono::seconds>(now);
    h.stamp().sec(static_cast<int32_t>(sec.count()));
    h.stamp().nanosec(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - sec).count()));
    h.frame_id(frame_id);
}

void PrintUsage(const char *prog) {
    // Every other cyclonedds example prints this; without it the flag is undiscoverable from the binary.
    std::cout << "Usage: " << prog << " <domain_id> <namespace>" << std::endl;
    std::cout << "       " << prog << " 0 \"\" (no namespace)" << std::endl;
    std::cout << igris_c_sdk::topic_naming_usage() << std::endl;
}

void PrintHelp() {
    std::cout << "Publishing TwistStamped on cmd_vel at 20 Hz." << std::endl;
    std::cout << "Switch the robot into walkmode first (e.g. cyclonedds_service control-mode command)." << std::endl;
    std::cout << "Controls: w/s = linear.x +/- , a/d = linear.y +/- , q/e = angular.z +/- , space = zero, x = exit" << std::endl;
}
}  // namespace

int main(int argc, char **argv) {
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    // The naming flag may appear anywhere on the command line, so positional parsing has to skip it AND its value. Otherwise
    // "--dds-topic-naming ros_compatible" lands in domain_id and ns, and what follows is the silent failure the flag exists to
    // prevent: the two sides simply never discover each other.
    std::vector<const char *> positional;
    for (int i = 1; i < argc; ++i) {
        if (!igris_c_sdk::is_topic_naming_arg(argc, argv, i)) {
            positional.push_back(argv[i]);
        }
    }

    // Printed unconditionally, not on an argument error: both positionals are optional here, so this example has no error path
    // to hang a usage message on - and the naming flag has to be discoverable from the binary, which is the whole point.
    PrintUsage(argv[0]);
    const int domain_id  = !positional.empty() ? std::atoi(positional[0]) : 0;
    const std::string ns = positional.size() > 1 ? positional[1] : "";

    const igris_c_sdk::TopicNaming naming = igris_c_sdk::parse_topic_naming_args(argc, argv);
    igris_c_sdk_examples::printBanner("Twist Publisher", domain_id, ns, naming);

    ChannelFactory::Instance()->Init(domain_id, ns, "", naming);
    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    // Default QoS (~rclcpp::QoS(10)), matching the ROS twist publisher.
    Publisher<geometry_msgs::msg::dds_::TwistStamped_> twist_pub("cmd_vel", QosProfile::Default());
    if (!twist_pub.init()) {
        std::cerr << "Failed to initialize TwistStamped publisher" << std::endl;
        return 1;
    }

    TerminalRawMode raw_mode;
    PrintHelp();

    double vx             = 0.0;
    double vy             = 0.0;
    double wz             = 0.0;
    const double step_lin = 0.05;
    const double step_ang = 0.1;

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

        geometry_msgs::msg::dds_::TwistStamped_ msg;
        StampHeader(msg.header(), "cmd_vel");
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
