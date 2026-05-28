// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:msg/BmsState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/bms_state.h"


#ifndef IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__STRUCT_H_
#define IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Constant 'RELAY_OFF'.
enum
{
  igris_c_sdk__msg__BmsState__RELAY_OFF = 0
};

/// Constant 'RELAY_ON'.
enum
{
  igris_c_sdk__msg__BmsState__RELAY_ON = 1
};

/// Constant 'ESTOP_RELEASED'.
enum
{
  igris_c_sdk__msg__BmsState__ESTOP_RELEASED = 0
};

/// Constant 'ESTOP_PRESSED'.
enum
{
  igris_c_sdk__msg__BmsState__ESTOP_PRESSED = 1
};

/// Constant 'BMS_DISCONNECTED'.
enum
{
  igris_c_sdk__msg__BmsState__BMS_DISCONNECTED = 0
};

/// Constant 'BMS_CONNECTED'.
enum
{
  igris_c_sdk__msg__BmsState__BMS_CONNECTED = 1
};

/// Constant 'BMS_NOT_INITIALIZED'.
enum
{
  igris_c_sdk__msg__BmsState__BMS_NOT_INITIALIZED = 0
};

/// Constant 'BMS_INITIALIZED'.
enum
{
  igris_c_sdk__msg__BmsState__BMS_INITIALIZED = 1
};

/// Constant 'MOTOR_INITIALIZED'.
enum
{
  igris_c_sdk__msg__BmsState__MOTOR_INITIALIZED = 2
};

/// Constant 'BOTH_INITIALIZED'.
enum
{
  igris_c_sdk__msg__BmsState__BOTH_INITIALIZED = 3
};

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__struct.h"

/// Struct defined in msg/BmsState in the package igris_c_sdk.
typedef struct igris_c_sdk__msg__BmsState
{
  igris_c_sdk__msg__Header header;
  uint8_t body_power;
  uint8_t legs_power;
  uint8_t estop;
  uint8_t connect;
  float battery;
  uint8_t bms_init_state;
} igris_c_sdk__msg__BmsState;

// Struct for a sequence of igris_c_sdk__msg__BmsState.
typedef struct igris_c_sdk__msg__BmsState__Sequence
{
  igris_c_sdk__msg__BmsState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__msg__BmsState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__STRUCT_H_
