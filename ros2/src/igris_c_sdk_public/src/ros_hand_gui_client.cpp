/**
 * @file ros_hand_gui_client.cpp
 * @brief ROS2 hand GUI client, porting cyclonedds_hand_gui_client.cpp
 *
 * ROS2 Topics/Services:
 * - handcmd (HandCmd) - Publisher for motor commands
 * - handstate (HandState) - Subscriber for motor states
 * - service/hand_init (HandInitRequest) - Service client for hand init trigger
 */

#include "common.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <csignal>
#include <cstdio>
#include <ctime>
#include <deque>
#include <igris_c_sdk/msg/hand_cmd.hpp>
#include <igris_c_sdk/msg/hand_state.hpp>
#include <igris_c_sdk/srv/hand_init_request.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <mutex>
#include <rclcpp/rclcpp.hpp>
#include <thread>

using HandCmd     = igris_c_sdk::msg::HandCmd;
using HandState   = igris_c_sdk::msg::HandState;
using HandInitSrv = igris_c_sdk::srv::HandInitRequest;
using igris_c_sdk_examples::g_running;
using igris_c_sdk_examples::makeRequestId;
using igris_c_sdk_examples::resolveRobotNamespace;
using igris_c_sdk_examples::resolveRosTopic;
using igris_c_sdk_examples::signalHandler;

// Hand motor IDs (matches dxl_hand_controller order)
static const std::array<uint16_t, 12> HAND_MOTOR_IDS = {11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 25, 26};

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

static std::atomic<uint32_t> g_handstate_received_count(0);
static HandState g_latest_handstate;
static std::mutex g_handstate_mutex;
static bool g_first_state_received = false;

static std::array<float, 12> g_target_positions = {};

static std::atomic<bool> g_auto_publish(true);
static std::atomic<uint32_t> g_handcmd_publish_count(0);

static std::atomic<bool> g_initializing(false);

static std::deque<std::string> g_response_log;
static std::mutex g_log_mutex;
static const size_t MAX_LOG_LINES = 50;

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

void HandStateCallback(const HandState &state) {
    std::lock_guard<std::mutex> lock(g_handstate_mutex);
    g_latest_handstate = state;
    g_handstate_received_count++;

    if (!g_first_state_received) {
        g_first_state_received = true;
        AddLog("First HandState received");
    }
}

void SendHandCmd(rclcpp::Publisher<HandCmd>::SharedPtr publisher) {
    HandCmd cmd;

    for (int i = 0; i < 12; i++) {
        auto &motor = cmd.motor_cmd[i];
        motor.id    = HAND_MOTOR_IDS[i];
        motor.q     = g_target_positions[i];
        motor.dq    = 0.0f;
        motor.tau   = 0.0f;
        motor.kp    = 0.0f;
        motor.kd    = 0.0f;
    }

    publisher->publish(cmd);
    g_handcmd_publish_count++;
}

