/**
 * @file sdk_gui_client.cpp
 * @brief GUI client example demonstrating IGRIS SDK features
 *
 * This example demonstrates:
 * - LowState subscription (real-time robot state monitoring)
 * - LowCmd publishing (low-level motor control at 300Hz)
 * - Service API calls (BMS init, torque control, control mode switching)
 *
 * GUI Layout:
 * - Left panel: 31 sliders with per-chain(waist/ankle/wrist) space selection
 * - Center panel: IMU state display + LowCmd publishing status
 * - Right panel: Service API buttons + response log window
 */

#include <atomic>
#include <chrono>
#include <cmath>
#include <csignal>
#include <deque>
#include <functional>
#include <future>
#include <igris_c_sdk/channel_factory.hpp>
#include <igris_c_sdk/igris_c_client.hpp>
#include <igris_c_sdk/publisher.hpp>
#include <igris_c_sdk/subscriber.hpp>
#include <iostream>
#include <mutex>
#include <thread>

// ImGui includes
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace igris_c_sdk;
using namespace igris_c::msg::dds;

// Motor and Joint names (1-indexed in data, 0-indexed in array)
static const std::array<const char *, 31> MOTOR_NAMES = {
    "Waist_Yaw",    "Waist_L",          "Waist_R",         "Hip_Pitch_L",    "Hip_Roll_L",    "Hip_Yaw_L",    "Knee_Pitch_L",
    "Ankle_Out_L",  "Ankle_In_L",       "Hip_Pitch_R",     "Hip_Roll_R",     "Hip_Yaw_R",     "Knee_Pitch_R", "Ankle_Out_R",
    "Ankle_In_R",   "Shoulder_Pitch_L", "Shoulder_Roll_L", "Shoulder_Yaw_L", "Elbow_Pitch_L", "Wrist_Yaw_L",  "Wrist_Front_L",
    "Wrist_Back_L", "Shoulder_Pitch_R", "Shoulder_Roll_R", "Shoulder_Yaw_R", "Elbow_Pitch_R", "Wrist_Yaw_R",  "Wrist_Front_R",
    "Wrist_Back_R", "Neck_Yaw",         "Neck_Pitch"};

static const std::array<const char *, 31> JOINT_NAMES = {
    "Waist_Yaw",     "Waist_Roll",       "Waist_Pitch",     "Hip_Pitch_L",    "Hip_Roll_L",    "Hip_Yaw_L",    "Knee_Pitch_L",
    "Ankle_Pitch_L", "Ankle_Roll_L",     "Hip_Pitch_R",     "Hip_Roll_R",     "Hip_Yaw_R",     "Knee_Pitch_R", "Ankle_Pitch_R",
    "Ankle_Roll_R",  "Shoulder_Pitch_L", "Shoulder_Roll_L", "Shoulder_Yaw_L", "Elbow_Pitch_L", "Wrist_Yaw_L",  "Wrist_Roll_L",
    "Wrist_Pitch_L", "Shoulder_Pitch_R", "Shoulder_Roll_R", "Shoulder_Yaw_R", "Elbow_Pitch_R", "Wrist_Yaw_R",  "Wrist_Roll_R",
    "Wrist_Pitch_R", "Neck_Yaw",         "Neck_Pitch"};

// Joint position limits (rad) from params.yaml
static const std::array<float, 31> JOINT_POS_MAX = {
    1.57f,                                                   // Waist_Yaw
    0.310f, 0.28f,                                           // Waist_Roll, Waist_Pitch
    0.480f, 2.300f, 1.570f, 2.280f, 0.698f, 0.349f,          // Left leg
    0.480f, 0.330f, 1.570f, 2.280f, 0.698f, 0.349f,          // Right leg
    1.047f, 3.140f, 1.570f, 0.0f,   1.570f, 0.870f, 0.650f,  // Left arm
    1.047f, 0.170f, 1.570f, 0.0f,   1.570f, 1.221f, 0.650f,  // Right arm
    1.221f, 0.520f                                           // Neck
};

static const std::array<float, 31> JOINT_POS_MIN = {
    -1.57f,                                                       // Waist_Yaw
    -0.310f, -0.87f,                                              // Waist_Roll, Waist_Pitch
    -2.000f, -0.330f, -1.570f, 0.0f,  -0.70f,  -0.350f,           // Left leg
    -2.000f, -2.300f, -1.570f, 0.0f,  -0.70f,  -0.350f,           // Right leg
    -3.141f, -0.170f, -1.570f, -2.0f, -1.570f, -1.221f, -0.650f,  // Left arm
    -3.141f, -3.140f, -1.570f, -2.0f, -1.570f, -0.870f, -0.650f,  // Right arm
    -1.221f, -0.520f                                              // Neck
};

// Motor position limits (rad) from params.yaml
static const std::array<float, 31> MOTOR_POS_MAX = {
    1.57f,                                                  // Waist_Yaw
    0.87f,  0.87f,                                          // Waist_L, Waist_R
    0.480f, 2.300f, 1.570f, 2.280f, 0.609f, 0.523f,         // Left leg
    0.480f, 0.330f, 1.570f, 2.280f, 0.609f, 0.523f,         // Right leg
    1.047f, 3.140f, 1.570f, 0.0f,   1.570f, 0.75f,  0.75f,  // Left arm
    1.047f, 0.170f, 1.570f, 0.0f,   1.570f, 0.98f,  0.98f,  // Right arm
    1.221f, 0.520f                                          // Neck
};

static const std::array<float, 31> MOTOR_POS_MIN = {
    -1.57f,                                                      // Waist_Yaw
    -0.34f,  -0.34f,                                             // Waist_L, Waist_R
    -2.000f, -0.330f, -1.570f, 0.0f,  -0.630f, -0.617f,          // Left leg
    -2.000f, -2.300f, -1.570f, 0.0f,  -0.630f, -0.617f,          // Right leg
    -3.141f, -0.170f, -1.570f, -2.0f, -1.570f, -0.98f,  -0.98f,  // Left arm
    -3.141f, -3.140f, -1.570f, -2.0f, -1.570f, -0.75f,  -0.75f,  // Right arm
    -1.221f, -0.520f                                             // Neck
};

// Global state
static std::atomic<bool> g_running(true);
static std::atomic<uint32_t> g_lowstate_received_count(0);

// Latest state data
static LowState g_latest_lowstate;
static std::mutex g_lowstate_mutex;
static bool g_first_state_received = false;

// RobotState subscription (auto-subscribe at startup)
static RobotState g_latest_robotstate;
static std::mutex g_robotstate_mutex;
static std::atomic<bool> g_robotstate_connected(false);
static std::atomic<uint32_t> g_robotstate_received_count(0);
static std::unique_ptr<Subscriber<RobotState>> g_robotstate_sub;
static std::chrono::steady_clock::time_point g_last_robotstate_time;

// Initial reference values (set on first state receive)
static std::array<float, 31> g_initial_motor_pos = {};
static std::array<float, 31> g_initial_joint_pos = {};

// GUI state
static std::array<float, 31> g_slider_values = {};

enum class SpaceSelection : int {
    MOTOR = 0,
    JOINT = 1,
};

static constexpr int kParallelLinkCount                     = 5;  // waist, left_ankle, right_ankle, left_wrist, right_wrist
static std::array<int, kParallelLinkCount> g_parallel_space = {
    static_cast<int>(SpaceSelection::MOTOR),  // waist
    static_cast<int>(SpaceSelection::MOTOR),  // left_ankle
    static_cast<int>(SpaceSelection::MOTOR),  // right_ankle
    static_cast<int>(SpaceSelection::MOTOR),  // left_wrist
    static_cast<int>(SpaceSelection::MOTOR),  // right_wrist
};

// LOW_LEVEL control state
static std::atomic<bool> g_lowlevel_active(false);
static std::array<float, 31> g_target_joint_pos  = {};  // Target joint positions for LOW_LEVEL mode
static std::array<float, 31> g_target_motor_pos  = {};  // Target motor positions for LOW_LEVEL mode
static std::array<float, 31> g_cmd_joint_pos     = {};  // Command joint positions (gradually converging)
static std::array<float, 31> g_cmd_motor_pos     = {};  // Command motor positions (gradually converging)
static std::array<float, 31> g_current_joint_pos = {};  // Current actual joint positions
static std::array<float, 31> g_current_motor_pos = {};  // Current actual motor positions
static std::mutex g_target_mutex;
static float g_convergence_time   = 1.5f;             // Convergence time in seconds (0=instant, 1.5=default, 3=slow)
static const float RESET_DURATION = 5.0f;             // Fixed duration for Reset/Home buttons
static std::atomic<bool> g_reset_mode_active(false);  // Use fixed duration for Reset/Home
static std::atomic<uint32_t> g_lowcmd_publish_count(0);
static LowCmd g_last_published_cmd;
static std::mutex g_last_cmd_mutex;

