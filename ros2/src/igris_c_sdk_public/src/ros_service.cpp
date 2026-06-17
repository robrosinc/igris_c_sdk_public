#include "common.hpp"

#include <igris_c_sdk/srv/bms_init_cmd.hpp>
#include <igris_c_sdk/srv/control_mode_command_request.hpp>
#include <igris_c_sdk/srv/hand_init_request.hpp>
#include <igris_c_sdk/srv/mujoco_sim_cmd.hpp>
#include <igris_c_sdk/srv/torque_cmd.hpp>
#include <iostream>
#include <limits>
#include <rclcpp/rclcpp.hpp>

using igris_c_sdk_examples::g_running;
using igris_c_sdk_examples::signalHandler;

using BmsInitCmd                = igris_c_sdk::srv::BmsInitCmd;
using TorqueCmd                 = igris_c_sdk::srv::TorqueCmd;
using HandInitRequest           = igris_c_sdk::srv::HandInitRequest;
using ControlModeCommandRequest = igris_c_sdk::srv::ControlModeCommandRequest;
using MujocoSimCmd              = igris_c_sdk::srv::MujocoSimCmd;

static void PrintMenu() {
    std::cout << "\n=== Service API Menu ===" << std::endl;
    std::cout << "[BMS / Motor]" << std::endl;
    std::cout << "  1. Init BMS" << std::endl;
    std::cout << "  2. Init Motor" << std::endl;
    std::cout << "  3. Init BMS and Motor" << std::endl;
    std::cout << "  4. BMS OFF" << std::endl;
    std::cout << "[Torque]" << std::endl;
    std::cout << "  5. Torque ON" << std::endl;
    std::cout << "  6. Torque OFF" << std::endl;
    std::cout << "[Hand]" << std::endl;
    std::cout << "  7. Init Hand" << std::endl;
    std::cout << "[Control Mode]" << std::endl;
    std::cout << " 10. Motion Preset: HOME" << std::endl;
    std::cout << " 11. Motion Preset: HI_WAVE" << std::endl;
    std::cout << " 12. Joint Position Hold" << std::endl;
    std::cout << " 13. Motion Stop" << std::endl;
    std::cout << " 14. Walkmode ON" << std::endl;
    std::cout << " 15. Low Level Joint Control" << std::endl;
    std::cout << " 16. Low Level Walkmode ON" << std::endl;
    std::cout << " 17. High Level Joint Control" << std::endl;
    std::cout << " 18. High Level Walkmode ON" << std::endl;
    std::cout << " 19. Custom Mode 1" << std::endl;
    std::cout << " 20. Custom Mode 2" << std::endl;
    std::cout << " 21. Custom Mode 3" << std::endl;
    std::cout << " 22. Custom Mode 4" << std::endl;
    std::cout << " 23. Custom Mode 5" << std::endl;
    std::cout << " 24. Custom Mode 6" << std::endl;
    std::cout << " 25. Custom Mode 7" << std::endl;
    std::cout << " 26. Custom Mode 8" << std::endl;
    std::cout << " 27. Custom Mode 9" << std::endl;
    std::cout << " 28. Custom Mode 10" << std::endl;
    std::cout << "[Mujoco Sim (sim-only)]" << std::endl;
    std::cout << " 30. Mujoco Sim Pause" << std::endl;
    std::cout << " 31. Mujoco Sim Resume" << std::endl;
    std::cout << " 32. Mujoco Sim Reload" << std::endl;
    std::cout << " 33. Mujoco Sim Reset" << std::endl;
    std::cout << "  0. Exit" << std::endl;
    std::cout << "\nSelect: ";
}

template <typename ResponseT> static void PrintResult(const std::string &action, const std::shared_ptr<ResponseT> &res) {
    std::cout << "\n[" << action << "] " << (res->success ? "SUCCESS" : "FAILED") << " - " << res->message << std::endl;
}

