/**
 * @file cyclonedds_service.cpp
 * @brief Service API example using IGRIS SDK
 *
 * This example demonstrates:
 * - IgrisC_Client initialization
 * - BMS and motor initialization
 * - Torque control
 * - Hand initialization
 * - Control mode switching (motion preset / walkmode / custom modes)
 * - Mujoco sim control (sim-only)
 *
 * Usage: ./cyclonedds_service <domain_id> <namespace>
 */

#include <igris_c_sdk/channel_factory.hpp>
#include <igris_c_sdk/igris_c_client.hpp>
#include <iostream>
#include <limits>
#include <string>

using namespace igris_c_sdk;
using namespace igris_c::msg::dds;

void PrintMenu() {
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

void PrintResult(const std::string &action, const ServiceResponse &res) {
    std::cout << "\n[" << action << "] " << (res.success() ? "SUCCESS" : "FAILED") << " - " << res.message() << std::endl;
}

int main(int argc, char **argv) {
    std::cout << "=== IGRIS SDK Service Example ===" << std::endl;

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <domain_id> <namespace>" << std::endl;
        std::cerr << "       " << argv[0] << " 0 \"\" (no namespace)" << std::endl;
        return 1;
    }
    const int domain_id  = std::atoi(argv[1]);
    const std::string ns = argv[2];
    std::cout << "Domain ID: " << domain_id << std::endl;
    std::cout << "Namespace: " << ns << std::endl;

    // Initialize SDK
    ChannelFactory::Instance()->Init(domain_id, ns);
    if (!ChannelFactory::Instance()->IsInitialized()) {
        std::cerr << "Failed to initialize ChannelFactory" << std::endl;
        return 1;
    }

    // Initialize service client
    IgrisC_Client client;
    client.Init();
    client.SetTimeout(10.0f);
    std::cout << "Service client initialized (timeout: 10s)" << std::endl;

    // Main loop
    int choice = 0;
    ServiceResponse res;

    while (true) {
        PrintMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input" << std::endl;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            std::cout << "Calling InitBms(BMS_INIT)..." << std::endl;
            res = client.InitBms(BmsInitType::BMS_INIT, 60000);
            PrintResult("Init BMS", res);
            break;

        case 2:
            std::cout << "Calling InitBms(MOTOR_INIT)..." << std::endl;
            res = client.InitBms(BmsInitType::MOTOR_INIT, 60000);
            PrintResult("Init Motor", res);
            break;

        case 3:
            std::cout << "Calling InitBms(BMS_AND_MOTOR_INIT)..." << std::endl;
            res = client.InitBms(BmsInitType::BMS_AND_MOTOR_INIT, 60000);
            PrintResult("Init BMS and Motor", res);
            break;

        case 4:
            std::cout << "Calling InitBms(BMS_INIT_NONE)..." << std::endl;
            res = client.InitBms(BmsInitType::BMS_INIT_NONE, 60000);
            PrintResult("BMS OFF", res);
            break;

        case 5:
            std::cout << "Calling SetTorque(TORQUE_ON)..." << std::endl;
            res = client.SetTorque(TorqueType::TORQUE_ON, 60000);
            PrintResult("Torque ON", res);
            break;

        case 6:
            std::cout << "Calling SetTorque(TORQUE_OFF)..." << std::endl;
            res = client.SetTorque(TorqueType::TORQUE_OFF, 60000);
            PrintResult("Torque OFF", res);
            break;

        case 7:
            std::cout << "Calling InitHand()..." << std::endl;
            res = client.InitHand(60000);
            PrintResult("Init Hand", res);
            break;

        case 10:
            std::cout << "Calling SendControlModeCommand(MOTION_PRESET, preset_id=\"HOME\")..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_MOTION_PRESET, "HOME", false, 60000);
            PrintResult("Motion Preset: HOME", res);
            break;

        case 11:
            std::cout << "Calling SendControlModeCommand(MOTION_PRESET, preset_id=\"HI_WAVE\")..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_MOTION_PRESET, "HI_WAVE", false, 60000);
            PrintResult("Motion Preset: HI_WAVE", res);
            break;

        case 12:
            std::cout << "Calling SendControlModeCommand(JOINT_POSITION_HOLD)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_JOINT_POSITION_HOLD, "", false, 60000);
            PrintResult("Joint Position Hold", res);
            break;

        case 13:
            std::cout << "Calling SendControlModeCommand(MOTION_STOP)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_MOTION_STOP, "", false, 60000);
            PrintResult("Motion Stop", res);
            break;

        case 14:
            std::cout << "Calling SendControlModeCommand(WALKMODE_ON)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_WALKMODE_ON, "", false, 60000);
            PrintResult("Walkmode ON", res);
            break;

        case 15:
            std::cout << "Calling SendControlModeCommand(LOW_LEVEL_JOINT_CONTROL)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_LOW_LEVEL_JOINT_CONTROL, "", false, 60000);
            PrintResult("Low Level Joint Control", res);
            break;

        case 16:
            std::cout << "Calling SendControlModeCommand(LOW_LEVEL_WALKMODE_ON)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_LOW_LEVEL_WALKMODE_ON, "", false, 60000);
            PrintResult("Low Level Walkmode ON", res);
            break;

        case 17:
            std::cout << "Calling SendControlModeCommand(HIGH_LEVEL_JOINT_CONTROL)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_HIGH_LEVEL_JOINT_CONTROL, "", false, 60000);
            PrintResult("High Level Joint Control", res);
            break;

        case 18:
            std::cout << "Calling SendControlModeCommand(HIGH_LEVEL_WALKMODE_ON)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_HIGH_LEVEL_WALKMODE_ON, "", false, 60000);
            PrintResult("High Level Walkmode ON", res);
            break;

        case 19:
            std::cout << "Calling SendControlModeCommand(CUSTOM_MODE_1)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_1, "", false, 60000);
            PrintResult("Custom Mode 1", res);
            break;

        case 20:
            std::cout << "Calling SendControlModeCommand(CUSTOM_MODE_2)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_2, "", false, 60000);
            PrintResult("Custom Mode 2", res);
            break;

        case 21:
            std::cout << "Calling SendControlModeCommand(CUSTOM_MODE_3)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_3, "", false, 60000);
            PrintResult("Custom Mode 3", res);
            break;

        case 22:
            std::cout << "Calling SendControlModeCommand(CUSTOM_MODE_4)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_4, "", false, 60000);
            PrintResult("Custom Mode 4", res);
            break;

        case 23:
            std::cout << "Calling SendControlModeCommand(CUSTOM_MODE_5)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_5, "", false, 60000);
            PrintResult("Custom Mode 5", res);
            break;

        case 24:
            std::cout << "Calling SendControlModeCommand(CUSTOM_MODE_6)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_6, "", false, 60000);
            PrintResult("Custom Mode 6", res);
            break;

        case 25:
            std::cout << "Calling SendControlModeCommand(CUSTOM_MODE_7)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_7, "", false, 60000);
            PrintResult("Custom Mode 7", res);
            break;

        case 26:
            std::cout << "Calling SendControlModeCommand(CUSTOM_MODE_8)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_8, "", false, 60000);
            PrintResult("Custom Mode 8", res);
            break;

        case 27:
            std::cout << "Calling SendControlModeCommand(CUSTOM_MODE_9)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_9, "", false, 60000);
            PrintResult("Custom Mode 9", res);
            break;

        case 28:
            std::cout << "Calling SendControlModeCommand(CUSTOM_MODE_10)..." << std::endl;
            res = client.SendControlModeCommand(ControlModeCommandType::CONTROL_MODE_CMD_CUSTOM_MODE_10, "", false, 60000);
            PrintResult("Custom Mode 10", res);
            break;

        case 30:
            std::cout << "Calling SendMujocoSimCmd(MUJOCO_SIM_PAUSE)..." << std::endl;
            res = client.SendMujocoSimCmd(MujocoSimCommandType::MUJOCO_SIM_PAUSE, 60000);
            PrintResult("Mujoco Sim Pause", res);
            break;

        case 31:
            std::cout << "Calling SendMujocoSimCmd(MUJOCO_SIM_RESUME)..." << std::endl;
            res = client.SendMujocoSimCmd(MujocoSimCommandType::MUJOCO_SIM_RESUME, 60000);
            PrintResult("Mujoco Sim Resume", res);
            break;

        case 32:
            std::cout << "Calling SendMujocoSimCmd(MUJOCO_SIM_RELOAD)..." << std::endl;
            res = client.SendMujocoSimCmd(MujocoSimCommandType::MUJOCO_SIM_RELOAD, 60000);
            PrintResult("Mujoco Sim Reload", res);
            break;

        case 33:
            std::cout << "Calling SendMujocoSimCmd(MUJOCO_SIM_RESET)..." << std::endl;
            res = client.SendMujocoSimCmd(MujocoSimCommandType::MUJOCO_SIM_RESET, 60000);
            PrintResult("Mujoco Sim Reset", res);
            break;

        case 0:
            std::cout << "Exiting..." << std::endl;
            return 0;

        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }

    return 0;
}
