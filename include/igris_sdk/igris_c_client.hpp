#pragma once

#include "igris_sdk/channel_factory.hpp"
#include "igris_sdk/igris_c_msgs.hpp"
#include "igris_sdk/publisher.hpp"
#include "igris_sdk/subscriber.hpp"

#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace igris_sdk {

/**
 * @brief High-level client for IGRIS-C robot control
 *
 * Provides simplified API for common robot operations.
 * Currently under development - use ChannelFactory + Publisher/Subscriber
 * for low-level control.
 *
 * Example (future):
 * @code
 * ChannelFactory::Instance()->Init(0);
 * IgrisC_Client client;
 * client.Init();
 * client.StandUp();
 * client.Walk(0.5, 0, 0);
 * @endcode
 */
class IgrisC_Client {
  public:
    IgrisC_Client();
    ~IgrisC_Client();

    /**
     * @brief Initialize client
     * @note Must call ChannelFactory::Instance()->Init() first
     */
    void Init();

    /**
     * @brief Set timeout for operations
     * @param timeout_sec Timeout in seconds
     */
    void SetTimeout(float timeout_sec);

    // ========== Service API (Synchronous) ==========

    /**
     * @brief Initialize BMS/Motor (blocking)
     * @param init_type BMS_INIT, MOTOR_INIT, or BMS_AND_MOTOR_INIT
     * @param timeout_ms Timeout in milliseconds (default: 5000)
     * @return ServiceResponse with result
     */
    igris_c::msg::dds::ServiceResponse InitBms(igris_c::msg::dds::BmsInitType init_type, int timeout_ms = 5000);

    /**
     * @brief Set torque on/off (blocking)
     * @param torque TORQUE_ON or TORQUE_OFF
     * @param timeout_ms Timeout in milliseconds (default: 5000)
     * @return ServiceResponse with result
     */
    igris_c::msg::dds::ServiceResponse SetTorque(igris_c::msg::dds::TorqueType torque, int timeout_ms = 5000);

    /**
     * @brief Set control mode (blocking)
     * @param mode CONTROL_MODE_LOW_LEVEL or CONTROL_MODE_HIGH_LEVEL
     * @param timeout_ms Timeout in milliseconds (default: 5000)
     * @return ServiceResponse with result
     */
    igris_c::msg::dds::ServiceResponse SetControlMode(igris_c::msg::dds::ControlMode mode, int timeout_ms = 5000);

  private:
    // ========== Internal Async Implementation ==========
    std::future<igris_c::msg::dds::ServiceResponse> InitBmsAsync(igris_c::msg::dds::BmsInitType init_type);
    std::future<igris_c::msg::dds::ServiceResponse> SetTorqueAsync(igris_c::msg::dds::TorqueType torque);
    std::future<igris_c::msg::dds::ServiceResponse> SetControlModeAsync(igris_c::msg::dds::ControlMode mode);

    bool initialized_;
    float timeout_;

    // Request Publishers
    std::unique_ptr<Publisher<igris_c::msg::dds::BmsInitCmd>> bms_init_req_pub_;
    std::unique_ptr<Publisher<igris_c::msg::dds::TorqueCmd>> torque_req_pub_;
    std::unique_ptr<Publisher<igris_c::msg::dds::ControlModeCmd>> control_mode_req_pub_;

    // Response Subscribers
    std::unique_ptr<Subscriber<igris_c::msg::dds::ServiceResponse>> bms_init_res_sub_;
    std::unique_ptr<Subscriber<igris_c::msg::dds::ServiceResponse>> torque_res_sub_;
    std::unique_ptr<Subscriber<igris_c::msg::dds::ServiceResponse>> control_mode_res_sub_;

    // Response callbacks
    void bmsInitResponseCallback(const igris_c::msg::dds::ServiceResponse &res);
    void torqueResponseCallback(const igris_c::msg::dds::ServiceResponse &res);
    void controlModeResponseCallback(const igris_c::msg::dds::ServiceResponse &res);

    // Promise management (for Future-based API)
    std::map<std::string, std::shared_ptr<std::promise<igris_c::msg::dds::ServiceResponse>>> bms_init_promises_;
    std::map<std::string, std::shared_ptr<std::promise<igris_c::msg::dds::ServiceResponse>>> torque_promises_;
    std::map<std::string, std::shared_ptr<std::promise<igris_c::msg::dds::ServiceResponse>>> control_mode_promises_;
    std::mutex promise_mtx_;

    // Helper functions
    std::string generateRequestId();
};

}  // namespace igris_sdk
