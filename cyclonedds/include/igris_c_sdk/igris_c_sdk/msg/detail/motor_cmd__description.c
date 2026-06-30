// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from igris_c_sdk:msg/MotorCmd.idl
// generated code does not contain a copyright notice

#include "igris_c_sdk/msg/detail/motor_cmd__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__msg__MotorCmd__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xbe, 0x5a, 0x84, 0xfb, 0x4a, 0x0f, 0x68, 0x69,
      0x76, 0x26, 0xbc, 0xc8, 0xe3, 0x41, 0x10, 0xd6,
      0x07, 0x6f, 0x72, 0x04, 0x10, 0x9f, 0xcb, 0x15,
      0xf7, 0xb0, 0xff, 0x46, 0xf9, 0xbb, 0xdd, 0x7f,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char igris_c_sdk__msg__MotorCmd__TYPE_NAME[] = "igris_c_sdk/msg/MotorCmd";

// Define type names, field names, and default values
static char igris_c_sdk__msg__MotorCmd__FIELD_NAME__id[] = "id";
static char igris_c_sdk__msg__MotorCmd__FIELD_NAME__q[] = "q";
static char igris_c_sdk__msg__MotorCmd__FIELD_NAME__dq[] = "dq";
static char igris_c_sdk__msg__MotorCmd__FIELD_NAME__tau[] = "tau";
static char igris_c_sdk__msg__MotorCmd__FIELD_NAME__kp[] = "kp";
static char igris_c_sdk__msg__MotorCmd__FIELD_NAME__kd[] = "kd";

static rosidl_runtime_c__type_description__Field igris_c_sdk__msg__MotorCmd__FIELDS[] = {
  {
    {igris_c_sdk__msg__MotorCmd__FIELD_NAME__id, 2, 2},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__MotorCmd__FIELD_NAME__q, 1, 1},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__MotorCmd__FIELD_NAME__dq, 2, 2},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__MotorCmd__FIELD_NAME__tau, 3, 3},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__MotorCmd__FIELD_NAME__kp, 2, 2},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__MotorCmd__FIELD_NAME__kd, 2, 2},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__msg__MotorCmd__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__msg__MotorCmd__TYPE_NAME, 24, 24},
      {igris_c_sdk__msg__MotorCmd__FIELDS, 6, 6},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "uint16 id\n"
  "float32 q\n"
  "float32 dq\n"
  "float32 tau\n"
  "float32 kp\n"
  "float32 kd";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__msg__MotorCmd__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__msg__MotorCmd__TYPE_NAME, 24, 24},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 65, 65},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__msg__MotorCmd__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__msg__MotorCmd__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
