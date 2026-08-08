// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:srv/BmsInitCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/srv/bms_init_cmd.h"


#ifndef IGRIS_C_SDK__SRV__DETAIL__BMS_INIT_CMD__STRUCT_H_
#define IGRIS_C_SDK__SRV__DETAIL__BMS_INIT_CMD__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Constant 'BMS_INIT_NONE'.
enum
{
  igris_c_sdk__srv__BmsInitCmd_Request__BMS_INIT_NONE = 0
};

/// Constant 'BMS_INIT'.
enum
{
  igris_c_sdk__srv__BmsInitCmd_Request__BMS_INIT = 1
};

/// Constant 'MOTOR_INIT'.
enum
{
  igris_c_sdk__srv__BmsInitCmd_Request__MOTOR_INIT = 2
};

/// Constant 'BMS_AND_MOTOR_INIT'.
enum
{
  igris_c_sdk__srv__BmsInitCmd_Request__BMS_AND_MOTOR_INIT = 3
};

/// Constant 'BMS_OFF'.
enum
{
  igris_c_sdk__srv__BmsInitCmd_Request__BMS_OFF = 4
};

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__struct.h"
// Member 'request_id'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/BmsInitCmd in the package igris_c_sdk.
typedef struct igris_c_sdk__srv__BmsInitCmd_Request
{
  igris_c_sdk__msg__Header header;
  rosidl_runtime_c__String request_id;
  uint8_t init;
} igris_c_sdk__srv__BmsInitCmd_Request;

// Struct for a sequence of igris_c_sdk__srv__BmsInitCmd_Request.
typedef struct igris_c_sdk__srv__BmsInitCmd_Request__Sequence
{
  igris_c_sdk__srv__BmsInitCmd_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__srv__BmsInitCmd_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'header'
// already included above
// #include "igris_c_sdk/msg/detail/header__struct.h"
// Member 'request_id'
// Member 'message'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in srv/BmsInitCmd in the package igris_c_sdk.
typedef struct igris_c_sdk__srv__BmsInitCmd_Response
{
  igris_c_sdk__msg__Header header;
  rosidl_runtime_c__String request_id;
  bool success;
  rosidl_runtime_c__String message;
  int32_t error_code;
} igris_c_sdk__srv__BmsInitCmd_Response;

// Struct for a sequence of igris_c_sdk__srv__BmsInitCmd_Response.
typedef struct igris_c_sdk__srv__BmsInitCmd_Response__Sequence
{
  igris_c_sdk__srv__BmsInitCmd_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__srv__BmsInitCmd_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  igris_c_sdk__srv__BmsInitCmd_Event__request__MAX_SIZE = 1
};
// response
enum
{
  igris_c_sdk__srv__BmsInitCmd_Event__response__MAX_SIZE = 1
};

/// Struct defined in srv/BmsInitCmd in the package igris_c_sdk.
typedef struct igris_c_sdk__srv__BmsInitCmd_Event
{
  service_msgs__msg__ServiceEventInfo info;
  igris_c_sdk__srv__BmsInitCmd_Request__Sequence request;
  igris_c_sdk__srv__BmsInitCmd_Response__Sequence response;
} igris_c_sdk__srv__BmsInitCmd_Event;

// Struct for a sequence of igris_c_sdk__srv__BmsInitCmd_Event.
typedef struct igris_c_sdk__srv__BmsInitCmd_Event__Sequence
{
  igris_c_sdk__srv__BmsInitCmd_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__srv__BmsInitCmd_Event__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__SRV__DETAIL__BMS_INIT_CMD__STRUCT_H_
