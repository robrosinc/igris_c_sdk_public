/**
 * @file igris_c_client.hpp
 * @author type your name (type your email)
 * @brief
 * @version 0.1
 * @date 2026-05-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef __IGRIS_C_CLIENT_C9799B42_D40C_4BE6_932B_BFE0482BE0B9_H__
#define __IGRIS_C_CLIENT_C9799B42_D40C_4BE6_932B_BFE0482BE0B9_H__

#include "igris_c_sdk/channel_factory.hpp"
#include "igris_c_sdk/igris_c_msgs.hpp"
#include "igris_c_sdk/publisher.hpp"
#include "igris_c_sdk/subscriber.hpp"

#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace igris_c_sdk {

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
     * @brief Initialize hand (blocking)
     * @param timeout_ms Timeout in milliseconds (default: 5000)
     * @return ServiceResponse with result
     */
    igris_c::msg::dds::ServiceResponse InitHand(int timeout_ms = 5000);

    /**
     * @brief Send a control mode command (blocking).
     * Mirrors the bridge<->controller SHM BridgeCommandType set:
     * MOTION_PRESET / MOTION_PRESET_CYCLIC_TOGGLE / JOINT_POSITION_HOLD /
     * MOTION_STOP / WALKMODE_ON / *_JOINT_CONTROL / *_WALKMODE_ON.
     * @param command_type Command kind
     * @param preset_id Optional preset id (used by MOTION_PRESET)
     * @param is_cyclic Optional cyclic flag (used by MOTION_PRESET_CYCLIC_TOGGLE)
     * @param timeout_ms Timeout in milliseconds (default: 5000)
     */
    igris_c::msg::dds::ServiceResponse SendControlModeCommand(igris_c::msg::dds::ControlModeCommandType command_type,
                                                              const std::string &preset_id = "", bool is_cyclic = false,
                                                              int timeout_ms = 5000);

    /**
     * @brief Send a Mujoco sim-control command (sim-only).
     * Bridge_sim translates this to mujocoShm_st.sim_command. bridge_real
     * rejects these requests because the service is never registered there.
     */
    igris_c::msg::dds::ServiceResponse SendMujocoSimCmd(igris_c::msg::dds::MujocoSimCommandType command_type, int timeout_ms = 5000);

  private:
    // ========== Internal Async Implementation ==========
    std::future<igris_c::msg::dds::ServiceResponse> InitBmsAsync(igris_c::msg::dds::BmsInitType init_type);
    std::future<igris_c::msg::dds::ServiceResponse> SetTorqueAsync(igris_c::msg::dds::TorqueType torque);
    std::future<igris_c::msg::dds::ServiceResponse> InitHandAsync();
    std::future<igris_c::msg::dds::ServiceResponse> SendControlModeCommandAsync(igris_c::msg::dds::ControlModeCommandType command_type,
                                                                                const std::string &preset_id, bool is_cyclic);
    std::future<igris_c::msg::dds::ServiceResponse> SendMujocoSimCmdAsync(igris_c::msg::dds::MujocoSimCommandType command_type);

    bool initialized_;
    float timeout_;

    // Request Publishers
    std::unique_ptr<Publisher<igris_c::msg::dds::BmsInitCmd>> bms_init_req_pub_;
    std::unique_ptr<Publisher<igris_c::msg::dds::TorqueCmd>> torque_req_pub_;
    std::unique_ptr<Publisher<igris_c::msg::dds::HandInitRequest>> hand_init_req_pub_;
    std::unique_ptr<Publisher<igris_c::msg::dds::ControlModeCommandRequest>> control_mode_req_pub_;
    std::unique_ptr<Publisher<igris_c::msg::dds::MujocoSimCmd>> mujoco_sim_req_pub_;

    // Response Subscribers
    std::unique_ptr<Subscriber<igris_c::msg::dds::ServiceResponse>> bms_init_res_sub_;
    std::unique_ptr<Subscriber<igris_c::msg::dds::ServiceResponse>> torque_res_sub_;
    std::unique_ptr<Subscriber<igris_c::msg::dds::ServiceResponse>> hand_init_res_sub_;
    std::unique_ptr<Subscriber<igris_c::msg::dds::ServiceResponse>> control_mode_res_sub_;
    std::unique_ptr<Subscriber<igris_c::msg::dds::ServiceResponse>> mujoco_sim_res_sub_;

    // Response callbacks
    void bmsInitResponseCallback(const igris_c::msg::dds::ServiceResponse &res);
    void torqueResponseCallback(const igris_c::msg::dds::ServiceResponse &res);
    void handInitResponseCallback(const igris_c::msg::dds::ServiceResponse &res);
    void controlModeResponseCallback(const igris_c::msg::dds::ServiceResponse &res);
    void mujocoSimResponseCallback(const igris_c::msg::dds::ServiceResponse &res);

    // Promise management (for Future-based API)
    std::map<std::string, std::shared_ptr<std::promise<igris_c::msg::dds::ServiceResponse>>> bms_init_promises_;
    std::map<std::string, std::shared_ptr<std::promise<igris_c::msg::dds::ServiceResponse>>> torque_promises_;
    std::map<std::string, std::shared_ptr<std::promise<igris_c::msg::dds::ServiceResponse>>> hand_init_promises_;
    std::map<std::string, std::shared_ptr<std::promise<igris_c::msg::dds::ServiceResponse>>> control_mode_promises_;
    std::map<std::string, std::shared_ptr<std::promise<igris_c::msg::dds::ServiceResponse>>> mujoco_sim_promises_;
    std::mutex promise_mtx_;

    // Helper functions
    std::string generateRequestId();
};

}  // namespace igris_c_sdk

#endif /* __IGRIS_C_CLIENT_C9799B42_D40C_4BE6_932B_BFE0482BE0B9_H__ */
