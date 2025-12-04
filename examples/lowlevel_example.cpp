/**
 * @file lowlevel_example.cpp
 * @brief Simple low-level control example using IGRIS SDK
 *
 * This example demonstrates:
 * - LowState subscription (robot state monitoring)
 * - LowCmd publishing (position control at 300Hz)
 * - Simple sine wave motion on neck joints
 *
 * Usage: ./lowlevel_example [domain_id]
 */

#include <atomic>
#include <chrono>
#include <cmath>
#include <csignal>
#include <igris_sdk/channel_factory.hpp>
#include <igris_sdk/igris_c_client.hpp>
#include <igris_sdk/publisher.hpp>
#include <igris_sdk/subscriber.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

using namespace igris_sdk;
using namespace igris_c::msg::dds;

// Constants
static const int NUM_MOTORS = 31;
static const int NECK_PITCH = 30;

// Global state
static std::atomic<bool> g_running(true);
static std::mutex g_state_mutex;
static LowState g_latest_state;
static bool g_state_received = false;

// Initial positions (captured on first state receive)
static std::array<float, NUM_MOTORS> g_initial_pos = {};

// Signal handler
void SignalHandler(int) { g_running = false; }

// LowState callback
void LowStateCallback(const LowState &state) {
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

    std::cout << "=== IGRIS SDK Low-Level Example ===" << std::endl;

    // Parse domain ID
    int domain_id = 0;
    if (argc > 1) {
        domain_id = std::atoi(argv[1]);
    }
    std::cout << "Domain ID: " << domain_id << std::endl;

    // Initialize SDK
    ChannelFactory::Instance()->Init(domain_id);
    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    // Create subscriber
    Subscriber<LowState> state_sub("rt/lowstate");
    if (!state_sub.init(LowStateCallback)) {
        std::cerr << "Failed to initialize LowState subscriber" << std::endl;
        return 1;
    }
    std::cout << "LowState subscriber initialized" << std::endl;

    // Create publisher
    Publisher<LowCmd> cmd_pub("rt/lowcmd");
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

    int count = 0;
    while (g_running) {
        // Create command
        LowCmd cmd;
        cmd.kinematic_mode(KinematicMode::PJS);  // Joint Space (전체 적용)

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

        // Print status every second
        if (++count % 300 == 0) {
            std::lock_guard<std::mutex> lock(g_state_mutex);
            auto &imu = g_latest_state.imu_state();
            std::cout << "Time: " << std::fixed << std::setprecision(1) << time << "s"
                      << " | IMU RPY: [" << std::setprecision(2) << imu.rpy()[0] << ", " << imu.rpy()[1] << ", " << imu.rpy()[2] << "]"
                      << " | Neck Pitch: " << g_latest_state.joint_state()[NECK_PITCH].q() << std::endl;
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
