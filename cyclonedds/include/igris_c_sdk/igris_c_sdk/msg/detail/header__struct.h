// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:msg/Header.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/header.h"


#ifndef IGRIS_C_SDK__MSG__DETAIL__HEADER__STRUCT_H_
#define IGRIS_C_SDK__MSG__DETAIL__HEADER__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'frame_id'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/Header in the package igris_c_sdk.
typedef struct igris_c_sdk__msg__Header
{
  uint32_t seq;
  uint32_t sec;
  uint32_t nanosec;
  rosidl_runtime_c__String frame_id;
} igris_c_sdk__msg__Header;

// Struct for a sequence of igris_c_sdk__msg__Header.
typedef struct igris_c_sdk__msg__Header__Sequence
{
  igris_c_sdk__msg__Header * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__msg__Header__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__HEADER__STRUCT_H_
