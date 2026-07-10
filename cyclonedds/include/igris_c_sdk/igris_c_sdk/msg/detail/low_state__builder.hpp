// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/LowState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/low_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/low_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_LowState_joint_state
{
public:
  explicit Init_LowState_joint_state(::igris_c_sdk::msg::LowState & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::LowState joint_state(::igris_c_sdk::msg::LowState::_joint_state_type arg)
  {
    msg_.joint_state = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::LowState msg_;
};

class Init_LowState_motor_state
{
public:
  explicit Init_LowState_motor_state(::igris_c_sdk::msg::LowState & msg)
  : msg_(msg)
  {}
  Init_LowState_joint_state motor_state(::igris_c_sdk::msg::LowState::_motor_state_type arg)
  {
    msg_.motor_state = std::move(arg);
    return Init_LowState_joint_state(msg_);
  }

private:
  ::igris_c_sdk::msg::LowState msg_;
};

class Init_LowState_imu_state
{
public:
  explicit Init_LowState_imu_state(::igris_c_sdk::msg::LowState & msg)
  : msg_(msg)
  {}
  Init_LowState_motor_state imu_state(::igris_c_sdk::msg::LowState::_imu_state_type arg)
  {
    msg_.imu_state = std::move(arg);
    return Init_LowState_motor_state(msg_);
  }

private:
  ::igris_c_sdk::msg::LowState msg_;
};

class Init_LowState_header
{
public:
  Init_LowState_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LowState_imu_state header(::igris_c_sdk::msg::LowState::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_LowState_imu_state(msg_);
  }

private:
  ::igris_c_sdk::msg::LowState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::LowState>()
{
  return igris_c_sdk::msg::builder::Init_LowState_header();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__BUILDER_HPP_
