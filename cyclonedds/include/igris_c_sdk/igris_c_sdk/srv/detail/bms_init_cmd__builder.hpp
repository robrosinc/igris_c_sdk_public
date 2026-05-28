// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:srv/BmsInitCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/srv/bms_init_cmd.hpp"


#ifndef IGRIS_C_SDK__SRV__DETAIL__BMS_INIT_CMD__BUILDER_HPP_
#define IGRIS_C_SDK__SRV__DETAIL__BMS_INIT_CMD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/srv/detail/bms_init_cmd__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace srv
{

namespace builder
{

class Init_BmsInitCmd_Request_init
{
public:
  explicit Init_BmsInitCmd_Request_init(::igris_c_sdk::srv::BmsInitCmd_Request & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::srv::BmsInitCmd_Request init(::igris_c_sdk::srv::BmsInitCmd_Request::_init_type arg)
  {
    msg_.init = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::srv::BmsInitCmd_Request msg_;
};

class Init_BmsInitCmd_Request_request_id
{
public:
  explicit Init_BmsInitCmd_Request_request_id(::igris_c_sdk::srv::BmsInitCmd_Request & msg)
  : msg_(msg)
  {}
  Init_BmsInitCmd_Request_init request_id(::igris_c_sdk::srv::BmsInitCmd_Request::_request_id_type arg)
  {
    msg_.request_id = std::move(arg);
    return Init_BmsInitCmd_Request_init(msg_);
  }

private:
  ::igris_c_sdk::srv::BmsInitCmd_Request msg_;
};

class Init_BmsInitCmd_Request_header
{
public:
  Init_BmsInitCmd_Request_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_BmsInitCmd_Request_request_id header(::igris_c_sdk::srv::BmsInitCmd_Request::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_BmsInitCmd_Request_request_id(msg_);
  }

private:
  ::igris_c_sdk::srv::BmsInitCmd_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::srv::BmsInitCmd_Request>()
{
  return igris_c_sdk::srv::builder::Init_BmsInitCmd_Request_header();
}

}  // namespace igris_c_sdk


namespace igris_c_sdk
{

namespace srv
{

namespace builder
{

class Init_BmsInitCmd_Response_error_code
{
public:
  explicit Init_BmsInitCmd_Response_error_code(::igris_c_sdk::srv::BmsInitCmd_Response & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::srv::BmsInitCmd_Response error_code(::igris_c_sdk::srv::BmsInitCmd_Response::_error_code_type arg)
  {
    msg_.error_code = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::srv::BmsInitCmd_Response msg_;
};

class Init_BmsInitCmd_Response_message
{
public:
  explicit Init_BmsInitCmd_Response_message(::igris_c_sdk::srv::BmsInitCmd_Response & msg)
  : msg_(msg)
  {}
  Init_BmsInitCmd_Response_error_code message(::igris_c_sdk::srv::BmsInitCmd_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return Init_BmsInitCmd_Response_error_code(msg_);
  }

private:
  ::igris_c_sdk::srv::BmsInitCmd_Response msg_;
};

class Init_BmsInitCmd_Response_success
{
public:
  explicit Init_BmsInitCmd_Response_success(::igris_c_sdk::srv::BmsInitCmd_Response & msg)
  : msg_(msg)
  {}
  Init_BmsInitCmd_Response_message success(::igris_c_sdk::srv::BmsInitCmd_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_BmsInitCmd_Response_message(msg_);
  }

private:
  ::igris_c_sdk::srv::BmsInitCmd_Response msg_;
};

class Init_BmsInitCmd_Response_request_id
{
public:
  explicit Init_BmsInitCmd_Response_request_id(::igris_c_sdk::srv::BmsInitCmd_Response & msg)
  : msg_(msg)
  {}
  Init_BmsInitCmd_Response_success request_id(::igris_c_sdk::srv::BmsInitCmd_Response::_request_id_type arg)
  {
    msg_.request_id = std::move(arg);
    return Init_BmsInitCmd_Response_success(msg_);
  }

private:
  ::igris_c_sdk::srv::BmsInitCmd_Response msg_;
};

class Init_BmsInitCmd_Response_header
{
public:
  Init_BmsInitCmd_Response_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_BmsInitCmd_Response_request_id header(::igris_c_sdk::srv::BmsInitCmd_Response::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_BmsInitCmd_Response_request_id(msg_);
  }

private:
  ::igris_c_sdk::srv::BmsInitCmd_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::srv::BmsInitCmd_Response>()
{
  return igris_c_sdk::srv::builder::Init_BmsInitCmd_Response_header();
}

}  // namespace igris_c_sdk


namespace igris_c_sdk
{

namespace srv
{

namespace builder
{

class Init_BmsInitCmd_Event_response
{
public:
  explicit Init_BmsInitCmd_Event_response(::igris_c_sdk::srv::BmsInitCmd_Event & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::srv::BmsInitCmd_Event response(::igris_c_sdk::srv::BmsInitCmd_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::srv::BmsInitCmd_Event msg_;
};

class Init_BmsInitCmd_Event_request
{
public:
  explicit Init_BmsInitCmd_Event_request(::igris_c_sdk::srv::BmsInitCmd_Event & msg)
  : msg_(msg)
  {}
  Init_BmsInitCmd_Event_response request(::igris_c_sdk::srv::BmsInitCmd_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_BmsInitCmd_Event_response(msg_);
  }

private:
  ::igris_c_sdk::srv::BmsInitCmd_Event msg_;
};

class Init_BmsInitCmd_Event_info
{
public:
  Init_BmsInitCmd_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_BmsInitCmd_Event_request info(::igris_c_sdk::srv::BmsInitCmd_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_BmsInitCmd_Event_request(msg_);
  }

private:
  ::igris_c_sdk::srv::BmsInitCmd_Event msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::srv::BmsInitCmd_Event>()
{
  return igris_c_sdk::srv::builder::Init_BmsInitCmd_Event_info();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__SRV__DETAIL__BMS_INIT_CMD__BUILDER_HPP_
