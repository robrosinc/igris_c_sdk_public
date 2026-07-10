// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from igris_c_sdk:msg/RobotState.idl
// generated code does not contain a copyright notice

#include "igris_c_sdk/msg/detail/robot_state__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__msg__RobotState__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x36, 0x29, 0x89, 0x0e, 0x5d, 0x5c, 0x6c, 0xfa,
      0x25, 0x74, 0xb9, 0x0b, 0x8b, 0x4c, 0x47, 0x2b,
      0x0b, 0x28, 0xa2, 0x50, 0x68, 0x8a, 0xad, 0xd0,
      0x25, 0x13, 0xe1, 0x65, 0xae, 0xaa, 0x8f, 0xe0,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "igris_c_sdk/msg/detail/header__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t igris_c_sdk__msg__Header__EXPECTED_HASH = {1, {
    0xa0, 0x38, 0x67, 0xa4, 0x9d, 0x23, 0x8c, 0x57,
    0x5b, 0xaa, 0x17, 0x0d, 0xec, 0x56, 0x59, 0xbb,
    0x1a, 0x09, 0xce, 0x10, 0xaa, 0x67, 0x67, 0x1c,
    0x36, 0x62, 0x85, 0x02, 0x77, 0x72, 0xd8, 0x99,
  }};
#endif

static char igris_c_sdk__msg__RobotState__TYPE_NAME[] = "igris_c_sdk/msg/RobotState";
static char igris_c_sdk__msg__Header__TYPE_NAME[] = "igris_c_sdk/msg/Header";

// Define type names, field names, and default values
static char igris_c_sdk__msg__RobotState__FIELD_NAME__header[] = "header";
static char igris_c_sdk__msg__RobotState__FIELD_NAME__state[] = "state";
static char igris_c_sdk__msg__RobotState__FIELD_NAME__environment[] = "environment";

static rosidl_runtime_c__type_description__Field igris_c_sdk__msg__RobotState__FIELDS[] = {
  {
    {igris_c_sdk__msg__RobotState__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__RobotState__FIELD_NAME__state, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__RobotState__FIELD_NAME__environment, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription igris_c_sdk__msg__RobotState__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__msg__RobotState__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__msg__RobotState__TYPE_NAME, 26, 26},
      {igris_c_sdk__msg__RobotState__FIELDS, 3, 3},
    },
    {igris_c_sdk__msg__RobotState__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&igris_c_sdk__msg__Header__EXPECTED_HASH, igris_c_sdk__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = igris_c_sdk__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "uint8 ROBOT_STATE_NOT_READY=0\n"
  "uint8 ROBOT_STATE_IDLE=1\n"
  "uint8 ROBOT_STATE_STOP=2\n"
  "uint8 ROBOT_STATE_LOW=3\n"
  "uint8 ROBOT_STATE_HIGH_MOTION_ACTIVE=4\n"
  "uint8 ROBOT_STATE_HIGH_MOTION_STANDBY=5\n"
  "uint8 ROBOT_STATE_WALK_LOW=6\n"
  "uint8 ROBOT_STATE_WALK_HIGH_MOTION_ACTIVE=7\n"
  "uint8 ROBOT_STATE_WALK_HIGH_MOTION_STANDBY=8\n"
  "uint8 ROBOT_STATE_WHOLE_BODY_CONTROL=9\n"
  "uint8 ROBOT_ENV_REAL=0\n"
  "uint8 ROBOT_ENV_SIM=1\n"
  "Header header\n"
  "uint8 state\n"
  "uint8 environment";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__msg__RobotState__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__msg__RobotState__TYPE_NAME, 26, 26},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 429, 429},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__msg__RobotState__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__msg__RobotState__get_individual_type_description_source(NULL),
    sources[1] = *igris_c_sdk__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
