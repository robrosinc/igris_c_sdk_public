// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:msg/IMUState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/imu_state.h"


#ifndef IGRIS_C_SDK__MSG__DETAIL__IMU_STATE__STRUCT_H_
#define IGRIS_C_SDK__MSG__DETAIL__IMU_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Struct defined in msg/IMUState in the package igris_c_sdk.
typedef struct igris_c_sdk__msg__IMUState
{
  float quaternion[4];
  float gyroscope[3];
  float accelerometer[3];
  float rpy[3];
} igris_c_sdk__msg__IMUState;

// Struct for a sequence of igris_c_sdk__msg__IMUState.
typedef struct igris_c_sdk__msg__IMUState__Sequence
{
  igris_c_sdk__msg__IMUState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__msg__IMUState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__IMU_STATE__STRUCT_H_
