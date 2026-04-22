#pragma once

#include "igris_c_msgs.hpp"

#include <cstdint>

namespace igris_sdk {

// Re-export IDL types for convenience
using KinematicMode = igris_c::msg::dds::KinematicMode;
using MotorCmd      = igris_c::msg::dds::MotorCmd;
using MotorState    = igris_c::msg::dds::MotorState;
using JointState    = igris_c::msg::dds::JointState;
using LowCmd        = igris_c::msg::dds::LowCmd;
using LowState      = igris_c::msg::dds::LowState;
using HandCmd       = igris_c::msg::dds::HandCmd;
using HandState     = igris_c::msg::dds::HandState;
using IMUState      = igris_c::msg::dds::IMUState;

// BMS types
using RelayState   = igris_c::msg::dds::RelayState;
using EStopState   = igris_c::msg::dds::EStopState;
using BmsConnState = igris_c::msg::dds::BmsConnState;
using BmsInitState = igris_c::msg::dds::BmsInitState;
using BmsState     = igris_c::msg::dds::BmsState;
using BmsInitType  = igris_c::msg::dds::BmsInitType;
using BmsInitCmd   = igris_c::msg::dds::BmsInitCmd;
using TorqueType   = igris_c::msg::dds::TorqueType;
using TorqueCmd    = igris_c::msg::dds::TorqueCmd;

// Control mode types
using ControlMode      = igris_c::msg::dds::ControlMode;
using ControlModeCmd   = igris_c::msg::dds::ControlModeCmd;
using ControlModeState = igris_c::msg::dds::ControlModeState;

// Service response
using ServiceResponse        = igris_c::msg::dds::ServiceResponse;
using StereoConfigRequest    = igris_c::msg::dds::StereoConfigRequest;
using RealsenseConfigRequest = igris_c::msg::dds::RealsenseConfigRequest;
using MonoConfigRequest      = igris_c::msg::dds::MonoConfigRequest;
using Vec3f                  = igris_c::msg::dds::Vec3f;
using Vec4f                  = igris_c::msg::dds::Vec4f;
using QuestController        = igris_c::msg::dds::QuestController;
using QuestHand              = igris_c::msg::dds::QuestHand;
using QuestMessage           = igris_c::msg::dds::QuestMessage;

// Motor error flags (from status_bits field in MotorState)
enum MotorError : uint32_t {
    // clang-format off
    NO_ERROR                                 = 0x00000000,

    INITIALIZE_ERROR                         = 0x00000001,
    PACKET_NOT_RECEIVED_ERROR                = 0x00000002,

    ROBOT_LEVEL_MASK                         = 0x00000003,

    /* After the current reaches the stall current, the speed is
    very low and continues for a period of time.
    It indicates that the motor load is too large. */
    MOTOR_STALL_ERROR                        = 0x00000004,
    /* The power input is lower than the set undervoltage
    value */
    LOW_VOLTAGE_ERROR                        = 0x00000008,
    /* The power input is higher than the set overvoltage
    value */
    OVER_VOLTAGE_ERROR                       = 0x00000010,
    /* The software detects that the motor current exceeds
    the limit value,and there may be short circuit,phase
    loss,loss of control,motor damage,etc. */
    OVER_CURRENT_ERROR                       = 0x00000020,
    /* If the input current of the power supply exceeds the
    limit value,there may be a situation where the load is
    too large or the speed is too high. */
    POWER_OVERRUN_ERROR                      = 0x00000040,
    /* Failed to write parameters causing parameter loss. */
    CALIBRATION_PARAMETER_WRITING_ERROR      = 0x00000080,
    /* The motor running speed exceeds the limit
    value,there may be over pressure and drag use. */
    SPEEDING_ERROR                           = 0x00000100,
    /* If the motor temperature exceeds the set value,there
    may be short circuit,parameter error,and long-term
    overload use. */
    COMPONENT_OVERTEMPERATURE_ERROR          = 0x00000200,
    MOTOR_TEMPERATURE_OVER_TEMPERATURE_ERROR = 0x00000400,
    /* The encoder calibration result deviates too much
    from the standard value. */
    ENCODER_CALIBRATION_ERROR                = 0x00000800,
    /* Encoder data error */
    ENCODER_DATA_ERROR                       = 0x00001000,
    /* Motor brake voltage too high */
    MOTOR_BRAKE_VOLTAGE_TOO_HIGH_ERROR       = 0x00002000,
    /* DRV drive error */
    DRV_DRIVE_ERROR                          = 0x00004000,
    // clang-format on
};

// IGRIS-C specifications (from IDL: N_JOINTS = 31)
constexpr int NUM_MOTORS    = 31;
constexpr uint32_t N_JOINTS = igris_c::msg::dds::N_JOINTS;

}  // namespace igris_sdk
