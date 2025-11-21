/**
 * @file service_example.cpp
 * @brief Service API example using IGRIS SDK
 *
 * This example demonstrates:
 * - IgrisC_Client initialization
 * - BMS and motor initialization
 * - Torque control
 * - Control mode switching
 *
 * Usage: ./service_example [domain_id]
 */

#include <igris_sdk/channel_factory.hpp>
#include <igris_sdk/igris_c_client.hpp>
#include <iostream>
#include <string>

using namespace igris_sdk;
using namespace igris_c::msg::dds;

void PrintMenu() {
    std::cout << "\n=== Service API Menu ===" << std::endl;
    std::cout << "1. Init BMS" << std::endl;
    std::cout << "2. Init Motor" << std::endl;
    std::cout << "3. Init BMS and Motor" << std::endl;
    std::cout << "4. BMS OFF" << std::endl;
    std::cout << "5. Torque ON" << std::endl;
    std::cout << "6. Torque OFF" << std::endl;
    std::cout << "7. Control Mode: LOW_LEVEL" << std::endl;
    std::cout << "8. Control Mode: HIGH_LEVEL" << std::endl;
    std::cout << "9. Exit" << std::endl;
    std::cout << "\nSelect (1-9): ";
}

void PrintResult(const std::string &action, const ServiceResponse &res) {
    std::cout << "\n[" << action << "] " << (res.success() ? "SUCCESS" : "FAILED") << " - " << res.message() << std::endl;
}

int main(int argc, char **argv) {
    std::cout << "=== IGRIS SDK Service Example ===" << std::endl;

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
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input" << std::endl;
            continue;
        }

        switch (choice) {
        case 1:
            std::cout << "Calling InitBms(BMS_INIT)..." << std::endl;
            res = client.InitBms(BmsInitType::BMS_INIT, 30000);
            PrintResult("Init BMS", res);
            break;

        case 2:
            std::cout << "Calling InitBms(MOTOR_INIT)..." << std::endl;
            res = client.InitBms(BmsInitType::MOTOR_INIT, 30000);
            PrintResult("Init Motor", res);
            break;

        case 3:
            std::cout << "Calling InitBms(BMS_AND_MOTOR_INIT)..." << std::endl;
            res = client.InitBms(BmsInitType::BMS_AND_MOTOR_INIT, 30000);
            PrintResult("Init BMS and Motor", res);
            break;

        case 4:
            std::cout << "Calling InitBms(BMS_INIT_NONE)..." << std::endl;
            res = client.InitBms(BmsInitType::BMS_INIT_NONE, 30000);
            PrintResult("BMS OFF", res);
            break;

        case 5:
            std::cout << "Calling SetTorque(TORQUE_ON)..." << std::endl;
            res = client.SetTorque(TorqueType::TORQUE_ON, 30000);
            PrintResult("Torque ON", res);
            break;

        case 6:
            std::cout << "Calling SetTorque(TORQUE_OFF)..." << std::endl;
            res = client.SetTorque(TorqueType::TORQUE_OFF, 30000);
            PrintResult("Torque OFF", res);
            break;

        case 7:
            std::cout << "Calling SetControlMode(LOW_LEVEL)..." << std::endl;
            res = client.SetControlMode(ControlMode::CONTROL_MODE_LOW_LEVEL, 30000);
            PrintResult("Control Mode: LOW_LEVEL", res);
            break;

        case 8:
            std::cout << "Calling SetControlMode(HIGH_LEVEL)..." << std::endl;
            res = client.SetControlMode(ControlMode::CONTROL_MODE_HIGH_LEVEL, 30000);
            PrintResult("Control Mode: HIGH_LEVEL", res);
            break;

        case 9:
            std::cout << "Exiting..." << std::endl;
            return 0;

        default:
            std::cout << "Invalid choice (1-9)" << std::endl;
            break;
        }
    }

    return 0;
}
