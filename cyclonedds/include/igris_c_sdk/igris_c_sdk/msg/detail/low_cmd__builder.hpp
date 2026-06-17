// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/LowCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/low_cmd.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/low_cmd__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_LowCmd_motors
{
public:
  explicit Init_LowCmd_motors(::igris_c_sdk::msg::LowCmd & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::LowCmd motors(::igris_c_sdk::msg::LowCmd::_motors_type arg)
  {
    msg_.motors = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::LowCmd msg_;
};

class Init_LowCmd_kinematic_modes
{
public:
  explicit Init_LowCmd_kinematic_modes(::igris_c_sdk::msg::LowCmd & msg)
  : msg_(msg)
  {}
  Init_LowCmd_motors kinematic_modes(::igris_c_sdk::msg::LowCmd::_kinematic_modes_type arg)
  {
    msg_.kinematic_modes = std::move(arg);
    return Init_LowCmd_motors(msg_);
  }

private:
  ::igris_c_sdk::msg::LowCmd msg_;
};

class Init_LowCmd_header
{
public:
  Init_LowCmd_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LowCmd_kinematic_modes header(::igris_c_sdk::msg::LowCmd::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_LowCmd_kinematic_modes(msg_);
  }

private:
  ::igris_c_sdk::msg::LowCmd msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::LowCmd>()
{
  return igris_c_sdk::msg::builder::Init_LowCmd_header();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__BUILDER_HPP_
