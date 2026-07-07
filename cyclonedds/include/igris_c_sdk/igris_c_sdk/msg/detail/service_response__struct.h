// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:msg/ServiceResponse.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/service_response.h"


#ifndef IGRIS_C_SDK__MSG__DETAIL__SERVICE_RESPONSE__STRUCT_H_
#define IGRIS_C_SDK__MSG__DETAIL__SERVICE_RESPONSE__STRUCT_H_

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
// Member 'request_id'
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/ServiceResponse in the package igris_c_sdk.
typedef struct igris_c_sdk__msg__ServiceResponse
{
  igris_c_sdk__msg__Header header;
  rosidl_runtime_c__String request_id;
  bool success;
  rosidl_runtime_c__String message;
  int32_t error_code;
} igris_c_sdk__msg__ServiceResponse;

// Struct for a sequence of igris_c_sdk__msg__ServiceResponse.
typedef struct igris_c_sdk__msg__ServiceResponse__Sequence
{
  igris_c_sdk__msg__ServiceResponse * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__msg__ServiceResponse__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__SERVICE_RESPONSE__STRUCT_H_
