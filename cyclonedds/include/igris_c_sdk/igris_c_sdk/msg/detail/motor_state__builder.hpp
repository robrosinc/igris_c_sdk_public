// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/MotorState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/motor_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__MOTOR_STATE__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__MOTOR_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/motor_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_MotorState_status_bits
{
public:
  explicit Init_MotorState_status_bits(::igris_c_sdk::msg::MotorState & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::MotorState status_bits(::igris_c_sdk::msg::MotorState::_status_bits_type arg)
  {
    msg_.status_bits = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::MotorState msg_;
};

class Init_MotorState_temperature
{
public:
  explicit Init_MotorState_temperature(::igris_c_sdk::msg::MotorState & msg)
  : msg_(msg)
  {}
  Init_MotorState_status_bits temperature(::igris_c_sdk::msg::MotorState::_temperature_type arg)
  {
    msg_.temperature = std::move(arg);
    return Init_MotorState_status_bits(msg_);
  }

private:
  ::igris_c_sdk::msg::MotorState msg_;
};

class Init_MotorState_tau_est
{
public:
  explicit Init_MotorState_tau_est(::igris_c_sdk::msg::MotorState & msg)
  : msg_(msg)
  {}
  Init_MotorState_temperature tau_est(::igris_c_sdk::msg::MotorState::_tau_est_type arg)
  {
    msg_.tau_est = std::move(arg);
    return Init_MotorState_temperature(msg_);
  }

private:
  ::igris_c_sdk::msg::MotorState msg_;
};

class Init_MotorState_dq
{
public:
  explicit Init_MotorState_dq(::igris_c_sdk::msg::MotorState & msg)
  : msg_(msg)
  {}
  Init_MotorState_tau_est dq(::igris_c_sdk::msg::MotorState::_dq_type arg)
  {
    msg_.dq = std::move(arg);
    return Init_MotorState_tau_est(msg_);
  }

private:
  ::igris_c_sdk::msg::MotorState msg_;
};

class Init_MotorState_q
{
public:
  Init_MotorState_q()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MotorState_dq q(::igris_c_sdk::msg::MotorState::_q_type arg)
  {
    msg_.q = std::move(arg);
    return Init_MotorState_dq(msg_);
  }

private:
  ::igris_c_sdk::msg::MotorState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::MotorState>()
{
  return igris_c_sdk::msg::builder::Init_MotorState_q();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__MOTOR_STATE__BUILDER_HPP_
