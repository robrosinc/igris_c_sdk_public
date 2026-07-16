// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from igris_c_sdk:msg/JointState.idl
// generated code does not contain a copyright notice
#ifndef IGRIS_C_SDK__MSG__DETAIL__JOINT_STATE__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define IGRIS_C_SDK__MSG__DETAIL__JOINT_STATE__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "igris_c_sdk/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "igris_c_sdk/msg/detail/joint_state__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_igris_c_sdk
bool cdr_serialize_igris_c_sdk__msg__JointState(
  const igris_c_sdk__msg__JointState * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_igris_c_sdk
bool cdr_deserialize_igris_c_sdk__msg__JointState(
  eprosima::fastcdr::Cdr &,
  igris_c_sdk__msg__JointState * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_igris_c_sdk
size_t get_serialized_size_igris_c_sdk__msg__JointState(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_igris_c_sdk
size_t max_serialized_size_igris_c_sdk__msg__JointState(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_igris_c_sdk
bool cdr_serialize_key_igris_c_sdk__msg__JointState(
  const igris_c_sdk__msg__JointState * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_igris_c_sdk
size_t get_serialized_size_key_igris_c_sdk__msg__JointState(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_igris_c_sdk
size_t max_serialized_size_key_igris_c_sdk__msg__JointState(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_igris_c_sdk
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, igris_c_sdk, msg, JointState)();

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__JOINT_STATE__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
