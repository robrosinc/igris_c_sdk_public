/**
 * @file ros_lowstate_listener.cpp
 * @brief Minimal LowState subscriber over ROS 2.
 *
 * This example demonstrates:
 * - LowState subscription on the resolved <ns>/lowstate topic
 * - Redrawing the latest sample in place instead of printing one line per message, which at
 *   the robot's publish rate scrolls far too fast to read
 *
 * Usage: ros2 run igris_c_sdk ros_lowstate_listener --ros-args -p robot_namespace:=<ns>
 */
#include "common.hpp"

#include <cstdint>
#include <igris_c_sdk/msg/low_state.hpp>
#include <mutex>
#include <rclcpp/rclcpp.hpp>

using igris_c_sdk_examples::g_running;
using igris_c_sdk_examples::signalHandler;

int main(int argc, char **argv) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    igris_c_sdk_examples::initRclcpp(argc, argv);
    auto node                         = rclcpp::Node::make_shared("igris_c_ros_lowstate_listener");
    const std::string robot_namespace = igris_c_sdk_examples::resolveRobotNamespace(node);
    igris_c_sdk_examples::printBanner("LowState Listener", robot_namespace);
    const std::string lowstate_topic = igris_c_sdk_examples::resolveRosTopic(robot_namespace, "lowstate");

    // The callback records; a timer draws. Printing a line per message instead put lowstate's
    // full publish rate into the terminal - hundreds of lines a second, which scrolls far too
    // fast to read and grows the scrollback without bound. The received counter is what makes
    // the stream visible; the latest sample is what is worth looking at.
    std::mutex mutex;
    igris_c_sdk::msg::LowState latest;
    std::uint64_t received = 0;

    auto sub = node->create_subscription<igris_c_sdk::msg::LowState>(lowstate_topic, rclcpp::SensorDataQoS(),
                                                                     [&mutex, &latest, &received](const igris_c_sdk::msg::LowState &msg) {
                                                                         const std::lock_guard<std::mutex> lock(mutex);
                                                                         latest = msg;
                                                                         ++received;
                                                                     });

    auto timer = node->create_wall_timer(std::chrono::milliseconds(200), [&]() {
        const std::lock_guard<std::mutex> lock(mutex);
        std::cout << igris_c_sdk_examples::kRedraw;
        igris_c_sdk_examples::printBanner("LowState Listener", robot_namespace);
        std::cout << "topic          : " << lowstate_topic << "\n";
        std::cout << "messages       : " << received << "\n";
        if (received == 0) {
            std::cout << "\nwaiting for the robot to publish..." << std::endl;
            return;
        }
        std::cout << "stamp          : " << latest.header.stamp.sec << "." << latest.header.stamp.nanosec << "\n";
        std::cout << "first joint q  : " << latest.joint_state[0].q << "\n";
        std::cout << "\nPress Ctrl+C to stop." << std::endl;
    });

    (void)sub;
    (void)timer;
    // Everything from here is redrawn in place, so it runs on the alternate screen: the wiring
    // printed above stays in the real terminal as a record, and Ctrl+C restores it untouched.
    const igris_c_sdk_examples::AltScreen alt_screen;

    // spin(), not a spin_some() loop: receiving must not be paced by anything this example
    // does on the side. See the note in ros_sensor_viewer.cpp for what that costs when it is.
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
