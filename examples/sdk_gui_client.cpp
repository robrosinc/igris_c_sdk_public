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
 * - Left panel: 31 motor/joint sliders with Motor/Joint toggle
 * - Center panel: IMU state display + LowCmd publishing status
 * - Right panel: Service API buttons + response log window
 */

#include <atomic>
#include <chrono>
#include <csignal>
#include <deque>
#include <future>
#include <igris_sdk/channel_factory.hpp>
#include <igris_sdk/igris_c_client.hpp>
#include <igris_sdk/publisher.hpp>
#include <igris_sdk/subscriber.hpp>
#include <iostream>
#include <mutex>
#include <thread>

// ImGui includes
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace igris_sdk;
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

// ControlModeState subscription
static ControlModeState g_latest_controlmodestate;
static std::mutex g_controlmodestate_mutex;
static std::atomic<bool> g_controlmodestate_subscribed(false);
static std::atomic<uint32_t> g_controlmodestate_received_count(0);
static std::unique_ptr<Subscriber<ControlModeState>> g_controlmodestate_sub;

// Initial reference values (set on first state receive)
static std::array<float, 31> g_initial_motor_pos = {};
static std::array<float, 31> g_initial_joint_pos = {};

// GUI state
static int g_show_motor                      = 1;  // 1 = motor, 0 = joint
static int g_prev_show_motor                 = 1;  // Previous mode for detecting changes
static std::array<float, 31> g_slider_values = {};

// LOW_LEVEL control state
static std::atomic<bool> g_lowlevel_active(false);
static std::array<float, 31> g_target_joint_pos  = {};  // Target joint positions for LOW_LEVEL mode
static std::array<float, 31> g_target_motor_pos  = {};  // Target motor positions for LOW_LEVEL mode
static std::array<float, 31> g_current_joint_pos = {};  // Current actual joint positions
static std::array<float, 31> g_current_motor_pos = {};  // Current actual motor positions
static std::mutex g_target_mutex;
static std::atomic<uint32_t> g_lowcmd_publish_count(0);
static LowCmd g_last_published_cmd;
static std::mutex g_last_cmd_mutex;

// Response log
static std::deque<std::string> g_response_log;
static std::mutex g_log_mutex;
static const size_t MAX_LOG_LINES = 50;

void SignalHandler(int) { g_running = false; }

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

// ControlModeState callback
void ControlModeStateCallback(const ControlModeState &state) {
    std::lock_guard<std::mutex> lock(g_controlmodestate_mutex);
    g_latest_controlmodestate = state;
    g_controlmodestate_received_count++;
}

// Helper to convert ControlMode to string
const char *ControlModeToString(ControlMode mode) {
    switch (mode) {
        case ControlMode::CONTROL_MODE_LOW_LEVEL:
            return "LOW_LEVEL";
        case ControlMode::CONTROL_MODE_HIGH_LEVEL:
            return "HIGH_LEVEL";
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
        auto res = client->InitBms(type, 30000);
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
        auto res = client->SetTorque(type, 30000);
        std::string result =
            std::string("SetTorque(") + type_name_copy + "): " + (res.success() ? "SUCCESS" : "FAILED") + " - " + res.message();
        AddLog(result);
        g_service_call_in_progress = false;
    }).detach();
}

void CallSetControlModeAsync(IgrisC_Client *client, ControlMode mode, const char *mode_name) {
    if (g_service_call_in_progress.load()) {
        AddLog("Service call already in progress, please wait...");
        return;
    }
    g_service_call_in_progress = true;
    AddLog(std::string("Calling SetControlMode(") + mode_name + ")...");

    std::string mode_name_copy(mode_name);
    std::thread([client, mode, mode_name_copy]() {
        auto res = client->SetControlMode(mode, 30000);
        std::string result =
            std::string("SetControlMode(") + mode_name_copy + "): " + (res.success() ? "SUCCESS" : "FAILED") + " - " + res.message();
        AddLog(result);

        // Activate LOW_LEVEL publishing if mode is LOW_LEVEL
        if (res.success() && mode == ControlMode::CONTROL_MODE_LOW_LEVEL) {
            // Initialize target positions to current positions
            {
                std::lock_guard<std::mutex> lock_state(g_lowstate_mutex);
                std::lock_guard<std::mutex> lock_target(g_target_mutex);
                for (int i = 0; i < 31; i++) {
                    g_target_joint_pos[i] = g_latest_lowstate.joint_state()[i].q();
                    g_target_motor_pos[i] = g_latest_lowstate.motor_state()[i].q();
                }
            }
            g_lowlevel_active = true;
            AddLog("LOW_LEVEL mode activated - initialized to current positions");
        } else if (mode != ControlMode::CONTROL_MODE_LOW_LEVEL) {
            g_lowlevel_active = false;
            AddLog("LOW_LEVEL mode deactivated");
        }
        g_service_call_in_progress = false;
    }).detach();
}

