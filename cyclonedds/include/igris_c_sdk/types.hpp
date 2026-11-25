/**
 * @file types.hpp
 * @brief SDK-wide constants.
 *
 * Deliberately carries NO type aliases. Contract types are written out in
 * full at every use site — igris_c_sdk::msg::dds_::LowState_,
 * sensor_msgs::msg::dds_::Imu_, igris_c_sdk::srv::dds_::PduInitCmd_Request_.
 *
 * The reason is that two lanes now carry near-identical names: the native
 * CycloneDDS types above and the ROS ones rosidl generates
 * (igris_c_sdk::msg::LowState, sensor_msgs::msg::Imu). A short alias hides
 * which lane a value belongs to, and the bridge holds both lanes in one
 * binary. The full name is also what appears in `ddsls` output and in DDS
 * type-mismatch errors, so the code and the wire read the same.
 *
 * Constants that replaced the old IDL enums live in the generated namespaces:
 *   igris_c_sdk::msg::dds_::PduState_Constants, ::RobotState_Constants,
 *   ::LowCmd_Constants, ::MasterArmState_Constants, ::MotorState_Constants,
 *   igris_c_sdk::srv::dds_::PduInitCmd_Request_Constants,
 *   ::TorqueCmd_Request_Constants,
 *   ::ControlModeCommandRequest_Request_Constants,
 *   ::MujocoSimCmd_Request_Constants.
 */

#ifndef __TYPES_AED0D606_CA44_44EE_801F_90F2DEB22275_H__
#define __TYPES_AED0D606_CA44_44EE_801F_90F2DEB22275_H__

#include "igris_c_msgs.hpp"

#include <cstdint>
#include <tuple>
#include <type_traits>

namespace igris_c_sdk {

// IGRIS-C specifications (contract arrays are MotorCmd[31] / MotorState[31]).
constexpr int NUM_MOTORS    = 31;
constexpr uint32_t N_JOINTS = 31;

// These used to be defined as the contract constant itself. They are plain literals now, so the
// comment above is a claim the compiler no longer checks - and utils.cpp uses N_JOINTS as the
// bounds check for indexing those very arrays. Shrink LowState to [30] without touching this
// header and get_motor_state(state, 30) would pass its guard and read out of bounds. Tie them
// back together here, where both are visible.
static_assert(N_JOINTS == std::tuple_size<std::remove_reference_t<decltype(std::declval<msg::dds_::LowState_ &>().motor_state())>>::value,
              "N_JOINTS must equal LowState.motor_state size");
static_assert(N_JOINTS == std::tuple_size<std::remove_reference_t<decltype(std::declval<msg::dds_::LowState_ &>().joint_state())>>::value,
              "N_JOINTS must equal LowState.joint_state size");
static_assert(NUM_MOTORS == static_cast<int>(N_JOINTS), "NUM_MOTORS and N_JOINTS describe the same array");

}  // namespace igris_c_sdk

#endif /* __TYPES_AED0D606_CA44_44EE_801F_90F2DEB22275_H__ */
