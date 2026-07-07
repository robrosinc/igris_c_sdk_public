// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from igris_c_sdk:msg/IMUState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "igris_c_sdk/msg/detail/imu_state__rosidl_typesupport_introspection_c.h"
#include "igris_c_sdk/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "igris_c_sdk/msg/detail/imu_state__functions.h"
#include "igris_c_sdk/msg/detail/imu_state__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  igris_c_sdk__msg__IMUState__init(message_memory);
}

void igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_fini_function(void * message_memory)
{
  igris_c_sdk__msg__IMUState__fini(message_memory);
}

size_t igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__size_function__IMUState__quaternion(
  const void * untyped_member)
{
  (void)untyped_member;
  return 4;
}

const void * igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__quaternion(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__quaternion(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__fetch_function__IMUState__quaternion(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__quaternion(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__assign_function__IMUState__quaternion(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__quaternion(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

size_t igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__size_function__IMUState__gyroscope(
  const void * untyped_member)
{
  (void)untyped_member;
  return 3;
}

const void * igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__gyroscope(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__gyroscope(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__fetch_function__IMUState__gyroscope(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__gyroscope(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__assign_function__IMUState__gyroscope(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__gyroscope(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

size_t igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__size_function__IMUState__accelerometer(
  const void * untyped_member)
{
  (void)untyped_member;
  return 3;
}

const void * igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__accelerometer(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__accelerometer(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__fetch_function__IMUState__accelerometer(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__accelerometer(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__assign_function__IMUState__accelerometer(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__accelerometer(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

size_t igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__size_function__IMUState__rpy(
  const void * untyped_member)
{
  (void)untyped_member;
  return 3;
}

const void * igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__rpy(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__rpy(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__fetch_function__IMUState__rpy(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__rpy(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__assign_function__IMUState__rpy(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__rpy(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_message_member_array[4] = {
  {
    "quaternion",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    4,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__IMUState, quaternion),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__size_function__IMUState__quaternion,  // size() function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__quaternion,  // get_const(index) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__quaternion,  // get(index) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__fetch_function__IMUState__quaternion,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__assign_function__IMUState__quaternion,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "gyroscope",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    3,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__IMUState, gyroscope),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__size_function__IMUState__gyroscope,  // size() function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__gyroscope,  // get_const(index) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__gyroscope,  // get(index) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__fetch_function__IMUState__gyroscope,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__assign_function__IMUState__gyroscope,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "accelerometer",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    3,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__IMUState, accelerometer),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__size_function__IMUState__accelerometer,  // size() function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__accelerometer,  // get_const(index) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__accelerometer,  // get(index) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__fetch_function__IMUState__accelerometer,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__assign_function__IMUState__accelerometer,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "rpy",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    3,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__IMUState, rpy),  // bytes offset in struct
    NULL,  // default value
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__size_function__IMUState__rpy,  // size() function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_const_function__IMUState__rpy,  // get_const(index) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__get_function__IMUState__rpy,  // get(index) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__fetch_function__IMUState__rpy,  // fetch(index, &value) function pointer
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__assign_function__IMUState__rpy,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_message_members = {
  "igris_c_sdk__msg",  // message namespace
  "IMUState",  // message name
  4,  // number of fields
  sizeof(igris_c_sdk__msg__IMUState),
  false,  // has_any_key_member_
  igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_message_member_array,  // message members
  igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_init_function,  // function to initialize message memory (memory has to be allocated)
  igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_message_type_support_handle = {
  0,
  &igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_message_members,
  get_message_typesupport_handle_function,
  &igris_c_sdk__msg__IMUState__get_type_hash,
  &igris_c_sdk__msg__IMUState__get_type_description,
  &igris_c_sdk__msg__IMUState__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_igris_c_sdk
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, IMUState)() {
  if (!igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_message_type_support_handle.typesupport_identifier) {
    igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &igris_c_sdk__msg__IMUState__rosidl_typesupport_introspection_c__IMUState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
