// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/RobotState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/robot_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/robot_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_RobotState_environment
{
public:
  explicit Init_RobotState_environment(::igris_c_sdk::msg::RobotState & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::RobotState environment(::igris_c_sdk::msg::RobotState::_environment_type arg)
  {
    msg_.environment = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::RobotState msg_;
};

class Init_RobotState_state
{
public:
  explicit Init_RobotState_state(::igris_c_sdk::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_environment state(::igris_c_sdk::msg::RobotState::_state_type arg)
  {
    msg_.state = std::move(arg);
    return Init_RobotState_environment(msg_);
  }

private:
  ::igris_c_sdk::msg::RobotState msg_;
};

class Init_RobotState_header
{
public:
  Init_RobotState_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_RobotState_state header(::igris_c_sdk::msg::RobotState::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_RobotState_state(msg_);
  }

private:
  ::igris_c_sdk::msg::RobotState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::RobotState>()
{
  return igris_c_sdk::msg::builder::Init_RobotState_header();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_
