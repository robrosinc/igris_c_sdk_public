// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from igris_c_sdk:srv/MujocoSimCmd.idl
// generated code does not contain a copyright notice

#include "igris_c_sdk/srv/detail/mujoco_sim_cmd__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__srv__MujocoSimCmd__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x04, 0x47, 0xf6, 0x54, 0x14, 0x9a, 0x51, 0x47,
      0x9f, 0xe1, 0x61, 0xb9, 0x64, 0x62, 0xa3, 0x46,
      0x81, 0x89, 0xcd, 0x3d, 0x49, 0x25, 0xb2, 0xd2,
      0x5b, 0xc1, 0x57, 0xce, 0xb2, 0xee, 0x0e, 0x31,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__srv__MujocoSimCmd_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xb6, 0x51, 0xad, 0x1d, 0x43, 0xb9, 0x88, 0x71,
      0x7d, 0x6a, 0x32, 0xb4, 0xb7, 0xc1, 0x45, 0x7f,
      0x7b, 0xf1, 0x20, 0xe2, 0xd9, 0xf4, 0xcf, 0x7c,
      0x0c, 0xcf, 0xfa, 0xef, 0x0d, 0xa4, 0x43, 0xe4,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__srv__MujocoSimCmd_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x39, 0xa4, 0x9b, 0x95, 0xb6, 0x83, 0xee, 0xdf,
      0x04, 0x80, 0xe7, 0x58, 0xf3, 0xab, 0x1a, 0x61,
      0xa5, 0x2e, 0x8e, 0xc3, 0x1e, 0x4e, 0x4d, 0xda,
      0xbb, 0x34, 0x5b, 0xa7, 0x93, 0xbc, 0x10, 0xe9,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__srv__MujocoSimCmd_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xa1, 0xc0, 0xbd, 0x53, 0xe2, 0x90, 0x8b, 0x7a,
      0x64, 0xa3, 0x96, 0xda, 0x19, 0xe4, 0xa2, 0x55,
      0x11, 0x96, 0x8a, 0x97, 0x22, 0xb2, 0xc0, 0xeb,
      0x51, 0x98, 0x21, 0xb1, 0x44, 0x40, 0x4a, 0xf3,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "service_msgs/msg/detail/service_event_info__functions.h"
#include "igris_c_sdk/msg/detail/header__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t igris_c_sdk__msg__Header__EXPECTED_HASH = {1, {
    0xa0, 0x38, 0x67, 0xa4, 0x9d, 0x23, 0x8c, 0x57,
    0x5b, 0xaa, 0x17, 0x0d, 0xec, 0x56, 0x59, 0xbb,
    0x1a, 0x09, 0xce, 0x10, 0xaa, 0x67, 0x67, 0x1c,
    0x36, 0x62, 0x85, 0x02, 0x77, 0x72, 0xd8, 0x99,
  }};
static const rosidl_type_hash_t service_msgs__msg__ServiceEventInfo__EXPECTED_HASH = {1, {
    0x41, 0xbc, 0xbb, 0xe0, 0x7a, 0x75, 0xc9, 0xb5,
    0x2b, 0xc9, 0x6b, 0xfd, 0x5c, 0x24, 0xd7, 0xf0,
    0xfc, 0x0a, 0x08, 0xc0, 0xcb, 0x79, 0x21, 0xb3,
    0x37, 0x3c, 0x57, 0x32, 0x34, 0x5a, 0x6f, 0x45,
  }};
#endif

