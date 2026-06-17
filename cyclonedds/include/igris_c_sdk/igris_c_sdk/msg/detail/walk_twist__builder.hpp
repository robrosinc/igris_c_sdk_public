// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/WalkTwist.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/walk_twist.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__WALK_TWIST__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__WALK_TWIST__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/walk_twist__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_WalkTwist_twist
{
public:
  explicit Init_WalkTwist_twist(::igris_c_sdk::msg::WalkTwist & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::WalkTwist twist(::igris_c_sdk::msg::WalkTwist::_twist_type arg)
  {
    msg_.twist = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::WalkTwist msg_;
};

class Init_WalkTwist_header
{
public:
  Init_WalkTwist_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_WalkTwist_twist header(::igris_c_sdk::msg::WalkTwist::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_WalkTwist_twist(msg_);
  }

private:
  ::igris_c_sdk::msg::WalkTwist msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::WalkTwist>()
{
  return igris_c_sdk::msg::builder::Init_WalkTwist_header();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__WALK_TWIST__BUILDER_HPP_
