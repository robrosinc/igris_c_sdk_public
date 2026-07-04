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
    auto pub = node->create_publisher<igris_c_sdk::msg::LowCmd>(igris_c_sdk_examples::resolveRtName(robot_namespace, "rt/lowcmd"),
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
        cmd.header.seq++;
        node->get_clock()->now();
        pub->publish(cmd);
        rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}
