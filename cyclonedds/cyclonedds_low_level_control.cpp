/**
 * @file cyclonedds_low_level_control.cpp
 * @brief Simple low-level control example using IGRIS SDK
 *
 * This example demonstrates:
 * - LowState subscription (robot state monitoring)
 * - LowCmd publishing (position control at 300Hz)
 * - Simple sine wave motion on neck joints
 *
 * Usage: ./cyclonedds_low_level_control <domain_id> <namespace>
 */

#include "common.hpp"
#include "igris_c_sdk/namespace_resolver.hpp"

#include <atomic>
#include <chrono>
#include <cmath>
#include <csignal>
#include <igris_c_sdk/channel_factory.hpp>
#include <igris_c_sdk/igris_c_client.hpp>
#include <igris_c_sdk/publisher.hpp>
#include <igris_c_sdk/subscriber.hpp>
#include <igris_c_sdk/types.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace igris_c_sdk;

// Constants (NUM_MOTORS comes from igris_c_sdk/types.hpp)
static const int NECK_PITCH = 30;

// Global state
static std::atomic<bool> g_running(true);
static std::mutex g_state_mutex;
static igris_c_sdk::msg::dds_::LowState_ g_latest_state;
static bool g_state_received = false;

// Initial positions (captured on first state receive)
static std::array<float, NUM_MOTORS> g_initial_pos = {};

// Signal handler
void SignalHandler(int) { g_running = false; }

// LowState callback
void LowStateCallback(const igris_c_sdk::msg::dds_::LowState_ &state) {
    std::lock_guard<std::mutex> lock(g_state_mutex);
    g_latest_state = state;

    // Capture initial positions on first receive
    if (!g_state_received) {
        for (int i = 0; i < NUM_MOTORS; i++) {
            g_initial_pos[i] = state.joint_state()[i].q();
        }
        g_state_received = true;
        std::cout << "Initial state captured" << std::endl;
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
        std::cerr << "Usage: " << argv[0] << " <domain_id> <namespace>" << std::endl;
        std::cerr << "       " << argv[0] << " 0 \"\" (no namespace)" << std::endl;
        std::cerr << igris_c_sdk::topic_naming_usage() << std::endl;
        return 1;
    }
    const int domain_id  = std::atoi(positional[0]);
    const std::string ns = positional[1];

    // Initialize SDK
    const igris_c_sdk::TopicNaming naming = igris_c_sdk::parse_topic_naming_args(argc, argv);
    igris_c_sdk_examples::printBanner("Low-Level Control", domain_id, ns, naming);

    ChannelFactory::Instance()->Init(domain_id, ns, "", naming);
    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    // Create subscriber
    Subscriber<igris_c_sdk::msg::dds_::LowState_> state_sub("lowstate", QosProfile::SensorData());
    if (!state_sub.init(LowStateCallback)) {
        std::cerr << "Failed to initialize LowState subscriber" << std::endl;
        return 1;
    }
    std::cout << "LowState subscriber initialized" << std::endl;

    // Create publisher
    Publisher<igris_c_sdk::msg::dds_::LowCmd_> cmd_pub("lowcmd", QosProfile::SensorData());
    if (!cmd_pub.init()) {
        std::cerr << "Failed to initialize LowCmd publisher" << std::endl;
        return 1;
    }
    std::cout << "LowCmd publisher initialized" << std::endl;

    // Wait for first state
    std::cout << "Waiting for robot state..." << std::endl;
    while (!g_state_received && g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!g_running) {
        std::cout << "Interrupted" << std::endl;
        return 0;
    }

    // Example PD gains (adjust for your robot)
    static const std::array<float, NUM_MOTORS> kp = {
        50.0,  25.0,  25.0,                            // Waist
        500.0, 200.0, 50.0, 500.0, 300.0, 300.0,       // Left leg
        500.0, 200.0, 50.0, 500.0, 300.0, 300.0,       // Right leg
        50.0,  50.0,  30.0, 30.0,  5.0,   5.0,   5.0,  // Left arm
        50.0,  50.0,  30.0, 30.0,  5.0,   5.0,   5.0,  // Right arm
        2.0,   5.0                                     // Neck
    };
    static const std::array<float, NUM_MOTORS> kd = {
        0.8,  0.8, 0.8,                        // Waist
        3.0,  0.5, 0.5,  3.0,  1.5, 1.5,       // Left leg
        3.0,  0.5, 0.5,  3.0,  1.5, 1.5,       // Right leg
        0.5,  0.5, 0.15, 0.15, 0.1, 0.1, 0.1,  // Left arm
        0.5,  0.5, 0.15, 0.15, 0.1, 0.1, 0.1,  // Right arm
        0.05, 0.1                              // Neck
    };

    // Control loop parameters
    const auto control_period = std::chrono::microseconds(3333);  // ~300Hz
    auto next_time            = std::chrono::steady_clock::now();
    double time               = 0.0;
    const double dt           = 0.003333;

    // Motion parameters
    const double amplitude = 0.3;  // radians
    const double frequency = 0.5;  // Hz

    std::cout << "\nStarting control loop (300Hz)" << std::endl;
    std::cout << "Neck pitch will nod up and down" << std::endl;
    std::cout << "Press Ctrl+C to stop\n" << std::endl;

    // Everything from here is redrawn in place, so it runs on the alternate screen: the wiring
    // printed above stays in the real terminal as a record, and Ctrl+C restores it untouched.
    const igris_c_sdk_examples::AltScreen alt_screen;

    int count = 0;
    while (g_running) {
        // Create command
        igris_c_sdk::msg::dds_::LowCmd_ cmd;
        cmd.kinematic_modes().fill(igris_c_sdk::msg::dds_::LowCmd_Constants::KINEMATIC_MODE_PJS);  // Joint space for all parallel links

        // Set all motors to hold initial position
        for (int i = 0; i < NUM_MOTORS; i++) {
            auto &motor_cmd = cmd.motors()[i];
            motor_cmd.id(i);
            motor_cmd.q(g_initial_pos[i]);
            motor_cmd.dq(0.0f);
            motor_cmd.tau(0.0f);
            motor_cmd.kp(kp[i]);
            motor_cmd.kd(kd[i]);
        }

        // Apply sine wave motion to neck pitch (nodding from zero position)
        double neck_pitch_target = amplitude * std::sin(2.0 * M_PI * frequency * time);
        cmd.motors()[NECK_PITCH].q(neck_pitch_target);

        // Publish command
        cmd_pub.write(cmd);

        // Redraw the status block every second IN PLACE. Appending a line per second instead
        // grew the scrollback for as long as the example ran, which buries the banner and the
        // controls under output nobody scrolls back to read.
        if (++count % 300 == 0) {
            std::lock_guard<std::mutex> lock(g_state_mutex);
            std::cout << igris_c_sdk_examples::kRedraw;
            igris_c_sdk_examples::printBanner("Low-Level Control", domain_id, ns, naming);
            std::cout << "elapsed        : " << std::fixed << std::setprecision(1) << time << " s\n";
            std::cout << "neck pitch [q] : " << std::setprecision(2) << g_latest_state.joint_state()[NECK_PITCH].q() << "\n";
            std::cout << "\nPress Ctrl+C to stop." << std::endl;
        }

        // Update time
        time += dt;

        // Sleep until next cycle
        next_time += control_period;
        std::this_thread::sleep_until(next_time);
    }

    std::cout << "\nControl loop stopped" << std::endl;
    return 0;
}
