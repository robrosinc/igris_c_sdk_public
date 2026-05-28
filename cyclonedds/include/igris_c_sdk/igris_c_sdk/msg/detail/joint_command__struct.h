// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:msg/JointCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/joint_command.h"


#ifndef IGRIS_C_SDK__MSG__DETAIL__JOINT_COMMAND__STRUCT_H_
#define IGRIS_C_SDK__MSG__DETAIL__JOINT_COMMAND__STRUCT_H_

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
#include "std_msgs/msg/detail/header__struct.h"
// Member 'name'
#include "rosidl_runtime_c/string.h"
// Member 'mode'
// Member 'position'
// Member 'velocity'
// Member 'effort'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/JointCommand in the package igris_c_sdk.
/**
  * JointCommand.msg
  * mode :
  *    CyclicSynchronousPositionmode = 8,
  *    CyclicSynchronousVelocitymode = 9,
  *    CyclicSynchronousTorquemode = 10,
 */
typedef struct igris_c_sdk__msg__JointCommand
{
  std_msgs__msg__Header header;
  rosidl_runtime_c__String__Sequence name;
  rosidl_runtime_c__int8__Sequence mode;
  rosidl_runtime_c__double__Sequence position;
  rosidl_runtime_c__double__Sequence velocity;
  rosidl_runtime_c__double__Sequence effort;
} igris_c_sdk__msg__JointCommand;

// Struct for a sequence of igris_c_sdk__msg__JointCommand.
typedef struct igris_c_sdk__msg__JointCommand__Sequence
{
  igris_c_sdk__msg__JointCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__msg__JointCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__JOINT_COMMAND__STRUCT_H_
