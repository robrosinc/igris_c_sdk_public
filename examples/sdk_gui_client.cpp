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
#include <cmath>
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

// ControlModeState subscription (auto-subscribe at startup)
static ControlModeState g_latest_controlmodestate;
static std::mutex g_controlmodestate_mutex;
static std::atomic<bool> g_controlmodestate_connected(false);
static std::atomic<uint32_t> g_controlmodestate_received_count(0);
static std::unique_ptr<Subscriber<ControlModeState>> g_controlmodestate_sub;
static std::chrono::steady_clock::time_point g_last_controlmodestate_time;

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
    50.0f,  25.0f,  25.0f,                              // Waist
    500.0f, 200.0f, 50.0f, 500.0f, 50.0f, 50.0f,        // Left leg
    500.0f, 200.0f, 50.0f, 500.0f, 50.0f, 50.0f,        // Right leg
    50.0f,  50.0f,  30.0f, 30.0f,  5.0f,  5.0f,  5.0f,  // Left arm
    50.0f,  50.0f,  30.0f, 30.0f,  5.0f,  5.0f,  5.0f,  // Right arm
    2.0f,   5.0f                                        // Neck
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
    g_last_controlmodestate_time = std::chrono::steady_clock::now();
    g_controlmodestate_connected = true;
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

