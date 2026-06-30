// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/BmsState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/bms_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/bms_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_BmsState_bms_init_state
{
public:
  explicit Init_BmsState_bms_init_state(::igris_c_sdk::msg::BmsState & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::BmsState bms_init_state(::igris_c_sdk::msg::BmsState::_bms_init_state_type arg)
  {
    msg_.bms_init_state = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::BmsState msg_;
};

class Init_BmsState_battery
{
public:
  explicit Init_BmsState_battery(::igris_c_sdk::msg::BmsState & msg)
  : msg_(msg)
  {}
  Init_BmsState_bms_init_state battery(::igris_c_sdk::msg::BmsState::_battery_type arg)
  {
    msg_.battery = std::move(arg);
    return Init_BmsState_bms_init_state(msg_);
  }

private:
  ::igris_c_sdk::msg::BmsState msg_;
};

class Init_BmsState_connect
{
public:
  explicit Init_BmsState_connect(::igris_c_sdk::msg::BmsState & msg)
  : msg_(msg)
  {}
  Init_BmsState_battery connect(::igris_c_sdk::msg::BmsState::_connect_type arg)
  {
    msg_.connect = std::move(arg);
    return Init_BmsState_battery(msg_);
  }

private:
  ::igris_c_sdk::msg::BmsState msg_;
};

class Init_BmsState_estop
{
public:
  explicit Init_BmsState_estop(::igris_c_sdk::msg::BmsState & msg)
  : msg_(msg)
  {}
  Init_BmsState_connect estop(::igris_c_sdk::msg::BmsState::_estop_type arg)
  {
    msg_.estop = std::move(arg);
    return Init_BmsState_connect(msg_);
  }

private:
  ::igris_c_sdk::msg::BmsState msg_;
};

class Init_BmsState_legs_power
{
public:
  explicit Init_BmsState_legs_power(::igris_c_sdk::msg::BmsState & msg)
  : msg_(msg)
  {}
  Init_BmsState_estop legs_power(::igris_c_sdk::msg::BmsState::_legs_power_type arg)
  {
    msg_.legs_power = std::move(arg);
    return Init_BmsState_estop(msg_);
  }

private:
  ::igris_c_sdk::msg::BmsState msg_;
};

class Init_BmsState_body_power
{
public:
  explicit Init_BmsState_body_power(::igris_c_sdk::msg::BmsState & msg)
  : msg_(msg)
  {}
  Init_BmsState_legs_power body_power(::igris_c_sdk::msg::BmsState::_body_power_type arg)
  {
    msg_.body_power = std::move(arg);
    return Init_BmsState_legs_power(msg_);
  }

private:
  ::igris_c_sdk::msg::BmsState msg_;
};

class Init_BmsState_header
{
public:
  Init_BmsState_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_BmsState_body_power header(::igris_c_sdk::msg::BmsState::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_BmsState_body_power(msg_);
  }

private:
  ::igris_c_sdk::msg::BmsState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::BmsState>()
{
  return igris_c_sdk::msg::builder::Init_BmsState_header();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__BUILDER_HPP_
