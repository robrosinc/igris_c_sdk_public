/**
 * @file ros_sensor_listener.cpp
 * @brief Dependency-light text probe for the camera streams on the ROS 2 lane.
 *
 * This example demonstrates:
 * - Subscribing to every camera image topic plus the d435 IMU using only rclcpp and
 *   sensor_msgs - no OpenCV, no display, so it builds and runs anywhere
 * - A once-per-second per-stream report (rate, throughput, encoding and dimensions),
 *   redrawn in place rather than appended
 *
 * The counterpart with pictures is ros_sensor_viewer.
 *
 * Usage: ros2 run igris_c_sdk ros_sensor_listener --ros-args -p robot_namespace:=<ns>
 */
// Camera stream listener for the ROS 2 lane of igris_c_camera.
//
// The camera publishes STANDARD sensor_msgs on this lane (not igris_c_sdk types),
// so this example needs no SDK message headers. Topic names match
// RosImageSink::resolveName / RosImuSink: "/<robot_namespace>/<topic>", with the
// compressed variant on "<topic>/compressed".
//
// Each image topic carries at most one of the two forms at a time, decided by the
// per-stream config: a raw stream publishes sensor_msgs/Image, an encoded stream
// (jpeg color, png16 depth) publishes sensor_msgs/CompressedImage. Subscribing to
// both and reporting whichever arrives is what makes this usable as a config probe.

#include "common.hpp"

#include <cstdio>
#include <map>
#include <mutex>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <string>
#include <vector>

using igris_c_sdk_examples::g_running;
using igris_c_sdk_examples::signalHandler;

namespace {

// One row of the once-per-second report. Counts are reset after each report so the
// printed value is a rate, not a running total.
struct StreamStat {
    std::uint64_t frames = 0;
    std::uint64_t bytes  = 0;
    std::string detail;  // encoding/format plus dimensions, from the last message
};

std::mutex g_mutex;
std::map<std::string, StreamStat> g_stats;

void record(const std::string &name, std::size_t bytes, const std::string &detail) {
    std::lock_guard<std::mutex> lock(g_mutex);
    StreamStat &s = g_stats[name];
    s.frames += 1;
    s.bytes += bytes;
    s.detail = detail;
}

}  // namespace

