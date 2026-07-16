// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:msg/ServiceResponse.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/service_response.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__SERVICE_RESPONSE__BUILDER_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__SERVICE_RESPONSE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/msg/detail/service_response__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace msg
{

namespace builder
{

class Init_ServiceResponse_error_code
{
public:
  explicit Init_ServiceResponse_error_code(::igris_c_sdk::msg::ServiceResponse & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::msg::ServiceResponse error_code(::igris_c_sdk::msg::ServiceResponse::_error_code_type arg)
  {
    msg_.error_code = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::msg::ServiceResponse msg_;
};

class Init_ServiceResponse_message
{
public:
  explicit Init_ServiceResponse_message(::igris_c_sdk::msg::ServiceResponse & msg)
  : msg_(msg)
  {}
  Init_ServiceResponse_error_code message(::igris_c_sdk::msg::ServiceResponse::_message_type arg)
  {
    msg_.message = std::move(arg);
    return Init_ServiceResponse_error_code(msg_);
  }

private:
  ::igris_c_sdk::msg::ServiceResponse msg_;
};

class Init_ServiceResponse_success
{
public:
  explicit Init_ServiceResponse_success(::igris_c_sdk::msg::ServiceResponse & msg)
  : msg_(msg)
  {}
  Init_ServiceResponse_message success(::igris_c_sdk::msg::ServiceResponse::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_ServiceResponse_message(msg_);
  }

private:
  ::igris_c_sdk::msg::ServiceResponse msg_;
};

class Init_ServiceResponse_request_id
{
public:
  explicit Init_ServiceResponse_request_id(::igris_c_sdk::msg::ServiceResponse & msg)
  : msg_(msg)
  {}
  Init_ServiceResponse_success request_id(::igris_c_sdk::msg::ServiceResponse::_request_id_type arg)
  {
    msg_.request_id = std::move(arg);
    return Init_ServiceResponse_success(msg_);
  }

private:
  ::igris_c_sdk::msg::ServiceResponse msg_;
};

class Init_ServiceResponse_header
{
public:
  Init_ServiceResponse_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ServiceResponse_request_id header(::igris_c_sdk::msg::ServiceResponse::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ServiceResponse_request_id(msg_);
  }

private:
  ::igris_c_sdk::msg::ServiceResponse msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::msg::ServiceResponse>()
{
  return igris_c_sdk::msg::builder::Init_ServiceResponse_header();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__SERVICE_RESPONSE__BUILDER_HPP_
