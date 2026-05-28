// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/IMUState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/imu_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__IMU_STATE__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__IMU_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/imu_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_IMUState_rpy
{
public:
  explicit Init_IMUState_rpy(::igris_c_sdk::msg::IMUState & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::IMUState rpy(::igris_c_sdk::msg::IMUState::_rpy_type arg)
  {
    msg_.rpy = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::IMUState msg_;
};

class Init_IMUState_accelerometer
{
public:
  explicit Init_IMUState_accelerometer(::igris_c_sdk::msg::IMUState & msg)
  : msg_(msg)
  {}
  Init_IMUState_rpy accelerometer(::igris_c_sdk::msg::IMUState::_accelerometer_type arg)
  {
    msg_.accelerometer = std::move(arg);
    return Init_IMUState_rpy(msg_);
  }

private:
  ::igris_c_sdk::msg::IMUState msg_;
};

class Init_IMUState_gyroscope
{
public:
  explicit Init_IMUState_gyroscope(::igris_c_sdk::msg::IMUState & msg)
  : msg_(msg)
  {}
  Init_IMUState_accelerometer gyroscope(::igris_c_sdk::msg::IMUState::_gyroscope_type arg)
  {
    msg_.gyroscope = std::move(arg);
    return Init_IMUState_accelerometer(msg_);
  }

private:
  ::igris_c_sdk::msg::IMUState msg_;
};

class Init_IMUState_quaternion
{
public:
  Init_IMUState_quaternion()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_IMUState_gyroscope quaternion(::igris_c_sdk::msg::IMUState::_quaternion_type arg)
  {
    msg_.quaternion = std::move(arg);
    return Init_IMUState_gyroscope(msg_);
  }

private:
  ::igris_c_sdk::msg::IMUState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::IMUState>()
{
  return igris_c_sdk::msg::builder::Init_IMUState_quaternion();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__IMU_STATE__BUILDER_HPP_
