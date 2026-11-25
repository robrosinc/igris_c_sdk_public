/**
 * @file cyclonedds_tow3_state.cpp
 * @brief Read-only MasterArmState monitor for the TOW3 master arm using the IGRIS SDK (igris_c_sdk).
 *
 * Subscribes to masterarmstate, the telemetry the TOW3 master arm publishes every tick, and redraws the latest sample in
 * place:
 *  - state (MASTERARM_NOT_INITIALIZED / INITIALIZED / PUBLISHING), receive rate and sample age
 *  - arm joints: left 15..21 and right 22..28 (joint_state q/dq, motor_state q)
 *  - waist joints 0..2, which the master arm fills from the backpack IMU pose
 *  - imu_state (backpack IMU) and joy_state (hand controllers)
 *
 * Nothing is published. The example only reads, so it can run next to a live teleop session.
 *
 * joint_state is in the robot's joint convention, not raw leader-arm angles: the master arm applies axis_dir * scale and
 * clamps to the robot joint limits before publishing. motor_state equals joint_state except for the parallel pairs (wrist
 * roll/pitch, waist roll/pitch), which carry the converted motor angles.
 *
 * Robot namespace: every topic is resolved under the namespace passed to ChannelFactory::Init(). Use the same domain id,
 * namespace and topic naming as the master arm; e.g. namespace "igris_c_IG01" turns the bare topic "masterarmstate"
 * into "igris_c_IG01/masterarmstate".
 *
 * Usage: ./cyclonedds_tow3_state <domain_id> <namespace>
 *        ./cyclonedds_tow3_state 0 igris_c_IG01
 */

#include "common.hpp"
#include "igris_c_sdk/namespace_resolver.hpp"

#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <igris_c_sdk/channel_factory.hpp>
#include <igris_c_sdk/subscriber.hpp>
#include <igris_c_sdk/types.hpp>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace igris_c_sdk;
using MasterArmState = igris_c_sdk::msg::dds_::MasterArmState_;
using Clock          = std::chrono::steady_clock;

static std::atomic<bool> g_running(true);
static void SignalHandler(int) { g_running = false; }

// joint_state / motor_state are indexed by robot motor id (N_JOINTS = 31). The TOW3 master arm fills the arms (15..28)
// and the waist (0..2); every other slot stays zero.
static constexpr std::size_t kArmJointCount                              = 7;
static constexpr std::size_t kLeftArmBase                                = 15;  // 15..21 = left arm
static constexpr std::size_t kRightArmBase                               = 22;  // 22..28 = right arm
static constexpr std::array<const char *, kArmJointCount> kArmJointNames = {
    "Shoulder_Pitch", "Shoulder_Roll", "Shoulder_Yaw", "Elbow_Pitch", "Wrist_Yaw", "Wrist_Roll", "Wrist_Pitch"};
static constexpr std::array<const char *, 3> kWaistJointNames = {"Waist_Yaw", "Waist_Roll", "Waist_Pitch"};  // ids 0..2

// Shown as STALE when the newest sample is older than this. Display threshold only.
static constexpr double kStaleAfterSec = 0.5;

// Latest sample, written by the DDS delivery thread and read by the redraw loop.
static std::mutex g_mutex;
static MasterArmState g_latest;
static bool g_received = false;
static Clock::time_point g_latest_at;
static std::atomic<uint64_t> g_rx_count(0);

static void OnMasterArmState(const MasterArmState &msg) {
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_latest    = msg;
        g_received  = true;
        g_latest_at = Clock::now();
    }
    g_rx_count.fetch_add(1, std::memory_order_relaxed);
}

static const char *StateName(uint8_t state) {
    namespace c = igris_c_sdk::msg::dds_::MasterArmState_Constants;
    switch (state) {
    case c::MASTERARM_NOT_INITIALIZED:
        return "NOT_INITIALIZED";
    case c::MASTERARM_INITIALIZED:
        return "INITIALIZED";
    case c::MASTERARM_PUBLISHING:
        return "PUBLISHING";
    default:
        return "UNKNOWN";
    }
}

static double RadToDeg(double rad) { return rad * 180.0 / M_PI; }

static void PrintWaiting() {
    std::printf("Waiting for masterarmstate...\n\n");
    std::printf("No sample yet. Check that:\n");
    std::printf("  - domain id, namespace and --dds-topic-naming match the master arm's settings\n");
    std::printf("  - this host is on the network the master arm uses for DDS\n");
}

