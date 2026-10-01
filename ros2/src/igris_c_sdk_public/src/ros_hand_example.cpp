/**
 * @file ros_hand_example.cpp
 * @brief Headless end-effector (hand) control example over ROS2, mirroring
 *        cyclonedds/cyclonedds_hand_example.cpp.
 *
 * Demonstrates the hand control flow now that the bridge owns the hand ROS2 topics:
 *  - Initialization via the service/hand_init service
 *  - HandCmd publishing (normalized 0~1 joint targets) on handcmd
 *  - HandState subscription on handstate
 *
 * Works for any end effector type (dexterous hand / 1-dof / magnet); only the driven
 * motor ids differ (hand: 11~16/21~26, 1-dof: 11/21, magnet: 11/12/21/22).
 *
 * Usage:
 *   ros2 run igris_c_sdk ros_hand_example --ros-args -p robot_namespace:=<ns>
 */

#include "common.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <igris_c_sdk/msg/hand_cmd.hpp>
#include <igris_c_sdk/msg/hand_state.hpp>
#include <igris_c_sdk/srv/hand_init_request.hpp>
#include <rclcpp/rclcpp.hpp>
#include <thread>

using igris_c_sdk_examples::g_running;
using igris_c_sdk_examples::signalHandler;

// Hand motor ids: right 11~16, left 21~26. (1-dof: {11,21}, magnet: {11,12,21,22})
static constexpr std::array<uint16_t, 12> kMotorIds = {11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 25, 26};

// Build a HandCmd from 12 normalized [0,1] targets (one per motor id slot).
static igris_c_sdk::msg::HandCmd makeTargets(const std::array<float, 12> &q) {
    igris_c_sdk::msg::HandCmd cmd;
    for (size_t i = 0; i < kMotorIds.size(); ++i) {
        cmd.motor_cmd[i].id = kMotorIds[i];
        cmd.motor_cmd[i].q  = std::clamp(q[i], 0.0f, 1.0f);
    }
    return cmd;
}

int main(int argc, char **argv) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    igris_c_sdk_examples::initRclcpp(argc, argv);
    auto node            = rclcpp::Node::make_shared("igris_c_ros_hand_example");
    const std::string ns = igris_c_sdk_examples::resolveRobotNamespace(node);
    igris_c_sdk_examples::printBanner("Hand Example", ns);

    auto cmd_pub =
        node->create_publisher<igris_c_sdk::msg::HandCmd>(igris_c_sdk_examples::resolveRosTopic(ns, "handcmd"), rclcpp::SensorDataQoS());
    auto state_sub = node->create_subscription<igris_c_sdk::msg::HandState>(
        igris_c_sdk_examples::resolveRosTopic(ns, "handstate"), rclcpp::SensorDataQoS(), [](const igris_c_sdk::msg::HandState &s) {
            if (s.motor_state.size() >= 3) {
                std::printf("q[0..2]=%.3f %.3f %.3f\n", s.motor_state[0].q, s.motor_state[1].q, s.motor_state[2].q);
            }
        });
    auto init_client =
        node->create_client<igris_c_sdk::srv::HandInitRequest>(igris_c_sdk_examples::resolveRosTopic(ns, "service/hand_init"));

    // Spin the subscription + service response in a background executor.
    rclcpp::executors::SingleThreadedExecutor exec;
    exec.add_node(node);
    std::thread spin_thread([&exec]() { exec.spin(); });

    auto sleep_ms = [](int ms) {
        if (g_running.load(std::memory_order_relaxed)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }
    };

    // 1) Trigger initialization via the hand_init service.
    if (init_client->wait_for_service(std::chrono::seconds(3))) {
        auto req        = std::make_shared<igris_c_sdk::srv::HandInitRequest::Request>();
        req->request_id = igris_c_sdk_examples::makeRequestId("hand_init");
        auto future     = init_client->async_send_request(req);
        if (future.wait_for(std::chrono::seconds(10)) == std::future_status::ready) {
            auto res = future.get();
            std::printf("[hand_init] success=%d msg=%s\n", res->success, res->message.c_str());
        } else {
            std::fprintf(stderr, "[hand_init] request timeout\n");
        }
    } else {
        std::fprintf(stderr, "hand_init service not available; continuing with handcmd only\n");
    }
    sleep_ms(1500);

    // 2) Fully open.
    cmd_pub->publish(makeTargets({0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f}));
    sleep_ms(2000);

    // 3) Close all joints.
    cmd_pub->publish(makeTargets({0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f}));
    sleep_ms(2500);

    // 4) Basic pinch (right thumb + index + middle), others relaxed.
    std::array<float, 12> pinch = {};
    pinch[0]                    = 0.80f;  // right thumb  (id=11)
    pinch[1]                    = 0.75f;  // right index  (id=12)
    pinch[2]                    = 0.60f;  // right middle (id=13)
    cmd_pub->publish(makeTargets(pinch));
    sleep_ms(2000);

    // 5) Back to open.
    cmd_pub->publish(makeTargets({0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f}));
    sleep_ms(1500);

    std::printf("Done.\n");
    exec.cancel();
    if (spin_thread.joinable()) {
        spin_thread.join();
    }
    rclcpp::shutdown();
    return 0;
}
