// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from igris_c_sdk:msg/BmsState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "igris_c_sdk/msg/detail/bms_state__rosidl_typesupport_introspection_c.h"
#include "igris_c_sdk/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "igris_c_sdk/msg/detail/bms_state__functions.h"
#include "igris_c_sdk/msg/detail/bms_state__struct.h"


// Include directives for member types
// Member `header`
#include "igris_c_sdk/msg/header.h"
// Member `header`
#include "igris_c_sdk/msg/detail/header__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  igris_c_sdk__msg__BmsState__init(message_memory);
}

void igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_fini_function(void * message_memory)
{
  igris_c_sdk__msg__BmsState__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_message_member_array[7] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__BmsState, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "body_power",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__BmsState, body_power),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "legs_power",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__BmsState, legs_power),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "estop",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__BmsState, estop),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "connect",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__BmsState, connect),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "battery",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__BmsState, battery),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "bms_init_state",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(igris_c_sdk__msg__BmsState, bms_init_state),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_message_members = {
  "igris_c_sdk__msg",  // message namespace
  "BmsState",  // message name
  7,  // number of fields
  sizeof(igris_c_sdk__msg__BmsState),
  false,  // has_any_key_member_
  igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_message_member_array,  // message members
  igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_init_function,  // function to initialize message memory (memory has to be allocated)
  igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_message_type_support_handle = {
  0,
  &igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_message_members,
  get_message_typesupport_handle_function,
  &igris_c_sdk__msg__BmsState__get_type_hash,
  &igris_c_sdk__msg__BmsState__get_type_description,
  &igris_c_sdk__msg__BmsState__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_igris_c_sdk
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, BmsState)() {
  igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, igris_c_sdk, msg, Header)();
  if (!igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_message_type_support_handle.typesupport_identifier) {
    igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &igris_c_sdk__msg__BmsState__rosidl_typesupport_introspection_c__BmsState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
