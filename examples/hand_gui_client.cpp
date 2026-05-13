/**
 * @file hand_gui_client.cpp
 * @brief Hand GUI client example for IGRIS SDK
 *
 * This example demonstrates:
 * - HandState subscription (real-time hand motor state monitoring)
 * - HandCmd publishing (hand motor position control)
 *
 * GUI Layout:
 * - Left panel: 12 hand motor sliders (normalized 0.0~1.0)
 * - Right panel: Status display, command buttons, response log
 *
 * DDS Topics:
 * - rt/handcmd (HandCmd) - Publisher for motor commands
 * - rt/handstate (HandState) - Subscriber for motor states
 * - rt/service/hand_init/request (HandInitRequest) - Publisher for hand init trigger
 * - rt/service/hand_init/response (ServiceResponse) - Subscriber for hand init result
 */

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <csignal>
#include <deque>
#include <igris_c_sdk/channel_factory.hpp>
#include <igris_c_sdk/namespace_resolver.hpp>
#include <igris_c_sdk/publisher.hpp>
#include <igris_c_sdk/subscriber.hpp>
#include <igris_c_sdk/types.hpp>
#include <iostream>
#include <mutex>
#include <thread>

// ImGui includes
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace igris_c_sdk;

// Hand motor IDs (matches dxl_hand_controller order)
static const std::array<uint16_t, 12> HAND_MOTOR_IDS = {11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 25, 26};

// Motor names for display
static const std::array<const char *, 12> HAND_MOTOR_NAMES = {
    "R_Thumb",   // ID 11
    "R_Index",   // ID 12
    "R_Middle",  // ID 13
    "R_Ring",    // ID 14
    "R_Pinky",   // ID 15
    "R_Spread",  // ID 16
    "L_Thumb",   // ID 21
    "L_Index",   // ID 22
    "L_Middle",  // ID 23
    "L_Ring",    // ID 24
    "L_Pinky",   // ID 25
    "L_Spread"   // ID 26
};

static constexpr float BASE_WINDOW_WIDTH  = 1200.0f;
static constexpr float BASE_WINDOW_HEIGHT = 700.0f;

float ClampUiScale(float scale) { return std::clamp(scale, 1.0f, 2.5f); }

float ComputeUiScale(float width, float height) { return ClampUiScale(std::min(width / BASE_WINDOW_WIDTH, height / BASE_WINDOW_HEIGHT)); }

// Global state
static std::atomic<bool> g_running(true);
static std::atomic<uint32_t> g_handstate_received_count(0);

// Latest state data
static HandState g_latest_handstate;
static std::mutex g_handstate_mutex;
static bool g_first_state_received = false;

// GUI state - target positions (normalized 0.0~1.0)
static std::array<float, 12> g_target_positions = {};

// Auto publish state
static std::atomic<bool> g_auto_publish(true);
static std::atomic<uint32_t> g_handcmd_publish_count(0);

// Init state
static std::atomic<bool> g_initializing(false);

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

// HandState callback
void HandStateCallback(const HandState &state) {
    std::lock_guard<std::mutex> lock(g_handstate_mutex);
    g_latest_handstate = state;
    g_handstate_received_count++;

    if (!g_first_state_received) {
        g_first_state_received = true;
        AddLog("First HandState received");
    }
}

// Send HandCmd
void SendHandCmd(Publisher<HandCmd> *publisher) {
    HandCmd cmd;
    cmd.motor_cmd().resize(12);

    for (int i = 0; i < 12; i++) {
        auto &motor = cmd.motor_cmd()[i];
        motor.id(HAND_MOTOR_IDS[i]);
        motor.q(g_target_positions[i]);
        motor.dq(0.0f);
        motor.tau(0.0f);
        motor.kp(0.0f);
        motor.kd(0.0f);
    }

    if (publisher->write(cmd)) {
        g_handcmd_publish_count++;
    }
}

// Auto publish thread (20Hz)
void AutoPublishThread(Publisher<HandCmd> *publisher) {
    const auto period = std::chrono::milliseconds(50);  // 20Hz
    auto next_time    = std::chrono::steady_clock::now();

    while (g_running) {
        if (g_auto_publish) {
            SendHandCmd(publisher);
        }

        next_time += period;
        std::this_thread::sleep_until(next_time);
    }
}

// Preset functions
void SetHomePose() {
    for (int i = 0; i < 12; i++) {
        g_target_positions[i] = 0.0f;
    }
    AddLog("Home pose set (all 0.0)");
}

void SetClosedPose() {
    for (int i = 0; i < 12; i++) {
        g_target_positions[i] = 0.6f;
    }
    AddLog("Closed pose set (all 0.6)");
}

