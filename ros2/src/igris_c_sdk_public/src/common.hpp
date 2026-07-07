#pragma once

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <stdexcept>
#include <string>

namespace igris_c_sdk_examples {

inline std::atomic<bool> g_running{true};

inline void signalHandler(int) { g_running.store(false, std::memory_order_relaxed); }

inline std::string makeRequestId(const std::string &prefix) {
    return prefix + "_" +
           std::to_string(
               std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
}

inline std::string normalizeNamespace(std::string ns) {
    while (!ns.empty() && ns.front() == '/') {
        ns.erase(ns.begin());
    }
    while (!ns.empty() && ns.back() == '/') {
        ns.pop_back();
    }
    return ns;
}

// Reads the robot namespace from the ROS parameter `robot_namespace`.
// Pass it explicitly when running an example:
//
//   ros2 run igris_c_sdk_examples <node> --ros-args -p robot_namespace:=<ns>
//
// or via a launch file. Empty string (default) means "no namespace".
inline std::string resolveRobotNamespace(const rclcpp::Node::SharedPtr &node) {
    return normalizeNamespace(node->declare_parameter<std::string>("robot_namespace", ""));
}

// ROS 2 binds the DDS domain id at participant-creation time (inside
// rclcpp::init), so it cannot be steered via a ROS parameter declared
// on the node — by then the participant already exists. Examples
// honour the standard `ROS_DOMAIN_ID` env var:
//
//   ROS_DOMAIN_ID=5 ros2 run igris_c_sdk_examples <node> \
//       --ros-args -p robot_namespace:=robot1
//
// rclcpp::init reads the env var implicitly, but we go through
// InitOptions so the policy is visible at the call site and a future
// override (CLI flag, config file, ...) only needs to be wired here.
inline std::size_t resolveRosDomainId(std::size_t default_id = 0) {
    const char *env = std::getenv("ROS_DOMAIN_ID");
    if (env == nullptr || env[0] == '\0') {
        return default_id;
    }
    try {
        return static_cast<std::size_t>(std::stoul(env));
    } catch (const std::exception &) {
        return default_id;
    }
}

inline void initRclcpp(int argc, char **argv) {
    rclcpp::InitOptions options;
    options.set_domain_id(resolveRosDomainId());
    rclcpp::init(argc, argv, options);
}

inline std::string resolveRtName(const std::string &robot_namespace, const std::string &relative_name) {
    if (relative_name.empty()) {
        return robot_namespace.empty() ? "/" : "/" + robot_namespace;
    }
    if (robot_namespace.empty()) {
        return "/" + relative_name;
    }
    return "/" + robot_namespace + "/" + relative_name;
}

}  // namespace igris_c_sdk_examples
