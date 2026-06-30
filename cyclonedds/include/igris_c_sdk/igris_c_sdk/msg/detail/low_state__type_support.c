// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from igris_c_sdk:msg/LowState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "igris_c_sdk/msg/detail/low_state__rosidl_typesupport_introspection_c.h"
#include "igris_c_sdk/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "igris_c_sdk/msg/detail/low_state__functions.h"
#include "igris_c_sdk/msg/detail/low_state__struct.h"


// Include directives for member types
// Member `header`
#include "igris_c_sdk/msg/header.h"
// Member `header`
#include "igris_c_sdk/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `imu_state`
#include "igris_c_sdk/msg/imu_state.h"
// Member `imu_state`
#include "igris_c_sdk/msg/detail/imu_state__rosidl_typesupport_introspection_c.h"
// Member `motor_state`
#include "igris_c_sdk/msg/motor_state.h"
// Member `motor_state`
#include "igris_c_sdk/msg/detail/motor_state__rosidl_typesupport_introspection_c.h"
// Member `joint_state`
#include "igris_c_sdk/msg/joint_state.h"
// Member `joint_state`
#include "igris_c_sdk/msg/detail/joint_state__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  igris_c_sdk__msg__LowState__init(message_memory);
}

void igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_fini_function(void * message_memory)
{
  igris_c_sdk__msg__LowState__fini(message_memory);
}

size_t igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__size_function__LowState__motor_state(
  const void * untyped_member)
{
  (void)untyped_member;
  return 31;
}

const void * igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_const_function__LowState__motor_state(
  const void * untyped_member, size_t index)
{
  const igris_c_sdk__msg__MotorState * member =
    (const igris_c_sdk__msg__MotorState *)(untyped_member);
  return &member[index];
}

void * igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_function__LowState__motor_state(
  void * untyped_member, size_t index)
{
  igris_c_sdk__msg__MotorState * member =
    (igris_c_sdk__msg__MotorState *)(untyped_member);
  return &member[index];
}

void igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__fetch_function__LowState__motor_state(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const igris_c_sdk__msg__MotorState * item =
    ((const igris_c_sdk__msg__MotorState *)
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_const_function__LowState__motor_state(untyped_member, index));
  igris_c_sdk__msg__MotorState * value =
    (igris_c_sdk__msg__MotorState *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__assign_function__LowState__motor_state(
  void * untyped_member, size_t index, const void * untyped_value)
{
  igris_c_sdk__msg__MotorState * item =
    ((igris_c_sdk__msg__MotorState *)
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_function__LowState__motor_state(untyped_member, index));
  const igris_c_sdk__msg__MotorState * value =
    (const igris_c_sdk__msg__MotorState *)(untyped_value);
  *item = *value;
}

size_t igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__size_function__LowState__joint_state(
  const void * untyped_member)
{
  (void)untyped_member;
  return 31;
}

const void * igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_const_function__LowState__joint_state(
  const void * untyped_member, size_t index)
{
  const igris_c_sdk__msg__JointState * member =
    (const igris_c_sdk__msg__JointState *)(untyped_member);
  return &member[index];
}

void * igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_function__LowState__joint_state(
  void * untyped_member, size_t index)
{
  igris_c_sdk__msg__JointState * member =
    (igris_c_sdk__msg__JointState *)(untyped_member);
  return &member[index];
}

void igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__fetch_function__LowState__joint_state(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const igris_c_sdk__msg__JointState * item =
    ((const igris_c_sdk__msg__JointState *)
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_const_function__LowState__joint_state(untyped_member, index));
  igris_c_sdk__msg__JointState * value =
    (igris_c_sdk__msg__JointState *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__assign_function__LowState__joint_state(
  void * untyped_member, size_t index, const void * untyped_value)
{
  igris_c_sdk__msg__JointState * item =
    ((igris_c_sdk__msg__JointState *)
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_function__LowState__joint_state(untyped_member, index));
  const igris_c_sdk__msg__JointState * value =
    (const igris_c_sdk__msg__JointState *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_member_array[4] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__LowState, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "imu_state",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__LowState, imu_state),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "motor_state",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    31,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__LowState, motor_state),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__size_function__LowState__motor_state,  // size() function pointer
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_const_function__LowState__motor_state,  // get_const(index) function pointer
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_function__LowState__motor_state,  // get(index) function pointer
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__fetch_function__LowState__motor_state,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__assign_function__LowState__motor_state,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "joint_state",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    31,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__LowState, joint_state),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__size_function__LowState__joint_state,  // size() function pointer
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_const_function__LowState__joint_state,  // get_const(index) function pointer
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__get_function__LowState__joint_state,  // get(index) function pointer
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__fetch_function__LowState__joint_state,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__assign_function__LowState__joint_state,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_members = {
  "igris_c_sdk__msg",  // message namespace
  "LowState",  // message name
  4,  // number of fields
  sizeof(igris_c_sdk__msg__LowState),
  false,  // has_any_key_member_
  igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_member_array,  // message members
  igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_init_function,  // function to initialize message memory (memory has to be allocated)
  igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_type_support_handle = {
  0,
  &igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_members,
  get_message_typesupport_handle_function,
  &igris_c_sdk__msg__LowState__get_type_hash,
  &igris_c_sdk__msg__LowState__get_type_description,
  &igris_c_sdk__msg__LowState__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_igris_c_sdk
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, LowState)() {
  igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, Header)();
  igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, IMUState)();
  igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, MotorState)();
  igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_member_array[3].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, JointState)();
  if (!igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_type_support_handle.typesupport_identifier) {
    igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &igris_c_sdk__msg__LowState__rosidl_typesupport_introspection_c__LowState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
