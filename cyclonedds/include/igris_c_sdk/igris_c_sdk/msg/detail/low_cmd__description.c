// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from igris_c_sdk:msg/LowCmd.idl
// generated code does not contain a copyright notice

#include "igris_c_sdk/msg/detail/low_cmd__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__msg__LowCmd__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xb3, 0x3a, 0xec, 0xbb, 0x26, 0xef, 0x32, 0x0e,
      0x88, 0x87, 0xef, 0xf2, 0x9d, 0x89, 0x1d, 0x33,
      0x82, 0x96, 0x81, 0x59, 0x5d, 0x5c, 0x39, 0x49,
      0x60, 0x4e, 0x40, 0xa6, 0x39, 0xd8, 0xb6, 0x4c,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "igris_c_sdk/msg/detail/motor_cmd__functions.h"
#include "igris_c_sdk/msg/detail/header__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t igris_c_sdk__msg__Header__EXPECTED_HASH = {1, {
    0xa0, 0x38, 0x67, 0xa4, 0x9d, 0x23, 0x8c, 0x57,
    0x5b, 0xaa, 0x17, 0x0d, 0xec, 0x56, 0x59, 0xbb,
    0x1a, 0x09, 0xce, 0x10, 0xaa, 0x67, 0x67, 0x1c,
    0x36, 0x62, 0x85, 0x02, 0x77, 0x72, 0xd8, 0x99,
  }};
static const rosidl_type_hash_t igris_c_sdk__msg__MotorCmd__EXPECTED_HASH = {1, {
    0xbe, 0x5a, 0x84, 0xfb, 0x4a, 0x0f, 0x68, 0x69,
    0x76, 0x26, 0xbc, 0xc8, 0xe3, 0x41, 0x10, 0xd6,
    0x07, 0x6f, 0x72, 0x04, 0x10, 0x9f, 0xcb, 0x15,
    0xf7, 0xb0, 0xff, 0x46, 0xf9, 0xbb, 0xdd, 0x7f,
  }};
#endif

static char igris_c_sdk__msg__LowCmd__TYPE_NAME[] = "igris_c_sdk/msg/LowCmd";
static char igris_c_sdk__msg__Header__TYPE_NAME[] = "igris_c_sdk/msg/Header";
static char igris_c_sdk__msg__MotorCmd__TYPE_NAME[] = "igris_c_sdk/msg/MotorCmd";

// Define type names, field names, and default values
static char igris_c_sdk__msg__LowCmd__FIELD_NAME__header[] = "header";
static char igris_c_sdk__msg__LowCmd__FIELD_NAME__kinematic_modes[] = "kinematic_modes";
static char igris_c_sdk__msg__LowCmd__FIELD_NAME__motors[] = "motors";

static rosidl_runtime_c__type_description__Field igris_c_sdk__msg__LowCmd__FIELDS[] = {
  {
    {igris_c_sdk__msg__LowCmd__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__LowCmd__FIELD_NAME__kinematic_modes, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_ARRAY,
      5,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__LowCmd__FIELD_NAME__motors, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_ARRAY,
      31,
      0,
      {igris_c_sdk__msg__MotorCmd__TYPE_NAME, 24, 24},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription igris_c_sdk__msg__LowCmd__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__MotorCmd__TYPE_NAME, 24, 24},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__msg__LowCmd__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__msg__LowCmd__TYPE_NAME, 22, 22},
      {igris_c_sdk__msg__LowCmd__FIELDS, 3, 3},
    },
    {igris_c_sdk__msg__LowCmd__REFERENCED_TYPE_DESCRIPTIONS, 2, 2},
  };
  if (!constructed) {
    assert(0 == memcmp(&igris_c_sdk__msg__Header__EXPECTED_HASH, igris_c_sdk__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = igris_c_sdk__msg__Header__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&igris_c_sdk__msg__MotorCmd__EXPECTED_HASH, igris_c_sdk__msg__MotorCmd__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = igris_c_sdk__msg__MotorCmd__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "uint8 KINEMATIC_MODE_MS=0\n"
  "uint8 KINEMATIC_MODE_PJS=1\n"
  "Header header\n"
  "uint8[5] kinematic_modes\n"
  "MotorCmd[31] motors";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__msg__LowCmd__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__msg__LowCmd__TYPE_NAME, 22, 22},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 112, 112},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__msg__LowCmd__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[3];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 3, 3};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__msg__LowCmd__get_individual_type_description_source(NULL),
    sources[1] = *igris_c_sdk__msg__Header__get_individual_type_description_source(NULL);
    sources[2] = *igris_c_sdk__msg__MotorCmd__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
