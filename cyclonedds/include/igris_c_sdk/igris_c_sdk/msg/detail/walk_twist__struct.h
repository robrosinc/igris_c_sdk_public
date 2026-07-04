// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:msg/WalkTwist.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/walk_twist.h"


#ifndef IGRIS_C_SDK__MSG__DETAIL__WALK_TWIST__STRUCT_H_
#define IGRIS_C_SDK__MSG__DETAIL__WALK_TWIST__STRUCT_H_

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
// Member 'twist'
#include "geometry_msgs/msg/detail/twist__struct.h"

/// Struct defined in msg/WalkTwist in the package igris_c_sdk.
typedef struct igris_c_sdk__msg__WalkTwist
{
  igris_c_sdk__msg__Header header;
  geometry_msgs__msg__Twist twist;
} igris_c_sdk__msg__WalkTwist;

// Struct for a sequence of igris_c_sdk__msg__WalkTwist.
typedef struct igris_c_sdk__msg__WalkTwist__Sequence
{
  igris_c_sdk__msg__WalkTwist * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__msg__WalkTwist__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__WALK_TWIST__STRUCT_H_