template <typename SrvT>
static typename SrvT::Response::SharedPtr CallService(rclcpp::Node::SharedPtr node, typename rclcpp::Client<SrvT>::SharedPtr client,
                                                      typename SrvT::Request::SharedPtr req, const std::string &service_label) {
    if (!client->wait_for_service(std::chrono::seconds(3))) {
        std::cerr << service_label << " service not available" << std::endl;
        return nullptr;
    }
    auto future = client->async_send_request(req);
    rclcpp::executors::SingleThreadedExecutor exec;
    exec.add_node(node);
    if (exec.spin_until_future_complete(future, std::chrono::seconds(60)) != rclcpp::FutureReturnCode::SUCCESS) {
        std::cerr << service_label << " request timeout" << std::endl;
        return nullptr;
    }
    return future.get();
}

static void CallBms(rclcpp::Node::SharedPtr node, rclcpp::Client<BmsInitCmd>::SharedPtr client, uint8_t init, const std::string &label) {
    auto req        = std::make_shared<BmsInitCmd::Request>();
    req->request_id = igris_c_sdk_examples::makeRequestId("bms");
    req->init       = init;
    auto res        = CallService<BmsInitCmd>(node, client, req, "BMS init");
    if (res) {
        PrintResult(label, res);
    }
}

static void CallTorque(rclcpp::Node::SharedPtr node, rclcpp::Client<TorqueCmd>::SharedPtr client, uint8_t torque,
                       const std::string &label) {
    auto req        = std::make_shared<TorqueCmd::Request>();
    req->request_id = igris_c_sdk_examples::makeRequestId("torque");
    req->torque     = torque;
    auto res        = CallService<TorqueCmd>(node, client, req, "Torque");
    if (res) {
        PrintResult(label, res);
    }
}

static void CallHandInit(rclcpp::Node::SharedPtr node, rclcpp::Client<HandInitRequest>::SharedPtr client) {
    auto req        = std::make_shared<HandInitRequest::Request>();
    req->request_id = igris_c_sdk_examples::makeRequestId("hand_init");
    auto res        = CallService<HandInitRequest>(node, client, req, "Hand init");
    if (res) {
        PrintResult("Init Hand", res);
    }
}

static void CallControlMode(rclcpp::Node::SharedPtr node, rclcpp::Client<ControlModeCommandRequest>::SharedPtr client, uint8_t command_type,
                            const std::string &preset_id, bool is_cyclic, const std::string &label) {
    auto req          = std::make_shared<ControlModeCommandRequest::Request>();
    req->request_id   = igris_c_sdk_examples::makeRequestId("control_mode");
    req->command_type = command_type;
    req->preset_id    = preset_id;
    req->is_cyclic    = is_cyclic;
    auto res          = CallService<ControlModeCommandRequest>(node, client, req, "Control mode");
    if (res) {
        PrintResult(label, res);
    }
}

static void CallMujocoSim(rclcpp::Node::SharedPtr node, rclcpp::Client<MujocoSimCmd>::SharedPtr client, uint8_t command_type,
                          const std::string &label) {
    auto req          = std::make_shared<MujocoSimCmd::Request>();
    req->request_id   = igris_c_sdk_examples::makeRequestId("mujoco_sim");
    req->command_type = command_type;
    auto res          = CallService<MujocoSimCmd>(node, client, req, "Mujoco Sim");
    if (res) {
        PrintResult(label, res);
    }
}

