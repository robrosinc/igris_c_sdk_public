// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from igris_c_sdk:msg/WalkTwist.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "igris_c_sdk/msg/detail/walk_twist__functions.h"
#include "igris_c_sdk/msg/detail/walk_twist__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace igris_c_sdk
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void WalkTwist_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) igris_c_sdk::msg::WalkTwist(_init);
}

void WalkTwist_fini_function(void * message_memory)
{
  auto typed_message = static_cast<igris_c_sdk::msg::WalkTwist *>(message_memory);
  typed_message->~WalkTwist();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember WalkTwist_message_member_array[2] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<igris_c_sdk::msg::Header>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::msg::WalkTwist, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "twist",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<geometry_msgs::msg::Twist>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::msg::WalkTwist, twist),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers WalkTwist_message_members = {
  "igris_c_sdk::msg",  // message namespace
  "WalkTwist",  // message name
  2,  // number of fields
  sizeof(igris_c_sdk::msg::WalkTwist),
  false,  // has_any_key_member_
  WalkTwist_message_member_array,  // message members
  WalkTwist_init_function,  // function to initialize message memory (memory has to be allocated)
  WalkTwist_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t WalkTwist_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &WalkTwist_message_members,
  get_message_typesupport_handle_function,
  &igris_c_sdk__msg__WalkTwist__get_type_hash,
  &igris_c_sdk__msg__WalkTwist__get_type_description,
  &igris_c_sdk__msg__WalkTwist__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace igris_c_sdk


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<igris_c_sdk::msg::WalkTwist>()
{
  return &::igris_c_sdk::msg::rosidl_typesupport_introspection_cpp::WalkTwist_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, igris_c_sdk, msg, WalkTwist)() {
  return &::igris_c_sdk::msg::rosidl_typesupport_introspection_cpp::WalkTwist_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
