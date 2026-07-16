// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from igris_c_sdk:msg/LowState.idl
// generated code does not contain a copyright notice

#include "igris_c_sdk/msg/detail/low_state__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__msg__LowState__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x35, 0x86, 0xc5, 0xdc, 0xbf, 0x86, 0x65, 0x98,
      0x3a, 0x43, 0x2a, 0x08, 0xe3, 0x15, 0xfb, 0xbe,
      0x73, 0x30, 0xfe, 0x79, 0xdc, 0x48, 0xfd, 0x2b,
      0xbd, 0xe8, 0x03, 0xd0, 0xca, 0xcd, 0x51, 0x1b,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "igris_c_sdk/msg/detail/joint_state__functions.h"
#include "igris_c_sdk/msg/detail/motor_state__functions.h"
#include "igris_c_sdk/msg/detail/header__functions.h"
#include "igris_c_sdk/msg/detail/imu_state__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t igris_c_sdk__msg__Header__EXPECTED_HASH = {1, {
    0xa0, 0x38, 0x67, 0xa4, 0x9d, 0x23, 0x8c, 0x57,
    0x5b, 0xaa, 0x17, 0x0d, 0xec, 0x56, 0x59, 0xbb,
    0x1a, 0x09, 0xce, 0x10, 0xaa, 0x67, 0x67, 0x1c,
    0x36, 0x62, 0x85, 0x02, 0x77, 0x72, 0xd8, 0x99,
  }};
static const rosidl_type_hash_t igris_c_sdk__msg__IMUState__EXPECTED_HASH = {1, {
    0xc6, 0xb0, 0x59, 0x2d, 0x26, 0xc3, 0xcd, 0xdd,
    0xa9, 0xbe, 0x0d, 0x28, 0x5d, 0x56, 0xb5, 0xff,
    0x65, 0xe2, 0x2f, 0xba, 0x05, 0x83, 0x5e, 0xb4,
    0xf4, 0x17, 0xd5, 0x63, 0x8e, 0xee, 0xe0, 0x57,
  }};
static const rosidl_type_hash_t igris_c_sdk__msg__JointState__EXPECTED_HASH = {1, {
    0x09, 0xaf, 0x96, 0x70, 0x7c, 0xc2, 0xd5, 0xe0,
    0xa5, 0x03, 0xf8, 0x8f, 0x49, 0xc6, 0x57, 0xd5,
    0x32, 0x5a, 0x46, 0xe3, 0x24, 0x47, 0x44, 0x52,
    0xf9, 0x8a, 0x1e, 0xb7, 0xb3, 0x22, 0x37, 0x52,
  }};
static const rosidl_type_hash_t igris_c_sdk__msg__MotorState__EXPECTED_HASH = {1, {
    0x0c, 0xb6, 0x6e, 0xc1, 0xb2, 0x75, 0xb0, 0xeb,
    0x83, 0x4c, 0xce, 0x95, 0x8f, 0x3c, 0x72, 0x1d,
    0xb6, 0x37, 0x5b, 0x5b, 0x50, 0xc6, 0x8e, 0xd1,
    0x0d, 0x5c, 0x8f, 0x5d, 0x27, 0x2d, 0xa7, 0xf3,
  }};
#endif

static char igris_c_sdk__msg__LowState__TYPE_NAME[] = "igris_c_sdk/msg/LowState";
static char igris_c_sdk__msg__Header__TYPE_NAME[] = "igris_c_sdk/msg/Header";
static char igris_c_sdk__msg__IMUState__TYPE_NAME[] = "igris_c_sdk/msg/IMUState";
static char igris_c_sdk__msg__JointState__TYPE_NAME[] = "igris_c_sdk/msg/JointState";
static char igris_c_sdk__msg__MotorState__TYPE_NAME[] = "igris_c_sdk/msg/MotorState";

// Define type names, field names, and default values
static char igris_c_sdk__msg__LowState__FIELD_NAME__header[] = "header";
static char igris_c_sdk__msg__LowState__FIELD_NAME__imu_state[] = "imu_state";
static char igris_c_sdk__msg__LowState__FIELD_NAME__motor_state[] = "motor_state";
static char igris_c_sdk__msg__LowState__FIELD_NAME__joint_state[] = "joint_state";

static rosidl_runtime_c__type_description__Field igris_c_sdk__msg__LowState__FIELDS[] = {
  {
    {igris_c_sdk__msg__LowState__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__LowState__FIELD_NAME__imu_state, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {igris_c_sdk__msg__IMUState__TYPE_NAME, 24, 24},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__LowState__FIELD_NAME__motor_state, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_ARRAY,
      31,
      0,
      {igris_c_sdk__msg__MotorState__TYPE_NAME, 26, 26},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__LowState__FIELD_NAME__joint_state, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_ARRAY,
      31,
      0,
      {igris_c_sdk__msg__JointState__TYPE_NAME, 26, 26},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription igris_c_sdk__msg__LowState__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__IMUState__TYPE_NAME, 24, 24},
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__JointState__TYPE_NAME, 26, 26},
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__MotorState__TYPE_NAME, 26, 26},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__msg__LowState__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__msg__LowState__TYPE_NAME, 24, 24},
      {igris_c_sdk__msg__LowState__FIELDS, 4, 4},
    },
    {igris_c_sdk__msg__LowState__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    assert(0 == memcmp(&igris_c_sdk__msg__Header__EXPECTED_HASH, igris_c_sdk__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = igris_c_sdk__msg__Header__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&igris_c_sdk__msg__IMUState__EXPECTED_HASH, igris_c_sdk__msg__IMUState__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = igris_c_sdk__msg__IMUState__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&igris_c_sdk__msg__JointState__EXPECTED_HASH, igris_c_sdk__msg__JointState__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = igris_c_sdk__msg__JointState__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&igris_c_sdk__msg__MotorState__EXPECTED_HASH, igris_c_sdk__msg__MotorState__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = igris_c_sdk__msg__MotorState__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "Header header\n"
  "IMUState imu_state\n"
  "MotorState[31] motor_state\n"
  "JointState[31] joint_state";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__msg__LowState__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__msg__LowState__TYPE_NAME, 24, 24},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 87, 87},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__msg__LowState__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__msg__LowState__get_individual_type_description_source(NULL),
    sources[1] = *igris_c_sdk__msg__Header__get_individual_type_description_source(NULL);
    sources[2] = *igris_c_sdk__msg__IMUState__get_individual_type_description_source(NULL);
    sources[3] = *igris_c_sdk__msg__JointState__get_individual_type_description_source(NULL);
    sources[4] = *igris_c_sdk__msg__MotorState__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