static char igris_c_sdk__srv__MujocoSimCmd__TYPE_NAME[] = "igris_c_sdk/srv/MujocoSimCmd";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char igris_c_sdk__msg__Header__TYPE_NAME[] = "igris_c_sdk/msg/Header";
static char igris_c_sdk__srv__MujocoSimCmd_Event__TYPE_NAME[] = "igris_c_sdk/srv/MujocoSimCmd_Event";
static char igris_c_sdk__srv__MujocoSimCmd_Request__TYPE_NAME[] = "igris_c_sdk/srv/MujocoSimCmd_Request";
static char igris_c_sdk__srv__MujocoSimCmd_Response__TYPE_NAME[] = "igris_c_sdk/srv/MujocoSimCmd_Response";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char igris_c_sdk__srv__MujocoSimCmd__FIELD_NAME__request_message[] = "request_message";
static char igris_c_sdk__srv__MujocoSimCmd__FIELD_NAME__response_message[] = "response_message";
static char igris_c_sdk__srv__MujocoSimCmd__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field igris_c_sdk__srv__MujocoSimCmd__FIELDS[] = {
  {
    {igris_c_sdk__srv__MujocoSimCmd__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {igris_c_sdk__srv__MujocoSimCmd_Request__TYPE_NAME, 36, 36},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {igris_c_sdk__srv__MujocoSimCmd_Response__TYPE_NAME, 37, 37},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {igris_c_sdk__srv__MujocoSimCmd_Event__TYPE_NAME, 34, 34},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription igris_c_sdk__srv__MujocoSimCmd__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Event__TYPE_NAME, 34, 34},
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Request__TYPE_NAME, 36, 36},
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Response__TYPE_NAME, 37, 37},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__srv__MujocoSimCmd__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__srv__MujocoSimCmd__TYPE_NAME, 28, 28},
      {igris_c_sdk__srv__MujocoSimCmd__FIELDS, 3, 3},
    },
    {igris_c_sdk__srv__MujocoSimCmd__REFERENCED_TYPE_DESCRIPTIONS, 6, 6},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&igris_c_sdk__msg__Header__EXPECTED_HASH, igris_c_sdk__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = igris_c_sdk__msg__Header__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = igris_c_sdk__srv__MujocoSimCmd_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = igris_c_sdk__srv__MujocoSimCmd_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[4].fields = igris_c_sdk__srv__MujocoSimCmd_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[5].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char igris_c_sdk__srv__MujocoSimCmd_Request__FIELD_NAME__header[] = "header";
static char igris_c_sdk__srv__MujocoSimCmd_Request__FIELD_NAME__request_id[] = "request_id";
static char igris_c_sdk__srv__MujocoSimCmd_Request__FIELD_NAME__command_type[] = "command_type";