int main(int argc, char **argv) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    igris_c_sdk_examples::initRclcpp(argc, argv);
    auto node                         = rclcpp::Node::make_shared("igris_c_ros_sensor_listener");
    const std::string robot_namespace = igris_c_sdk_examples::resolveRobotNamespace(node);

    // Every stream igris_c_camera can enable. A disabled stream simply never fires,
    // so an absent row in the report means "not published", not "subscription failed".
    const std::vector<std::string> image_topics = {
        "sensor/d435_color",  "sensor/d435_depth", "sensor/d435_ir_left", "sensor/d435_ir_right",
        "sensor/eyes_stereo", "sensor/left_hand",  "sensor/right_hand",
    };
    const std::string imu_topic = "sensor/d435_imu";

    // Echo the resolved wiring before subscribing. An empty robot_namespace and a
    // ROS_DOMAIN_ID that does not match the publisher are the two ways this example sits
    // silent while everything "looks" fine, so print both plus every resolved topic
    // rather than making the reader reconstruct them from the parameter.
    igris_c_sdk_examples::printBanner("Sensor Listener", robot_namespace);
    for (std::size_t i = 0; i < image_topics.size(); ++i) {
        std::printf("%-16s %s[/compressed]\n", (i == 0) ? "image topics    :" : "",
                    igris_c_sdk_examples::resolveRosTopic(robot_namespace, image_topics[i]).c_str());
    }
    std::printf("imu topic       : %s\n", igris_c_sdk_examples::resolveRosTopic(robot_namespace, imu_topic).c_str());

    std::vector<rclcpp::SubscriptionBase::SharedPtr> subs;
    for (const std::string &topic : image_topics) {
        const std::string raw_name = igris_c_sdk_examples::resolveRosTopic(robot_namespace, topic);

        // SensorDataQoS matches the publisher side; a reliable subscriber would not
        // match the camera's best-effort writer and would receive nothing.
        subs.push_back(node->create_subscription<sensor_msgs::msg::Image>(
            raw_name, rclcpp::SensorDataQoS(), [topic](const sensor_msgs::msg::Image &msg) {
                record(topic, msg.data.size(),
                       msg.encoding + " " + std::to_string(msg.width) + "x" + std::to_string(msg.height) +
                           " step=" + std::to_string(msg.step));
            }));

        subs.push_back(node->create_subscription<sensor_msgs::msg::CompressedImage>(
            raw_name + "/compressed", rclcpp::SensorDataQoS(),
            [topic](const sensor_msgs::msg::CompressedImage &msg) { record(topic + "/compressed", msg.data.size(), msg.format); }));
    }

    subs.push_back(node->create_subscription<sensor_msgs::msg::Imu>(igris_c_sdk_examples::resolveRosTopic(robot_namespace, imu_topic),
                                                                    rclcpp::SensorDataQoS(), [imu_topic](const sensor_msgs::msg::Imu &msg) {
                                                                        char detail[128];
                                                                        std::snprintf(detail, sizeof(detail),
                                                                                      "accel=[%.2f %.2f %.2f] gyro=[%.2f %.2f %.2f]",
                                                                                      msg.linear_acceleration.x, msg.linear_acceleration.y,
                                                                                      msg.linear_acceleration.z, msg.angular_velocity.x,
                                                                                      msg.angular_velocity.y, msg.angular_velocity.z);
                                                                        record(imu_topic, 0, detail);
                                                                    }));

    // Redraw the report IN PLACE once a second rather than appending it. Appending grew the
    // scrollback by one block per second for as long as the probe ran, which is what made a
    // long run unreadable; the numbers are a current snapshot, so nothing is lost by
    // overwriting. The banner is reprinted because the clear wipes it.
    auto timer = node->create_wall_timer(std::chrono::seconds(1), [robot_namespace] {
        std::lock_guard<std::mutex> lock(g_mutex);
        std::printf("%s", igris_c_sdk_examples::kRedraw);
        igris_c_sdk_examples::printBanner("Sensor Listener", robot_namespace);
        if (g_stats.empty()) {
            std::printf("no camera streams received\n");
            std::fflush(stdout);
            return;
        }
        for (auto &entry : g_stats) {
            StreamStat &s = entry.second;
            std::printf("%-32s %3lu hz  %8.1f KiB/s  %s\n", entry.first.c_str(), static_cast<unsigned long>(s.frames),
                        static_cast<double>(s.bytes) / 1024.0, s.detail.c_str());
            s.frames = 0;
            s.bytes  = 0;
        }
        std::printf("\n");
        std::fflush(stdout);
    });

    // Everything from here is redrawn in place, so it runs on the alternate screen: the wiring
    // printed above stays in the real terminal as a record, and Ctrl+C restores it untouched.
    const igris_c_sdk_examples::AltScreen alt_screen;

    // A steady spin_some() loop, NOT rclcpp::spin().
    //
    // spin() was tried and is worse here. Throughput is identical - 13 one-second windows
    // against the robot summed to ~162 Hz on the d435 IMU either way, because on this single
    // -threaded node the eight image subscriptions, not the spin call, are what the executor
    // spends its time on. What changes is the report: with spin() the wall timer competes with
    // those callbacks and fires late, and since the report prints the frame COUNT as "hz" it
    // then swings between 7 and 413 for a stream that never left ~200. The 1 ms loop keeps the
    // timer punctual, which is what makes the number mean what it says.
    while (rclcpp::ok() && g_running.load(std::memory_order_relaxed)) {
        rclcpp::spin_some(node);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    rclcpp::shutdown();
    return 0;
}
