// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from igris_c_sdk:msg/JointCommand.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "igris_c_sdk/msg/detail/joint_command__rosidl_typesupport_introspection_c.h"
#include "igris_c_sdk/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "igris_c_sdk/msg/detail/joint_command__functions.h"
#include "igris_c_sdk/msg/detail/joint_command__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `name`
#include "rosidl_runtime_c/string_functions.h"
// Member `mode`
// Member `position`
// Member `velocity`
// Member `effort`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  igris_c_sdk__msg__JointCommand__init(message_memory);
}

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_fini_function(void * message_memory)
{
  igris_c_sdk__msg__JointCommand__fini(message_memory);
}

size_t igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__size_function__JointCommand__name(
  const void * untyped_member)
{
  const rosidl_runtime_c__String__Sequence * member =
    (const rosidl_runtime_c__String__Sequence *)(untyped_member);
  return member->size;
}

const void * igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__name(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__String__Sequence * member =
    (const rosidl_runtime_c__String__Sequence *)(untyped_member);
  return &member->data[index];
}

void * igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__name(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__String__Sequence * member =
    (rosidl_runtime_c__String__Sequence *)(untyped_member);
  return &member->data[index];
}

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__fetch_function__JointCommand__name(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const rosidl_runtime_c__String * item =
    ((const rosidl_runtime_c__String *)
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__name(untyped_member, index));
  rosidl_runtime_c__String * value =
    (rosidl_runtime_c__String *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__assign_function__JointCommand__name(
  void * untyped_member, size_t index, const void * untyped_value)
{
  rosidl_runtime_c__String * item =
    ((rosidl_runtime_c__String *)
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__name(untyped_member, index));
  const rosidl_runtime_c__String * value =
    (const rosidl_runtime_c__String *)(untyped_value);
  *item = *value;
}

bool igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__resize_function__JointCommand__name(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__String__Sequence * member =
    (rosidl_runtime_c__String__Sequence *)(untyped_member);
  rosidl_runtime_c__String__Sequence__fini(member);
  return rosidl_runtime_c__String__Sequence__init(member, size);
}

size_t igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__size_function__JointCommand__mode(
  const void * untyped_member)
{
  const rosidl_runtime_c__int8__Sequence * member =
    (const rosidl_runtime_c__int8__Sequence *)(untyped_member);
  return member->size;
}

const void * igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__mode(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__int8__Sequence * member =
    (const rosidl_runtime_c__int8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__mode(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__int8__Sequence * member =
    (rosidl_runtime_c__int8__Sequence *)(untyped_member);
  return &member->data[index];
}

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__fetch_function__JointCommand__mode(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const int8_t * item =
    ((const int8_t *)
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__mode(untyped_member, index));
  int8_t * value =
    (int8_t *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__assign_function__JointCommand__mode(
  void * untyped_member, size_t index, const void * untyped_value)
{
  int8_t * item =
    ((int8_t *)
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__mode(untyped_member, index));
  const int8_t * value =
    (const int8_t *)(untyped_value);
  *item = *value;
}

bool igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__resize_function__JointCommand__mode(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__int8__Sequence * member =
    (rosidl_runtime_c__int8__Sequence *)(untyped_member);
  rosidl_runtime_c__int8__Sequence__fini(member);
  return rosidl_runtime_c__int8__Sequence__init(member, size);
}

size_t igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__size_function__JointCommand__position(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__position(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__position(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__fetch_function__JointCommand__position(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__position(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__assign_function__JointCommand__position(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__position(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__resize_function__JointCommand__position(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

size_t igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__size_function__JointCommand__velocity(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__velocity(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__velocity(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__fetch_function__JointCommand__velocity(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__velocity(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__assign_function__JointCommand__velocity(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__velocity(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__resize_function__JointCommand__velocity(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

size_t igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__size_function__JointCommand__effort(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__effort(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__effort(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__fetch_function__JointCommand__effort(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__effort(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__assign_function__JointCommand__effort(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__effort(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__resize_function__JointCommand__effort(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_message_member_array[6] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__JointCommand, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "name",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__JointCommand, name),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__size_function__JointCommand__name,  // size() function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__name,  // get_const(index) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__name,  // get(index) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__fetch_function__JointCommand__name,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__assign_function__JointCommand__name,  // assign(index, value) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__resize_function__JointCommand__name  // resize(index) function pointer
  },
  {
    "mode",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__JointCommand, mode),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__size_function__JointCommand__mode,  // size() function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__mode,  // get_const(index) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__mode,  // get(index) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__fetch_function__JointCommand__mode,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__assign_function__JointCommand__mode,  // assign(index, value) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__resize_function__JointCommand__mode  // resize(index) function pointer
  },
  {
    "position",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__JointCommand, position),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__size_function__JointCommand__position,  // size() function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__position,  // get_const(index) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__position,  // get(index) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__fetch_function__JointCommand__position,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__assign_function__JointCommand__position,  // assign(index, value) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__resize_function__JointCommand__position  // resize(index) function pointer
  },
  {
    "velocity",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__JointCommand, velocity),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__size_function__JointCommand__velocity,  // size() function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__velocity,  // get_const(index) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__velocity,  // get(index) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__fetch_function__JointCommand__velocity,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__assign_function__JointCommand__velocity,  // assign(index, value) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__resize_function__JointCommand__velocity  // resize(index) function pointer
  },
  {
    "effort",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__JointCommand, effort),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__size_function__JointCommand__effort,  // size() function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_const_function__JointCommand__effort,  // get_const(index) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__get_function__JointCommand__effort,  // get(index) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__fetch_function__JointCommand__effort,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__assign_function__JointCommand__effort,  // assign(index, value) function pointer
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__resize_function__JointCommand__effort  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_message_members = {
  "igris_c_sdk__msg",  // message namespace
  "JointCommand",  // message name
  6,  // number of fields
  sizeof(igris_c_sdk__msg__JointCommand),
  false,  // has_any_key_member_
  igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_message_member_array,  // message members
  igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_init_function,  // function to initialize message memory (memory has to be allocated)
  igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_message_type_support_handle = {
  0,
  &igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_message_members,
  get_message_typesupport_handle_function,
  &igris_c_sdk__msg__JointCommand__get_type_hash,
  &igris_c_sdk__msg__JointCommand__get_type_description,
  &igris_c_sdk__msg__JointCommand__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_igris_c_sdk
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, JointCommand)() {
  igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_message_type_support_handle.typesupport_identifier) {
    igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &igris_c_sdk__msg__JointCommand__rosidl_typesupport_introspection_c__JointCommand_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