void CallSetControlModeAsync(IgrisC_Client *client, ControlMode mode, const char *mode_name) {
    if (g_service_call_in_progress.load()) {
        AddLog("Service call already in progress, please wait...");
        return;
    }
    g_service_call_in_progress = true;
    AddLog(std::string("Calling SetControlMode(") + mode_name + ")...");

    std::string mode_name_copy(mode_name);
    std::thread([client, mode, mode_name_copy]() {
        auto res = client->SetControlMode(mode, 60000);
        std::string result =
            std::string("SetControlMode(") + mode_name_copy + "): " + (res.success() ? "SUCCESS" : "FAILED") + " - " + res.message();
        AddLog(result);

        // Stop LowCmd publishing when switching away from LOW_LEVEL
        if (res.success() && mode != ControlMode::CONTROL_MODE_LOW_LEVEL) {
            if (g_lowlevel_active) {
                g_lowlevel_active = false;
                AddLog("LowCmd publishing stopped (mode changed)");
            }
        }
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
        if (g_lowlevel_active && g_first_state_received) {
            LowCmd cmd;

            {
                std::lock_guard<std::mutex> lock_target(g_target_mutex);

                // Initialize LowCmd with proper control calculations
                // Determine which mode based on current GUI state
                bool use_joint_mode = (g_show_motor == 0);  // 0 = joint, 1 = motor

                // Set kinematic mode at LowCmd level (전체 적용)
                cmd.kinematic_mode(use_joint_mode ? KinematicMode::PJS : KinematicMode::MS);

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

                    float &q_cmd_ref = use_joint_mode ? g_cmd_joint_pos[i] : g_cmd_motor_pos[i];
                    float q_target   = use_joint_mode ? g_target_joint_pos[i] : g_target_motor_pos[i];

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
    GLFWwindow *window = glfwCreateWindow(1800, 900, "IGRIS-SDK Dev Tool", NULL, NULL);
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

    // Auto-subscribe to ControlModeState
    std::cout << "Subscribing to ControlModeState..." << std::endl;
    g_controlmodestate_sub = std::make_unique<Subscriber<ControlModeState>>("rt/controlmodestate");
    if (g_controlmodestate_sub->init(ControlModeStateCallback)) {
        AddLog("ControlModeState subscription started");
    } else {
        AddLog("Failed to subscribe to ControlModeState");
        g_controlmodestate_sub.reset();
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
        ImGui::Begin("IGRIS-SDK Dev Tool", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBringToFrontOnFocus);

        // Left panel: Sliders
        ImGui::BeginChild("LeftPanel", ImVec2(720, 0), true);
        {
            ImGui::Text("Motor/Joint State Sliders");
            ImGui::Separator();

            // Motor/Joint toggle
            ImGui::RadioButton("Motor State", &g_show_motor, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Joint State", &g_show_motor, 0);

            // Detect mode change and update target/cmd positions to current state
            if (g_show_motor != g_prev_show_motor && g_lowlevel_active) {
                std::lock_guard<std::mutex> lock_target(g_target_mutex);
                if (g_show_motor == 1) {
                    // Switched to Motor mode: set motor targets and cmd to current motor positions
                    for (int i = 0; i < 31; i++) {
                        g_target_motor_pos[i] = g_current_motor_pos[i];
                        g_cmd_motor_pos[i]    = g_current_motor_pos[i];
                    }
                    AddLog("Switched to Motor mode - synchronized to current motor positions");
                } else {
                    // Switched to Joint mode: set joint targets and cmd to current joint positions
                    for (int i = 0; i < 31; i++) {
                        g_target_joint_pos[i] = g_current_joint_pos[i];
                        g_cmd_joint_pos[i]    = g_current_joint_pos[i];
                    }
                    AddLog("Switched to Joint mode - synchronized to current joint positions");
                }
                g_prev_show_motor = g_show_motor;
            }
            ImGui::Separator();

            // Reset, Reset Gains, and Home buttons (3 buttons side by side)
            float button_width = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2) / 3.0f;
            if (ImGui::Button("Reset Pos", ImVec2(button_width, 30))) {
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
            if (ImGui::Button("Reset Gains", ImVec2(button_width, 30))) {
                for (int i = 0; i < 31; i++) {
                    g_kp[i]      = DEFAULT_KP[i];
                    g_kd[i]      = DEFAULT_KD[i];
                    g_kp_edit[i] = DEFAULT_KP[i];
                    g_kd_edit[i] = DEFAULT_KD[i];
                }
                AddLog("All Kp/Kd gains reset to defaults");
            }
            ImGui::SameLine();
            if (ImGui::Button("Home", ImVec2(button_width, 30))) {
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
            ImGui::Separator();

            // Status and Convergence Time slider on same line
            ImGui::Text("LowState: %u", g_lowstate_received_count.load());
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 130);
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
            float kd_width      = 50;
            float kp_width      = 60;
            float spacing       = ImGui::GetStyle().ItemSpacing.x;

            ImGui::Text("Name");
            ImGui::SameLine(190);
            ImGui::Text("Position");
            ImGui::SameLine(content_width - kd_width - spacing - kp_width);
            ImGui::Text("  Kp");
            ImGui::SameLine(content_width - kd_width);
            ImGui::Text("  Kd");
            ImGui::Separator();

            const float FINE_STEP     = 0.001f;  // Fine adjustment step for arrow keys
            static int selected_joint = -1;      // Currently selected joint for fine control

            // Handle Up/Down arrow keys for joint selection (outside loop to prevent multiple triggers)
            if (g_lowlevel_active) {
                if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
                    if (selected_joint < 0) {
                        selected_joint = 0;
                    } else {
                        selected_joint = (selected_joint > 0) ? selected_joint - 1 : 30;
                    }
                }
                if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
                    if (selected_joint < 0) {
                        selected_joint = 0;
                    } else {
                        selected_joint = (selected_joint < 30) ? selected_joint + 1 : 0;
                    }
                }
            }

            for (int i = 0; i < 31; i++) {
                char label[64];
                char slider_label[64];
                const char *name = (g_show_motor == 1) ? MOTOR_NAMES[i] : JOINT_NAMES[i];
                snprintf(label, sizeof(label), "%2d. %s", i, name);
                snprintf(slider_label, sizeof(slider_label), "##slider%d", i);

                if (g_lowlevel_active) {
                    // LOW_LEVEL mode: Show editable target positions
                    std::lock_guard<std::mutex> lock_target(g_target_mutex);
                    float *target_val;
                    float pos_min, pos_max;
                    if (g_show_motor == 1) {
                        target_val = &g_target_motor_pos[i];
                        pos_min    = MOTOR_POS_MIN[i];
                        pos_max    = MOTOR_POS_MAX[i];
                    } else {
                        target_val = &g_target_joint_pos[i];
                        pos_min    = JOINT_POS_MIN[i];
                        pos_max    = JOINT_POS_MAX[i];
                    }

                    // Selectable name - click to select for fine control
                    bool is_selected = (selected_joint == i);
                    if (is_selected) {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));  // Green when selected
                    }
                    if (ImGui::Selectable(label, is_selected, 0, ImVec2(180, 0))) {
                        selected_joint = (selected_joint == i) ? -1 : i;  // Toggle selection
                    }
                    if (is_selected) {
                        ImGui::PopStyleColor();
                        ImGui::SetScrollHereY(0.5f);  // Scroll to keep selected item visible
                    }
                    ImGui::SameLine();

                    // Slider (fill remaining space minus kp/kd fields) - immediate apply while dragging
                    float kp_kd_width = 60 + 50 + ImGui::GetStyle().ItemSpacing.x * 2;
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - kp_kd_width);
                    if (ImGui::SliderFloat(slider_label, target_val, pos_min, pos_max, "%.3f", ImGuiSliderFlags_AlwaysClamp)) {
                        // Slider value changed - use normal duration instead of reset duration
                        g_reset_mode_active = false;
                    }

                    // kp input (with confirmation popup)
                    ImGui::SameLine();
                    char kp_label[32];
                    snprintf(kp_label, sizeof(kp_label), "##kp%d", i);
                    ImGui::SetNextItemWidth(60);
                    ImGui::InputFloat(kp_label, &g_kp_edit[i], 0, 0, "%.1f");
                    if (ImGui::IsItemDeactivatedAfterEdit() && g_kp_edit[i] != g_kp[i]) {
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
                    ImGui::SetNextItemWidth(50);
                    ImGui::InputFloat(kd_label, &g_kd_edit[i], 0, 0, "%.2f");
                    if (ImGui::IsItemDeactivatedAfterEdit() && g_kd_edit[i] != g_kd[i]) {
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
                        if (g_show_motor == 1) {
                            g_slider_values[i] = g_latest_lowstate.motor_state()[i].q();
                        } else {
                            g_slider_values[i] = g_latest_lowstate.joint_state()[i].q();
                        }
                    }
                    // Use appropriate limits based on current mode
                    float pos_min = (g_show_motor == 1) ? MOTOR_POS_MIN[i] : JOINT_POS_MIN[i];
                    float pos_max = (g_show_motor == 1) ? MOTOR_POS_MAX[i] : JOINT_POS_MAX[i];

                    // Match layout with LOW_LEVEL mode
                    ImGui::SetNextItemWidth(180);
                    ImGui::Text("%s", label);
                    ImGui::SameLine(190);

                    // Slider (fill remaining space minus kp/kd fields)
                    float kp_kd_width = 60 + 50 + ImGui::GetStyle().ItemSpacing.x * 2;
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - kp_kd_width);
                    ImGui::SliderFloat(slider_label, &g_slider_values[i], pos_min, pos_max, "%.3f");

                    // kp input (with confirmation popup)
                    ImGui::SameLine();
                    char kp_label[32];
                    snprintf(kp_label, sizeof(kp_label), "##kp%d", i);
                    ImGui::SetNextItemWidth(60);
                    ImGui::InputFloat(kp_label, &g_kp_edit[i], 0, 0, "%.1f");
                    if (ImGui::IsItemDeactivatedAfterEdit() && g_kp_edit[i] != g_kp[i]) {
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
                    ImGui::SetNextItemWidth(50);
                    ImGui::InputFloat(kd_label, &g_kd_edit[i], 0, 0, "%.2f");
                    if (ImGui::IsItemDeactivatedAfterEdit() && g_kd_edit[i] != g_kd[i]) {
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
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Center panel: IMU State
        ImGui::BeginChild("CenterPanel", ImVec2(450, 0), true);
        {
            // DDS Connection Status (top of center panel)
            ImGui::Text("DDS Connection Status");
            ImGui::Separator();

            // Check connection timeout (2 seconds without message = disconnected)
            auto now          = std::chrono::steady_clock::now();
            auto elapsed      = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_last_controlmodestate_time).count();
            bool is_connected = g_controlmodestate_connected && (elapsed < 2000);

            if (is_connected) {
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "Status: Connected");
                {
                    std::lock_guard<std::mutex> lock(g_controlmodestate_mutex);
                    ImGui::Text("Control Mode: %s", ControlModeToString(g_latest_controlmodestate.mode()));
                }
            } else {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Status: Disconnected");
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Control Mode: --");
            }

            ImGui::Separator();
            ImGui::Separator();

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

                // Tab bar for LowCmd and Cmd vs State comparison
                if (ImGui::BeginTabBar("CmdStateTabBar")) {
                    // Tab 1: LowCmd only
                    if (ImGui::BeginTabItem("LowCmd")) {
                        std::lock_guard<std::mutex> lock(g_last_cmd_mutex);
                        ImGui::BeginChild("CmdScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
                        for (int i = 0; i < 31; i++) {
                            const auto &cmd = g_last_published_cmd.motors()[i];
                            ImGui::Text("J%2d: q=%7.3f  tau=%6.2f  kp=%5.1f  kd=%4.2f", i, cmd.q(), cmd.tau(), cmd.kp(), cmd.kd());
                        }
                        ImGui::EndChild();
                        ImGui::EndTabItem();
                    }

                    // Tab 2: Cmd vs State comparison
                    if (ImGui::BeginTabItem("Cmd vs State")) {
                        std::lock_guard<std::mutex> lock_cmd(g_last_cmd_mutex);
                        std::lock_guard<std::mutex> lock_state(g_lowstate_mutex);
                        ImGui::BeginChild("CmdStateScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
                        for (int i = 0; i < 31; i++) {
                            float cmd_q   = g_last_published_cmd.motors()[i].q();
                            float state_q = g_latest_lowstate.joint_state()[i].q();
                            float diff    = cmd_q - state_q;
                            // Color red if difference > 0.05 rad
                            if (std::abs(diff) > 0.05f) {
                                ImGui::TextColored(ImVec4(1, 0.3f, 0.3f, 1), "J%2d: cmd=%7.3f : state=%7.3f  (diff=%+.3f)", i, cmd_q,
                                                   state_q, diff);
                            } else {
                                ImGui::Text("J%2d: cmd=%7.3f : state=%7.3f  (diff=%+.3f)", i, cmd_q, state_q, diff);
                            }
                        }
                        ImGui::EndChild();
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

            // LowLevel + Start button: Switch to LOW_LEVEL mode and start LowCmd publishing
            if (ImGui::Button("   LowLevel + Start", ImVec2(-1, 40))) {
                if (g_service_call_in_progress.load()) {
                    AddLog("Service call already in progress, please wait...");
                } else if (!g_first_state_received) {
                    AddLog("Waiting for first state - cannot start yet");
                } else {
                    g_service_call_in_progress = true;
                    AddLog("Calling SetControlMode(LOW_LEVEL) + StartLowCmd...");

                    std::thread([&client]() {
                        auto res = client.SetControlMode(ControlMode::CONTROL_MODE_LOW_LEVEL, 60000);
                        if (res.success()) {
                            AddLog("SetControlMode(LOW_LEVEL): SUCCESS - " + std::string(res.message()));
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
                        } else {
                            AddLog("SetControlMode(LOW_LEVEL): FAILED - " + std::string(res.message()));
                        }
                        g_service_call_in_progress = false;
                    }).detach();
                }
            }

            // LowCmd Publish Start/Stop buttons (only enabled in LOW_LEVEL mode)
            {
                bool is_lowlevel_mode = false;
                {
                    std::lock_guard<std::mutex> lock(g_controlmodestate_mutex);
                    is_lowlevel_mode = (g_latest_controlmodestate.mode() == ControlMode::CONTROL_MODE_LOW_LEVEL);
                }

                if (!g_lowlevel_active) {
                    if (!is_lowlevel_mode) {
                        ImGui::BeginDisabled();
                    }
                    if (ImGui::Button("   LowCmd Publish Start", ImVec2(-1, 40))) {
                        StartLowCmdPublishing();
                    }
                    if (!is_lowlevel_mode) {
                        ImGui::EndDisabled();
                    }
                } else {
                    if (ImGui::Button("   LowCmd Publish Stop", ImVec2(-1, 40))) {
                        StopLowCmdPublishing();
                    }
                }
            }

            if (ImGui::Button("8. Control Mode: HIGH_LEVEL", ImVec2(-1, 40))) {
                CallSetControlModeAsync(&client, ControlMode::CONTROL_MODE_HIGH_LEVEL, "HIGH_LEVEL");
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

        // Kp/Kd change confirmation popup
        if (g_show_gain_confirm_popup) {
            ImGui::OpenPopup("Confirm Gain Change");
            g_show_gain_confirm_popup = false;
        }

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("Confirm Gain Change", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            const char *gain_name  = g_pending_is_kp ? "Kp" : "Kd";
            const char *joint_name = (g_show_motor == 1) ? MOTOR_NAMES[g_pending_gain_joint] : JOINT_NAMES[g_pending_gain_joint];

            ImGui::Text("Change %s gain for Joint %d (%s)?", gain_name, g_pending_gain_joint, joint_name);
            ImGui::Separator();
            ImGui::Text("Current: %.2f", g_pending_old_value);
            ImGui::Text("New:     %.2f", g_pending_new_value);
            ImGui::Separator();

            if (ImGui::Button("Apply", ImVec2(120, 0))) {
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
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
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