// Response log
static std::deque<std::string> g_response_log;
static std::mutex g_log_mutex;
static const size_t MAX_LOG_LINES = 50;

// PD gains default values
static const std::array<float, 31> DEFAULT_KP = {
    50.0f,  25.0f,  25.0f,                               // Waist
    500.0f, 200.0f, 50.0f, 500.0f, 50.0f, 50.0f,         // Left leg
    500.0f, 200.0f, 50.0f, 500.0f, 50.0f, 50.0f,         // Right leg
    50.0f,  50.0f,  30.0f, 30.0f,  15.0f, 15.0f, 15.0f,  // Left arm  (wrist Yaw/Front/Back: 15)
    50.0f,  50.0f,  30.0f, 30.0f,  15.0f, 15.0f, 15.0f,  // Right arm (wrist Yaw/Front/Back: 15)
    2.0f,   5.0f                                         // Neck
};
static const std::array<float, 31> DEFAULT_KD = {
    0.8f,  0.8f, 0.8f,                            // Waist
    3.0f,  0.5f, 0.5f,  3.0f,  3.0f, 3.0f,        // Left leg
    3.0f,  0.5f, 0.5f,  3.0f,  3.0f, 3.0f,        // Right leg
    0.5f,  0.5f, 0.15f, 0.15f, 0.1f, 0.1f, 0.1f,  // Left arm
    0.5f,  0.5f, 0.15f, 0.15f, 0.1f, 0.1f, 0.1f,  // Right arm
    0.05f, 0.1f                                   // Neck
};

// PD gains (editable, initialized from defaults)
static std::array<float, 31> g_kp = DEFAULT_KP;
static std::array<float, 31> g_kd = DEFAULT_KD;
// Temporary edit buffers for kp/kd (applied on enter or focus loss)
static std::array<float, 31> g_kp_edit = DEFAULT_KP;
static std::array<float, 31> g_kd_edit = DEFAULT_KD;
// Temporary edit buffers for position (applied on enter or focus loss)
static std::array<float, 31> g_pos_edit = {};

// Kp/Kd confirmation popup state
static bool g_show_gain_confirm_popup = false;
static int g_pending_gain_joint       = -1;    // Joint index for pending change
static bool g_pending_is_kp           = true;  // true = kp, false = kd
static float g_pending_old_value      = 0.0f;
static float g_pending_new_value      = 0.0f;

// Service-command confirmation popup state.
// Power/Torque and Control-Mode buttons stage their action in g_pending_service_action;
// the modal popup runs the action only after explicit user confirmation.
static bool g_show_service_confirm_popup = false;
static std::string g_pending_service_label;
static std::function<void()> g_pending_service_action;

void RequestServiceConfirm(const char *label, std::function<void()> action) {
    g_pending_service_label      = label;
    g_pending_service_action     = std::move(action);
    g_show_service_confirm_popup = true;
}

static constexpr float BASE_WINDOW_WIDTH  = 1800.0f;
static constexpr float BASE_WINDOW_HEIGHT = 900.0f;

float ClampUiScale(float scale) { return std::clamp(scale, 1.15f, 2.5f); }

float ComputeUiScale(float width, float height) { return ClampUiScale(std::min(width / BASE_WINDOW_WIDTH, height / BASE_WINDOW_HEIGHT)); }

bool IsWaistParallelJoint(int idx) { return idx == 1 || idx == 2; }

bool IsAnkleParallelJoint(int idx) { return idx == 7 || idx == 8 || idx == 13 || idx == 14; }

bool IsWristParallelJoint(int idx) { return idx == 20 || idx == 21 || idx == 27 || idx == 28; }

int GetParallelLinkIndexForJoint(int idx) {
    if (IsWaistParallelJoint(idx)) {
        return 0;
    }
    if (idx == 7 || idx == 8) {
        return 1;
    }
    if (idx == 13 || idx == 14) {
        return 2;
    }
    if (idx == 20 || idx == 21) {
        return 3;
    }
    if (idx == 27 || idx == 28) {
        return 4;
    }
    return -1;
}

bool UseMotorSpaceForJoint(int idx) {
    const int link_idx = GetParallelLinkIndexForJoint(idx);
    if (link_idx >= 0) {
        return g_parallel_space[static_cast<size_t>(link_idx)] == static_cast<int>(SpaceSelection::MOTOR);
    }
    return false;
}

// Render the per-row MS / PJS radio buttons. For joints that don't belong to
// any parallel link the radios are drawn but disabled (they show a dummy
// selection that doesn't match either choice, so neither is highlighted).
void RenderModeRadiosForJoint(int joint_idx) {
    const int link_idx = GetParallelLinkIndexForJoint(joint_idx);
    const bool active  = (link_idx >= 0);

    static int kDisabledNoSelection = -1;  // does not match MOTOR (0) nor JOINT (1)
    int *space                      = active ? &g_parallel_space[static_cast<size_t>(link_idx)] : &kDisabledNoSelection;

    if (!active) {
        ImGui::BeginDisabled();
    }

    char ms_id[32];
    char pjs_id[32];
    std::snprintf(ms_id, sizeof(ms_id), "MS##ms_%d", joint_idx);
    std::snprintf(pjs_id, sizeof(pjs_id), "PJS##pjs_%d", joint_idx);

    ImGui::RadioButton(ms_id, space, static_cast<int>(SpaceSelection::MOTOR));
    ImGui::SameLine();
    ImGui::RadioButton(pjs_id, space, static_cast<int>(SpaceSelection::JOINT));

    if (!active) {
        ImGui::EndDisabled();
    }
}

void SyncParallelLinkTargetsToCurrentState(int link_idx) {
    static constexpr std::array<std::array<int, 2>, kParallelLinkCount> kJointPairs = {{
        {1, 2},
        {7, 8},
        {13, 14},
        {20, 21},
        {27, 28},
    }};

    if (link_idx < 0 || link_idx >= kParallelLinkCount) {
        return;
    }

    const bool use_motor_space = g_parallel_space[static_cast<size_t>(link_idx)] == static_cast<int>(SpaceSelection::MOTOR);
    for (int joint_idx : kJointPairs[static_cast<size_t>(link_idx)]) {
        // Keep both spaces fresh and make selected-space command immediate.
        g_target_motor_pos[joint_idx] = g_current_motor_pos[joint_idx];
        g_cmd_motor_pos[joint_idx]    = g_current_motor_pos[joint_idx];
        g_target_joint_pos[joint_idx] = g_current_joint_pos[joint_idx];
        g_cmd_joint_pos[joint_idx]    = g_current_joint_pos[joint_idx];

        if (use_motor_space) {
            g_cmd_motor_pos[joint_idx] = g_target_motor_pos[joint_idx];
        } else {
            g_cmd_joint_pos[joint_idx] = g_target_joint_pos[joint_idx];
        }
    }
}

void SignalHandler(int) { g_running = false; }

// Render a section header in a slightly larger font for visual hierarchy.
void SectionHeader(const char *text) {
    constexpr float kHeaderFontScale = 1.35f;
    ImGui::SetWindowFontScale(kHeaderFontScale);
    ImGui::TextUnformatted(text);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::Separator();
}

// Sub-group label: smaller than SectionHeader but still bigger than body, used
// to label related groups of buttons within a section.
void SubsectionHeader(const char *text) {
    constexpr float kSubFontScale = 1.15f;
    ImGui::Spacing();
    ImGui::SetWindowFontScale(kSubFontScale);
    ImGui::TextColored(ImVec4(0.6f, 0.85f, 1.0f, 1.0f), "%s", text);
    ImGui::SetWindowFontScale(1.0f);
}

// Action button with a larger, visually heavier label. Used for Reset/Home and
// Service API command buttons so they share one look-and-feel policy.
bool ScaledButton(const char *label, const ImVec2 &size) {
    constexpr float kButtonFontScale = 1.2f;
    ImGui::SetWindowFontScale(kButtonFontScale);
    const bool clicked = ImGui::Button(label, size);
    ImGui::SetWindowFontScale(1.0f);
    return clicked;
}

// Add log entry
void AddLog(const std::string &msg) {
    std::lock_guard<std::mutex> lock(g_log_mutex);
    auto now    = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm     = *std::localtime(&time_t);

    char time_str[32];
    std::strftime(time_str, sizeof(time_str), "%H:%M:%S", &tm);

    g_response_log.push_back(std::string("[") + time_str + "] " + msg);
    if (g_response_log.size() > MAX_LOG_LINES) {
        g_response_log.pop_front();
    }
}

