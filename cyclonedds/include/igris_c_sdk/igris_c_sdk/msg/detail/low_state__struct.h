// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:msg/LowState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/low_state.h"


#ifndef IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__STRUCT_H_
#define IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__struct.h"
// Member 'imu_state'
#include "igris_c_sdk/msg/detail/imu_state__struct.h"
// Member 'motor_state'
#include "igris_c_sdk/msg/detail/motor_state__struct.h"
// Member 'joint_state'
#include "igris_c_sdk/msg/detail/joint_state__struct.h"

/// Struct defined in msg/LowState in the package igris_c_sdk.
typedef struct igris_c_sdk__msg__LowState
{
  igris_c_sdk__msg__Header header;
  igris_c_sdk__msg__IMUState imu_state;
  igris_c_sdk__msg__MotorState motor_state[31];
  igris_c_sdk__msg__JointState joint_state[31];
} igris_c_sdk__msg__LowState;

// Struct for a sequence of igris_c_sdk__msg__LowState.
typedef struct igris_c_sdk__msg__LowState__Sequence
{
  igris_c_sdk__msg__LowState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__msg__LowState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__STRUCT_H_
