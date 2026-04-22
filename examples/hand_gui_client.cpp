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
 */

#include <atomic>
#include <chrono>
#include <csignal>
#include <deque>
#include <igris_sdk/channel_factory.hpp>
#include <igris_sdk/publisher.hpp>
#include <igris_sdk/subscriber.hpp>
#include <igris_sdk/types.hpp>
#include <iostream>
#include <mutex>
#include <thread>

// ImGui includes
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace igris_sdk;

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

// Send hand init command (id=99)
void SendHandInitCmd(Publisher<HandCmd> *publisher) {
    // Stop auto publishing during init
    g_auto_publish = false;
    g_initializing = true;
    AddLog("Stopped publishing for init...");

    // Small delay to ensure no commands in flight
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    HandCmd cmd;
    igris_sdk::MotorCmd init_trigger;
    init_trigger.id(99);  // Special ID for init
    init_trigger.q(0.0f);
    init_trigger.dq(0.0f);
    init_trigger.tau(0.0f);
    init_trigger.kp(0.0f);
    init_trigger.kd(0.0f);
    cmd.motor_cmd().push_back(init_trigger);

    if (publisher->write(cmd)) {
        AddLog("Hand init command sent (id=99)");
        AddLog("Wait for init to complete, then click 'Resume Publishing'");
    } else {
        AddLog("Failed to send init command");
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

    // Create window
    GLFWwindow *window = glfwCreateWindow(1200, 700, "IGRIS-SDK Hand GUI Client", NULL, NULL);
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

        // Create main window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("IGRIS-SDK Hand Control", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBringToFrontOnFocus);

        // Left panel: Motor sliders
        ImGui::BeginChild("LeftPanel", ImVec2(500, 0), true);
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
            if (ImGui::Button("Reset All (0.0)", ImVec2(-1, 35))) {
                SetHomePose();
            }

            if (ImGui::Button("Closed Pose (0.6)", ImVec2(-1, 35))) {
                SetClosedPose();
            }

            ImGui::Separator();

            // Hand init button
            if (g_initializing) {
                ImGui::BeginDisabled();
            }
            if (ImGui::Button("Initialize Hand (Calibrate)", ImVec2(-1, 40))) {
                SendHandInitCmd(&handcmd_pub);
            }
            if (g_initializing) {
                ImGui::EndDisabled();
            }

            // Resume button (shown when initializing)
            if (g_initializing) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Publishing PAUSED for init");
                if (ImGui::Button("Resume Publishing", ImVec2(-1, 35))) {
                    g_initializing = false;
                    g_auto_publish = true;
                    AddLog("Publishing resumed");
                }
            }

            ImGui::Separator();

            // Hand-specific controls
            ImGui::Text("Left Hand Only:");
            if (ImGui::Button("L: Open##L", ImVec2(120, 30))) {
                SetLeftHandOnly(0.0f);
            }
            ImGui::SameLine();
            if (ImGui::Button("L: Close##L", ImVec2(120, 30))) {
                SetLeftHandOnly(0.6f);
            }

            ImGui::Text("Right Hand Only:");
            if (ImGui::Button("R: Open##R", ImVec2(120, 30))) {
                SetRightHandOnly(0.0f);
            }
            ImGui::SameLine();
            if (ImGui::Button("R: Close##R", ImVec2(120, 30))) {
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

                ImGui::BeginChild("StateScroll", ImVec2(0, 200), false, ImGuiWindowFlags_HorizontalScrollbar);
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
