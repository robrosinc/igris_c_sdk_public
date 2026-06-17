// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:msg/LowCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/low_cmd.h"


#ifndef IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__STRUCT_H_
#define IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Constant 'KINEMATIC_MODE_MS'.
enum
{
  igris_c_sdk__msg__LowCmd__KINEMATIC_MODE_MS = 0
};

/// Constant 'KINEMATIC_MODE_PJS'.
enum
{
  igris_c_sdk__msg__LowCmd__KINEMATIC_MODE_PJS = 1
};

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__struct.h"
// Member 'motors'
#include "igris_c_sdk/msg/detail/motor_cmd__struct.h"

/// Struct defined in msg/LowCmd in the package igris_c_sdk.
typedef struct igris_c_sdk__msg__LowCmd
{
  igris_c_sdk__msg__Header header;
  uint8_t kinematic_modes[5];
  igris_c_sdk__msg__MotorCmd motors[31];
} igris_c_sdk__msg__LowCmd;

// Struct for a sequence of igris_c_sdk__msg__LowCmd.
typedef struct igris_c_sdk__msg__LowCmd__Sequence
{
  igris_c_sdk__msg__LowCmd * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__msg__LowCmd__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__STRUCT_H_
