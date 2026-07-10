// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:msg/JointState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/joint_state.h"


#ifndef IGRIS_C_SDK__MSG__DETAIL__JOINT_STATE__STRUCT_H_
#define IGRIS_C_SDK__MSG__DETAIL__JOINT_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Struct defined in msg/JointState in the package igris_c_sdk.
typedef struct igris_c_sdk__msg__JointState
{
  float q;
  float dq;
  float tau_est;
  uint32_t status_bits;
} igris_c_sdk__msg__JointState;

// Struct for a sequence of igris_c_sdk__msg__JointState.
typedef struct igris_c_sdk__msg__JointState__Sequence
{
  igris_c_sdk__msg__JointState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__msg__JointState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__JOINT_STATE__STRUCT_H_
