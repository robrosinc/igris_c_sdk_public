// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/MotorCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/motor_cmd.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__MOTOR_CMD__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__MOTOR_CMD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/motor_cmd__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_MotorCmd_kd
{
public:
  explicit Init_MotorCmd_kd(::igris_c_sdk::msg::MotorCmd & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::MotorCmd kd(::igris_c_sdk::msg::MotorCmd::_kd_type arg)
  {
    msg_.kd = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::MotorCmd msg_;
};

class Init_MotorCmd_kp
{
public:
  explicit Init_MotorCmd_kp(::igris_c_sdk::msg::MotorCmd & msg)
  : msg_(msg)
  {}
  Init_MotorCmd_kd kp(::igris_c_sdk::msg::MotorCmd::_kp_type arg)
  {
    msg_.kp = std::move(arg);
    return Init_MotorCmd_kd(msg_);
  }

private:
  ::igris_c_sdk::msg::MotorCmd msg_;
};

class Init_MotorCmd_tau
{
public:
  explicit Init_MotorCmd_tau(::igris_c_sdk::msg::MotorCmd & msg)
  : msg_(msg)
  {}
  Init_MotorCmd_kp tau(::igris_c_sdk::msg::MotorCmd::_tau_type arg)
  {
    msg_.tau = std::move(arg);
    return Init_MotorCmd_kp(msg_);
  }

private:
  ::igris_c_sdk::msg::MotorCmd msg_;
};

class Init_MotorCmd_dq
{
public:
  explicit Init_MotorCmd_dq(::igris_c_sdk::msg::MotorCmd & msg)
  : msg_(msg)
  {}
  Init_MotorCmd_tau dq(::igris_c_sdk::msg::MotorCmd::_dq_type arg)
  {
    msg_.dq = std::move(arg);
    return Init_MotorCmd_tau(msg_);
  }

private:
  ::igris_c_sdk::msg::MotorCmd msg_;
};

class Init_MotorCmd_q
{
public:
  explicit Init_MotorCmd_q(::igris_c_sdk::msg::MotorCmd & msg)
  : msg_(msg)
  {}
  Init_MotorCmd_dq q(::igris_c_sdk::msg::MotorCmd::_q_type arg)
  {
    msg_.q = std::move(arg);
    return Init_MotorCmd_dq(msg_);
  }

private:
  ::igris_c_sdk::msg::MotorCmd msg_;
};

class Init_MotorCmd_id
{
public:
  Init_MotorCmd_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MotorCmd_q id(::igris_c_sdk::msg::MotorCmd::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_MotorCmd_q(msg_);
  }

private:
  ::igris_c_sdk::msg::MotorCmd msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::MotorCmd>()
{
  return igris_c_sdk::msg::builder::Init_MotorCmd_id();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__MOTOR_CMD__BUILDER_HPP_
