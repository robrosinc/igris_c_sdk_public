// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from igris_c_sdk:msg/BmsState.idl
// generated code does not contain a copyright notice

#include "igris_c_sdk/msg/detail/bms_state__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__msg__BmsState__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xfa, 0x7c, 0xfa, 0x42, 0xa4, 0x59, 0x6d, 0x25,
      0x1b, 0xde, 0x9a, 0x84, 0x00, 0x5c, 0xf3, 0x94,
      0x91, 0x97, 0x7f, 0x71, 0x1c, 0x64, 0x89, 0xd9,
      0x7f, 0x87, 0x36, 0x4d, 0xd0, 0x4f, 0xa4, 0x46,
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

static char igris_c_sdk__msg__BmsState__TYPE_NAME[] = "igris_c_sdk/msg/BmsState";
static char igris_c_sdk__msg__Header__TYPE_NAME[] = "igris_c_sdk/msg/Header";

// Define type names, field names, and default values
static char igris_c_sdk__msg__BmsState__FIELD_NAME__header[] = "header";
static char igris_c_sdk__msg__BmsState__FIELD_NAME__body_power[] = "body_power";
static char igris_c_sdk__msg__BmsState__FIELD_NAME__legs_power[] = "legs_power";
static char igris_c_sdk__msg__BmsState__FIELD_NAME__estop[] = "estop";
static char igris_c_sdk__msg__BmsState__FIELD_NAME__connect[] = "connect";
static char igris_c_sdk__msg__BmsState__FIELD_NAME__battery[] = "battery";
static char igris_c_sdk__msg__BmsState__FIELD_NAME__bms_init_state[] = "bms_init_state";

static rosidl_runtime_c__type_description__Field igris_c_sdk__msg__BmsState__FIELDS[] = {
  {
    {igris_c_sdk__msg__BmsState__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__BmsState__FIELD_NAME__body_power, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__BmsState__FIELD_NAME__legs_power, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__BmsState__FIELD_NAME__estop, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__BmsState__FIELD_NAME__connect, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__BmsState__FIELD_NAME__battery, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__BmsState__FIELD_NAME__bms_init_state, 14, 14},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription igris_c_sdk__msg__BmsState__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__msg__BmsState__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__msg__BmsState__TYPE_NAME, 24, 24},
      {igris_c_sdk__msg__BmsState__FIELDS, 7, 7},
    },
    {igris_c_sdk__msg__BmsState__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&igris_c_sdk__msg__Header__EXPECTED_HASH, igris_c_sdk__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = igris_c_sdk__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "uint8 RELAY_OFF=0\n"
  "uint8 RELAY_ON=1\n"
  "uint8 ESTOP_RELEASED=0\n"
  "uint8 ESTOP_PRESSED=1\n"
  "uint8 BMS_DISCONNECTED=0\n"
  "uint8 BMS_CONNECTED=1\n"
  "uint8 BMS_NOT_INITIALIZED=0\n"
  "uint8 BMS_INITIALIZED=1\n"
  "uint8 MOTOR_INITIALIZED=2\n"
  "uint8 BOTH_INITIALIZED=3\n"
  "Header header\n"
  "uint8 body_power\n"
  "uint8 legs_power\n"
  "uint8 estop\n"
  "uint8 connect\n"
  "float32 battery\n"
  "uint8 bms_init_state";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__msg__BmsState__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__msg__BmsState__TYPE_NAME, 24, 24},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 341, 341},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__msg__BmsState__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__msg__BmsState__get_individual_type_description_source(NULL),
    sources[1] = *igris_c_sdk__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