// 300Hz LowCmd publishing thread
void LowCmdPublishThread(Publisher<LowCmd> *publisher) {
    const auto period = std::chrono::microseconds(3333);  // ~300Hz
    auto next_time    = std::chrono::steady_clock::now();

    // Example default PD gains - adjust these values based on your robot configuration
    static const std::array<float, 31> default_kp = {
        50.0,  25.0,  25.0,                            // Waist
        500.0, 200.0, 50.0, 500.0, 300.0, 300.0,       // Left leg
        500.0, 200.0, 50.0, 500.0, 300.0, 300.0,       // Right leg
        50.0,  50.0,  30.0, 30.0,  5.0,   5.0,   5.0,  // Left arm
        50.0,  50.0,  30.0, 30.0,  5.0,   5.0,   5.0,  // Right arm
        2.0,   5.0                                     // Neck
    };
    static const std::array<float, 31> default_kd = {
        0.8,  0.8, 0.8,                        // Waist
        3.0,  0.5, 0.5,  3.0,  1.5, 1.5,       // Left leg
        3.0,  0.5, 0.5,  3.0,  1.5, 1.5,       // Right leg
        0.5,  0.5, 0.15, 0.15, 0.1, 0.1, 0.1,  // Left arm
        0.5,  0.5, 0.15, 0.15, 0.1, 0.1, 0.1,  // Right arm
        0.05, 0.1                              // Neck
    };

    while (g_running) {
        if (g_lowlevel_active && g_first_state_received) {
            LowCmd cmd;

            {
                std::lock_guard<std::mutex> lock_target(g_target_mutex);

                // Initialize LowCmd with proper control calculations
                // Determine which mode based on current GUI state
                bool use_joint_mode = (g_show_motor == 0);  // 0 = joint, 1 = motor

                // Set kinematic mode at LowCmd level (전체 적용)
                cmd.kinematic_mode(use_joint_mode ? KinematicMode::PJS : KinematicMode::MS);

                for (int i = 0; i < 31; i++) {
                    auto &motor_cmd = cmd.motors()[i];

                    // Set motor ID
                    motor_cmd.id(i);

                    float q_target;

                    if (use_joint_mode) {
                        // Joint Space (PJS) mode
                        q_target = g_target_joint_pos[i];
                    } else {
                        // Motor Space (MS) mode
                        q_target = g_target_motor_pos[i];
                    }

                    // Set commands
                    motor_cmd.q(q_target);
                    motor_cmd.dq(0.0f);
                    motor_cmd.tau(0.0f);
                    motor_cmd.kp(default_kp[i]);
                    motor_cmd.kd(default_kd[i]);
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
    std::cout << "║  IGRIS-SDK Service API GUI Client     ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝\n" << std::endl;

    // Parse arguments
    int domain_id = 0;
    if (argc > 1) {
        domain_id = std::atoi(argv[1]);
    }

    std::cout << "Domain ID: " << domain_id << std::endl;
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

    // Create window
    GLFWwindow *window = glfwCreateWindow(1800, 900, "IGRIS-SDK Service API Test GUI", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
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

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Initialize ChannelFactory
    std::cout << "Initializing ChannelFactory..." << std::endl;
    ChannelFactory::Instance()->Init(domain_id);

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

        // Create main window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("IGRIS-SDK Service API Test", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBringToFrontOnFocus);

        // Left panel: Sliders
        ImGui::BeginChild("LeftPanel", ImVec2(650, 0), true);
        {
            ImGui::Text("Motor/Joint State Sliders");
            ImGui::Separator();

            // Motor/Joint toggle
            ImGui::RadioButton("Motor State", &g_show_motor, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Joint State", &g_show_motor, 0);

            // Detect mode change and update target positions to current state
            if (g_show_motor != g_prev_show_motor && g_lowlevel_active) {
                std::lock_guard<std::mutex> lock_target(g_target_mutex);
                if (g_show_motor == 1) {
                    // Switched to Motor mode: set motor targets to current motor positions
                    for (int i = 0; i < 31; i++) {
                        g_target_motor_pos[i] = g_current_motor_pos[i];
                    }
                    AddLog("Switched to Motor mode - targets set to current motor positions");
                } else {
                    // Switched to Joint mode: set joint targets to current joint positions
                    for (int i = 0; i < 31; i++) {
                        g_target_joint_pos[i] = g_current_joint_pos[i];
                    }
                    AddLog("Switched to Joint mode - targets set to current joint positions");
                }
                g_prev_show_motor = g_show_motor;
            }
            ImGui::Separator();

            // Reset button (always visible but only works in LOW_LEVEL mode)
            if (ImGui::Button("Reset to Initial", ImVec2(-1, 30))) {
                if (g_lowlevel_active && g_first_state_received) {
                    std::lock_guard<std::mutex> lock_target(g_target_mutex);
                    for (int i = 0; i < 31; i++) {
                        g_target_motor_pos[i] = g_initial_motor_pos[i];
                        g_target_joint_pos[i] = g_initial_joint_pos[i];
                    }
                    AddLog("Target positions reset to initial values");
                } else if (!g_lowlevel_active) {
                    AddLog("Enable LOW_LEVEL mode first to use Reset");
                } else {
                    AddLog("Waiting for initial state to be received");
                }
            }
            ImGui::Separator();

            // Status
            ImGui::Text("LowState messages received: %u", g_lowstate_received_count.load());
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
                ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "LOW_LEVEL Mode - Sliders control target positions");
                ImGui::Separator();
            }

            for (int i = 0; i < 31; i++) {
                char label[64];
                const char *name = (g_show_motor == 1) ? MOTOR_NAMES[i] : JOINT_NAMES[i];
                snprintf(label, sizeof(label), "%d. %s", i, name);

                if (g_lowlevel_active) {
                    // LOW_LEVEL mode: Show editable target positions
                    std::lock_guard<std::mutex> lock_target(g_target_mutex);
                    if (g_show_motor == 1) {
                        ImGui::SliderFloat(label, &g_target_motor_pos[i], MOTOR_POS_MIN[i], MOTOR_POS_MAX[i], "%.3f rad",
                                           ImGuiSliderFlags_AlwaysClamp);
                    } else {
                        ImGui::SliderFloat(label, &g_target_joint_pos[i], JOINT_POS_MIN[i], JOINT_POS_MAX[i], "%.3f rad",
                                           ImGuiSliderFlags_AlwaysClamp);
                    }
                } else {
                    // Display mode: Show current state (read-only)
                    if (g_first_state_received) {
                        std::lock_guard<std::mutex> lock(g_lowstate_mutex);
                        if (g_show_motor == 1) {
                            g_slider_values[i] = g_latest_lowstate.motor_state()[i].q();
                        } else {
                            g_slider_values[i] = g_latest_lowstate.joint_state()[i].q();
                        }
                    }
                    // Use appropriate limits based on current mode
                    float pos_min = (g_show_motor == 1) ? MOTOR_POS_MIN[i] : JOINT_POS_MIN[i];
                    float pos_max = (g_show_motor == 1) ? MOTOR_POS_MAX[i] : JOINT_POS_MAX[i];
                    ImGui::SliderFloat(label, &g_slider_values[i], pos_min, pos_max, "%.3f rad");
                }
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Center panel: IMU State
        ImGui::BeginChild("CenterPanel", ImVec2(450, 0), true);
        {
            ImGui::Text("IMU State");
            ImGui::Separator();

            if (g_first_state_received) {
                std::lock_guard<std::mutex> lock(g_lowstate_mutex);
                const auto &imu = g_latest_lowstate.imu_state();

                // Quaternion
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Quaternion:");
                ImGui::Text("  w: %.4f", imu.quaternion()[0]);
                ImGui::Text("  x: %.4f", imu.quaternion()[1]);
                ImGui::Text("  y: %.4f", imu.quaternion()[2]);
                ImGui::Text("  z: %.4f", imu.quaternion()[3]);
                ImGui::Separator();

                // Gyroscope
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Gyroscope (rad/s):");
                ImGui::Text("  x: %.4f", imu.gyroscope()[0]);
                ImGui::Text("  y: %.4f", imu.gyroscope()[1]);
                ImGui::Text("  z: %.4f", imu.gyroscope()[2]);
                ImGui::Separator();

                // Accelerometer
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Accelerometer (m/s^2):");
                ImGui::Text("  x: %.4f", imu.accelerometer()[0]);
                ImGui::Text("  y: %.4f", imu.accelerometer()[1]);
                ImGui::Text("  z: %.4f", imu.accelerometer()[2]);
                ImGui::Separator();

                // RPY
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Roll-Pitch-Yaw (rad):");
                ImGui::Text("  Roll:  %.4f", imu.rpy()[0]);
                ImGui::Text("  Pitch: %.4f", imu.rpy()[1]);
                ImGui::Text("  Yaw:   %.4f", imu.rpy()[2]);
            } else {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Waiting for IMU data...");
            }

            ImGui::Separator();
            ImGui::Separator();

            // LowCmd Publish Info
            ImGui::Text("LowCmd Publishing");
            ImGui::Separator();

            if (g_lowlevel_active) {
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "Status: ACTIVE");
                ImGui::Text("Published: %u msgs", g_lowcmd_publish_count.load());
                ImGui::Separator();

                // Show all 31 joint commands in scrollable area
                std::lock_guard<std::mutex> lock(g_last_cmd_mutex);

                ImGui::BeginChild("CmdScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

                for (int i = 0; i < 31; i++) {
                    const auto &cmd = g_last_published_cmd.motors()[i];
                    ImGui::Text("J%2d: q=%7.3f  tau=%6.2f  kp=%5.1f  kd=%4.2f", i, cmd.q(), cmd.tau(), cmd.kp(), cmd.kd());
                }

                ImGui::EndChild();
            } else {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Status: INACTIVE");
                ImGui::Text("Enable LOW_LEVEL mode to start");
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Right panel: Buttons and Log
        ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
        {
            ImGui::Text("Service API Commands");
            ImGui::Separator();

            // Service buttons
            if (ImGui::Button("1. Init BMS", ImVec2(-1, 40))) {
                CallInitBmsAsync(&client, BmsInitType::BMS_INIT, "BMS_INIT");
            }

            if (ImGui::Button("2. Init Motor", ImVec2(-1, 40))) {
                CallInitBmsAsync(&client, BmsInitType::MOTOR_INIT, "MOTOR_INIT");
            }

            if (ImGui::Button("3. Init BMS and Motor", ImVec2(-1, 40))) {
                CallInitBmsAsync(&client, BmsInitType::BMS_AND_MOTOR_INIT, "BMS_AND_MOTOR_INIT");
            }

            if (ImGui::Button("4. BMS OFF", ImVec2(-1, 40))) {
                CallInitBmsAsync(&client, BmsInitType::BMS_INIT_NONE, "BMS_INIT_NONE");
            }

            if (ImGui::Button("5. Set Torque ON", ImVec2(-1, 40))) {
                CallSetTorqueAsync(&client, TorqueType::TORQUE_ON, "TORQUE_ON");
            }

            if (ImGui::Button("6. Set Torque OFF", ImVec2(-1, 40))) {
                CallSetTorqueAsync(&client, TorqueType::TORQUE_OFF, "TORQUE_OFF");
            }

            if (ImGui::Button("7. Control Mode: LOW_LEVEL", ImVec2(-1, 40))) {
                CallSetControlModeAsync(&client, ControlMode::CONTROL_MODE_LOW_LEVEL, "LOW_LEVEL");
            }

            if (ImGui::Button("8. Control Mode: HIGH_LEVEL", ImVec2(-1, 40))) {
                CallSetControlModeAsync(&client, ControlMode::CONTROL_MODE_HIGH_LEVEL, "HIGH_LEVEL");
            }

            ImGui::Separator();

            // ControlModeState subscription button
            if (!g_controlmodestate_subscribed) {
                if (ImGui::Button("9. Subscribe ControlModeState", ImVec2(-1, 40))) {
                    g_controlmodestate_sub = std::make_unique<Subscriber<ControlModeState>>("rt/controlmodestate");
                    if (g_controlmodestate_sub->init(ControlModeStateCallback)) {
                        g_controlmodestate_subscribed = true;
                        AddLog("ControlModeState subscription started");
                    } else {
                        AddLog("Failed to subscribe to ControlModeState");
                        g_controlmodestate_sub.reset();
                    }
                }
            } else {
                if (ImGui::Button("9. Unsubscribe ControlModeState", ImVec2(-1, 40))) {
                    g_controlmodestate_sub.reset();
                    g_controlmodestate_subscribed = false;
                    g_controlmodestate_received_count = 0;
                    AddLog("ControlModeState subscription stopped");
                }
            }

            // Display ControlModeState if subscribed
            if (g_controlmodestate_subscribed) {
                ImGui::Separator();
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "ControlModeState:");
                ImGui::Text("  Received: %u msgs", g_controlmodestate_received_count.load());
                {
                    std::lock_guard<std::mutex> lock(g_controlmodestate_mutex);
                    ImGui::Text("  Tick: %u", g_latest_controlmodestate.tick());
                    ImGui::Text("  Mode: %s", ControlModeToString(g_latest_controlmodestate.mode()));
                }
            }

            ImGui::Separator();

            // Response log
            ImGui::Text("Response Log");
            ImGui::Separator();

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

    // Cleanup
    std::cout << "\nShutting down GUI..." << std::endl;

    // Wait for publish thread to finish
    if (publish_thread.joinable()) {
        publish_thread.join();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "GUI Client terminated" << std::endl;
    return 0;
}
