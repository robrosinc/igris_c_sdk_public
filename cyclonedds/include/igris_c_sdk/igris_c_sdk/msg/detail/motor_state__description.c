// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from igris_c_sdk:msg/MotorState.idl
// generated code does not contain a copyright notice

#include "igris_c_sdk/msg/detail/motor_state__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__msg__MotorState__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x0c, 0xb6, 0x6e, 0xc1, 0xb2, 0x75, 0xb0, 0xeb,
      0x83, 0x4c, 0xce, 0x95, 0x8f, 0x3c, 0x72, 0x1d,
      0xb6, 0x37, 0x5b, 0x5b, 0x50, 0xc6, 0x8e, 0xd1,
      0x0d, 0x5c, 0x8f, 0x5d, 0x27, 0x2d, 0xa7, 0xf3,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char igris_c_sdk__msg__MotorState__TYPE_NAME[] = "igris_c_sdk/msg/MotorState";

// Define type names, field names, and default values
static char igris_c_sdk__msg__MotorState__FIELD_NAME__q[] = "q";
static char igris_c_sdk__msg__MotorState__FIELD_NAME__dq[] = "dq";
static char igris_c_sdk__msg__MotorState__FIELD_NAME__tau_est[] = "tau_est";
static char igris_c_sdk__msg__MotorState__FIELD_NAME__temperature[] = "temperature";
static char igris_c_sdk__msg__MotorState__FIELD_NAME__status_bits[] = "status_bits";

static rosidl_runtime_c__type_description__Field igris_c_sdk__msg__MotorState__FIELDS[] = {
  {
    {igris_c_sdk__msg__MotorState__FIELD_NAME__q, 1, 1},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__MotorState__FIELD_NAME__dq, 2, 2},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__MotorState__FIELD_NAME__tau_est, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__MotorState__FIELD_NAME__temperature, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__MotorState__FIELD_NAME__status_bits, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__msg__MotorState__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__msg__MotorState__TYPE_NAME, 26, 26},
      {igris_c_sdk__msg__MotorState__FIELDS, 5, 5},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "float32 q\n"
  "float32 dq\n"
  "float32 tau_est\n"
  "int16 temperature\n"
  "uint32 status_bits";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__msg__MotorState__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__msg__MotorState__TYPE_NAME, 26, 26},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 74, 74},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__msg__MotorState__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__msg__MotorState__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
