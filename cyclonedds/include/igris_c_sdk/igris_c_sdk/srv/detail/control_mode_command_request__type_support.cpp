// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from igris_c_sdk:srv/ControlModeCommandRequest.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "igris_c_sdk/srv/detail/control_mode_command_request__functions.h"
#include "igris_c_sdk/srv/detail/control_mode_command_request__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace igris_c_sdk
{

namespace srv
{

namespace rosidl_typesupport_introspection_cpp
{

void ControlModeCommandRequest_Request_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) igris_c_sdk::srv::ControlModeCommandRequest_Request(_init);
}

void ControlModeCommandRequest_Request_fini_function(void * message_memory)
{
  auto typed_message = static_cast<igris_c_sdk::srv::ControlModeCommandRequest_Request *>(message_memory);
  typed_message->~ControlModeCommandRequest_Request();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember ControlModeCommandRequest_Request_message_member_array[5] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<igris_c_sdk::msg::Header>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Request, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "request_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Request, request_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "command_type",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Request, command_type),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "preset_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Request, preset_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "is_cyclic",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Request, is_cyclic),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers ControlModeCommandRequest_Request_message_members = {
  "igris_c_sdk::srv",  // message namespace
  "ControlModeCommandRequest_Request",  // message name
  5,  // number of fields
  sizeof(igris_c_sdk::srv::ControlModeCommandRequest_Request),
  false,  // has_any_key_member_
  ControlModeCommandRequest_Request_message_member_array,  // message members
  ControlModeCommandRequest_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  ControlModeCommandRequest_Request_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t ControlModeCommandRequest_Request_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &ControlModeCommandRequest_Request_message_members,
  get_message_typesupport_handle_function,
  &igris_c_sdk__srv__ControlModeCommandRequest_Request__get_type_hash,
  &igris_c_sdk__srv__ControlModeCommandRequest_Request__get_type_description,
  &igris_c_sdk__srv__ControlModeCommandRequest_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace srv

}  // namespace igris_c_sdk


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<igris_c_sdk::srv::ControlModeCommandRequest_Request>()
{
  return &::igris_c_sdk::srv::rosidl_typesupport_introspection_cpp::ControlModeCommandRequest_Request_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, igris_c_sdk, srv, ControlModeCommandRequest_Request)() {
  return &::igris_c_sdk::srv::rosidl_typesupport_introspection_cpp::ControlModeCommandRequest_Request_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "igris_c_sdk/srv/detail/control_mode_command_request__functions.h"
// already included above
// #include "igris_c_sdk/srv/detail/control_mode_command_request__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace igris_c_sdk
{

namespace srv
{

namespace rosidl_typesupport_introspection_cpp
{

void ControlModeCommandRequest_Response_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) igris_c_sdk::srv::ControlModeCommandRequest_Response(_init);
}

void ControlModeCommandRequest_Response_fini_function(void * message_memory)
{
  auto typed_message = static_cast<igris_c_sdk::srv::ControlModeCommandRequest_Response *>(message_memory);
  typed_message->~ControlModeCommandRequest_Response();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember ControlModeCommandRequest_Response_message_member_array[5] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<igris_c_sdk::msg::Header>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Response, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "request_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Response, request_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "success",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Response, success),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "message",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Response, message),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "error_code",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Response, error_code),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers ControlModeCommandRequest_Response_message_members = {
  "igris_c_sdk::srv",  // message namespace
  "ControlModeCommandRequest_Response",  // message name
  5,  // number of fields
  sizeof(igris_c_sdk::srv::ControlModeCommandRequest_Response),
  false,  // has_any_key_member_
  ControlModeCommandRequest_Response_message_member_array,  // message members
  ControlModeCommandRequest_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  ControlModeCommandRequest_Response_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t ControlModeCommandRequest_Response_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &ControlModeCommandRequest_Response_message_members,
  get_message_typesupport_handle_function,
  &igris_c_sdk__srv__ControlModeCommandRequest_Response__get_type_hash,
  &igris_c_sdk__srv__ControlModeCommandRequest_Response__get_type_description,
  &igris_c_sdk__srv__ControlModeCommandRequest_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace srv

}  // namespace igris_c_sdk


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<igris_c_sdk::srv::ControlModeCommandRequest_Response>()
{
  return &::igris_c_sdk::srv::rosidl_typesupport_introspection_cpp::ControlModeCommandRequest_Response_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, igris_c_sdk, srv, ControlModeCommandRequest_Response)() {
  return &::igris_c_sdk::srv::rosidl_typesupport_introspection_cpp::ControlModeCommandRequest_Response_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "igris_c_sdk/srv/detail/control_mode_command_request__functions.h"
// already included above
// #include "igris_c_sdk/srv/detail/control_mode_command_request__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace igris_c_sdk
{

namespace srv
{

namespace rosidl_typesupport_introspection_cpp
{

void ControlModeCommandRequest_Event_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) igris_c_sdk::srv::ControlModeCommandRequest_Event(_init);
}

void ControlModeCommandRequest_Event_fini_function(void * message_memory)
{
  auto typed_message = static_cast<igris_c_sdk::srv::ControlModeCommandRequest_Event *>(message_memory);
  typed_message->~ControlModeCommandRequest_Event();
}

size_t size_function__ControlModeCommandRequest_Event__request(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<igris_c_sdk::srv::ControlModeCommandRequest_Request> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ControlModeCommandRequest_Event__request(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<igris_c_sdk::srv::ControlModeCommandRequest_Request> *>(untyped_member);
  return &member[index];
}

void * get_function__ControlModeCommandRequest_Event__request(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<igris_c_sdk::srv::ControlModeCommandRequest_Request> *>(untyped_member);
  return &member[index];
}

void fetch_function__ControlModeCommandRequest_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const igris_c_sdk::srv::ControlModeCommandRequest_Request *>(
    get_const_function__ControlModeCommandRequest_Event__request(untyped_member, index));
  auto & value = *reinterpret_cast<igris_c_sdk::srv::ControlModeCommandRequest_Request *>(untyped_value);
  value = item;
}

void assign_function__ControlModeCommandRequest_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<igris_c_sdk::srv::ControlModeCommandRequest_Request *>(
    get_function__ControlModeCommandRequest_Event__request(untyped_member, index));
  const auto & value = *reinterpret_cast<const igris_c_sdk::srv::ControlModeCommandRequest_Request *>(untyped_value);
  item = value;
}

void resize_function__ControlModeCommandRequest_Event__request(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<igris_c_sdk::srv::ControlModeCommandRequest_Request> *>(untyped_member);
  member->resize(size);
}

size_t size_function__ControlModeCommandRequest_Event__response(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<igris_c_sdk::srv::ControlModeCommandRequest_Response> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ControlModeCommandRequest_Event__response(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<igris_c_sdk::srv::ControlModeCommandRequest_Response> *>(untyped_member);
  return &member[index];
}

void * get_function__ControlModeCommandRequest_Event__response(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<igris_c_sdk::srv::ControlModeCommandRequest_Response> *>(untyped_member);
  return &member[index];
}

void fetch_function__ControlModeCommandRequest_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const igris_c_sdk::srv::ControlModeCommandRequest_Response *>(
    get_const_function__ControlModeCommandRequest_Event__response(untyped_member, index));
  auto & value = *reinterpret_cast<igris_c_sdk::srv::ControlModeCommandRequest_Response *>(untyped_value);
  value = item;
}

void assign_function__ControlModeCommandRequest_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<igris_c_sdk::srv::ControlModeCommandRequest_Response *>(
    get_function__ControlModeCommandRequest_Event__response(untyped_member, index));
  const auto & value = *reinterpret_cast<const igris_c_sdk::srv::ControlModeCommandRequest_Response *>(untyped_value);
  item = value;
}

void resize_function__ControlModeCommandRequest_Event__response(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<igris_c_sdk::srv::ControlModeCommandRequest_Response> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember ControlModeCommandRequest_Event_message_member_array[3] = {
  {
    "info",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<service_msgs::msg::ServiceEventInfo>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Event, info),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "request",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<igris_c_sdk::srv::ControlModeCommandRequest_Request>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Event, request),  // bytes offset in struct
    nullptr,  // default value
    size_function__ControlModeCommandRequest_Event__request,  // size() function pointer
    get_const_function__ControlModeCommandRequest_Event__request,  // get_const(index) function pointer
    get_function__ControlModeCommandRequest_Event__request,  // get(index) function pointer
    fetch_function__ControlModeCommandRequest_Event__request,  // fetch(index, &value) function pointer
    assign_function__ControlModeCommandRequest_Event__request,  // assign(index, value) function pointer
    resize_function__ControlModeCommandRequest_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<igris_c_sdk::srv::ControlModeCommandRequest_Response>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(igris_c_sdk::srv::ControlModeCommandRequest_Event, response),  // bytes offset in struct
    nullptr,  // default value
    size_function__ControlModeCommandRequest_Event__response,  // size() function pointer
    get_const_function__ControlModeCommandRequest_Event__response,  // get_const(index) function pointer
    get_function__ControlModeCommandRequest_Event__response,  // get(index) function pointer
    fetch_function__ControlModeCommandRequest_Event__response,  // fetch(index, &value) function pointer
    assign_function__ControlModeCommandRequest_Event__response,  // assign(index, value) function pointer
    resize_function__ControlModeCommandRequest_Event__response  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers ControlModeCommandRequest_Event_message_members = {
  "igris_c_sdk::srv",  // message namespace
  "ControlModeCommandRequest_Event",  // message name
  3,  // number of fields
  sizeof(igris_c_sdk::srv::ControlModeCommandRequest_Event),
  false,  // has_any_key_member_
  ControlModeCommandRequest_Event_message_member_array,  // message members
  ControlModeCommandRequest_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  ControlModeCommandRequest_Event_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t ControlModeCommandRequest_Event_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &ControlModeCommandRequest_Event_message_members,
  get_message_typesupport_handle_function,
  &igris_c_sdk__srv__ControlModeCommandRequest_Event__get_type_hash,
  &igris_c_sdk__srv__ControlModeCommandRequest_Event__get_type_description,
  &igris_c_sdk__srv__ControlModeCommandRequest_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace srv

}  // namespace igris_c_sdk


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<igris_c_sdk::srv::ControlModeCommandRequest_Event>()
{
  return &::igris_c_sdk::srv::rosidl_typesupport_introspection_cpp::ControlModeCommandRequest_Event_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, igris_c_sdk, srv, ControlModeCommandRequest_Event)() {
  return &::igris_c_sdk::srv::rosidl_typesupport_introspection_cpp::ControlModeCommandRequest_Event_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"
// already included above
// #include "igris_c_sdk/srv/detail/control_mode_command_request__functions.h"
// already included above
// #include "igris_c_sdk/srv/detail/control_mode_command_request__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/service_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/service_type_support_decl.hpp"

namespace igris_c_sdk
{

namespace srv
{

namespace rosidl_typesupport_introspection_cpp
{

// this is intentionally not const to allow initialization later to prevent an initialization race
static ::rosidl_typesupport_introspection_cpp::ServiceMembers ControlModeCommandRequest_service_members = {
  "igris_c_sdk::srv",  // service namespace
  "ControlModeCommandRequest",  // service name
  // the following fields are initialized below on first access
  // see get_service_type_support_handle<igris_c_sdk::srv::ControlModeCommandRequest>()
  nullptr,  // request message
  nullptr,  // response message
  nullptr,  // event message
};

static const rosidl_service_type_support_t ControlModeCommandRequest_service_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &ControlModeCommandRequest_service_members,
  get_service_typesupport_handle_function,
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<igris_c_sdk::srv::ControlModeCommandRequest_Request>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<igris_c_sdk::srv::ControlModeCommandRequest_Response>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<igris_c_sdk::srv::ControlModeCommandRequest_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<igris_c_sdk::srv::ControlModeCommandRequest>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<igris_c_sdk::srv::ControlModeCommandRequest>,
  &igris_c_sdk__srv__ControlModeCommandRequest__get_type_hash,
  &igris_c_sdk__srv__ControlModeCommandRequest__get_type_description,
  &igris_c_sdk__srv__ControlModeCommandRequest__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace srv

}  // namespace igris_c_sdk


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<igris_c_sdk::srv::ControlModeCommandRequest>()
{
  // get a handle to the value to be returned
  auto service_type_support =
    &::igris_c_sdk::srv::rosidl_typesupport_introspection_cpp::ControlModeCommandRequest_service_type_support_handle;
  // get a non-const and properly typed version of the data void *
  auto service_members = const_cast<::rosidl_typesupport_introspection_cpp::ServiceMembers *>(
    static_cast<const ::rosidl_typesupport_introspection_cpp::ServiceMembers *>(
      service_type_support->data));
  // make sure all of the service_members are initialized
  // if they are not, initialize them
  if (
    service_members->request_members_ == nullptr ||
    service_members->response_members_ == nullptr ||
    service_members->event_members_ == nullptr)
  {
    // initialize the request_members_ with the static function from the external library
    service_members->request_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::igris_c_sdk::srv::ControlModeCommandRequest_Request
      >()->data
      );
    // initialize the response_members_ with the static function from the external library
    service_members->response_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::igris_c_sdk::srv::ControlModeCommandRequest_Response
      >()->data
      );
    // initialize the event_members_ with the static function from the external library
    service_members->event_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::igris_c_sdk::srv::ControlModeCommandRequest_Event
      >()->data
      );
  }
  // finally return the properly initialized service_type_support handle
  return service_type_support;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, igris_c_sdk, srv, ControlModeCommandRequest)() {
  return ::rosidl_typesupport_introspection_cpp::get_service_type_support_handle<igris_c_sdk::srv::ControlModeCommandRequest>();
}

#ifdef __cplusplus
}
#endif
