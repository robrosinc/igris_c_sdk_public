#include "common.hpp"

#include <fcntl.h>
#include <igris_c_sdk/msg/walk_twist.hpp>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <termios.h>
#include <unistd.h>

using igris_c_sdk_examples::g_running;
using igris_c_sdk_examples::signalHandler;

namespace {
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
        if (active) {
            tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
        }
    }
};

bool ReadKey(char &key) {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeval timeout{};
    timeout.tv_sec  = 0;
    timeout.tv_usec = 0;
    if (select(STDIN_FILENO + 1, &set, nullptr, nullptr, &timeout) <= 0) {
        return false;
    }
    return ::read(STDIN_FILENO, &key, 1) == 1;
}

void PrintHelp() {
    std::cout << "Publishing geometry_msgs/Twist on rt/cmd_vel at 20 Hz." << std::endl;
    std::cout << "Switch the robot into walkmode first (e.g. ros_service option 14)." << std::endl;
    std::cout << "Controls: w/s = linear.x +/- , a/d = linear.y +/- , q/e = angular.z +/- , space = zero, x = exit" << std::endl;
}
}  // namespace

int main(int argc, char **argv) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    igris_c_sdk_examples::initRclcpp(argc, argv);
    auto node                         = rclcpp::Node::make_shared("igris_c_ros_twist_publisher");
    const std::string robot_namespace = igris_c_sdk_examples::resolveRobotNamespace(node);
    auto twist_pub = node->create_publisher<igris_c_sdk::msg::WalkTwist>(igris_c_sdk_examples::resolveRtName(robot_namespace, "rt/cmd_vel"),
                                                                         rclcpp::QoS(10));

    TerminalRawMode raw_mode;
    PrintHelp();

    double vx             = 0.0;
    double vy             = 0.0;
    double wz             = 0.0;
    const double step_lin = 0.05;
    const double step_ang = 0.1;
    rclcpp::Rate rate(20.0);

    while (rclcpp::ok() && g_running.load(std::memory_order_relaxed)) {
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

        igris_c_sdk::msg::WalkTwist msg;
        msg.twist.linear.x  = vx;
        msg.twist.linear.y  = vy;
        msg.twist.angular.z = wz;
        twist_pub->publish(msg);

        std::cout << "\r\033[K" << "linear.x=" << vx << " linear.y=" << vy << " angular.z=" << wz << "  " << std::flush;
        rate.sleep();
    }

    std::cout << std::endl;
    rclcpp::shutdown();
    return 0;
}
