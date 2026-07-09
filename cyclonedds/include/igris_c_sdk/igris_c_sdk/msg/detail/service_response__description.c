// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from igris_c_sdk:msg/ServiceResponse.idl
// generated code does not contain a copyright notice

#include "igris_c_sdk/msg/detail/service_response__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__msg__ServiceResponse__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x84, 0x01, 0x06, 0xf1, 0xa2, 0x15, 0xaf, 0x51,
      0xd6, 0x64, 0x25, 0x75, 0x60, 0x5d, 0x2d, 0x11,
      0xa4, 0x11, 0xd0, 0x7f, 0xaa, 0x11, 0xaa, 0x0f,
      0x10, 0x07, 0x37, 0xab, 0xeb, 0x2f, 0x5d, 0x56,
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

static char igris_c_sdk__msg__ServiceResponse__TYPE_NAME[] = "igris_c_sdk/msg/ServiceResponse";
static char igris_c_sdk__msg__Header__TYPE_NAME[] = "igris_c_sdk/msg/Header";

// Define type names, field names, and default values
static char igris_c_sdk__msg__ServiceResponse__FIELD_NAME__header[] = "header";
static char igris_c_sdk__msg__ServiceResponse__FIELD_NAME__request_id[] = "request_id";
static char igris_c_sdk__msg__ServiceResponse__FIELD_NAME__success[] = "success";
static char igris_c_sdk__msg__ServiceResponse__FIELD_NAME__message[] = "message";
static char igris_c_sdk__msg__ServiceResponse__FIELD_NAME__error_code[] = "error_code";

static rosidl_runtime_c__type_description__Field igris_c_sdk__msg__ServiceResponse__FIELDS[] = {
  {
    {igris_c_sdk__msg__ServiceResponse__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__ServiceResponse__FIELD_NAME__request_id, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__ServiceResponse__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__ServiceResponse__FIELD_NAME__message, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__ServiceResponse__FIELD_NAME__error_code, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription igris_c_sdk__msg__ServiceResponse__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__msg__ServiceResponse__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__msg__ServiceResponse__TYPE_NAME, 31, 31},
      {igris_c_sdk__msg__ServiceResponse__FIELDS, 5, 5},
    },
    {igris_c_sdk__msg__ServiceResponse__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&igris_c_sdk__msg__Header__EXPECTED_HASH, igris_c_sdk__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = igris_c_sdk__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "Header header\n"
  "string request_id\n"
  "bool success\n"
  "string message\n"
  "int32 error_code";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__msg__ServiceResponse__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__msg__ServiceResponse__TYPE_NAME, 31, 31},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 77, 77},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__msg__ServiceResponse__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__msg__ServiceResponse__get_individual_type_description_source(NULL),
    sources[1] = *igris_c_sdk__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