void SetOpenPose() {
    for (int i = 0; i < 12; i++) {
        g_target_positions[i] = 0.0f;
    }
    AddLog("Open pose set (all 0.0)");
}

void SetRightHandOnly(float value) {
    for (int i = 0; i < 6; i++) {
        g_target_positions[i] = value;
    }
    AddLog("Right hand set to " + std::to_string(value));
}

void SetLeftHandOnly(float value) {
    for (int i = 6; i < 12; i++) {
        g_target_positions[i] = value;
    }
    AddLog("Left hand set to " + std::to_string(value));
}

// HandInit response callback
void HandInitResponseCallback(const ServiceResponse &res) {
    std::string result = res.success() ? "OK" : "FAIL";
    AddLog("HandInit response [" + res.request_id() + "] " + result + ": " + res.message());
}

// Send HandInit service request
void SendHandInitRequest(Publisher<HandInitRequest> *publisher) {
    // Stop auto publishing during init
    g_auto_publish = false;
    g_initializing = true;
    AddLog("Stopped publishing for init...");

    // Small delay to ensure no commands in flight
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    HandInitRequest req;
    req.request_id() = "gui_hand_init";

    if (publisher->write(req)) {
        AddLog("HandInit request sent");
        AddLog("Wait for init to complete, then click 'Resume Publishing'");
    } else {
        AddLog("Failed to send HandInit request");
        // Resume on failure
        g_initializing = false;
        g_auto_publish = true;
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║    IGRIS-SDK Hand GUI Client          ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝\n" << std::endl;

    // Parse arguments
    int domain_id = 0;
    if (argc > 1) {
        domain_id = std::atoi(argv[1]);
    }

    std::cout << "Domain ID: " << domain_id << std::endl;
    std::cout << "Make sure the hand controller node is running!\n" << std::endl;

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

    // Create window (use monitor workarea for better initial size)
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
    GLFWwindow *window = glfwCreateWindow(initial_width, initial_height, "IGRIS-SDK Hand GUI Client", NULL, NULL);
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
    ImGuiStyle base_style  = ImGui::GetStyle();
    float applied_ui_scale = 1.0f;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Initialize ChannelFactory
    const std::string robot_namespace = igris_c_sdk::resolve_robot_namespace();
    std::cout << "Initializing ChannelFactory (namespace=\"" << robot_namespace << "\")..." << std::endl;
    ChannelFactory::Instance()->Init(domain_id, robot_namespace);

    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    // Create HandState subscriber
    std::cout << "Initializing HandState subscriber..." << std::endl;
    Subscriber<HandState> handstate_sub("rt/handstate");
    if (!handstate_sub.init(HandStateCallback)) {
        std::cerr << "Failed to initialize HandState subscriber" << std::endl;
        return 1;
    }

    // Create HandCmd publisher
    std::cout << "Initializing HandCmd publisher..." << std::endl;
    Publisher<HandCmd> handcmd_pub("rt/handcmd");
    if (!handcmd_pub.init()) {
        std::cerr << "Failed to initialize HandCmd publisher" << std::endl;
        return 1;
    }

    // Create HandInit request publisher and response subscriber
    std::cout << "Initializing HandInit service publisher/subscriber..." << std::endl;
    Publisher<HandInitRequest> hand_init_req_pub("rt/service/hand_init/request");
    if (!hand_init_req_pub.init()) {
        std::cerr << "Failed to initialize HandInit request publisher" << std::endl;
        return 1;
    }
    Subscriber<ServiceResponse> hand_init_res_sub("rt/service/hand_init/response");
    if (!hand_init_res_sub.init(HandInitResponseCallback)) {
        std::cerr << "Failed to initialize HandInit response subscriber" << std::endl;
        return 1;
    }

    // Start auto publish thread
    std::thread auto_thread(AutoPublishThread, &handcmd_pub);

    std::cout << "Hand GUI Client ready!\n" << std::endl;
    AddLog("Hand GUI Client initialized successfully");

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

        const float left_panel_width       = io.DisplaySize.x * (500.0f / BASE_WINDOW_WIDTH);
        const float preset_button_height   = 35.0f * ui_scale;
        const float init_button_height     = 40.0f * ui_scale;
        const float small_button_width     = 120.0f * ui_scale;
        const float small_button_height    = 30.0f * ui_scale;
        const float motor_state_box_height = 200.0f * ui_scale;

        // Create main window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("IGRIS-SDK Hand Control", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBringToFrontOnFocus);

        // Left panel: Motor sliders
        ImGui::BeginChild("LeftPanel", ImVec2(left_panel_width, 0), true);
        {
            ImGui::Text("Hand Motor Targets (Normalized 0.0 ~ 1.0)");
            ImGui::Separator();

            // Status
            ImGui::Text("HandState messages: %u", g_handstate_received_count.load());
            if (!g_first_state_received) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Waiting for state...");
            } else {
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "Connected");
            }
            ImGui::Separator();

            // Right hand sliders
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.5f, 1.0f), "Right Hand (ID 11-16)");
            for (int i = 0; i < 6; i++) {
                char label[64];
                snprintf(label, sizeof(label), "ID %d: %s", HAND_MOTOR_IDS[i], HAND_MOTOR_NAMES[i]);
                ImGui::SliderFloat(label, &g_target_positions[i], 0.0f, 1.0f, "%.2f");
            }

            ImGui::Separator();

            // Left hand sliders
            ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "Left Hand (ID 21-26)");
            for (int i = 6; i < 12; i++) {
                char label[64];
                snprintf(label, sizeof(label), "ID %d: %s", HAND_MOTOR_IDS[i], HAND_MOTOR_NAMES[i]);
                ImGui::SliderFloat(label, &g_target_positions[i], 0.0f, 1.0f, "%.2f");
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Right panel: Commands and Status
        ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
        {
            ImGui::Text("Commands");
            ImGui::Separator();

            // Preset buttons
            if (ImGui::Button("Reset All (0.0)", ImVec2(-1, preset_button_height))) {
                SetHomePose();
            }

            if (ImGui::Button("Closed Pose (0.6)", ImVec2(-1, preset_button_height))) {
                SetClosedPose();
            }

            ImGui::Separator();

            // Hand init button
            if (g_initializing) {
                ImGui::BeginDisabled();
            }
            if (ImGui::Button("Initialize Hand (Calibrate)", ImVec2(-1, init_button_height))) {
                SendHandInitRequest(&hand_init_req_pub);
            }
            if (g_initializing) {
                ImGui::EndDisabled();
            }

            // Resume button (shown when initializing)
            if (g_initializing) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Publishing PAUSED for init");
                if (ImGui::Button("Resume Publishing", ImVec2(-1, preset_button_height))) {
                    g_initializing = false;
                    g_auto_publish = true;
                    AddLog("Publishing resumed");
                }
            }

            ImGui::Separator();

            // Hand-specific controls
            ImGui::Text("Left Hand Only:");
            if (ImGui::Button("L: Open##L", ImVec2(small_button_width, small_button_height))) {
                SetLeftHandOnly(0.0f);
            }
            ImGui::SameLine();
            if (ImGui::Button("L: Close##L", ImVec2(small_button_width, small_button_height))) {
                SetLeftHandOnly(0.6f);
            }

            ImGui::Text("Right Hand Only:");
            if (ImGui::Button("R: Open##R", ImVec2(small_button_width, small_button_height))) {
                SetRightHandOnly(0.0f);
            }
            ImGui::SameLine();
            if (ImGui::Button("R: Close##R", ImVec2(small_button_width, small_button_height))) {
                SetRightHandOnly(0.6f);
            }

            ImGui::Separator();

            if (g_initializing) {
                ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Publishing PAUSED | Commands: %u", g_handcmd_publish_count.load());
            } else {
                ImGui::Text("Publishing at 20Hz | Commands: %u", g_handcmd_publish_count.load());
            }

            ImGui::Separator();

            // Motor state display
            ImGui::Text("Motor State (from HandState)");
            ImGui::Separator();

            if (g_first_state_received) {
                std::lock_guard<std::mutex> lock(g_handstate_mutex);
                const auto &states = g_latest_handstate.motor_state();

                ImGui::BeginChild("StateScroll", ImVec2(0, motor_state_box_height), false, ImGuiWindowFlags_HorizontalScrollbar);
                for (size_t i = 0; i < states.size() && i < 12; i++) {
                    const auto &s = states[i];
                    ImGui::Text("M%2zu: pos=%.3f vel=%.3f cur=%.2f temp=%d err=0x%X", i, s.q(), s.dq(), s.tau_est(), s.temperature(),
                                s.status_bits());
                }
                ImGui::EndChild();
            } else {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Waiting for motor state...");
            }

            ImGui::Separator();

            // Response log
            ImGui::Text("Log");
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
    std::cout << "\nShutting down Hand GUI..." << std::endl;
    g_running = false;

    // Wait for auto publish thread to finish
    if (auto_thread.joinable()) {
        auto_thread.join();
    }

    // Stop subscribers/publishers
    handstate_sub.stop();
    handcmd_pub.stop();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "Hand GUI Client terminated" << std::endl;
    return 0;
}