static void PrintSample(const MasterArmState &m, double rate_hz, double age_s) {
    const auto &js = m.joint_state();
    const auto &ms = m.motor_state();

    std::printf("state          : %s (%u)\n", StateName(m.state()), static_cast<unsigned>(m.state()));
    std::printf("rx rate        : %.1f Hz\n", rate_hz);
    std::printf("sample age     : %.3f s%s\n", age_s, age_s > kStaleAfterSec ? "   STALE" : "");
    std::printf("header stamp   : %d.%09u\n\n", m.header().stamp().sec(), m.header().stamp().nanosec());

    // Arms, left and right side by side.
    std::printf("%-14s | %-41s | %-41s\n", "ARM", "LEFT", "RIGHT");
    std::printf("%-14s | %2s %8s %8s %9s %9s | %2s %8s %8s %9s %9s\n", "joint", "id", "q[rad]", "q[deg]", "dq[rad/s]", "motor_q", "id",
                "q[rad]", "q[deg]", "dq[rad/s]", "motor_q");
    for (std::size_t i = 0; i < kArmJointCount; ++i) {
        const std::size_t l = kLeftArmBase + i;
        const std::size_t r = kRightArmBase + i;
        std::printf("%-14s | %2zu %+8.3f %+8.2f %+9.3f %+9.3f | %2zu %+8.3f %+8.2f %+9.3f %+9.3f\n", kArmJointNames[i], l, js[l].q(),
                    RadToDeg(js[l].q()), js[l].dq(), ms[l].q(), r, js[r].q(), RadToDeg(js[r].q()), js[r].dq(), ms[r].q());
    }

    std::printf("\n%-14s | %2s %8s %8s %9s\n", "WAIST (IMU)", "id", "q[rad]", "q[deg]", "motor_q");
    for (std::size_t i = 0; i < kWaistJointNames.size(); ++i) {
        std::printf("%-14s | %2zu %+8.3f %+8.2f %+9.3f\n", kWaistJointNames[i], i, js[i].q(), RadToDeg(js[i].q()), ms[i].q());
    }

    const auto &imu = m.imu_state();
    std::printf("\nIMU quat (x y z w)   : %+7.4f %+7.4f %+7.4f %+7.4f\n", imu.orientation().x(), imu.orientation().y(),
                imu.orientation().z(), imu.orientation().w());
    std::printf("IMU gyro [rad/s]     : %+8.3f %+8.3f %+8.3f\n", imu.angular_velocity().x(), imu.angular_velocity().y(),
                imu.angular_velocity().z());
    std::printf("IMU accel [m/s^2]    : %+8.3f %+8.3f %+8.3f\n", imu.linear_acceleration().x(), imu.linear_acceleration().y(),
                imu.linear_acceleration().z());

    // joy_state layout as written by the master arm:
    //   axes[6]    = [L.x, L.y, L.trigger, R.x, R.y, R.trigger]
    //   buttons[8] = [L.sw1, L.sw2, L.sw3, L.sel, R.sw1, R.sw2, R.sw3, R.sel]  (0/1; an offline controller reports zeros)
    const auto &joy = m.joy_state();
    std::printf("\nJOY axes    [L.x L.y L.trig R.x R.y R.trig]          :");
    for (const float a : joy.axes()) {
        std::printf(" %+5.2f", a);
    }
    std::printf("\nJOY buttons [L.sw1 sw2 sw3 sel  R.sw1 sw2 sw3 sel]   :");
    for (const int32_t b : joy.buttons()) {
        std::printf(" %d", static_cast<int>(b));
    }
    std::printf("\n");
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
    const std::string ns = positional[1];  // same namespace as the master arm

    // Bare topic names are laid out on the wire by resolve_topic_name() using the namespace set here.
    const igris_c_sdk::TopicNaming naming = igris_c_sdk::parse_topic_naming_args(argc, argv);
    igris_c_sdk_examples::printBanner("TOW3 State", domain_id, ns, naming);

    ChannelFactory::Instance()->Init(domain_id, ns, "", naming);
    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    // Best-effort reader: it matches the master arm's writer whatever its reliability, and sends no acknowledgements
    // back to the master arm, so a monitor on a slow link cannot add traffic to it. Dropped samples only cost a frame.
    Subscriber<MasterArmState> state_sub("masterarmstate", QosProfile::SensorData());
    if (!state_sub.init(OnMasterArmState)) {
        std::cerr << "Failed to initialize MasterArmState subscriber" << std::endl;
        return 1;
    }
    std::printf("MasterArmState subscriber initialized. Press Ctrl+C to stop.\n");
    std::fflush(stdout);

    {
        // Everything from here is redrawn in place, so it runs on the alternate screen: the wiring printed above stays in
        // the real terminal as a record, and Ctrl+C restores it untouched.
        const igris_c_sdk_examples::AltScreen alt_screen;

        auto rate_since    = Clock::now();
        uint64_t rate_from = 0;
        double rate_hz     = 0.0;

        while (g_running) {
            const auto now        = Clock::now();
            const double window_s = std::chrono::duration<double>(now - rate_since).count();
            if (window_s >= 1.0) {
                const uint64_t count = g_rx_count.load(std::memory_order_relaxed);
                rate_hz              = static_cast<double>(count - rate_from) / window_s;
                rate_from            = count;
                rate_since           = now;
            }

            MasterArmState snapshot;
            bool received = false;
            double age_s  = 0.0;
            {
                std::lock_guard<std::mutex> lock(g_mutex);
                received = g_received;
                if (received) {
                    snapshot = g_latest;
                    age_s    = std::chrono::duration<double>(now - g_latest_at).count();
                }
            }

            std::fputs(igris_c_sdk_examples::kRedraw, stdout);
            igris_c_sdk_examples::printBanner("TOW3 State", domain_id, ns, naming);
            if (received) {
                PrintSample(snapshot, rate_hz, age_s);
            } else {
                PrintWaiting();
            }
            std::printf("\nPress Ctrl+C to stop.\n");
            std::fflush(stdout);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    state_sub.stop();
    std::printf("Stopped.\n");
    return 0;
}
