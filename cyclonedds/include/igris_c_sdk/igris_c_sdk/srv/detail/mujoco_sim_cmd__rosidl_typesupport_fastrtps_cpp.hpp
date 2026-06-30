// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from igris_c_sdk:srv/MujocoSimCmd.idl
// generated code does not contain a copyright notice

#ifndef IGRIS_C_SDK__SRV__DETAIL__MUJOCO_SIM_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define IGRIS_C_SDK__SRV__DETAIL__MUJOCO_SIM_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include <cstddef>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "igris_c_sdk/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "igris_c_sdk/srv/detail/mujoco_sim_cmd__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace igris_c_sdk
{

namespace srv
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
cdr_serialize(
  const igris_c_sdk::srv::MujocoSimCmd_Request & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  igris_c_sdk::srv::MujocoSimCmd_Request & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
get_serialized_size(
  const igris_c_sdk::srv::MujocoSimCmd_Request & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
max_serialized_size_MujocoSimCmd_Request(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
cdr_serialize_key(
  const igris_c_sdk::srv::MujocoSimCmd_Request & ros_message,
  eprosima::fastcdr::Cdr &);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
get_serialized_size_key(
  const igris_c_sdk::srv::MujocoSimCmd_Request & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
max_serialized_size_key_MujocoSimCmd_Request(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace srv

}  // namespace igris_c_sdk

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, igris_c_sdk, srv, MujocoSimCmd_Request)();

#ifdef __cplusplus
}
#endif

// already included above
// #include <cstddef>
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "igris_c_sdk/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
// already included above
// #include "igris_c_sdk/srv/detail/mujoco_sim_cmd__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// already included above
// #include "fastcdr/Cdr.h"

namespace igris_c_sdk
{

namespace srv
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
cdr_serialize(
  const igris_c_sdk::srv::MujocoSimCmd_Response & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  igris_c_sdk::srv::MujocoSimCmd_Response & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
get_serialized_size(
  const igris_c_sdk::srv::MujocoSimCmd_Response & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
max_serialized_size_MujocoSimCmd_Response(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
cdr_serialize_key(
  const igris_c_sdk::srv::MujocoSimCmd_Response & ros_message,
  eprosima::fastcdr::Cdr &);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
get_serialized_size_key(
  const igris_c_sdk::srv::MujocoSimCmd_Response & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
max_serialized_size_key_MujocoSimCmd_Response(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace srv

}  // namespace igris_c_sdk

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, igris_c_sdk, srv, MujocoSimCmd_Response)();

#ifdef __cplusplus
}
#endif

// already included above
// #include <cstddef>
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "igris_c_sdk/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
// already included above
// #include "igris_c_sdk/srv/detail/mujoco_sim_cmd__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// already included above
// #include "fastcdr/Cdr.h"

namespace igris_c_sdk
{

namespace srv
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
cdr_serialize(
  const igris_c_sdk::srv::MujocoSimCmd_Event & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  igris_c_sdk::srv::MujocoSimCmd_Event & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
get_serialized_size(
  const igris_c_sdk::srv::MujocoSimCmd_Event & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
max_serialized_size_MujocoSimCmd_Event(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
cdr_serialize_key(
  const igris_c_sdk::srv::MujocoSimCmd_Event & ros_message,
  eprosima::fastcdr::Cdr &);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
get_serialized_size_key(
  const igris_c_sdk::srv::MujocoSimCmd_Event & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
max_serialized_size_key_MujocoSimCmd_Event(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace srv

}  // namespace igris_c_sdk

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, igris_c_sdk, srv, MujocoSimCmd_Event)();

#ifdef __cplusplus
}
#endif

#include "rmw/types.h"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "igris_c_sdk/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_igris_c_sdk
const rosidl_service_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, igris_c_sdk, srv, MujocoSimCmd)();

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__SRV__DETAIL__MUJOCO_SIM_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
