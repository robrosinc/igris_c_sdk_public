// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from igris_c_sdk:msg/LowCmd.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "igris_c_sdk/msg/detail/low_cmd__functions.h"
#include "igris_c_sdk/msg/detail/low_cmd__struct.hpp"
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

void LowCmd_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) igris_c_sdk::msg::LowCmd(_init);
}

void LowCmd_fini_function(void * message_memory)
{
  auto typed_message = static_cast<igris_c_sdk::msg::LowCmd *>(message_memory);
  typed_message->~LowCmd();
}

size_t size_function__LowCmd__kinematic_modes(const void * untyped_member)
{
  (void)untyped_member;
  return 5;
}

const void * get_const_function__LowCmd__kinematic_modes(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<uint8_t, 5> *>(untyped_member);
  return &member[index];
}

void * get_function__LowCmd__kinematic_modes(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<uint8_t, 5> *>(untyped_member);
  return &member[index];
}

void fetch_function__LowCmd__kinematic_modes(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint8_t *>(
    get_const_function__LowCmd__kinematic_modes(untyped_member, index));
  auto & value = *reinterpret_cast<uint8_t *>(untyped_value);
  value = item;
}

void assign_function__LowCmd__kinematic_modes(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint8_t *>(
    get_function__LowCmd__kinematic_modes(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint8_t *>(untyped_value);
  item = value;
}

size_t size_function__LowCmd__motors(const void * untyped_member)
{
  (void)untyped_member;
  return 31;
}

const void * get_const_function__LowCmd__motors(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<igris_c_sdk::msg::MotorCmd, 31> *>(untyped_member);
  return &member[index];
}

void * get_function__LowCmd__motors(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<igris_c_sdk::msg::MotorCmd, 31> *>(untyped_member);
  return &member[index];
}

void fetch_function__LowCmd__motors(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const igris_c_sdk::msg::MotorCmd *>(
    get_const_function__LowCmd__motors(untyped_member, index));
  auto & value = *reinterpret_cast<igris_c_sdk::msg::MotorCmd *>(untyped_value);
  value = item;
}

void assign_function__LowCmd__motors(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<igris_c_sdk::msg::MotorCmd *>(
    get_function__LowCmd__motors(untyped_member, index));
  const auto & value = *reinterpret_cast<const igris_c_sdk::msg::MotorCmd *>(untyped_value);
  item = value;
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember LowCmd_message_member_array[3] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<igris_c_sdk::msg::Header>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::msg::LowCmd, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "kinematic_modes",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    5,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::msg::LowCmd, kinematic_modes),  // bytes offset in struct
    nullptr,  // default value
    size_function__LowCmd__kinematic_modes,  // size() function pointer
    get_const_function__LowCmd__kinematic_modes,  // get_const(index) function pointer
    get_function__LowCmd__kinematic_modes,  // get(index) function pointer
    fetch_function__LowCmd__kinematic_modes,  // fetch(index, &value) function pointer
    assign_function__LowCmd__kinematic_modes,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "motors",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<igris_c_sdk::msg::MotorCmd>(),  // members of sub message
    false,  // is key
    true,  // is array
    31,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk::msg::LowCmd, motors),  // bytes offset in struct
    nullptr,  // default value
    size_function__LowCmd__motors,  // size() function pointer
    get_const_function__LowCmd__motors,  // get_const(index) function pointer
    get_function__LowCmd__motors,  // get(index) function pointer
    fetch_function__LowCmd__motors,  // fetch(index, &value) function pointer
    assign_function__LowCmd__motors,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers LowCmd_message_members = {
  "igris_c_sdk::msg",  // message namespace
  "LowCmd",  // message name
  3,  // number of fields
  sizeof(igris_c_sdk::msg::LowCmd),
  false,  // has_any_key_member_
  LowCmd_message_member_array,  // message members
  LowCmd_init_function,  // function to initialize message memory (memory has to be allocated)
  LowCmd_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t LowCmd_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &LowCmd_message_members,
  get_message_typesupport_handle_function,
  &igris_c_sdk__msg__LowCmd__get_type_hash,
  &igris_c_sdk__msg__LowCmd__get_type_description,
  &igris_c_sdk__msg__LowCmd__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace igris_c_sdk


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<igris_c_sdk::msg::LowCmd>()
{
  return &::igris_c_sdk::msg::rosidl_typesupport_introspection_cpp::LowCmd_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, igris_c_sdk, msg, LowCmd)() {
  return &::igris_c_sdk::msg::rosidl_typesupport_introspection_cpp::LowCmd_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
