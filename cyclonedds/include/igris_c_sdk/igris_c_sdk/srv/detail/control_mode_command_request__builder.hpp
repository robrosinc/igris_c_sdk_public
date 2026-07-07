// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from igris_c_sdk:srv/ControlModeCommandRequest.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/srv/control_mode_command_request.hpp"


#ifndef IGRIS_C_SDK__SRV__DETAIL__CONTROL_MODE_COMMAND_REQUEST__BUILDER_HPP_
#define IGRIS_C_SDK__SRV__DETAIL__CONTROL_MODE_COMMAND_REQUEST__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "igris_c_sdk/srv/detail/control_mode_command_request__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace igris_c_sdk
{

namespace srv
{

namespace builder
{

class Init_ControlModeCommandRequest_Request_is_cyclic
{
public:
  explicit Init_ControlModeCommandRequest_Request_is_cyclic(::igris_c_sdk::srv::ControlModeCommandRequest_Request & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::srv::ControlModeCommandRequest_Request is_cyclic(::igris_c_sdk::srv::ControlModeCommandRequest_Request::_is_cyclic_type arg)
  {
    msg_.is_cyclic = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Request msg_;
};

class Init_ControlModeCommandRequest_Request_preset_id
{
public:
  explicit Init_ControlModeCommandRequest_Request_preset_id(::igris_c_sdk::srv::ControlModeCommandRequest_Request & msg)
  : msg_(msg)
  {}
  Init_ControlModeCommandRequest_Request_is_cyclic preset_id(::igris_c_sdk::srv::ControlModeCommandRequest_Request::_preset_id_type arg)
  {
    msg_.preset_id = std::move(arg);
    return Init_ControlModeCommandRequest_Request_is_cyclic(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Request msg_;
};

class Init_ControlModeCommandRequest_Request_command_type
{
public:
  explicit Init_ControlModeCommandRequest_Request_command_type(::igris_c_sdk::srv::ControlModeCommandRequest_Request & msg)
  : msg_(msg)
  {}
  Init_ControlModeCommandRequest_Request_preset_id command_type(::igris_c_sdk::srv::ControlModeCommandRequest_Request::_command_type_type arg)
  {
    msg_.command_type = std::move(arg);
    return Init_ControlModeCommandRequest_Request_preset_id(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Request msg_;
};

class Init_ControlModeCommandRequest_Request_request_id
{
public:
  explicit Init_ControlModeCommandRequest_Request_request_id(::igris_c_sdk::srv::ControlModeCommandRequest_Request & msg)
  : msg_(msg)
  {}
  Init_ControlModeCommandRequest_Request_command_type request_id(::igris_c_sdk::srv::ControlModeCommandRequest_Request::_request_id_type arg)
  {
    msg_.request_id = std::move(arg);
    return Init_ControlModeCommandRequest_Request_command_type(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Request msg_;
};

class Init_ControlModeCommandRequest_Request_header
{
public:
  Init_ControlModeCommandRequest_Request_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ControlModeCommandRequest_Request_request_id header(::igris_c_sdk::srv::ControlModeCommandRequest_Request::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ControlModeCommandRequest_Request_request_id(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::srv::ControlModeCommandRequest_Request>()
{
  return igris_c_sdk::srv::builder::Init_ControlModeCommandRequest_Request_header();
}

}  // namespace igris_c_sdk


namespace igris_c_sdk
{

namespace srv
{

namespace builder
{

class Init_ControlModeCommandRequest_Response_error_code
{
public:
  explicit Init_ControlModeCommandRequest_Response_error_code(::igris_c_sdk::srv::ControlModeCommandRequest_Response & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::srv::ControlModeCommandRequest_Response error_code(::igris_c_sdk::srv::ControlModeCommandRequest_Response::_error_code_type arg)
  {
    msg_.error_code = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Response msg_;
};

class Init_ControlModeCommandRequest_Response_message
{
public:
  explicit Init_ControlModeCommandRequest_Response_message(::igris_c_sdk::srv::ControlModeCommandRequest_Response & msg)
  : msg_(msg)
  {}
  Init_ControlModeCommandRequest_Response_error_code message(::igris_c_sdk::srv::ControlModeCommandRequest_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return Init_ControlModeCommandRequest_Response_error_code(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Response msg_;
};

class Init_ControlModeCommandRequest_Response_success
{
public:
  explicit Init_ControlModeCommandRequest_Response_success(::igris_c_sdk::srv::ControlModeCommandRequest_Response & msg)
  : msg_(msg)
  {}
  Init_ControlModeCommandRequest_Response_message success(::igris_c_sdk::srv::ControlModeCommandRequest_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_ControlModeCommandRequest_Response_message(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Response msg_;
};

class Init_ControlModeCommandRequest_Response_request_id
{
public:
  explicit Init_ControlModeCommandRequest_Response_request_id(::igris_c_sdk::srv::ControlModeCommandRequest_Response & msg)
  : msg_(msg)
  {}
  Init_ControlModeCommandRequest_Response_success request_id(::igris_c_sdk::srv::ControlModeCommandRequest_Response::_request_id_type arg)
  {
    msg_.request_id = std::move(arg);
    return Init_ControlModeCommandRequest_Response_success(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Response msg_;
};

class Init_ControlModeCommandRequest_Response_header
{
public:
  Init_ControlModeCommandRequest_Response_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ControlModeCommandRequest_Response_request_id header(::igris_c_sdk::srv::ControlModeCommandRequest_Response::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ControlModeCommandRequest_Response_request_id(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::srv::ControlModeCommandRequest_Response>()
{
  return igris_c_sdk::srv::builder::Init_ControlModeCommandRequest_Response_header();
}

}  // namespace igris_c_sdk


namespace igris_c_sdk
{

namespace srv
{

namespace builder
{

class Init_ControlModeCommandRequest_Event_response
{
public:
  explicit Init_ControlModeCommandRequest_Event_response(::igris_c_sdk::srv::ControlModeCommandRequest_Event & msg)
  : msg_(msg)
  {}
  ::igris_c_sdk::srv::ControlModeCommandRequest_Event response(::igris_c_sdk::srv::ControlModeCommandRequest_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Event msg_;
};

class Init_ControlModeCommandRequest_Event_request
{
public:
  explicit Init_ControlModeCommandRequest_Event_request(::igris_c_sdk::srv::ControlModeCommandRequest_Event & msg)
  : msg_(msg)
  {}
  Init_ControlModeCommandRequest_Event_response request(::igris_c_sdk::srv::ControlModeCommandRequest_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_ControlModeCommandRequest_Event_response(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Event msg_;
};

class Init_ControlModeCommandRequest_Event_info
{
public:
  Init_ControlModeCommandRequest_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ControlModeCommandRequest_Event_request info(::igris_c_sdk::srv::ControlModeCommandRequest_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_ControlModeCommandRequest_Event_request(msg_);
  }

private:
  ::igris_c_sdk::srv::ControlModeCommandRequest_Event msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::igris_c_sdk::srv::ControlModeCommandRequest_Event>()
{
  return igris_c_sdk::srv::builder::Init_ControlModeCommandRequest_Event_info();
}

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__SRV__DETAIL__CONTROL_MODE_COMMAND_REQUEST__BUILDER_HPP_