// RobotState callback
void RobotStateCallback(const RobotState &state) {
    std::lock_guard<std::mutex> lock(g_robotstate_mutex);
    g_latest_robotstate = state;
    g_robotstate_received_count++;
    g_last_robotstate_time = std::chrono::steady_clock::now();
    g_robotstate_connected = true;
}

// Helper to convert RobotControlState to string. Mirrors the labels used by
// robros::lib::common::RobotControlState::toString() in igris_c_lib.
const char *RobotControlStateToString(RobotControlState state) {
    switch (state) {
    case RobotControlState::ROBOT_STATE_NOT_READY:
        return "NOT_READY";
    case RobotControlState::ROBOT_STATE_IDLE:
        return "IDLE";
    case RobotControlState::ROBOT_STATE_STOP:
        return "STOP";
    case RobotControlState::ROBOT_STATE_LOW:
        return "LOW";
    case RobotControlState::ROBOT_STATE_HIGH_MOTION_ACTIVE:
        return "HIGH_MOTION_ACTIVE";
    case RobotControlState::ROBOT_STATE_HIGH_MOTION_STANDBY:
        return "HIGH_MOTION_STANDBY";
    case RobotControlState::ROBOT_STATE_WALK_LOW:
        return "WALK_LOW";
    case RobotControlState::ROBOT_STATE_WALK_HIGH_MOTION_ACTIVE:
        return "WALK_HIGH_MOTION_ACTIVE";
    case RobotControlState::ROBOT_STATE_WALK_HIGH_MOTION_STANDBY:
        return "WALK_HIGH_MOTION_STANDBY";
    case RobotControlState::ROBOT_STATE_WHOLE_BODY_CONTROL:
        return "WHOLE_BODY_CONTROL";
    default:
        return "UNKNOWN";
    }
}

// LowState callback
void LowStateCallback(const LowState &state) {
    std::lock_guard<std::mutex> lock(g_lowstate_mutex);
    g_latest_lowstate = state;
    g_lowstate_received_count++;

    // Update current joint and motor state for control
    {
        std::lock_guard<std::mutex> lock_target(g_target_mutex);
        for (int i = 0; i < 31; i++) {
            g_current_joint_pos[i] = state.joint_state()[i].q();
            g_current_motor_pos[i] = state.motor_state()[i].q();
        }
    }

    // Set initial reference on first receive
    if (!g_first_state_received) {
        for (int i = 0; i < 31; i++) {
            g_initial_motor_pos[i] = state.motor_state()[i].q();
            g_initial_joint_pos[i] = state.joint_state()[i].q();
            g_slider_values[i]     = 0.0f;  // Start at 0 (relative to initial)
        }
        g_first_state_received = true;
        AddLog("First state received - reference values initialized");
    }
}

// Async service call state
static std::atomic<bool> g_service_call_in_progress(false);

// Service call helpers (async versions to prevent GUI freeze)
void CallInitBmsAsync(IgrisC_Client *client, BmsInitType type, const char *type_name) {
    if (g_service_call_in_progress.load()) {
        AddLog("Service call already in progress, please wait...");
        return;
    }
    g_service_call_in_progress = true;
    AddLog(std::string("Calling InitBms(") + type_name + ")...");

    std::string type_name_copy(type_name);
    std::thread([client, type, type_name_copy]() {
        auto res = client->InitBms(type, 60000);
        std::string result =
            std::string("InitBms(") + type_name_copy + "): " + (res.success() ? "SUCCESS" : "FAILED") + " - " + res.message();
        AddLog(result);
        g_service_call_in_progress = false;
    }).detach();
}

void CallSetTorqueAsync(IgrisC_Client *client, TorqueType type, const char *type_name) {
    if (g_service_call_in_progress.load()) {
        AddLog("Service call already in progress, please wait...");
        return;
    }
    g_service_call_in_progress = true;
    AddLog(std::string("Calling SetTorque(") + type_name + ")...");

    std::string type_name_copy(type_name);
    std::thread([client, type, type_name_copy]() {
        auto res = client->SetTorque(type, 60000);
        std::string result =
            std::string("SetTorque(") + type_name_copy + "): " + (res.success() ? "SUCCESS" : "FAILED") + " - " + res.message();
        AddLog(result);
        g_service_call_in_progress = false;
    }).detach();
}

void CallSendControlModeCommandAsync(IgrisC_Client *client, ControlModeCommandType cmd_type, const char *label,
                                     const std::string &preset_id = "", bool is_cyclic = false) {
    if (g_service_call_in_progress.load()) {
        AddLog("Service call already in progress, please wait...");
        return;
    }
    g_service_call_in_progress = true;
    AddLog(std::string("Calling ControlModeCommand(") + label + ")...");

    std::string label_copy(label);
    std::thread([client, cmd_type, label_copy, preset_id, is_cyclic]() {
        auto res = client->SendControlModeCommand(cmd_type, preset_id, is_cyclic, 60000);
        AddLog(std::string("ControlModeCommand(") + label_copy + "): " + (res.success() ? "SUCCESS" : "FAILED") + " - " + res.message());
        g_service_call_in_progress = false;
    }).detach();
}

void CallSendMujocoSimCmdAsync(IgrisC_Client *client, MujocoSimCommandType cmd_type, const char *label) {
    if (g_service_call_in_progress.load()) {
        AddLog("Service call already in progress, please wait...");
        return;
    }
    g_service_call_in_progress = true;
    AddLog(std::string("Calling MujocoSim(") + label + ")...");

    std::string label_copy(label);
    std::thread([client, cmd_type, label_copy]() {
        auto res = client->SendMujocoSimCmd(cmd_type, 5000);
        AddLog(std::string("MujocoSim(") + label_copy + "): " + (res.success() ? "SUCCESS" : "FAILED") + " - " + res.message());
        g_service_call_in_progress = false;
    }).detach();
}

// Start LowCmd publishing
void StartLowCmdPublishing() {
    if (g_lowlevel_active) {
        AddLog("LowCmd publishing already active");
        return;
    }
    // Initialize target and command positions to current positions
    {
        std::lock_guard<std::mutex> lock_state(g_lowstate_mutex);
        std::lock_guard<std::mutex> lock_target(g_target_mutex);
        for (int i = 0; i < 31; i++) {
            g_target_joint_pos[i] = g_latest_lowstate.joint_state()[i].q();
            g_target_motor_pos[i] = g_latest_lowstate.motor_state()[i].q();
            g_cmd_joint_pos[i]    = g_latest_lowstate.joint_state()[i].q();
            g_cmd_motor_pos[i]    = g_latest_lowstate.motor_state()[i].q();
        }
    }
    g_lowlevel_active = true;
    AddLog("LowCmd publishing started - initialized to current positions");
}

// Stop LowCmd publishing
void StopLowCmdPublishing() {
    if (!g_lowlevel_active) {
        AddLog("LowCmd publishing already stopped");
        return;
    }
    g_lowlevel_active = false;
    AddLog("LowCmd publishing stopped");
}

