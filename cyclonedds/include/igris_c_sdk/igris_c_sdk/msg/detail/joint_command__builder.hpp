// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/JointCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/joint_command.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__JOINT_COMMAND__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__JOINT_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/joint_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_JointCommand_effort
{
public:
  explicit Init_JointCommand_effort(::igris_c_sdk::msg::JointCommand & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::JointCommand effort(::igris_c_sdk::msg::JointCommand::_effort_type arg)
  {
    msg_.effort = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::JointCommand msg_;
};

class Init_JointCommand_velocity
{
public:
  explicit Init_JointCommand_velocity(::igris_c_sdk::msg::JointCommand & msg)
  : msg_(msg)
  {}
  Init_JointCommand_effort velocity(::igris_c_sdk::msg::JointCommand::_velocity_type arg)
  {
    msg_.velocity = std::move(arg);
    return Init_JointCommand_effort(msg_);
  }

private:
  ::igris_c_sdk::msg::JointCommand msg_;
};

class Init_JointCommand_position
{
public:
  explicit Init_JointCommand_position(::igris_c_sdk::msg::JointCommand & msg)
  : msg_(msg)
  {}
  Init_JointCommand_velocity position(::igris_c_sdk::msg::JointCommand::_position_type arg)
  {
    msg_.position = std::move(arg);
    return Init_JointCommand_velocity(msg_);
  }

private:
  ::igris_c_sdk::msg::JointCommand msg_;
};

class Init_JointCommand_mode
{
public:
  explicit Init_JointCommand_mode(::igris_c_sdk::msg::JointCommand & msg)
  : msg_(msg)
  {}
  Init_JointCommand_position mode(::igris_c_sdk::msg::JointCommand::_mode_type arg)
  {
    msg_.mode = std::move(arg);
    return Init_JointCommand_position(msg_);
  }

private:
  ::igris_c_sdk::msg::JointCommand msg_;
};

class Init_JointCommand_name
{
public:
  explicit Init_JointCommand_name(::igris_c_sdk::msg::JointCommand & msg)
  : msg_(msg)
  {}
  Init_JointCommand_mode name(::igris_c_sdk::msg::JointCommand::_name_type arg)
  {
    msg_.name = std::move(arg);
    return Init_JointCommand_mode(msg_);
  }

private:
  ::igris_c_sdk::msg::JointCommand msg_;
};

class Init_JointCommand_header
{
public:
  Init_JointCommand_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_JointCommand_name header(::igris_c_sdk::msg::JointCommand::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_JointCommand_name(msg_);
  }

private:
  ::igris_c_sdk::msg::JointCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::JointCommand>()
{
  return igris_c_sdk::msg::builder::Init_JointCommand_header();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__JOINT_COMMAND__BUILDER_HPP_
