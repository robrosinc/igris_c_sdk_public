// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/Header.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/header.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__HEADER__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__HEADER__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/header__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_Header_frame_id
{
public:
  explicit Init_Header_frame_id(::igris_c_sdk::msg::Header & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::Header frame_id(::igris_c_sdk::msg::Header::_frame_id_type arg)
  {
    msg_.frame_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::Header msg_;
};

class Init_Header_nanosec
{
public:
  explicit Init_Header_nanosec(::igris_c_sdk::msg::Header & msg)
  : msg_(msg)
  {}
  Init_Header_frame_id nanosec(::igris_c_sdk::msg::Header::_nanosec_type arg)
  {
    msg_.nanosec = std::move(arg);
    return Init_Header_frame_id(msg_);
  }

private:
  ::igris_c_sdk::msg::Header msg_;
};

class Init_Header_sec
{
public:
  explicit Init_Header_sec(::igris_c_sdk::msg::Header & msg)
  : msg_(msg)
  {}
  Init_Header_nanosec sec(::igris_c_sdk::msg::Header::_sec_type arg)
  {
    msg_.sec = std::move(arg);
    return Init_Header_nanosec(msg_);
  }

private:
  ::igris_c_sdk::msg::Header msg_;
};

class Init_Header_seq
{
public:
  Init_Header_seq()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Header_sec seq(::igris_c_sdk::msg::Header::_seq_type arg)
  {
    msg_.seq = std::move(arg);
    return Init_Header_sec(msg_);
  }

private:
  ::igris_c_sdk::msg::Header msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::Header>()
{
  return igris_c_sdk::msg::builder::Init_Header_seq();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__HEADER__BUILDER_HPP_
