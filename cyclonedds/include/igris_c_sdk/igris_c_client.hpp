/**
 * @file igris_c_client.hpp
 * @brief High-level synchronous service API over the DDS request/response
 *        topic pairs (service/<name>/{request,response}, namespace-resolved).
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
 * @brief High-level client for IGRIS-C robot control.
 *
 * Each service is a DDS topic pair; requests carry a payload request_id that
 * the robot echoes in the per-service *_Response. The public API returns the
 * common igris_c_sdk::msg::dds_::ServiceResponse_ (same field set as every service response).
 * Command arguments are uint8 constants from the generated *_Constants
 * namespaces (e.g. igris_c_sdk::srv::dds_::PduInitCmd_Request_Constants::PDU_INIT).
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
     * @brief Initialize PDU/Motor (blocking)
     * @param init_type PduInitCmd_Request_Constants: PDU_INIT, MOTOR_INIT,
     *                  PDU_AND_MOTOR_INIT, PDU_OFF
     * @param timeout_ms Timeout in milliseconds (default: 5000)
     */
    igris_c_sdk::msg::dds_::ServiceResponse_ InitPdu(uint8_t init_type, int timeout_ms = 5000);

    /**
     * @brief Set torque on/off (blocking)
     * @param torque TorqueCmd_Request_Constants: TORQUE_ON or TORQUE_OFF
     * @param timeout_ms Timeout in milliseconds (default: 5000)
     */
    igris_c_sdk::msg::dds_::ServiceResponse_ SetTorque(uint8_t torque, int timeout_ms = 5000);

    /**
     * @brief Initialize hand (blocking)
     * @param timeout_ms Timeout in milliseconds (default: 5000)
     */
    igris_c_sdk::msg::dds_::ServiceResponse_ InitHand(int timeout_ms = 5000);

    /**
     * @brief Send a control mode command (blocking).
     * @param command_type ControlModeCommandRequest_Request_Constants value
     * @param preset_id Optional preset id (used by MOTION_PRESET)
     * @param is_cyclic Optional cyclic flag (used by MOTION_PRESET_CYCLIC_TOGGLE)
     * @param timeout_ms Timeout in milliseconds (default: 5000)
     */
    igris_c_sdk::msg::dds_::ServiceResponse_ SendControlModeCommand(uint8_t command_type, const std::string &preset_id = "",
                                                                    bool is_cyclic = false, int timeout_ms = 5000);

    /**
     * @brief Send a Mujoco sim-control command (sim-only).
     * @param command_type MujocoSimCmd_Request_Constants value
     */
    igris_c_sdk::msg::dds_::ServiceResponse_ SendMujocoSimCmd(uint8_t command_type, int timeout_ms = 5000);

  private:
    using PromiseMap = std::map<std::string, std::shared_ptr<std::promise<igris_c_sdk::msg::dds_::ServiceResponse_>>>;

    // ========== Internal Async Implementation ==========
    std::future<igris_c_sdk::msg::dds_::ServiceResponse_> InitPduAsync(uint8_t init_type);
    std::future<igris_c_sdk::msg::dds_::ServiceResponse_> SetTorqueAsync(uint8_t torque);
    std::future<igris_c_sdk::msg::dds_::ServiceResponse_> InitHandAsync();
    std::future<igris_c_sdk::msg::dds_::ServiceResponse_> SendControlModeCommandAsync(uint8_t command_type, const std::string &preset_id,
                                                                                      bool is_cyclic);
    std::future<igris_c_sdk::msg::dds_::ServiceResponse_> SendMujocoSimCmdAsync(uint8_t command_type);

    // Resolve a per-service *_Response into the matching pending promise.
    template <typename ResponseT> void resolvePromise(PromiseMap &promises, const ResponseT &res);

    bool initialized_;
    float timeout_;

    // Request Publishers (per-service request types)
    std::unique_ptr<Publisher<igris_c_sdk::srv::dds_::PduInitCmd_Request_>> pdu_init_req_pub_;
    std::unique_ptr<Publisher<igris_c_sdk::srv::dds_::TorqueCmd_Request_>> torque_req_pub_;
    std::unique_ptr<Publisher<igris_c_sdk::srv::dds_::HandInitRequest_Request_>> hand_init_req_pub_;
    std::unique_ptr<Publisher<igris_c_sdk::srv::dds_::ControlModeCommandRequest_Request_>> control_mode_req_pub_;
    std::unique_ptr<Publisher<igris_c_sdk::srv::dds_::MujocoSimCmd_Request_>> mujoco_sim_req_pub_;

    // Response Subscribers (per-service response types)
    std::unique_ptr<Subscriber<igris_c_sdk::srv::dds_::PduInitCmd_Response_>> pdu_init_res_sub_;
    std::unique_ptr<Subscriber<igris_c_sdk::srv::dds_::TorqueCmd_Response_>> torque_res_sub_;
    std::unique_ptr<Subscriber<igris_c_sdk::srv::dds_::HandInitRequest_Response_>> hand_init_res_sub_;
    std::unique_ptr<Subscriber<igris_c_sdk::srv::dds_::ControlModeCommandRequest_Response_>> control_mode_res_sub_;
    std::unique_ptr<Subscriber<igris_c_sdk::srv::dds_::MujocoSimCmd_Response_>> mujoco_sim_res_sub_;

    // Promise management (for Future-based API)
    PromiseMap pdu_init_promises_;
    PromiseMap torque_promises_;
    PromiseMap hand_init_promises_;
    PromiseMap control_mode_promises_;
    PromiseMap mujoco_sim_promises_;
    std::mutex promise_mtx_;

    // Helper functions
    std::string generateRequestId();
};

}  // namespace igris_c_sdk

#endif /* __IGRIS_C_CLIENT_C9799B42_D40C_4BE6_932B_BFE0482BE0B9_H__ */