static rosidl_runtime_c__type_description__Field igris_c_sdk__srv__MujocoSimCmd_Request__FIELDS[] = {
  {
    {igris_c_sdk__srv__MujocoSimCmd_Request__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Request__FIELD_NAME__request_id, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Request__FIELD_NAME__command_type, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription igris_c_sdk__srv__MujocoSimCmd_Request__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__srv__MujocoSimCmd_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__srv__MujocoSimCmd_Request__TYPE_NAME, 36, 36},
      {igris_c_sdk__srv__MujocoSimCmd_Request__FIELDS, 3, 3},
    },
    {igris_c_sdk__srv__MujocoSimCmd_Request__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&igris_c_sdk__msg__Header__EXPECTED_HASH, igris_c_sdk__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = igris_c_sdk__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char igris_c_sdk__srv__MujocoSimCmd_Response__FIELD_NAME__header[] = "header";
static char igris_c_sdk__srv__MujocoSimCmd_Response__FIELD_NAME__request_id[] = "request_id";
static char igris_c_sdk__srv__MujocoSimCmd_Response__FIELD_NAME__success[] = "success";
static char igris_c_sdk__srv__MujocoSimCmd_Response__FIELD_NAME__message[] = "message";
static char igris_c_sdk__srv__MujocoSimCmd_Response__FIELD_NAME__error_code[] = "error_code";

static rosidl_runtime_c__type_description__Field igris_c_sdk__srv__MujocoSimCmd_Response__FIELDS[] = {
  {
    {igris_c_sdk__srv__MujocoSimCmd_Response__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Response__FIELD_NAME__request_id, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Response__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Response__FIELD_NAME__message, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Response__FIELD_NAME__error_code, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription igris_c_sdk__srv__MujocoSimCmd_Response__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__srv__MujocoSimCmd_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__srv__MujocoSimCmd_Response__TYPE_NAME, 37, 37},
      {igris_c_sdk__srv__MujocoSimCmd_Response__FIELDS, 5, 5},
    },
    {igris_c_sdk__srv__MujocoSimCmd_Response__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&igris_c_sdk__msg__Header__EXPECTED_HASH, igris_c_sdk__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = igris_c_sdk__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char igris_c_sdk__srv__MujocoSimCmd_Event__FIELD_NAME__info[] = "info";
static char igris_c_sdk__srv__MujocoSimCmd_Event__FIELD_NAME__request[] = "request";
static char igris_c_sdk__srv__MujocoSimCmd_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field igris_c_sdk__srv__MujocoSimCmd_Event__FIELDS[] = {
  {
    {igris_c_sdk__srv__MujocoSimCmd_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {igris_c_sdk__srv__MujocoSimCmd_Request__TYPE_NAME, 36, 36},
    },
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {igris_c_sdk__srv__MujocoSimCmd_Response__TYPE_NAME, 37, 37},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription igris_c_sdk__srv__MujocoSimCmd_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__msg__Header__TYPE_NAME, 22, 22},
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Request__TYPE_NAME, 36, 36},
    {NULL, 0, 0},
  },
  {
    {igris_c_sdk__srv__MujocoSimCmd_Response__TYPE_NAME, 37, 37},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__srv__MujocoSimCmd_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {igris_c_sdk__srv__MujocoSimCmd_Event__TYPE_NAME, 34, 34},
      {igris_c_sdk__srv__MujocoSimCmd_Event__FIELDS, 3, 3},
    },
    {igris_c_sdk__srv__MujocoSimCmd_Event__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&igris_c_sdk__msg__Header__EXPECTED_HASH, igris_c_sdk__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = igris_c_sdk__msg__Header__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = igris_c_sdk__srv__MujocoSimCmd_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = igris_c_sdk__srv__MujocoSimCmd_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "uint8 MUJOCO_SIM_PAUSE=0\n"
  "uint8 MUJOCO_SIM_RESUME=1\n"
  "uint8 MUJOCO_SIM_RELOAD=2\n"
  "uint8 MUJOCO_SIM_RESET=3\n"
  "Header header\n"
  "string request_id\n"
  "uint8 command_type\n"
  "---\n"
  "Header header\n"
  "string request_id\n"
  "bool success\n"
  "string message\n"
  "int32 error_code";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__srv__MujocoSimCmd__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__srv__MujocoSimCmd__TYPE_NAME, 28, 28},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 234, 234},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__srv__MujocoSimCmd_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__srv__MujocoSimCmd_Request__TYPE_NAME, 36, 36},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__srv__MujocoSimCmd_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__srv__MujocoSimCmd_Response__TYPE_NAME, 37, 37},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__srv__MujocoSimCmd_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {igris_c_sdk__srv__MujocoSimCmd_Event__TYPE_NAME, 34, 34},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__srv__MujocoSimCmd__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[7];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 7, 7};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__srv__MujocoSimCmd__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *igris_c_sdk__msg__Header__get_individual_type_description_source(NULL);
    sources[3] = *igris_c_sdk__srv__MujocoSimCmd_Event__get_individual_type_description_source(NULL);
    sources[4] = *igris_c_sdk__srv__MujocoSimCmd_Request__get_individual_type_description_source(NULL);
    sources[5] = *igris_c_sdk__srv__MujocoSimCmd_Response__get_individual_type_description_source(NULL);
    sources[6] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__srv__MujocoSimCmd_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__srv__MujocoSimCmd_Request__get_individual_type_description_source(NULL),
    sources[1] = *igris_c_sdk__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__srv__MujocoSimCmd_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__srv__MujocoSimCmd_Response__get_individual_type_description_source(NULL),
    sources[1] = *igris_c_sdk__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__srv__MujocoSimCmd_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *igris_c_sdk__srv__MujocoSimCmd_Event__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *igris_c_sdk__msg__Header__get_individual_type_description_source(NULL);
    sources[3] = *igris_c_sdk__srv__MujocoSimCmd_Request__get_individual_type_description_source(NULL);
    sources[4] = *igris_c_sdk__srv__MujocoSimCmd_Response__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