// 300Hz LowCmd publishing thread
void LowCmdPublishThread(Publisher<LowCmd> *publisher) {
    const auto period = std::chrono::microseconds(3333);  // ~300Hz
    auto next_time    = std::chrono::steady_clock::now();

    while (g_running) {
        if (g_lowlevel_active) {
            LowCmd cmd;

            {
                std::lock_guard<std::mutex> lock_target(g_target_mutex);

                // Set kinematic modes by 5 parallel links
                for (int mode_idx = 0; mode_idx < kParallelLinkCount; ++mode_idx) {
                    cmd.kinematic_modes()[mode_idx] =
                        (g_parallel_space[static_cast<size_t>(mode_idx)] == static_cast<int>(SpaceSelection::JOINT)) ? KinematicMode::PJS
                                                                                                                     : KinematicMode::MS;
                }

                // Calculate alpha for exponential smoothing based on convergence time
                // At 300Hz, to reach 95% in T seconds: alpha = 1 - 0.05^(1/(T*300))
                // T=0 -> instant (alpha=1), T=1.5 -> ~0.01, T=3 -> ~0.005
                // Use RESET_DURATION (5s) for Reset/Home, otherwise use slider value
                float duration = g_reset_mode_active.load() ? RESET_DURATION : g_convergence_time;
                float alpha;
                const float min_delta = 0.0001f;  // Minimum delta for target reached
                if (duration <= 0.01f) {
                    alpha = 1.0f;  // Instant
                } else {
                    alpha = 1.0f - std::pow(0.05f, 1.0f / (duration * 300.0f));
                }

                bool all_reached = true;
                for (int i = 0; i < 31; i++) {
                    auto &motor_cmd = cmd.motors()[i];

                    // Set motor ID
                    motor_cmd.id(i);

                    const bool use_motor_space = UseMotorSpaceForJoint(i);
                    float &q_cmd_ref           = use_motor_space ? g_cmd_motor_pos[i] : g_cmd_joint_pos[i];
                    float q_target             = use_motor_space ? g_target_motor_pos[i] : g_target_joint_pos[i];

                    // Apply exponential smoothing (비율 기반 보간)
                    float delta = q_target - q_cmd_ref;
                    if (std::abs(delta) > min_delta) {
                        q_cmd_ref += delta * alpha;
                        all_reached = false;
                    } else {
                        q_cmd_ref = q_target;  // Target reached
                    }

                    // Set commands
                    motor_cmd.q(q_cmd_ref);
                    motor_cmd.dq(0.0f);
                    motor_cmd.tau(0.0f);
                    motor_cmd.kp(g_kp[i]);
                    motor_cmd.kd(g_kd[i]);
                }

                // Deactivate reset mode when all joints reached target
                if (all_reached && g_reset_mode_active.load()) {
                    g_reset_mode_active = false;
                }
            }

            // Publish
            publisher->write(cmd);

            // Update publish count and save last command
            g_lowcmd_publish_count++;
            {
                std::lock_guard<std::mutex> lock(g_last_cmd_mutex);
                g_last_published_cmd = cmd;
            }
        }

        // Sleep until next cycle
        next_time += period;
        std::this_thread::sleep_until(next_time);
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║         IGRIS-SDK Dev Tool            ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝\n" << std::endl;

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <domain_id> <namespace>" << std::endl;
        std::cerr << "         " << argv[0] << " 0 \"\" (no namespace)" << std::endl;
        return 1;
    }
    int domain_id  = std::atoi(argv[1]);
    std::string ns = argv[2];

    std::cout << "Domain ID: " << domain_id << std::endl;
    std::cout << "Namespace: \"" << ns << "\"" << std::endl;
    std::cout << "Make sure the robot controller is running!\n" << std::endl;

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    // GL 3.3 + GLSL 330
    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a large default window relative to the monitor so the UI stays usable
    // on high-resolution displays instead of opening at a fixed pixel size.
    int initial_width  = static_cast<int>(BASE_WINDOW_WIDTH);
    int initial_height = static_cast<int>(BASE_WINDOW_HEIGHT);
    if (GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor()) {
        int work_x = 0, work_y = 0, work_width = 0, work_height = 0;
        glfwGetMonitorWorkarea(primary_monitor, &work_x, &work_y, &work_width, &work_height);
        if (work_width > 0 && work_height > 0) {
            initial_width  = std::max(static_cast<int>(BASE_WINDOW_WIDTH), static_cast<int>(work_width * 0.9f));
            initial_height = std::max(static_cast<int>(BASE_WINDOW_HEIGHT), static_cast<int>(work_height * 0.9f));
        }
    }

    GLFWwindow *window = glfwCreateWindow(initial_width, initial_height, "IGRIS-SDK Dev Tool", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    // Enforce a minimum window size proportional to the monitor's work area so
    // the 3-panel + log-strip layout never collapses. On 4K monitors the floor
    // ends up much larger than on 1080p, keeping fonts/buttons readable.
    {
        int min_width  = 1100;
        int min_height = 650;
        if (GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor()) {
            int work_x = 0, work_y = 0, work_width = 0, work_height = 0;
            glfwGetMonitorWorkarea(primary_monitor, &work_x, &work_y, &work_width, &work_height);
            if (work_width > 0 && work_height > 0) {
                // ~60% of the monitor's usable area, clamped to a sane floor.
                min_width  = std::max(min_width, static_cast<int>(work_width * 0.6f));
                min_height = std::max(min_height, static_cast<int>(work_height * 0.6f));
            }
        }
        glfwSetWindowSizeLimits(window, min_width, min_height, GLFW_DONT_CARE, GLFW_DONT_CARE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle base_style  = ImGui::GetStyle();
    float applied_ui_scale = 0.0f;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Initialize ChannelFactory
    std::cout << "Initializing ChannelFactory..." << std::endl;
    ChannelFactory::Instance()->Init(domain_id, ns);

    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    // Initialize IgrisC_Client
    std::cout << "Initializing IgrisC_Client..." << std::endl;
    IgrisC_Client client;
    client.Init();
    client.SetTimeout(5.0f);

    // Create LowState subscriber
    Subscriber<LowState> lowstate_sub("rt/lowstate");
    if (!lowstate_sub.init(LowStateCallback)) {
        std::cerr << "Failed to initialize LowState subscriber" << std::endl;
        return 1;
    }

    // Create LowCmd publisher
    std::cout << "Initializing LowCmd publisher..." << std::endl;
    Publisher<LowCmd> lowcmd_pub("rt/lowcmd");
    if (!lowcmd_pub.init()) {
        std::cerr << "Failed to initialize LowCmd publisher" << std::endl;
        return 1;
    }

    // Auto-subscribe to RobotState
    std::cout << "Subscribing to RobotState..." << std::endl;
    g_robotstate_sub = std::make_unique<Subscriber<RobotState>>("rt/robotstate");
    if (g_robotstate_sub->init(RobotStateCallback)) {
        AddLog("RobotState subscription started");
    } else {
        AddLog("Failed to subscribe to RobotState");
        g_robotstate_sub.reset();
    }

    // Start LowCmd publishing thread
    std::thread publish_thread(LowCmdPublishThread, &lowcmd_pub);

    std::cout << "GUI Client ready!\n" << std::endl;
    AddLog("GUI Client initialized successfully");

    // Main loop
    while (!glfwWindowShouldClose(window) && g_running) {
        // Poll events
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const float ui_scale = ComputeUiScale(io.DisplaySize.x, io.DisplaySize.y);
        if (std::abs(ui_scale - applied_ui_scale) > 0.001f) {
            ImGui::GetStyle() = base_style;
            ImGui::GetStyle().ScaleAllSizes(ui_scale);
            io.FontGlobalScale = ui_scale;
            applied_ui_scale   = ui_scale;
        }

        // Right and Center are fixed-width (sized for content); the Left panel
        // absorbs the remaining horizontal space so on wide monitors there is
        // no empty area to the right of the center sections.
        const float right_panel_width  = io.DisplaySize.x * (320.0f / BASE_WINDOW_WIDTH);
        const float center_panel_width = 720.0f * ui_scale;
        const float left_panel_width   = -(center_panel_width + right_panel_width + 2.0f * ImGui::GetStyle().ItemSpacing.x);
        // Unified height for action buttons (Reset/Home group + Service API group).
        const float action_btn_height = 38.0f * ui_scale;
        // Column layout: [Name] [Mode (MS/PJS)] [Position slider] [Kp] [Kd]
        const float name_column_width    = 180.0f * ui_scale;
        const float mode_column_width    = 90.0f * ui_scale;
        const float mode_column_offset   = name_column_width + 10.0f * ui_scale;
        const float slider_column_offset = mode_column_offset + mode_column_width + 10.0f * ui_scale;
        const float kp_width             = 60.0f * ui_scale;
        const float kd_width             = 50.0f * ui_scale;
        const float popup_button_width   = 180.0f * ui_scale;
        const float log_panel_height     = 160.0f * ui_scale;
        const float top_panel_height     = -(log_panel_height + ImGui::GetStyle().ItemSpacing.y);

        // Create main window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("IGRIS-SDK Dev Tool", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBringToFrontOnFocus);

        // Left panel: Sliders
        ImGui::BeginChild("LeftPanel", ImVec2(left_panel_width, top_panel_height), true);
        {
            SectionHeader("LowCmd Sliders");

            // Capture parallel-space before per-row radios are rendered; we
            // detect changes after the joint-row loop and sync each link once.
            static constexpr std::array<const char *, kParallelLinkCount> kParallelNames = {"Waist", "Left Ankle", "Right Ankle",
                                                                                            "Left Wrist", "Right Wrist"};
            std::array<int, kParallelLinkCount> prev_parallel_space                      = g_parallel_space;

            // Reset, Reset Gains, and Home buttons (3 buttons side by side)
            float button_width = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2) / 3.0f;
            if (ScaledButton("Reset Pos", ImVec2(button_width, action_btn_height))) {
                if (g_lowlevel_active) {
                    std::lock_guard<std::mutex> lock_target(g_target_mutex);
                    for (int i = 0; i < 31; i++) {
                        g_target_motor_pos[i] = 0.0f;
                        g_target_joint_pos[i] = 0.0f;
                        // Don't reset cmd - let it converge gradually
                        g_slider_values[i] = 0.0f;
                    }
                    g_reset_mode_active = true;  // Use 5s duration
                    AddLog("All target positions reset to zero (5s transition)");
                } else {
                    AddLog("Enable LOW_LEVEL mode first to use Reset");
                }
            }
            ImGui::SameLine();
            if (ScaledButton("Reset Gains", ImVec2(button_width, action_btn_height))) {
                for (int i = 0; i < 31; i++) {
                    g_kp[i]      = DEFAULT_KP[i];
                    g_kd[i]      = DEFAULT_KD[i];
                    g_kp_edit[i] = DEFAULT_KP[i];
                    g_kd_edit[i] = DEFAULT_KD[i];
                }
                AddLog("All Kp/Kd gains reset to defaults");
            }
            ImGui::SameLine();
            if (ScaledButton("Home", ImVec2(button_width, action_btn_height))) {
                if (g_lowlevel_active) {
                    std::lock_guard<std::mutex> lock_target(g_target_mutex);
                    // Set all targets to zero first (cmd will converge gradually)
                    for (int i = 0; i < 31; i++) {
                        g_target_motor_pos[i] = 0.0f;
                        g_target_joint_pos[i] = 0.0f;
                        // Don't reset cmd - let it converge gradually
                        g_slider_values[i] = 0.0f;
                    }
                    // Set Shoulder Roll L (16) to 0.4, Shoulder Roll R (23) to -0.4
                    const int SHOULDER_ROLL_L           = 16;
                    const int SHOULDER_ROLL_R           = 23;
                    g_target_joint_pos[SHOULDER_ROLL_L] = 0.4f;
                    g_target_joint_pos[SHOULDER_ROLL_R] = -0.4f;
                    g_target_motor_pos[SHOULDER_ROLL_L] = 0.4f;   // Also set motor pos
                    g_target_motor_pos[SHOULDER_ROLL_R] = -0.4f;  // Also set motor pos
                    g_slider_values[SHOULDER_ROLL_L]    = 0.4f;
                    g_slider_values[SHOULDER_ROLL_R]    = -0.4f;
                    g_reset_mode_active                 = true;  // Use 5s duration
                    AddLog("Home position (5s transition)");
                } else {
                    AddLog("Enable LOW_LEVEL mode first to use Home");
                }
            }

            // LowCmd publish toggle, sized to span the same row width as the
            // 3 buttons above so they align visually.
            if (!g_lowlevel_active) {
                if (ScaledButton("LowCmd Publish Start", ImVec2(-1, action_btn_height))) {
                    StartLowCmdPublishing();
                }
            } else {
                if (ScaledButton("LowCmd Publish Stop", ImVec2(-1, action_btn_height))) {
                    StopLowCmdPublishing();
                }
            }
            ImGui::Separator();

            // Status and Convergence Time slider on same line.
            // Pre-compute the trailing "Duration: X.Xs" width using the current
            // font scale so the slider fills the remaining space exactly and
            // never overflows the panel right edge.
            const std::string duration_text   = "Duration: 9.9s";  // worst-case width
            const float duration_text_width   = ImGui::CalcTextSize(duration_text.c_str()).x;
            const float duration_item_spacing = ImGui::GetStyle().ItemSpacing.x;
            ImGui::Text("LowState: %u", g_lowstate_received_count.load());
            ImGui::SameLine();
            const float slider_width = std::max(80.0f, ImGui::GetContentRegionAvail().x - duration_text_width - duration_item_spacing);
            ImGui::SetNextItemWidth(slider_width);
            ImGui::SliderFloat("##convergence", &g_convergence_time, 0.0f, 3.0f, "");
            ImGui::SameLine();
            ImGui::Text("Duration: %.1fs", g_convergence_time);
            if (!g_first_state_received) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Waiting for first state...");
            } else {
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "Reference initialized");
            }
            ImGui::Separator();

            // Sliders (31 motors/joints)
            ImGui::BeginChild("SliderScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            // Show LOW_LEVEL mode status
            if (g_lowlevel_active) {
                ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "LOW_LEVEL Mode - Arrow:0.001 Ctrl:0.01 Shift:0.1 | Ctrl+Click:Custom");
            }

            // Column headers (aligned with slider layout)
            float content_width = ImGui::GetContentRegionAvail().x;
            float spacing       = ImGui::GetStyle().ItemSpacing.x;

            ImGui::Text("Name");
            ImGui::SameLine(mode_column_offset);
            ImGui::Text("Space");
            ImGui::SameLine(slider_column_offset);
            ImGui::Text("Position");
            ImGui::SameLine(content_width - kd_width - spacing - kp_width);
            ImGui::Text("  Kp");
            ImGui::SameLine(content_width - kd_width);
            ImGui::Text("  Kd");
            ImGui::Separator();

            const float FINE_STEP     = 0.001f;  // Fine adjustment step for arrow keys
            static int selected_joint = -1;      // Currently selected joint for fine control
            // Scroll to the selected joint only on the frame the selection
            // changes. Continuous SetScrollHereY would fight manual scroll.
            static bool scroll_to_selected = false;

            // Handle Up/Down arrow keys for joint selection (outside loop to prevent multiple triggers)
            if (g_lowlevel_active) {
                if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
                    if (selected_joint < 0) {
                        selected_joint = 0;
                    } else {
                        selected_joint = (selected_joint > 0) ? selected_joint - 1 : 30;
                    }
                    scroll_to_selected = true;
                }
                if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
                    if (selected_joint < 0) {
                        selected_joint = 0;
                    } else {
                        selected_joint = (selected_joint < 30) ? selected_joint + 1 : 0;
                    }
                    scroll_to_selected = true;
                }
            }

            for (int i = 0; i < 31; i++) {
                // Separator between body sections (waist | L leg | R leg | L arm | R arm | neck)
                if (i == 3 || i == 9 || i == 15 || i == 22 || i == 29) {
                    ImGui::Separator();
                }

                char label[64];
                char slider_label[64];
                const bool use_motor_space_for_joint = UseMotorSpaceForJoint(i);
                const char *name                     = use_motor_space_for_joint ? MOTOR_NAMES[i] : JOINT_NAMES[i];
                snprintf(label, sizeof(label), "%2d. %s", i, name);
                snprintf(slider_label, sizeof(slider_label), "##slider%d", i);

                if (g_lowlevel_active) {
                    // LOW_LEVEL mode: Show editable target positions
                    std::lock_guard<std::mutex> lock_target(g_target_mutex);
                    float *target_val = use_motor_space_for_joint ? &g_target_motor_pos[i] : &g_target_joint_pos[i];
                    float pos_min     = use_motor_space_for_joint ? MOTOR_POS_MIN[i] : JOINT_POS_MIN[i];
                    float pos_max     = use_motor_space_for_joint ? MOTOR_POS_MAX[i] : JOINT_POS_MAX[i];

                    // Selectable name - click to select for fine control
                    bool is_selected = (selected_joint == i);
                    if (is_selected) {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));  // Green when selected
                    }
                    if (ImGui::Selectable(label, is_selected, 0, ImVec2(name_column_width, 0))) {
                        if (selected_joint == i) {
                            selected_joint = -1;  // Deselect
                        } else {
                            selected_joint     = i;
                            scroll_to_selected = true;  // Scroll once on new selection
                        }
                    }
                    if (is_selected) {
                        ImGui::PopStyleColor();
                        if (scroll_to_selected) {
                            ImGui::SetScrollHereY(0.5f);
                            scroll_to_selected = false;
                        }
                    }

                    ImGui::SameLine(mode_column_offset);
                    RenderModeRadiosForJoint(i);
                    ImGui::SameLine(slider_column_offset);

                    // Slider (fill remaining space minus kp/kd fields) - immediate apply while dragging
                    float kp_kd_width = kp_width + kd_width + ImGui::GetStyle().ItemSpacing.x * 2;
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - kp_kd_width);
                    if (ImGui::SliderFloat(slider_label, target_val, pos_min, pos_max, "%.3f", ImGuiSliderFlags_AlwaysClamp)) {
                        // Slider value changed - use normal duration instead of reset duration
                        g_reset_mode_active = false;
                    }

                    // kp input (with confirmation popup)
                    ImGui::SameLine();
                    char kp_label[32];
                    snprintf(kp_label, sizeof(kp_label), "##kp%d", i);
                    ImGui::SetNextItemWidth(kp_width);
                    const bool kp_enter_commit =
                        ImGui::InputFloat(kp_label, &g_kp_edit[i], 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue);
                    const bool kp_focus_out_commit = ImGui::IsItemDeactivatedAfterEdit();
                    if ((kp_enter_commit || kp_focus_out_commit) && g_kp_edit[i] != g_kp[i] && !ImGui::IsPopupOpen("Confirm Gain Change")) {
                        g_pending_gain_joint      = i;
                        g_pending_is_kp           = true;
                        g_pending_old_value       = g_kp[i];
                        g_pending_new_value       = g_kp_edit[i];
                        g_show_gain_confirm_popup = true;
                    } else if (!ImGui::IsItemActive()) {
                        g_kp_edit[i] = g_kp[i];
                    }

                    // kd input (with confirmation popup)
                    ImGui::SameLine();
                    char kd_label[32];
                    snprintf(kd_label, sizeof(kd_label), "##kd%d", i);
                    ImGui::SetNextItemWidth(kd_width);
                    const bool kd_enter_commit =
                        ImGui::InputFloat(kd_label, &g_kd_edit[i], 0, 0, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue);
                    const bool kd_focus_out_commit = ImGui::IsItemDeactivatedAfterEdit();
                    if ((kd_enter_commit || kd_focus_out_commit) && g_kd_edit[i] != g_kd[i] && !ImGui::IsPopupOpen("Confirm Gain Change")) {
                        g_pending_gain_joint      = i;
                        g_pending_is_kp           = false;
                        g_pending_old_value       = g_kd[i];
                        g_pending_new_value       = g_kd_edit[i];
                        g_show_gain_confirm_popup = true;
                    } else if (!ImGui::IsItemActive()) {
                        g_kd_edit[i] = g_kd[i];
                    }

                    // Arrow key fine adjustment when this joint is selected
                    // Shift+Arrow: 0.1, Ctrl+Arrow: 0.01, Arrow: 0.001
                    if (is_selected) {
                        float step = FINE_STEP;  // 0.001 default
                        if (ImGui::GetIO().KeyShift) {
                            step = 0.1f;
                        } else if (ImGui::GetIO().KeyCtrl) {
                            step = 0.01f;
                        }

                        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
                            *target_val         = std::max(pos_min, *target_val - step);
                            g_reset_mode_active = false;  // Use normal duration
                        }
                        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
                            *target_val         = std::min(pos_max, *target_val + step);
                            g_reset_mode_active = false;  // Use normal duration
                        }
                    }
                } else {
                    // Display mode: Show current state (read-only)
                    if (g_first_state_received) {
                        std::lock_guard<std::mutex> lock(g_lowstate_mutex);
                        g_slider_values[i] =
                            use_motor_space_for_joint ? g_latest_lowstate.motor_state()[i].q() : g_latest_lowstate.joint_state()[i].q();
                    }
                    // Use limits based on per-joint space selection
                    float pos_min = use_motor_space_for_joint ? MOTOR_POS_MIN[i] : JOINT_POS_MIN[i];
                    float pos_max = use_motor_space_for_joint ? MOTOR_POS_MAX[i] : JOINT_POS_MAX[i];

                    // Match layout with LOW_LEVEL mode
                    ImGui::SetNextItemWidth(name_column_width);
                    ImGui::Text("%s", label);

                    ImGui::SameLine(mode_column_offset);
                    RenderModeRadiosForJoint(i);
                    ImGui::SameLine(slider_column_offset);

                    // Slider (fill remaining space minus kp/kd fields)
                    float kp_kd_width = kp_width + kd_width + ImGui::GetStyle().ItemSpacing.x * 2;
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - kp_kd_width);
                    ImGui::SliderFloat(slider_label, &g_slider_values[i], pos_min, pos_max, "%.3f");

                    // kp input (with confirmation popup)
                    ImGui::SameLine();
                    char kp_label[32];
                    snprintf(kp_label, sizeof(kp_label), "##kp%d", i);
                    ImGui::SetNextItemWidth(kp_width);
                    const bool kp_enter_commit =
                        ImGui::InputFloat(kp_label, &g_kp_edit[i], 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue);
                    const bool kp_focus_out_commit = ImGui::IsItemDeactivatedAfterEdit();
                    if ((kp_enter_commit || kp_focus_out_commit) && g_kp_edit[i] != g_kp[i] && !ImGui::IsPopupOpen("Confirm Gain Change")) {
                        g_pending_gain_joint      = i;
                        g_pending_is_kp           = true;
                        g_pending_old_value       = g_kp[i];
                        g_pending_new_value       = g_kp_edit[i];
                        g_show_gain_confirm_popup = true;
                    } else if (!ImGui::IsItemActive()) {
                        g_kp_edit[i] = g_kp[i];
                    }

                    // kd input (with confirmation popup)
                    ImGui::SameLine();
                    char kd_label[32];
                    snprintf(kd_label, sizeof(kd_label), "##kd%d", i);
                    ImGui::SetNextItemWidth(kd_width);
                    const bool kd_enter_commit =
                        ImGui::InputFloat(kd_label, &g_kd_edit[i], 0, 0, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue);
                    const bool kd_focus_out_commit = ImGui::IsItemDeactivatedAfterEdit();
                    if ((kd_enter_commit || kd_focus_out_commit) && g_kd_edit[i] != g_kd[i] && !ImGui::IsPopupOpen("Confirm Gain Change")) {
                        g_pending_gain_joint      = i;
                        g_pending_is_kp           = false;
                        g_pending_old_value       = g_kd[i];
                        g_pending_new_value       = g_kd_edit[i];
                        g_show_gain_confirm_popup = true;
                    } else if (!ImGui::IsItemActive()) {
                        g_kd_edit[i] = g_kd[i];
                    }
                }
            }
            ImGui::EndChild();

            // Detect parallel-space changes from per-row radios and sync once.
            {
                std::lock_guard<std::mutex> lock_target(g_target_mutex);
                for (int link_idx = 0; link_idx < kParallelLinkCount; ++link_idx) {
                    if (g_parallel_space[static_cast<size_t>(link_idx)] == prev_parallel_space[static_cast<size_t>(link_idx)]) {
                        continue;
                    }
                    SyncParallelLinkTargetsToCurrentState(link_idx);
                    const bool now_motor = g_parallel_space[static_cast<size_t>(link_idx)] == static_cast<int>(SpaceSelection::MOTOR);
                    AddLog(std::string(kParallelNames[static_cast<size_t>(link_idx)]) + " space changed to " + (now_motor ? "MS" : "PJS") +
                           " (synced to current state)");
                }
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Center panel: IMU State
        ImGui::BeginChild("CenterPanel", ImVec2(center_panel_width, top_panel_height), true);
        {
            // CenterPanel itself is now fixed-width (sized for LowCmd table),
            // so each sub-section just fills the panel (width = 0).

            // DDS Connection Status — fixed-width sub-frame.
            ImGui::BeginChild("DdsConnInfo", ImVec2(0.0f, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);
            {
                SectionHeader("DDS Connection Status");

                auto *factory        = ChannelFactory::Instance();
                const std::string ns = factory->GetNamespace();
                ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
                ImGui::Text("Domain:    %d", factory->GetDomainId());
                ImGui::Text("Namespace: %s", ns.empty() ? "(none)" : ns.c_str());
                ImGui::PopTextWrapPos();
                ImGui::Separator();

                // Check connection timeout (2 seconds without message = disconnected)
                auto now          = std::chrono::steady_clock::now();
                auto elapsed      = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_last_robotstate_time).count();
                bool is_connected = g_robotstate_connected && (elapsed < 2000);

                bool is_sim_env = false;
                if (is_connected) {
                    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Status: Connected");
                    {
                        std::lock_guard<std::mutex> lock(g_robotstate_mutex);
                        ImGui::Text("Robot State: %s", RobotControlStateToString(g_latest_robotstate.state()));
                        is_sim_env = (g_latest_robotstate.environment() == RobotEnvironment::ROBOT_ENV_SIM);
                        ImGui::TextColored(is_sim_env ? ImVec4(0.5f, 0.8f, 1.0f, 1.0f) : ImVec4(1.0f, 0.5f, 0.3f, 1.0f), "Environment: %s",
                                           is_sim_env ? "SIM" : "REAL");
                    }
                } else {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Status: Disconnected");
                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Robot State: --");
                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Environment: --");
                }

                // Mujoco Sim controls — only shown when bridge reports SIM.
                if (is_connected && is_sim_env) {
                    ImGui::Separator();
                    SubsectionHeader("Mujoco Sim");
                    const float spacing      = ImGui::GetStyle().ItemSpacing.x;
                    const float button_width = (ImGui::GetContentRegionAvail().x - spacing) / 2.0f;
                    if (ScaledButton("Pause", ImVec2(button_width, action_btn_height))) {
                        CallSendMujocoSimCmdAsync(&client, MujocoSimCommandType::MUJOCO_SIM_PAUSE, "PAUSE");
                    }
                    ImGui::SameLine();
                    if (ScaledButton("Resume", ImVec2(button_width, action_btn_height))) {
                        CallSendMujocoSimCmdAsync(&client, MujocoSimCommandType::MUJOCO_SIM_RESUME, "RESUME");
                    }
                    if (ScaledButton("Reload Model", ImVec2(button_width, action_btn_height))) {
                        CallSendMujocoSimCmdAsync(&client, MujocoSimCommandType::MUJOCO_SIM_RELOAD, "RELOAD");
                    }
                    ImGui::SameLine();
                    if (ScaledButton("Reset State", ImVec2(button_width, action_btn_height))) {
                        CallSendMujocoSimCmdAsync(&client, MujocoSimCommandType::MUJOCO_SIM_RESET, "RESET");
                    }
                }
            }
            ImGui::EndChild();

            ImGui::Spacing();

            // IMU State — same compact width as DDS section.
            ImGui::BeginChild("ImuStateBox", ImVec2(0.0f, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);
            {
                SectionHeader("IMU State");

                if (g_first_state_received) {
                    std::lock_guard<std::mutex> lock(g_lowstate_mutex);
                    const auto &imu          = g_latest_lowstate.imu_state();
                    const ImVec4 label_color = ImVec4(0.5f, 1.0f, 0.5f, 1.0f);

                    ImGui::TextColored(label_color, "Quaternion:");
                    ImGui::Text("  w: %8.4f   x: %8.4f   y: %8.4f   z: %8.4f", imu.quaternion()[0], imu.quaternion()[1],
                                imu.quaternion()[2], imu.quaternion()[3]);

                    ImGui::TextColored(label_color, "Gyroscope (rad/s):");
                    ImGui::Text("  x: %8.4f   y: %8.4f   z: %8.4f", imu.gyroscope()[0], imu.gyroscope()[1], imu.gyroscope()[2]);

                    ImGui::TextColored(label_color, "Accelerometer (m/s^2):");
                    ImGui::Text("  x: %8.4f   y: %8.4f   z: %8.4f", imu.accelerometer()[0], imu.accelerometer()[1], imu.accelerometer()[2]);

                    ImGui::TextColored(label_color, "Roll-Pitch-Yaw (rad):");
                    ImGui::Text("  R: %8.4f   P: %8.4f   Y: %8.4f", imu.rpy()[0], imu.rpy()[1], imu.rpy()[2]);
                } else {
                    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Waiting for IMU data...");
                }
            }
            ImGui::EndChild();

            ImGui::Spacing();

            // LowCmd Publish Info — same width, fills remaining vertical space.
            ImGui::BeginChild("LowCmdPubBox", ImVec2(0.0f, 0), ImGuiChildFlags_Border);
            {
                SectionHeader("LowCmd Publishing");

                if (g_lowlevel_active) {
                    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Status: ACTIVE");
                    ImGui::Text("Published: %u msgs", g_lowcmd_publish_count.load());
                    ImGui::Separator();

                    // Tab bar for LowCmd and Cmd vs State comparison
                    if (ImGui::BeginTabBar("CmdStateTabBar")) {
                        constexpr ImGuiTableFlags kTableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV |
                                                                ImGuiTableFlags_BordersOuter | ImGuiTableFlags_ScrollY |
                                                                ImGuiTableFlags_SizingFixedFit;

                        // Tab 1: LowCmd only
                        if (ImGui::BeginTabItem("LowCmd")) {
                            std::lock_guard<std::mutex> lock(g_last_cmd_mutex);
                            if (ImGui::BeginTable("LowCmdTable", 6, kTableFlags)) {
                                ImGui::TableSetupScrollFreeze(0, 1);
                                ImGui::TableSetupColumn("#");
                                ImGui::TableSetupColumn("Joint");
                                ImGui::TableSetupColumn("q (rad)");
                                ImGui::TableSetupColumn("tau (Nm)");
                                ImGui::TableSetupColumn("kp");
                                ImGui::TableSetupColumn("kd");
                                ImGui::TableHeadersRow();
                                for (int i = 0; i < 31; i++) {
                                    const auto &cmd = g_last_published_cmd.motors()[i];
                                    ImGui::TableNextRow();
                                    ImGui::TableSetColumnIndex(0);
                                    ImGui::Text("%d", i);
                                    ImGui::TableSetColumnIndex(1);
                                    ImGui::TextUnformatted(JOINT_NAMES[i]);
                                    ImGui::TableSetColumnIndex(2);
                                    ImGui::Text("%7.3f", cmd.q());
                                    ImGui::TableSetColumnIndex(3);
                                    ImGui::Text("%6.2f", cmd.tau());
                                    ImGui::TableSetColumnIndex(4);
                                    ImGui::Text("%5.1f", cmd.kp());
                                    ImGui::TableSetColumnIndex(5);
                                    ImGui::Text("%4.2f", cmd.kd());
                                }
                                ImGui::EndTable();
                            }
                            ImGui::EndTabItem();
                        }

                        // Tab 2: Cmd vs State comparison.
                        // Each parallel-link joint can be commanded in motor (MS) or
                        // joint (PJS) space; the state we compare against must be in
                        // the SAME space, otherwise the diff is meaningless.
                        if (ImGui::BeginTabItem("Cmd vs State")) {
                            std::lock_guard<std::mutex> lock_cmd(g_last_cmd_mutex);
                            std::lock_guard<std::mutex> lock_state(g_lowstate_mutex);
                            if (ImGui::BeginTable("CmdStateTable", 6, kTableFlags)) {
                                ImGui::TableSetupScrollFreeze(0, 1);
                                ImGui::TableSetupColumn("#");
                                ImGui::TableSetupColumn("Joint");
                                ImGui::TableSetupColumn("Space");
                                ImGui::TableSetupColumn("cmd (rad)");
                                ImGui::TableSetupColumn("state (rad)");
                                ImGui::TableSetupColumn("diff");
                                ImGui::TableHeadersRow();
                                for (int i = 0; i < 31; i++) {
                                    // Non-parallel-link joints have no MS/PJS distinction
                                    // (motor space and joint space coincide), so tag them
                                    // "MS/PJS" instead of forcing one of the two labels.
                                    const bool has_parallel    = (GetParallelLinkIndexForJoint(i) >= 0);
                                    const bool use_motor_space = UseMotorSpaceForJoint(i);
                                    const char *space_tag      = has_parallel ? (use_motor_space ? "MS" : "PJS") : "MS/PJS";
                                    const char *display_name   = (has_parallel && use_motor_space) ? MOTOR_NAMES[i] : JOINT_NAMES[i];
                                    const float cmd_q          = g_last_published_cmd.motors()[i].q();
                                    const float state_q =
                                        use_motor_space ? g_latest_lowstate.motor_state()[i].q() : g_latest_lowstate.joint_state()[i].q();
                                    const float diff      = cmd_q - state_q;
                                    const bool out_of_tol = std::abs(diff) > 0.05f;

                                    ImGui::TableNextRow();
                                    if (out_of_tol) {
                                        // Tint the row to flag a deviation > 0.05 rad.
                                        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0,
                                                               ImGui::GetColorU32(ImVec4(0.5f, 0.1f, 0.1f, 0.35f)));
                                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                                    }

                                    ImGui::TableSetColumnIndex(0);
                                    ImGui::Text("%d", i);
                                    ImGui::TableSetColumnIndex(1);
                                    ImGui::TextUnformatted(display_name);
                                    ImGui::TableSetColumnIndex(2);
                                    ImGui::TextUnformatted(space_tag);
                                    ImGui::TableSetColumnIndex(3);
                                    ImGui::Text("%7.3f", cmd_q);
                                    ImGui::TableSetColumnIndex(4);
                                    ImGui::Text("%7.3f", state_q);
                                    ImGui::TableSetColumnIndex(5);
                                    ImGui::Text("%+7.3f", diff);

                                    if (out_of_tol) {
                                        ImGui::PopStyleColor();
                                    }
                                }
                                ImGui::EndTable();
                            }
                            ImGui::EndTabItem();
                        }

                        ImGui::EndTabBar();
                    }
                } else {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Status: INACTIVE");
                    ImGui::Text("Press LowCmd Publish Start to begin");
                }
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Right panel: Service API buttons
        ImGui::BeginChild("RightPanel", ImVec2(right_panel_width, top_panel_height), true);
        {
            SectionHeader("Service API Commands");

            // --- Power & Torque -------------------------------------------------
            SubsectionHeader("Power & Torque");
            if (ScaledButton("Init BMS", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Init BMS", [&client]() { CallInitBmsAsync(&client, BmsInitType::BMS_INIT, "BMS_INIT"); });
            }
            if (ScaledButton("Init Motor", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Init Motor", [&client]() { CallInitBmsAsync(&client, BmsInitType::MOTOR_INIT, "MOTOR_INIT"); });
            }
            if (ScaledButton("Init BMS + Motor", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Init BMS + Motor",
                                      [&client]() { CallInitBmsAsync(&client, BmsInitType::BMS_AND_MOTOR_INIT, "BMS_AND_MOTOR_INIT"); });
            }
            if (ScaledButton("BMS OFF", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("BMS OFF", [&client]() { CallInitBmsAsync(&client, BmsInitType::BMS_INIT_NONE, "BMS_INIT_NONE"); });
            }
            if (ScaledButton("Torque ON", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Torque ON", [&client]() { CallSetTorqueAsync(&client, TorqueType::TORQUE_ON, "TORQUE_ON"); });
            }
            if (ScaledButton("Torque OFF", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Torque OFF", [&client]() { CallSetTorqueAsync(&client, TorqueType::TORQUE_OFF, "TORQUE_OFF"); });
            }

            // --- Control Mode ---------------------------------------------------
            SubsectionHeader("Control Mode");
            if (ScaledButton("High-Level", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Control Mode: High-Level", [&client]() {
                    CallSendControlModeCommandAsync(&client, ControlModeCommandType::CONTROL_MODE_CMD_HIGH_LEVEL_JOINT_CONTROL,
                                                    "HIGH_LEVEL_JOINT_CONTROL");
                });
            }
            if (ScaledButton("Low-Level", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Control Mode: Low-Level", [&client]() {
                    CallSendControlModeCommandAsync(&client, ControlModeCommandType::CONTROL_MODE_CMD_LOW_LEVEL_JOINT_CONTROL,
                                                    "LOW_LEVEL_JOINT_CONTROL");
                });
            }
            if (ScaledButton("Joint Position Hold", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Joint Position Hold", [&client]() {
                    CallSendControlModeCommandAsync(&client, ControlModeCommandType::CONTROL_MODE_CMD_JOINT_POSITION_HOLD,
                                                    "JOINT_POSITION_HOLD");
                });
            }
            if (ScaledButton("Motion Stop", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Motion Stop", [&client]() {
                    CallSendControlModeCommandAsync(&client, ControlModeCommandType::CONTROL_MODE_CMD_MOTION_STOP, "MOTION_STOP");
                });
            }
            if (ScaledButton("Walkmode", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Walkmode", [&client]() {
                    CallSendControlModeCommandAsync(&client, ControlModeCommandType::CONTROL_MODE_CMD_WALKMODE_ON, "WALKMODE_ON");
                });
            }
            if (ScaledButton("Low-Level Walkmode", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Low-Level Walkmode", [&client]() {
                    CallSendControlModeCommandAsync(&client, ControlModeCommandType::CONTROL_MODE_CMD_LOW_LEVEL_WALKMODE_ON,
                                                    "LOW_LEVEL_WALKMODE_ON");
                });
            }
            if (ScaledButton("High-Level Walkmode", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("High-Level Walkmode", [&client]() {
                    CallSendControlModeCommandAsync(&client, ControlModeCommandType::CONTROL_MODE_CMD_HIGH_LEVEL_WALKMODE_ON,
                                                    "HIGH_LEVEL_WALKMODE_ON");
                });
            }

            // --- Custom Modes ---------------------------------------------------
            SubsectionHeader("Custom Modes");
            if (ScaledButton("Custom Mode 1", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Custom Mode 1", [&client]() {
                    CallSendControlModeCommandAsync(&client, ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_1, "CUSTOM_MODE_1");
                });
            }
            if (ScaledButton("Custom Mode 2", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Custom Mode 2", [&client]() {
                    CallSendControlModeCommandAsync(&client, ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_2, "CUSTOM_MODE_2");
                });
            }
            if (ScaledButton("Custom Mode 3", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Custom Mode 3", [&client]() {
                    CallSendControlModeCommandAsync(&client, ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_3, "CUSTOM_MODE_3");
                });
            }

            // --- Motion Presets -------------------------------------------------
            SubsectionHeader("Motion Presets");
            if (ScaledButton("HOME", ImVec2(-1, action_btn_height))) {
                RequestServiceConfirm("Motion Preset: HOME", [&client]() {
                    CallSendControlModeCommandAsync(&client, ControlModeCommandType::CONTROL_MODE_CMD_MOTION_PRESET, "MOTION_PRESET(HOME)",
                                                    "HOME", false);
                });
            }
        }
        ImGui::EndChild();

        // Bottom log strip: full window width below the three panels.
        ImGui::BeginChild("LogPanel", ImVec2(0, 0), true);
        {
            SectionHeader("Response Log");

            ImGui::BeginChild("LogScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            {
                std::lock_guard<std::mutex> lock(g_log_mutex);
                for (const auto &line : g_response_log) {
                    ImGui::TextUnformatted(line.c_str());
                }
            }
            // Auto-scroll to bottom
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
        }
        ImGui::EndChild();

        // Service-command confirmation popup (Power/Torque + Control Mode)
        if (g_show_service_confirm_popup) {
            ImGui::OpenPopup("Confirm Service Command");
            g_show_service_confirm_popup = false;
        }

        ImVec2 svc_center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(svc_center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("Confirm Service Command", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextUnformatted("Execute the following command?");
            ImGui::Spacing();
            ImGui::SetWindowFontScale(1.3f);
            ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.4f, 1.0f), "%s", g_pending_service_label.c_str());
            ImGui::SetWindowFontScale(1.0f);
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "This affects the live robot.");
            ImGui::Separator();

            // Focus Confirm so the popup is keyboard-friendly: Enter to OK,
            // Esc to cancel.
            const bool popup_just_opened = ImGui::IsWindowAppearing();
            if (popup_just_opened) {
                ImGui::SetKeyboardFocusHere();
            }
            const bool enter_pressed =
                !popup_just_opened && (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter));
            const bool escape_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

            if (ScaledButton("Confirm", ImVec2(popup_button_width, action_btn_height)) || enter_pressed) {
                if (g_pending_service_action) {
                    g_pending_service_action();
                }
                g_pending_service_action = nullptr;
                g_pending_service_label.clear();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ScaledButton("Cancel", ImVec2(popup_button_width, action_btn_height)) || escape_pressed) {
                g_pending_service_action = nullptr;
                g_pending_service_label.clear();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        // Kp/Kd change confirmation popup
        if (g_show_gain_confirm_popup) {
            ImGui::OpenPopup("Confirm Gain Change");
            g_show_gain_confirm_popup = false;
        }

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("Confirm Gain Change", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            const char *gain_name                = g_pending_is_kp ? "Kp" : "Kd";
            const bool use_motor_space_for_joint = UseMotorSpaceForJoint(g_pending_gain_joint);
            const char *joint_name = use_motor_space_for_joint ? MOTOR_NAMES[g_pending_gain_joint] : JOINT_NAMES[g_pending_gain_joint];

            ImGui::Text("Change %s gain for Joint %d (%s)?", gain_name, g_pending_gain_joint, joint_name);
            ImGui::Separator();
            ImGui::Text("Current: %.2f", g_pending_old_value);
            ImGui::Text("New:     %.2f", g_pending_new_value);
            ImGui::Separator();

            const bool popup_just_opened = ImGui::IsWindowAppearing();
            if (popup_just_opened) {
                ImGui::SetKeyboardFocusHere();
            }
            const bool enter_pressed =
                !popup_just_opened && (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter));
            if (ScaledButton("Apply", ImVec2(popup_button_width, action_btn_height)) || enter_pressed) {
                if (g_pending_is_kp) {
                    g_kp[g_pending_gain_joint]      = g_pending_new_value;
                    g_kp_edit[g_pending_gain_joint] = g_pending_new_value;
                } else {
                    g_kd[g_pending_gain_joint]      = g_pending_new_value;
                    g_kd_edit[g_pending_gain_joint] = g_pending_new_value;
                }
                AddLog("Gain changed: Joint " + std::to_string(g_pending_gain_joint) + " " + gain_name + " = " +
                       std::to_string(g_pending_new_value));
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ScaledButton("Cancel", ImVec2(popup_button_width, action_btn_height))) {
                // Revert edit buffer
                if (g_pending_is_kp) {
                    g_kp_edit[g_pending_gain_joint] = g_pending_old_value;
                } else {
                    g_kd_edit[g_pending_gain_joint] = g_pending_old_value;
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Ensure background publish loop exits before joining.
    g_running = false;

    // Cleanup
    std::cout << "\nShutting down GUI..." << std::endl;

    // Wait for publish thread to finish
    if (publish_thread.joinable()) {
        publish_thread.join();
    }

    // Tear the global RobotState subscriber down explicitly so it does not
    // race with ChannelFactory's static-storage destruction (the undefined
    // order otherwise leads to use-after-free and SIGABRT).
    if (g_robotstate_sub) {
        g_robotstate_sub->stop();
        g_robotstate_sub.reset();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "GUI Client terminated" << std::endl;
    return 0;
}
