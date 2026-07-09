// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from igris_c_sdk:srv/ControlModeCommandRequest.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/srv/control_mode_command_request.h"


#ifndef IGRIS_C_SDK__SRV__DETAIL__CONTROL_MODE_COMMAND_REQUEST__STRUCT_H_
#define IGRIS_C_SDK__SRV__DETAIL__CONTROL_MODE_COMMAND_REQUEST__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Constant 'CONTROL_MODE_CMD_MOTION_PRESET'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_MOTION_PRESET = 0
};

/// Constant 'CONTROL_MODE_CMD_MOTION_PRESET_CYCLIC_TOGGLE'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_MOTION_PRESET_CYCLIC_TOGGLE = 1
};

/// Constant 'CONTROL_MODE_CMD_JOINT_POSITION_HOLD'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_JOINT_POSITION_HOLD = 2
};

/// Constant 'CONTROL_MODE_CMD_MOTION_STOP'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_MOTION_STOP = 3
};

/// Constant 'CONTROL_MODE_CMD_WALKMODE_ON'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_WALKMODE_ON = 4
};

/// Constant 'CONTROL_MODE_CMD_LOW_LEVEL_JOINT_CONTROL'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_LOW_LEVEL_JOINT_CONTROL = 5
};

/// Constant 'CONTROL_MODE_CMD_LOW_LEVEL_WALKMODE_ON'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_LOW_LEVEL_WALKMODE_ON = 6
};

/// Constant 'CONTROL_MODE_CMD_HIGH_LEVEL_WALKMODE_ON'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_HIGH_LEVEL_WALKMODE_ON = 7
};

/// Constant 'CONTROL_MODE_CMD_HIGH_LEVEL_JOINT_CONTROL'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_HIGH_LEVEL_JOINT_CONTROL = 8
};

/// Constant 'CONTROL_MODE_CMD_CUSTOM_MODE_1'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_CUSTOM_MODE_1 = 9
};

/// Constant 'CONTROL_MODE_CMD_CUSTOM_MODE_2'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_CUSTOM_MODE_2 = 10
};

/// Constant 'CONTROL_MODE_CMD_CUSTOM_MODE_3'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_CUSTOM_MODE_3 = 11
};

/// Constant 'CONTROL_MODE_CMD_CUSTOM_MODE_4'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_CUSTOM_MODE_4 = 12
};

/// Constant 'CONTROL_MODE_CMD_CUSTOM_MODE_5'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_CUSTOM_MODE_5 = 13
};

/// Constant 'CONTROL_MODE_CMD_CUSTOM_MODE_6'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_CUSTOM_MODE_6 = 14
};

/// Constant 'CONTROL_MODE_CMD_CUSTOM_MODE_7'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_CUSTOM_MODE_7 = 15
};

/// Constant 'CONTROL_MODE_CMD_CUSTOM_MODE_8'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_CUSTOM_MODE_8 = 16
};

/// Constant 'CONTROL_MODE_CMD_CUSTOM_MODE_9'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_CUSTOM_MODE_9 = 17
};

/// Constant 'CONTROL_MODE_CMD_CUSTOM_MODE_10'.
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request__CONTROL_MODE_CMD_CUSTOM_MODE_10 = 18
};

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__struct.h"
// Member 'request_id'
// Member 'preset_id'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/ControlModeCommandRequest in the package igris_c_sdk.
typedef struct igris_c_sdk__srv__ControlModeCommandRequest_Request
{
  igris_c_sdk__msg__Header header;
  rosidl_runtime_c__String request_id;
  uint8_t command_type;
  rosidl_runtime_c__String preset_id;
  bool is_cyclic;
} igris_c_sdk__srv__ControlModeCommandRequest_Request;

// Struct for a sequence of igris_c_sdk__srv__ControlModeCommandRequest_Request.
typedef struct igris_c_sdk__srv__ControlModeCommandRequest_Request__Sequence
{
  igris_c_sdk__srv__ControlModeCommandRequest_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__srv__ControlModeCommandRequest_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'header'
// already included above
// #include "igris_c_sdk/msg/detail/header__struct.h"
// Member 'request_id'
// Member 'message'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in srv/ControlModeCommandRequest in the package igris_c_sdk.
typedef struct igris_c_sdk__srv__ControlModeCommandRequest_Response
{
  igris_c_sdk__msg__Header header;
  rosidl_runtime_c__String request_id;
  bool success;
  rosidl_runtime_c__String message;
  int32_t error_code;
} igris_c_sdk__srv__ControlModeCommandRequest_Response;

// Struct for a sequence of igris_c_sdk__srv__ControlModeCommandRequest_Response.
typedef struct igris_c_sdk__srv__ControlModeCommandRequest_Response__Sequence
{
  igris_c_sdk__srv__ControlModeCommandRequest_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__srv__ControlModeCommandRequest_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Event__request__MAX_SIZE = 1
};
// response
enum
{
  igris_c_sdk__srv__ControlModeCommandRequest_Event__response__MAX_SIZE = 1
};

/// Struct defined in srv/ControlModeCommandRequest in the package igris_c_sdk.
typedef struct igris_c_sdk__srv__ControlModeCommandRequest_Event
{
  service_msgs__msg__ServiceEventInfo info;
  igris_c_sdk__srv__ControlModeCommandRequest_Request__Sequence request;
  igris_c_sdk__srv__ControlModeCommandRequest_Response__Sequence response;
} igris_c_sdk__srv__ControlModeCommandRequest_Event;

// Struct for a sequence of igris_c_sdk__srv__ControlModeCommandRequest_Event.
typedef struct igris_c_sdk__srv__ControlModeCommandRequest_Event__Sequence
{
  igris_c_sdk__srv__ControlModeCommandRequest_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} igris_c_sdk__srv__ControlModeCommandRequest_Event__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__SRV__DETAIL__CONTROL_MODE_COMMAND_REQUEST__STRUCT_H_
