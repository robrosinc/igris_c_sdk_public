// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/JointState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/joint_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__JOINT_STATE__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__JOINT_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/joint_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_JointState_status_bits
{
public:
  explicit Init_JointState_status_bits(::igris_c_sdk::msg::JointState & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::JointState status_bits(::igris_c_sdk::msg::JointState::_status_bits_type arg)
  {
    msg_.status_bits = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::JointState msg_;
};

class Init_JointState_tau_est
{
public:
  explicit Init_JointState_tau_est(::igris_c_sdk::msg::JointState & msg)
  : msg_(msg)
  {}
  Init_JointState_status_bits tau_est(::igris_c_sdk::msg::JointState::_tau_est_type arg)
  {
    msg_.tau_est = std::move(arg);
    return Init_JointState_status_bits(msg_);
  }

private:
  ::igris_c_sdk::msg::JointState msg_;
};

class Init_JointState_dq
{
public:
  explicit Init_JointState_dq(::igris_c_sdk::msg::JointState & msg)
  : msg_(msg)
  {}
  Init_JointState_tau_est dq(::igris_c_sdk::msg::JointState::_dq_type arg)
  {
    msg_.dq = std::move(arg);
    return Init_JointState_tau_est(msg_);
  }

private:
  ::igris_c_sdk::msg::JointState msg_;
};

class Init_JointState_q
{
public:
  Init_JointState_q()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_JointState_dq q(::igris_c_sdk::msg::JointState::_q_type arg)
  {
    msg_.q = std::move(arg);
    return Init_JointState_dq(msg_);
  }

private:
  ::igris_c_sdk::msg::JointState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::JointState>()
{
  return igris_c_sdk::msg::builder::Init_JointState_q();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__JOINT_STATE__BUILDER_HPP_
