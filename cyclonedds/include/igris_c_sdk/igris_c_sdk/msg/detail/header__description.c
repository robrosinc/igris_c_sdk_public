// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from igris_c_sdk:msg/Header.idl
// generated code does not contain a copyright notice

#include "igris_c_sdk/msg/detail/header__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__msg__Header__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xa0, 0x38, 0x67, 0xa4, 0x9d, 0x23, 0x8c, 0x57,
      0x5b, 0xaa, 0x17, 0x0d, 0xec, 0x56, 0x59, 0xbb,
      0x1a, 0x09, 0xce, 0x10, 0xaa, 0x67, 0x67, 0x1c,
      0x36, 0x62, 0x85, 0x02, 0x77, 0x72, 0xd8, 0x99,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char igris_c_sdk__msg__Header__TYPE_NAME[] = "igris_c_sdk/msg/Header";

// Define type names, field names, and default values
static char igris_c_sdk__msg__Header__FIELD_NAME__seq[] = "seq";
static char igris_c_sdk__msg__Header__FIELD_NAME__sec[] = "sec";
static char igris_c_sdk__msg__Header__FIELD_NAME__nanosec[] = "nanosec";
static char igris_c_sdk__msg__Header__FIELD_NAME__frame_id[] = "frame_id";

static rosidl_runtime_c__type_description__Field igris_c_sdk__msg__Header__FIELDS[] = {
  {
    {igris_c_sdk__msg__Header__FIELD_NAME__seq, 3, 3},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__Header__FIELD_NAME__sec, 3, 3},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__Header__FIELD_NAME__nanosec, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__Header__FIELD_NAME__frame_id, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__msg__Header__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
      {igris_c_sdk__msg__Header__FIELDS, 4, 4},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "uint32 seq\n"
  "uint32 sec\n"
  "uint32 nanosec\n"
  "string frame_id";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__msg__Header__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 53, 53},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__msg__Header__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__msg__Header__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
