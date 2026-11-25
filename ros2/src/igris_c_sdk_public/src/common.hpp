/**
 * @file common.hpp
 * @brief Shared helpers for the ROS 2 examples: signal handling, namespace/topic resolution,
 *        rclcpp initialization, and the startup banner.
 *
 * Header-only, and copied into the deliverable alongside the example sources. Mirrors
 * examples/cyclonedds/common.hpp, which plays the same role on the native lane.
 */
#pragma once

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <stdexcept>
#include <string>
#include <unistd.h>

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

// Mirrors igris_c_sdk::resolve_topic_name(ns, topic, TopicNaming::Ros). Duplicated here because this examples package depends only on the
// message package, not on the CycloneDDS SDK library that carries the shared implementation. Pass the BARE topic: rmw mangles the result
// into rt/<ns>/<topic> on the wire, so the ROS lane must not carry an rt/ segment of its own.
// Clear the screen and park the cursor at the top-left.
//
// For examples that reprint the same block forever: writing it again below the last one grows
// the scrollback without bound, which makes the terminal unusable long before the data gets
// interesting. Callers redraw the banner too, since this wipes it.
inline constexpr const char *kRedraw = "\x1B[2J\x1B[H";

// Run a redrawing example on the terminal's ALTERNATE screen, like less/vim/ncurses do.
//
// kRedraw alone is not enough. It erases what is displayed but does not touch the scrollback,
// so every frame is still stacked up there - scrolling back after a minute shows sixty copies
// of the same block - and whatever the last frame happened to be is left sitting in the
// terminal after Ctrl+C, on top of whatever the user had before. The alternate screen has no
// scrollback of its own and is discarded on exit, so the shell comes back exactly as it was.
//
// RAII rather than a pair of calls: every exit path has to restore, including the ones that
// return early. Guarded on isatty so a piped or redirected run writes plain text instead of
// escape sequences. SIGINT is safe because rclcpp's handler makes spin() return, so main()
// unwinds normally.
class AltScreen {
  public:
    AltScreen() : active_(::isatty(::fileno(stdout)) != 0) {
        if (active_) {
            std::fputs("\x1B[?1049h\x1B[?25l", stdout);  // enter alt screen, hide cursor
            std::fflush(stdout);
        }
    }
    ~AltScreen() {
        if (active_) {
            std::fputs("\x1B[?25h\x1B[?1049l", stdout);  // show cursor, restore screen
            std::fflush(stdout);
        }
    }
    AltScreen(const AltScreen &)            = delete;
    AltScreen &operator=(const AltScreen &) = delete;

  private:
    bool active_;
};

// The one startup block every example prints, so a reader can tell at a glance which robot a
// run is talking to. An empty namespace and a ROS_DOMAIN_ID that does not match the publisher
// are the two ways an example sits silent while everything "looks" fine - neither is an error,
// both are invisible - so both are printed rather than left to be reconstructed.
//
// The ROS lane has no topic-layout choice to report: rmw mangles the name itself, which is why
// the Cyclone DDS banner carries a line this one does not.
inline void printBanner(const std::string &title, const std::string &robot_namespace) {
    std::printf("=== IGRIS-C SDK %s (ROS 2) ===\n", title.c_str());
    std::printf("domain id      : %zu  (ROS_DOMAIN_ID)\n", resolveRosDomainId());
    std::printf("namespace      : %s\n", robot_namespace.empty() ? "(none)" : robot_namespace.c_str());
    std::printf("set the namespace with: --ros-args -p robot_namespace:=<ns>\n\n");
}

inline std::string resolveRosTopic(const std::string &robot_namespace, const std::string &topic) {
    if (topic.empty()) {
        return robot_namespace.empty() ? "/" : "/" + robot_namespace;
    }
    if (robot_namespace.empty()) {
        return "/" + topic;
    }
    return "/" + robot_namespace + "/" + topic;
}

}  // namespace igris_c_sdk_examples