void AutoPublishThread(rclcpp::Publisher<HandCmd>::SharedPtr publisher) {
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

void SendHandInitRequest(rclcpp::Client<HandInitSrv>::SharedPtr client) {
    if (g_initializing.load()) {
        AddLog("HandInit already in progress");
        return;
    }

    g_auto_publish = false;
    g_initializing = true;
    AddLog("Stopped publishing for init...");

    // Small delay to ensure no commands in flight
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::thread([client]() {
        if (!client->wait_for_service(std::chrono::seconds(3))) {
            AddLog("HandInit: FAILED - service not available");
            g_initializing = false;
            g_auto_publish = true;
            return;
        }

        auto req        = std::make_shared<HandInitSrv::Request>();
        req->request_id = makeRequestId("gui_hand_init");

        auto future         = client->async_send_request(req);
        const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(60);
        while (rclcpp::ok() && std::chrono::steady_clock::now() < deadline) {
            if (future.wait_for(std::chrono::milliseconds(20)) == std::future_status::ready) {
                auto res = future.get();
                AddLog(std::string("HandInit response: ") + (res->success ? "OK" : "FAIL") + ": " + res->message);
                AddLog("Wait for init to complete, then click 'Resume Publishing'");
                return;
            }
        }
        AddLog("HandInit: FAILED - request timeout");
        g_initializing = false;
        g_auto_publish = true;
    }).detach();
}

int main(int argc, char **argv) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    igris_c_sdk_examples::initRclcpp(argc, argv);
    auto node = rclcpp::Node::make_shared("igris_c_ros_hand_gui_client");

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    GLFWwindow *window = glfwCreateWindow(initial_width, initial_height, "IGRIS-C ROS2 Hand GUI Client", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        rclcpp::shutdown();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    ImGuiStyle base_style  = ImGui::GetStyle();
    float applied_ui_scale = 1.0f;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    const std::string robot_namespace = resolveRobotNamespace(node);
    igris_c_sdk_examples::printBanner("Hand GUI Client", robot_namespace);
    std::cout << "Make sure the hand controller node is running!\n" << std::endl;

    auto handstate_sub = node->create_subscription<HandState>(resolveRosTopic(robot_namespace, "handstate"), rclcpp::SensorDataQoS(),
                                                              [](const HandState &state) { HandStateCallback(state); });

    auto handcmd_pub = node->create_publisher<HandCmd>(resolveRosTopic(robot_namespace, "handcmd"), rclcpp::SensorDataQoS());

    auto hand_init_client = node->create_client<HandInitSrv>(resolveRosTopic(robot_namespace, "service/hand_init"));

    std::thread auto_thread(AutoPublishThread, handcmd_pub);

    AddLog("Hand GUI Client initialized successfully");
    std::cout << "Hand GUI Client ready!\n" << std::endl;

    while (!glfwWindowShouldClose(window) && g_running && rclcpp::ok()) {
        rclcpp::spin_some(node);

        glfwPollEvents();

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

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("IGRIS-C ROS2 Hand Control", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImGui::BeginChild("LeftPanel", ImVec2(left_panel_width, 0), true);
        {
            ImGui::Text("Hand Motor Targets (Normalized 0.0 ~ 1.0)");
            ImGui::Separator();

            ImGui::Text("HandState messages: %u", g_handstate_received_count.load());
            if (!g_first_state_received) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Waiting for state...");
            } else {
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "Connected");
            }
            ImGui::Separator();

            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.5f, 1.0f), "Right Hand (ID 11-16)");
            for (int i = 0; i < 6; i++) {
                char label[64];
                snprintf(label, sizeof(label), "ID %d: %s", HAND_MOTOR_IDS[i], HAND_MOTOR_NAMES[i]);
                ImGui::SliderFloat(label, &g_target_positions[i], 0.0f, 1.0f, "%.2f");
            }

            ImGui::Separator();

            ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "Left Hand (ID 21-26)");
            for (int i = 6; i < 12; i++) {
                char label[64];
                snprintf(label, sizeof(label), "ID %d: %s", HAND_MOTOR_IDS[i], HAND_MOTOR_NAMES[i]);
                ImGui::SliderFloat(label, &g_target_positions[i], 0.0f, 1.0f, "%.2f");
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
        {
            ImGui::Text("Commands");
            ImGui::Separator();

            if (ImGui::Button("Reset All (0.0)", ImVec2(-1, preset_button_height))) {
                SetHomePose();
            }

            if (ImGui::Button("Closed Pose (0.6)", ImVec2(-1, preset_button_height))) {
                SetClosedPose();
            }

            ImGui::Separator();

            // Hand init button. Snapshot g_initializing once so the BeginDisabled/
            // EndDisabled guards stay balanced within the frame — the button click
            // (SendHandInitRequest) sets g_initializing=true mid-frame, and it is also
            // written by other threads, so reading it twice could call EndDisabled
            // without a matching BeginDisabled (ImGui assert: DisabledStackSize > 0).
            const bool init_disabled = g_initializing;
            if (init_disabled) {
                ImGui::BeginDisabled();
            }
            if (ImGui::Button("Initialize Hand (Calibrate)", ImVec2(-1, init_button_height))) {
                SendHandInitRequest(hand_init_client);
            }
            if (init_disabled) {
                ImGui::EndDisabled();
            }

            if (g_initializing) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Publishing PAUSED for init");
                if (ImGui::Button("Resume Publishing", ImVec2(-1, preset_button_height))) {
                    g_initializing = false;
                    g_auto_publish = true;
                    AddLog("Publishing resumed");
                }
            }

            ImGui::Separator();

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

            ImGui::Text("Motor State (from HandState)");
            ImGui::Separator();

            if (g_first_state_received) {
                std::lock_guard<std::mutex> lock(g_handstate_mutex);
                const auto &states = g_latest_handstate.motor_state;

                ImGui::BeginChild("StateScroll", ImVec2(0, motor_state_box_height), false, ImGuiWindowFlags_HorizontalScrollbar);
                for (size_t i = 0; i < states.size() && i < 12; i++) {
                    const auto &s = states[i];
                    ImGui::Text("M%2zu: pos=%.3f vel=%.3f cur=%.2f temp=%d err=0x%X", i, s.q, s.dq, s.tau_est, s.temperature,
                                s.status_bits);
                }
                ImGui::EndChild();
            } else {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Waiting for motor state...");
            }

            ImGui::Separator();

            ImGui::Text("Log");
            ImGui::Separator();

            ImGui::BeginChild("LogScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            {
                std::lock_guard<std::mutex> lock(g_log_mutex);
                for (const auto &line : g_response_log) {
                    ImGui::TextUnformatted(line.c_str());
                }
            }
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
        }
        ImGui::EndChild();

        ImGui::End();

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

    std::cout << "\nShutting down Hand GUI..." << std::endl;

    if (auto_thread.joinable()) {
        auto_thread.join();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    rclcpp::shutdown();

    std::cout << "Hand GUI Client terminated" << std::endl;
    return 0;
}
