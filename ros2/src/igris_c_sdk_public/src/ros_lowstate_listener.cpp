#include "common.hpp"

#include <igris_c_sdk/msg/low_state.hpp>
#include <rclcpp/rclcpp.hpp>

using igris_c_sdk_examples::g_running;
using igris_c_sdk_examples::signalHandler;

int main(int argc, char **argv) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    igris_c_sdk_examples::initRclcpp(argc, argv);
    auto node                         = rclcpp::Node::make_shared("igris_c_ros_lowstate_listener");
    const std::string robot_namespace = igris_c_sdk_examples::resolveRobotNamespace(node);
    const std::string lowstate_topic  = igris_c_sdk_examples::resolveRtName(robot_namespace, "rt/lowstate");

    auto sub = node->create_subscription<igris_c_sdk::msg::LowState>(
        lowstate_topic, rclcpp::SensorDataQoS(), [](const igris_c_sdk::msg::LowState &msg) {
            std::cout << "seq=" << msg.header.seq << " time=" << msg.header.sec << "." << msg.header.nanosec
                      << " first_joint_q=" << msg.joint_state[0].q << std::endl;
        });

    (void)sub;
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