int main(int argc, char **argv) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    igris_c_sdk_examples::initRclcpp(argc, argv);
    auto node                         = rclcpp::Node::make_shared("igris_c_ros_service");
    const std::string robot_namespace = igris_c_sdk_examples::resolveRobotNamespace(node);

    auto bms_client    = node->create_client<BmsInitCmd>(igris_c_sdk_examples::resolveRtName(robot_namespace, "rt/service/bms_init"));
    auto torque_client = node->create_client<TorqueCmd>(igris_c_sdk_examples::resolveRtName(robot_namespace, "rt/service/torque"));
    auto hand_client   = node->create_client<HandInitRequest>(igris_c_sdk_examples::resolveRtName(robot_namespace, "rt/service/hand_init"));
    auto cmode_client =
        node->create_client<ControlModeCommandRequest>(igris_c_sdk_examples::resolveRtName(robot_namespace, "rt/service/control_mode"));
    auto mujoco_client = node->create_client<MujocoSimCmd>(igris_c_sdk_examples::resolveRtName(robot_namespace, "rt/service/mujoco_sim"));

    std::cout << "Service client initialized (namespace: \"" << robot_namespace << "\")" << std::endl;

    int choice = 0;
    while (rclcpp::ok() && g_running.load(std::memory_order_relaxed)) {
        PrintMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input" << std::endl;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        using CMReq = ControlModeCommandRequest::Request;

        switch (choice) {
        case 1:
            CallBms(node, bms_client, BmsInitCmd::Request::BMS_INIT, "Init BMS");
            break;
        case 2:
            CallBms(node, bms_client, BmsInitCmd::Request::MOTOR_INIT, "Init Motor");
            break;
        case 3:
            CallBms(node, bms_client, BmsInitCmd::Request::BMS_AND_MOTOR_INIT, "Init BMS and Motor");
            break;
        case 4:
            CallBms(node, bms_client, BmsInitCmd::Request::BMS_INIT_NONE, "BMS OFF");
            break;

        case 5:
            CallTorque(node, torque_client, TorqueCmd::Request::TORQUE_ON, "Torque ON");
            break;
        case 6:
            CallTorque(node, torque_client, TorqueCmd::Request::TORQUE_OFF, "Torque OFF");
            break;

        case 7:
            CallHandInit(node, hand_client);
            break;

        case 10:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_MOTION_PRESET, "HOME", false, "Motion Preset: HOME");
            break;
        case 11:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_MOTION_PRESET, "HI_WAVE", false, "Motion Preset: HI_WAVE");
            break;
        case 12:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_JOINT_POSITION_HOLD, "", false, "Joint Position Hold");
            break;
        case 13:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_MOTION_STOP, "", false, "Motion Stop");
            break;
        case 14:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_WALKMODE_ON, "", false, "Walkmode ON");
            break;
        case 15:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_LOW_LEVEL_JOINT_CONTROL, "", false, "Low Level Joint Control");
            break;
        case 16:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_LOW_LEVEL_WALKMODE_ON, "", false, "Low Level Walkmode ON");
            break;
        case 17:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_HIGH_LEVEL_JOINT_CONTROL, "", false, "High Level Joint Control");
            break;
        case 18:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_HIGH_LEVEL_WALKMODE_ON, "", false, "High Level Walkmode ON");
            break;
        case 19:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_CUSTOM_MODE_1, "", false, "Custom Mode 1");
            break;
        case 20:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_CUSTOM_MODE_2, "", false, "Custom Mode 2");
            break;
        case 21:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_CUSTOM_MODE_3, "", false, "Custom Mode 3");
            break;
        case 22:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_CUSTOM_MODE_4, "", false, "Custom Mode 4");
            break;
        case 23:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_CUSTOM_MODE_5, "", false, "Custom Mode 5");
            break;
        case 24:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_CUSTOM_MODE_6, "", false, "Custom Mode 6");
            break;
        case 25:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_CUSTOM_MODE_7, "", false, "Custom Mode 7");
            break;
        case 26:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_CUSTOM_MODE_8, "", false, "Custom Mode 8");
            break;
        case 27:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_CUSTOM_MODE_9, "", false, "Custom Mode 9");
            break;
        case 28:
            CallControlMode(node, cmode_client, CMReq::CONTROL_MODE_CMD_CUSTOM_MODE_10, "", false, "Custom Mode 10");
            break;

        case 30:
            CallMujocoSim(node, mujoco_client, MujocoSimCmd::Request::MUJOCO_SIM_PAUSE, "Mujoco Sim Pause");
            break;
        case 31:
            CallMujocoSim(node, mujoco_client, MujocoSimCmd::Request::MUJOCO_SIM_RESUME, "Mujoco Sim Resume");
            break;
        case 32:
            CallMujocoSim(node, mujoco_client, MujocoSimCmd::Request::MUJOCO_SIM_RELOAD, "Mujoco Sim Reload");
            break;
        case 33:
            CallMujocoSim(node, mujoco_client, MujocoSimCmd::Request::MUJOCO_SIM_RESET, "Mujoco Sim Reset");
            break;

        case 0:
            std::cout << "Exiting..." << std::endl;
            rclcpp::shutdown();
            return 0;

        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }

    rclcpp::shutdown();
    return 0;
}
