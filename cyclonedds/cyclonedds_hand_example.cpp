/**
 * @file cyclonedds_hand_example.cpp
 * @brief Headless hand control example using the IGRIS SDK (igris_c_sdk).
 *
 * Demonstrates the end-effector (hand) control flow over igris_c_sdk DDS channels:
 *  - Initialization via the service/hand_init request/response service
 *  - HandCmd publishing (normalized 0~1 joint targets) on handcmd
 *  - HandState subscription on handstate
 *
 * Works for any end effector type (dexterous hand / 1-dof / magnet); only the
 * driven motor ids differ (hand: 11~16/21~26, 1-dof: 11/21, magnet: 11/12/21/22).
 *
 * Robot namespace: every topic is resolved under the robot-unit namespace passed
 * to ChannelFactory::Init(). e.g. namespace "igris_c_IG01" turns the bare topic "handcmd" into "igris_c_IG01/handcmd".
 *
 * Usage: ./cyclonedds_hand_example <domain_id> <namespace>
 *        ./cyclonedds_hand_example 0 igris_c_IG01
 */

#include "common.hpp"
#include "igris_c_sdk/namespace_resolver.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <igris_c_sdk/channel_factory.hpp>
#include <igris_c_sdk/publisher.hpp>
#include <igris_c_sdk/subscriber.hpp>
#include <igris_c_sdk/types.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace igris_c_sdk;

static std::atomic<bool> g_running(true);
static void SignalHandler(int) { g_running = false; }

// Hand motor ids: right 11~16, left 21~26. (1-dof: {11,21}, magnet: {11,12,21,22})
static constexpr std::array<uint16_t, 12> kMotorIds = {11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 25, 26};

// Build a HandCmd from 12 normalized [0,1] targets (one per motor id slot).
static igris_c_sdk::msg::dds_::HandCmd_ make_targets(const std::array<float, 12> &q) {
    igris_c_sdk::msg::dds_::HandCmd_ cmd;
    for (size_t i = 0; i < kMotorIds.size(); ++i) {
        auto &m = cmd.motor_cmd()[i];
        m.id(kMotorIds[i]);
        m.q(std::clamp(q[i], 0.0f, 1.0f));
    }
    return cmd;
}

static void sleep_ms(int ms) {
    if (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    // The naming flag may appear anywhere on the command line, so positional parsing has to skip it AND its value. Otherwise
    // "--dds-topic-naming ros_compatible" lands in domain_id and ns, and what follows is the silent failure the flag exists to
    // prevent: the two sides simply never discover each other.
    std::vector<const char *> positional;
    for (int i = 1; i < argc; ++i) {
        if (!igris_c_sdk::is_topic_naming_arg(argc, argv, i)) {
            positional.push_back(argv[i]);
        }
    }

    if (positional.size() < 2) {
        std::cerr << "Usage: " << argv[0] << " <domain_id> <namespace>\n"
                  << "       " << argv[0] << " 0 igris_c_IG01\n"
                  << igris_c_sdk::topic_naming_usage() << "\n";
        return 1;
    }
    const int domain_id  = std::atoi(positional[0]);
    const std::string ns = positional[1];  // robot-unit namespace, e.g. igris_c_IG01

    // Bare topic names are laid out on the wire by resolve_topic_name() using the namespace set here.
    const igris_c_sdk::TopicNaming naming = igris_c_sdk::parse_topic_naming_args(argc, argv);
    igris_c_sdk_examples::printBanner("Hand Example", domain_id, ns, naming);

    ChannelFactory::Instance()->Init(domain_id, ns, "", naming);
    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    Publisher<igris_c_sdk::msg::dds_::HandCmd_> cmd_pub("handcmd", QosProfile::SensorData());
    cmd_pub.init();

    // Initialization is a request/response service (no id=99 in handcmd anymore).
    Publisher<igris_c_sdk::srv::dds_::HandInitRequest_Request_> init_req_pub("service/hand_init/request", QosProfile::Services());
    init_req_pub.init();

    Subscriber<igris_c_sdk::msg::dds_::HandState_> state_sub("handstate", QosProfile::SensorData());
    state_sub.init([](const igris_c_sdk::msg::dds_::HandState_ &s) {
        if (s.motor_state().size() >= 3) {
            std::printf("q[0..2]=%.3f %.3f %.3f\n", s.motor_state()[0].q(), s.motor_state()[1].q(), s.motor_state()[2].q());
        }
    });

    Subscriber<igris_c_sdk::srv::dds_::HandInitRequest_Response_> init_res_sub("service/hand_init/response", QosProfile::Services());
    init_res_sub.init([](const igris_c_sdk::srv::dds_::HandInitRequest_Response_ &r) {
        std::printf("[hand_init] success=%d code=%d msg=%s\n", r.success(), static_cast<int>(r.error_code()), r.message().c_str());
    });

    // 1) Trigger initialization via the hand_init service.
    igris_c_sdk::srv::dds_::HandInitRequest_Request_ init_req;
    init_req.request_id() = "hand_init";
    init_req_pub.write(init_req);
    sleep_ms(1500);

    // 2) Fully open.
    cmd_pub.write(make_targets({0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f}));
    sleep_ms(2000);

    // 3) Close all joints.
    cmd_pub.write(make_targets({0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f}));
    sleep_ms(2500);

    // 4) Basic pinch (right thumb + index + middle), others relaxed.
    std::array<float, 12> pinch = {};
    pinch[0]                    = 0.80f;  // right thumb  (id=11)
    pinch[1]                    = 0.75f;  // right index  (id=12)
    pinch[2]                    = 0.60f;  // right middle (id=13)
    cmd_pub.write(make_targets(pinch));
    sleep_ms(2000);

    // 5) Back to open.
    cmd_pub.write(make_targets({0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f}));
    sleep_ms(1500);

    cmd_pub.stop();
    state_sub.stop();
    init_res_sub.stop();
    init_req_pub.stop();
    std::cout << "Done." << std::endl;
    return 0;
}
