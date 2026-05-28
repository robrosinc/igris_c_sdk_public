// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:msg/RobotState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/robot_state.h"


#ifndef IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__STRUCT_H_
#define IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Constant 'ROBOT_STATE_NOT_READY'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_STATE_NOT_READY = 0
};

/// Constant 'ROBOT_STATE_IDLE'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_STATE_IDLE = 1
};

/// Constant 'ROBOT_STATE_STOP'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_STATE_STOP = 2
};

/// Constant 'ROBOT_STATE_LOW'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_STATE_LOW = 3
};

/// Constant 'ROBOT_STATE_HIGH_MOTION_ACTIVE'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_STATE_HIGH_MOTION_ACTIVE = 4
};

/// Constant 'ROBOT_STATE_HIGH_MOTION_STANDBY'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_STATE_HIGH_MOTION_STANDBY = 5
};

/// Constant 'ROBOT_STATE_WALK_LOW'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_STATE_WALK_LOW = 6
};

/// Constant 'ROBOT_STATE_WALK_HIGH_MOTION_ACTIVE'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_STATE_WALK_HIGH_MOTION_ACTIVE = 7
};

/// Constant 'ROBOT_STATE_WALK_HIGH_MOTION_STANDBY'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_STATE_WALK_HIGH_MOTION_STANDBY = 8
};

/// Constant 'ROBOT_STATE_WHOLE_BODY_CONTROL'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_STATE_WHOLE_BODY_CONTROL = 9
};

/// Constant 'ROBOT_ENV_REAL'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_ENV_REAL = 0
};

/// Constant 'ROBOT_ENV_SIM'.
enum
{
  igris_c_sdk__msg__RobotState__ROBOT_ENV_SIM = 1
};

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__struct.h"

/// Struct defined in msg/RobotState in the package igris_c_sdk.
typedef struct igris_c_sdk__msg__RobotState
{
  igris_c_sdk__msg__Header header;
  uint8_t state;
  uint8_t environment;
} igris_c_sdk__msg__RobotState;

// Struct for a sequence of igris_c_sdk__msg__RobotState.
typedef struct igris_c_sdk__msg__RobotState__Sequence
{
  igris_c_sdk__msg__RobotState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__msg__RobotState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__STRUCT_H_
