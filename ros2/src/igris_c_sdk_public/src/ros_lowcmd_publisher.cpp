/**
 * @file ros_lowcmd_publisher.cpp
 * @brief Minimal LowCmd publisher over ROS 2.
 *
 * This example demonstrates:
 * - LowCmd publishing on the resolved <ns>/lowcmd topic
 * - The zeroed-command shape a caller starts from before filling in real targets
 *
 * Usage: ros2 run igris_c_sdk ros_lowcmd_publisher --ros-args -p robot_namespace:=<ns>
 */
#include "common.hpp"

#include <igris_c_sdk/msg/low_cmd.hpp>
#include <rclcpp/rclcpp.hpp>

using igris_c_sdk_examples::g_running;
using igris_c_sdk_examples::signalHandler;

int main(int argc, char **argv) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    igris_c_sdk_examples::initRclcpp(argc, argv);
    auto node                         = rclcpp::Node::make_shared("igris_c_ros_lowcmd_publisher");
    const std::string robot_namespace = igris_c_sdk_examples::resolveRobotNamespace(node);
    igris_c_sdk_examples::printBanner("LowCmd Publisher", robot_namespace);
    auto pub = node->create_publisher<igris_c_sdk::msg::LowCmd>(igris_c_sdk_examples::resolveRosTopic(robot_namespace, "lowcmd"),
                                                                rclcpp::SensorDataQoS());

    igris_c_sdk::msg::LowCmd cmd;
    cmd.kinematic_modes.fill(igris_c_sdk::msg::LowCmd::KINEMATIC_MODE_PJS);
    for (std::size_t i = 0; i < cmd.motors.size(); ++i) {
        cmd.motors[i].id  = static_cast<uint16_t>(i);
        cmd.motors[i].kp  = 0.0f;
        cmd.motors[i].kd  = 0.0f;
        cmd.motors[i].q   = 0.0f;
        cmd.motors[i].dq  = 0.0f;
        cmd.motors[i].tau = 0.0f;
    }

    rclcpp::Rate rate(50.0);
    while (rclcpp::ok() && g_running.load(std::memory_order_relaxed)) {
        cmd.header.stamp = node->get_clock()->now();
        pub->publish(cmd);
        rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}
