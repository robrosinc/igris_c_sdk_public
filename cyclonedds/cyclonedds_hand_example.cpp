/**
 * @file cyclonedds_hand_example.cpp
 * @brief Headless hand control example using the IGRIS SDK (igris_c_sdk).
 *
 * Demonstrates the end-effector (hand) control flow over igris_c_sdk DDS channels:
 *  - Initialization via the rt/service/hand_init request/response service
 *  - HandCmd publishing (normalized 0~1 joint targets) on rt/handcmd
 *  - HandState subscription on rt/handstate
 *
 * Works for any end effector type (dexterous hand / 1-dof / magnet); only the
 * driven motor ids differ (hand: 11~16/21~26, 1-dof: 11/21, magnet: 11/12/21/22).
 *
 * Robot namespace: every topic is resolved under the robot-unit namespace passed
 * to ChannelFactory::Init(). e.g. namespace "igris_c_IG01" -> "igris_c_IG01/rt/handcmd".
 *
 * Usage: ./cyclonedds_hand_example <domain_id> <namespace>
 *        ./cyclonedds_hand_example 0 igris_c_IG01
 */

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

using namespace igris_c_sdk;
using namespace igris_c::msg::dds;

static std::atomic<bool> g_running(true);
static void SignalHandler(int) { g_running = false; }

// Hand motor ids: right 11~16, left 21~26. (1-dof: {11,21}, magnet: {11,12,21,22})
static constexpr std::array<uint16_t, 12> kMotorIds = {11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 25, 26};

// Build a HandCmd from 12 normalized [0,1] targets (one per motor id slot).
static HandCmd make_targets(const std::array<float, 12> &q) {
    HandCmd cmd;
    for (size_t i = 0; i < kMotorIds.size(); ++i) {
        MotorCmd m;
        m.id(kMotorIds[i]);
        m.q(std::clamp(q[i], 0.0f, 1.0f));
        cmd.motor_cmd().push_back(m);
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

    std::cout << "=== IGRIS SDK Hand Example ===" << std::endl;
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <domain_id> <namespace>\n"
                  << "       " << argv[0] << " 0 igris_c_IG01\n";
        return 1;
    }
    const int domain_id  = std::atoi(argv[1]);
    const std::string ns = argv[2];  // robot-unit namespace, e.g. igris_c_IG01
    std::cout << "Domain ID: " << domain_id << ", Namespace: " << ns << std::endl;

    // Topics written as "rt/..." resolve to "<ns>/rt/..." once the namespace is set here.
    ChannelFactory::Instance()->Init(domain_id, ns);
    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    Publisher<HandCmd> cmd_pub("rt/handcmd", QosProfile::SensorData());
    cmd_pub.init();

    // Initialization is a request/response service (no id=99 in rt/handcmd anymore).
    Publisher<HandInitRequest> init_req_pub("rt/service/hand_init/request", QosProfile::Services());
    init_req_pub.init();

    Subscriber<HandState> state_sub("rt/handstate", QosProfile::SensorData());
    state_sub.init([](const HandState &s) {
        if (s.motor_state().size() >= 3) {
            std::printf("q[0..2]=%.3f %.3f %.3f\n", s.motor_state()[0].q(), s.motor_state()[1].q(), s.motor_state()[2].q());
        }
    });

    Subscriber<ServiceResponse> init_res_sub("rt/service/hand_init/response", QosProfile::Services());
    init_res_sub.init([](const ServiceResponse &r) {
        std::printf("[hand_init] success=%d code=%d msg=%s\n", r.success(), static_cast<int>(r.error_code()), r.message().c_str());
    });

    // 1) Trigger initialization via the hand_init service.
    HandInitRequest init_req;
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
