// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from igris_c_sdk:msg/LowCmd.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "igris_c_sdk/msg/detail/low_cmd__rosidl_typesupport_introspection_c.h"
#include "igris_c_sdk/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "igris_c_sdk/msg/detail/low_cmd__functions.h"
#include "igris_c_sdk/msg/detail/low_cmd__struct.h"


// Include directives for member types
// Member `header`
#include "igris_c_sdk/msg/header.h"
// Member `header`
#include "igris_c_sdk/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `motors`
#include "igris_c_sdk/msg/motor_cmd.h"
// Member `motors`
#include "igris_c_sdk/msg/detail/motor_cmd__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  igris_c_sdk__msg__LowCmd__init(message_memory);
}

void igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_fini_function(void * message_memory)
{
  igris_c_sdk__msg__LowCmd__fini(message_memory);
}

size_t igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__size_function__LowCmd__kinematic_modes(
  const void * untyped_member)
{
  (void)untyped_member;
  return 5;
}

const void * igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_const_function__LowCmd__kinematic_modes(
  const void * untyped_member, size_t index)
{
  const uint8_t * member =
    (const uint8_t *)(untyped_member);
  return &member[index];
}

void * igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_function__LowCmd__kinematic_modes(
  void * untyped_member, size_t index)
{
  uint8_t * member =
    (uint8_t *)(untyped_member);
  return &member[index];
}

void igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__fetch_function__LowCmd__kinematic_modes(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_const_function__LowCmd__kinematic_modes(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__assign_function__LowCmd__kinematic_modes(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_function__LowCmd__kinematic_modes(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

size_t igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__size_function__LowCmd__motors(
  const void * untyped_member)
{
  (void)untyped_member;
  return 31;
}

const void * igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_const_function__LowCmd__motors(
  const void * untyped_member, size_t index)
{
  const igris_c_sdk__msg__MotorCmd * member =
    (const igris_c_sdk__msg__MotorCmd *)(untyped_member);
  return &member[index];
}

void * igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_function__LowCmd__motors(
  void * untyped_member, size_t index)
{
  igris_c_sdk__msg__MotorCmd * member =
    (igris_c_sdk__msg__MotorCmd *)(untyped_member);
  return &member[index];
}

void igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__fetch_function__LowCmd__motors(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const igris_c_sdk__msg__MotorCmd * item =
    ((const igris_c_sdk__msg__MotorCmd *)
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_const_function__LowCmd__motors(untyped_member, index));
  igris_c_sdk__msg__MotorCmd * value =
    (igris_c_sdk__msg__MotorCmd *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__assign_function__LowCmd__motors(
  void * untyped_member, size_t index, const void * untyped_value)
{
  igris_c_sdk__msg__MotorCmd * item =
    ((igris_c_sdk__msg__MotorCmd *)
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_function__LowCmd__motors(untyped_member, index));
  const igris_c_sdk__msg__MotorCmd * value =
    (const igris_c_sdk__msg__MotorCmd *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_message_member_array[3] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__LowCmd, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "kinematic_modes",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    5,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__LowCmd, kinematic_modes),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__size_function__LowCmd__kinematic_modes,  // size() function pointer
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_const_function__LowCmd__kinematic_modes,  // get_const(index) function pointer
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_function__LowCmd__kinematic_modes,  // get(index) function pointer
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__fetch_function__LowCmd__kinematic_modes,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__assign_function__LowCmd__kinematic_modes,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "motors",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    31,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__LowCmd, motors),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__size_function__LowCmd__motors,  // size() function pointer
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_const_function__LowCmd__motors,  // get_const(index) function pointer
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__get_function__LowCmd__motors,  // get(index) function pointer
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__fetch_function__LowCmd__motors,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__assign_function__LowCmd__motors,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_message_members = {
  "igris_c_sdk__msg",  // message namespace
  "LowCmd",  // message name
  3,  // number of fields
  sizeof(igris_c_sdk__msg__LowCmd),
  false,  // has_any_key_member_
  igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_message_member_array,  // message members
  igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_init_function,  // function to initialize message memory (memory has to be allocated)
  igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_message_type_support_handle = {
  0,
  &igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_message_members,
  get_message_typesupport_handle_function,
  &igris_c_sdk__msg__LowCmd__get_type_hash,
  &igris_c_sdk__msg__LowCmd__get_type_description,
  &igris_c_sdk__msg__LowCmd__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_igris_c_sdk
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, LowCmd)() {
  igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, Header)();
  igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, MotorCmd)();
  if (!igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_message_type_support_handle.typesupport_identifier) {
    igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &igris_c_sdk__msg__LowCmd__rosidl_typesupport_introspection_c__LowCmd_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
