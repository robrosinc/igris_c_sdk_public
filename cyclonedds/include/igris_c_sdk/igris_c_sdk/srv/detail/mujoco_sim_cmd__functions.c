// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from igris_c_sdk:srv/MujocoSimCmd.idl
// generated code does not contain a copyright notice
#include "igris_c_sdk/srv/detail/mujoco_sim_cmd__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

// Include directives for member types
// Member `header`
#include "igris_c_sdk/msg/detail/header__functions.h"
// Member `request_id`
#include "rosidl_runtime_c/string_functions.h"

bool
igris_c_sdk__srv__MujocoSimCmd_Request__init(igris_c_sdk__srv__MujocoSimCmd_Request * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!igris_c_sdk__msg__Header__init(&msg->header)) {
    igris_c_sdk__srv__MujocoSimCmd_Request__fini(msg);
    return false;
  }
  // request_id
  if (!rosidl_runtime_c__String__init(&msg->request_id)) {
    igris_c_sdk__srv__MujocoSimCmd_Request__fini(msg);
    return false;
  }
  // command_type
  return true;
}

void
igris_c_sdk__srv__MujocoSimCmd_Request__fini(igris_c_sdk__srv__MujocoSimCmd_Request * msg)
{
  if (!msg) {
    return;
  }
  // header
  igris_c_sdk__msg__Header__fini(&msg->header);
  // request_id
  rosidl_runtime_c__String__fini(&msg->request_id);
  // command_type
}

bool
igris_c_sdk__srv__MujocoSimCmd_Request__are_equal(const igris_c_sdk__srv__MujocoSimCmd_Request * lhs, const igris_c_sdk__srv__MujocoSimCmd_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!igris_c_sdk__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // request_id
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->request_id), &(rhs->request_id)))
  {
    return false;
  }
  // command_type
  if (lhs->command_type != rhs->command_type) {
    return false;
  }
  return true;
}

bool
igris_c_sdk__srv__MujocoSimCmd_Request__copy(
  const igris_c_sdk__srv__MujocoSimCmd_Request * input,
  igris_c_sdk__srv__MujocoSimCmd_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!igris_c_sdk__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // request_id
  if (!rosidl_runtime_c__String__copy(
      &(input->request_id), &(output->request_id)))
  {
    return false;
  }
  // command_type
  output->command_type = input->command_type;
  return true;
}

igris_c_sdk__srv__MujocoSimCmd_Request *
igris_c_sdk__srv__MujocoSimCmd_Request__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__srv__MujocoSimCmd_Request * msg = (igris_c_sdk__srv__MujocoSimCmd_Request *)allocator.allocate(sizeof(igris_c_sdk__srv__MujocoSimCmd_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(igris_c_sdk__srv__MujocoSimCmd_Request));
  bool success = igris_c_sdk__srv__MujocoSimCmd_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
igris_c_sdk__srv__MujocoSimCmd_Request__destroy(igris_c_sdk__srv__MujocoSimCmd_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    igris_c_sdk__srv__MujocoSimCmd_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__init(igris_c_sdk__srv__MujocoSimCmd_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__srv__MujocoSimCmd_Request * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(igris_c_sdk__srv__MujocoSimCmd_Request)) {
      return false;
    }
    data = (igris_c_sdk__srv__MujocoSimCmd_Request *)allocator.zero_allocate(size, sizeof(igris_c_sdk__srv__MujocoSimCmd_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = igris_c_sdk__srv__MujocoSimCmd_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        igris_c_sdk__srv__MujocoSimCmd_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__fini(igris_c_sdk__srv__MujocoSimCmd_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      igris_c_sdk__srv__MujocoSimCmd_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

igris_c_sdk__srv__MujocoSimCmd_Request__Sequence *
igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__srv__MujocoSimCmd_Request__Sequence * array = (igris_c_sdk__srv__MujocoSimCmd_Request__Sequence *)allocator.allocate(sizeof(igris_c_sdk__srv__MujocoSimCmd_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__destroy(igris_c_sdk__srv__MujocoSimCmd_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__are_equal(const igris_c_sdk__srv__MujocoSimCmd_Request__Sequence * lhs, const igris_c_sdk__srv__MujocoSimCmd_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!igris_c_sdk__srv__MujocoSimCmd_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__copy(
  const igris_c_sdk__srv__MujocoSimCmd_Request__Sequence * input,
  igris_c_sdk__srv__MujocoSimCmd_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(igris_c_sdk__srv__MujocoSimCmd_Request)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(igris_c_sdk__srv__MujocoSimCmd_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    igris_c_sdk__srv__MujocoSimCmd_Request * data =
      (igris_c_sdk__srv__MujocoSimCmd_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!igris_c_sdk__srv__MujocoSimCmd_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          igris_c_sdk__srv__MujocoSimCmd_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!igris_c_sdk__srv__MujocoSimCmd_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `header`
// already included above
// #include "igris_c_sdk/msg/detail/header__functions.h"
// Member `request_id`
// Member `message`
// already included above
// #include "rosidl_runtime_c/string_functions.h"

bool
igris_c_sdk__srv__MujocoSimCmd_Response__init(igris_c_sdk__srv__MujocoSimCmd_Response * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!igris_c_sdk__msg__Header__init(&msg->header)) {
    igris_c_sdk__srv__MujocoSimCmd_Response__fini(msg);
    return false;
  }
  // request_id
  if (!rosidl_runtime_c__String__init(&msg->request_id)) {
    igris_c_sdk__srv__MujocoSimCmd_Response__fini(msg);
    return false;
  }
  // success
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    igris_c_sdk__srv__MujocoSimCmd_Response__fini(msg);
    return false;
  }
  // error_code
  return true;
}

void
igris_c_sdk__srv__MujocoSimCmd_Response__fini(igris_c_sdk__srv__MujocoSimCmd_Response * msg)
{
  if (!msg) {
    return;
  }
  // header
  igris_c_sdk__msg__Header__fini(&msg->header);
  // request_id
  rosidl_runtime_c__String__fini(&msg->request_id);
  // success
  // message
  rosidl_runtime_c__String__fini(&msg->message);
  // error_code
}

bool
igris_c_sdk__srv__MujocoSimCmd_Response__are_equal(const igris_c_sdk__srv__MujocoSimCmd_Response * lhs, const igris_c_sdk__srv__MujocoSimCmd_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!igris_c_sdk__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // request_id
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->request_id), &(rhs->request_id)))
  {
    return false;
  }
  // success
  if (lhs->success != rhs->success) {
    return false;
  }
  // message
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->message), &(rhs->message)))
  {
    return false;
  }
  // error_code
  if (lhs->error_code != rhs->error_code) {
    return false;
  }
  return true;
}

bool
igris_c_sdk__srv__MujocoSimCmd_Response__copy(
  const igris_c_sdk__srv__MujocoSimCmd_Response * input,
  igris_c_sdk__srv__MujocoSimCmd_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!igris_c_sdk__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // request_id
  if (!rosidl_runtime_c__String__copy(
      &(input->request_id), &(output->request_id)))
  {
    return false;
  }
  // success
  output->success = input->success;
  // message
  if (!rosidl_runtime_c__String__copy(
      &(input->message), &(output->message)))
  {
    return false;
  }
  // error_code
  output->error_code = input->error_code;
  return true;
}

igris_c_sdk__srv__MujocoSimCmd_Response *
igris_c_sdk__srv__MujocoSimCmd_Response__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__srv__MujocoSimCmd_Response * msg = (igris_c_sdk__srv__MujocoSimCmd_Response *)allocator.allocate(sizeof(igris_c_sdk__srv__MujocoSimCmd_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(igris_c_sdk__srv__MujocoSimCmd_Response));
  bool success = igris_c_sdk__srv__MujocoSimCmd_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
igris_c_sdk__srv__MujocoSimCmd_Response__destroy(igris_c_sdk__srv__MujocoSimCmd_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    igris_c_sdk__srv__MujocoSimCmd_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__init(igris_c_sdk__srv__MujocoSimCmd_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__srv__MujocoSimCmd_Response * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(igris_c_sdk__srv__MujocoSimCmd_Response)) {
      return false;
    }
    data = (igris_c_sdk__srv__MujocoSimCmd_Response *)allocator.zero_allocate(size, sizeof(igris_c_sdk__srv__MujocoSimCmd_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = igris_c_sdk__srv__MujocoSimCmd_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        igris_c_sdk__srv__MujocoSimCmd_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__fini(igris_c_sdk__srv__MujocoSimCmd_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      igris_c_sdk__srv__MujocoSimCmd_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

igris_c_sdk__srv__MujocoSimCmd_Response__Sequence *
igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__srv__MujocoSimCmd_Response__Sequence * array = (igris_c_sdk__srv__MujocoSimCmd_Response__Sequence *)allocator.allocate(sizeof(igris_c_sdk__srv__MujocoSimCmd_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__destroy(igris_c_sdk__srv__MujocoSimCmd_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__are_equal(const igris_c_sdk__srv__MujocoSimCmd_Response__Sequence * lhs, const igris_c_sdk__srv__MujocoSimCmd_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!igris_c_sdk__srv__MujocoSimCmd_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__copy(
  const igris_c_sdk__srv__MujocoSimCmd_Response__Sequence * input,
  igris_c_sdk__srv__MujocoSimCmd_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(igris_c_sdk__srv__MujocoSimCmd_Response)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(igris_c_sdk__srv__MujocoSimCmd_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    igris_c_sdk__srv__MujocoSimCmd_Response * data =
      (igris_c_sdk__srv__MujocoSimCmd_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!igris_c_sdk__srv__MujocoSimCmd_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          igris_c_sdk__srv__MujocoSimCmd_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!igris_c_sdk__srv__MujocoSimCmd_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `info`
#include "service_msgs/msg/detail/service_event_info__functions.h"
// Member `request`
// Member `response`
// already included above
// #include "igris_c_sdk/srv/detail/mujoco_sim_cmd__functions.h"

bool
igris_c_sdk__srv__MujocoSimCmd_Event__init(igris_c_sdk__srv__MujocoSimCmd_Event * msg)
{
  if (!msg) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__init(&msg->info)) {
    igris_c_sdk__srv__MujocoSimCmd_Event__fini(msg);
    return false;
  }
  // request
  if (!igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__init(&msg->request, 0)) {
    igris_c_sdk__srv__MujocoSimCmd_Event__fini(msg);
    return false;
  }
  // response
  if (!igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__init(&msg->response, 0)) {
    igris_c_sdk__srv__MujocoSimCmd_Event__fini(msg);
    return false;
  }
  return true;
}

void
igris_c_sdk__srv__MujocoSimCmd_Event__fini(igris_c_sdk__srv__MujocoSimCmd_Event * msg)
{
  if (!msg) {
    return;
  }
  // info
  service_msgs__msg__ServiceEventInfo__fini(&msg->info);
  // request
  igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__fini(&msg->request);
  // response
  igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__fini(&msg->response);
}

bool
igris_c_sdk__srv__MujocoSimCmd_Event__are_equal(const igris_c_sdk__srv__MujocoSimCmd_Event * lhs, const igris_c_sdk__srv__MujocoSimCmd_Event * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__are_equal(
      &(lhs->info), &(rhs->info)))
  {
    return false;
  }
  // request
  if (!igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__are_equal(
      &(lhs->request), &(rhs->request)))
  {
    return false;
  }
  // response
  if (!igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__are_equal(
      &(lhs->response), &(rhs->response)))
  {
    return false;
  }
  return true;
}

bool
igris_c_sdk__srv__MujocoSimCmd_Event__copy(
  const igris_c_sdk__srv__MujocoSimCmd_Event * input,
  igris_c_sdk__srv__MujocoSimCmd_Event * output)
{
  if (!input || !output) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__copy(
      &(input->info), &(output->info)))
  {
    return false;
  }
  // request
  if (!igris_c_sdk__srv__MujocoSimCmd_Request__Sequence__copy(
      &(input->request), &(output->request)))
  {
    return false;
  }
  // response
  if (!igris_c_sdk__srv__MujocoSimCmd_Response__Sequence__copy(
      &(input->response), &(output->response)))
  {
    return false;
  }
  return true;
}

igris_c_sdk__srv__MujocoSimCmd_Event *
igris_c_sdk__srv__MujocoSimCmd_Event__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__srv__MujocoSimCmd_Event * msg = (igris_c_sdk__srv__MujocoSimCmd_Event *)allocator.allocate(sizeof(igris_c_sdk__srv__MujocoSimCmd_Event), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(igris_c_sdk__srv__MujocoSimCmd_Event));
  bool success = igris_c_sdk__srv__MujocoSimCmd_Event__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
igris_c_sdk__srv__MujocoSimCmd_Event__destroy(igris_c_sdk__srv__MujocoSimCmd_Event * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    igris_c_sdk__srv__MujocoSimCmd_Event__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
igris_c_sdk__srv__MujocoSimCmd_Event__Sequence__init(igris_c_sdk__srv__MujocoSimCmd_Event__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__srv__MujocoSimCmd_Event * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(igris_c_sdk__srv__MujocoSimCmd_Event)) {
      return false;
    }
    data = (igris_c_sdk__srv__MujocoSimCmd_Event *)allocator.zero_allocate(size, sizeof(igris_c_sdk__srv__MujocoSimCmd_Event), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = igris_c_sdk__srv__MujocoSimCmd_Event__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        igris_c_sdk__srv__MujocoSimCmd_Event__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
igris_c_sdk__srv__MujocoSimCmd_Event__Sequence__fini(igris_c_sdk__srv__MujocoSimCmd_Event__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      igris_c_sdk__srv__MujocoSimCmd_Event__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

igris_c_sdk__srv__MujocoSimCmd_Event__Sequence *
igris_c_sdk__srv__MujocoSimCmd_Event__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__srv__MujocoSimCmd_Event__Sequence * array = (igris_c_sdk__srv__MujocoSimCmd_Event__Sequence *)allocator.allocate(sizeof(igris_c_sdk__srv__MujocoSimCmd_Event__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = igris_c_sdk__srv__MujocoSimCmd_Event__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
igris_c_sdk__srv__MujocoSimCmd_Event__Sequence__destroy(igris_c_sdk__srv__MujocoSimCmd_Event__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    igris_c_sdk__srv__MujocoSimCmd_Event__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
igris_c_sdk__srv__MujocoSimCmd_Event__Sequence__are_equal(const igris_c_sdk__srv__MujocoSimCmd_Event__Sequence * lhs, const igris_c_sdk__srv__MujocoSimCmd_Event__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!igris_c_sdk__srv__MujocoSimCmd_Event__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
igris_c_sdk__srv__MujocoSimCmd_Event__Sequence__copy(
  const igris_c_sdk__srv__MujocoSimCmd_Event__Sequence * input,
  igris_c_sdk__srv__MujocoSimCmd_Event__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(igris_c_sdk__srv__MujocoSimCmd_Event)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(igris_c_sdk__srv__MujocoSimCmd_Event);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    igris_c_sdk__srv__MujocoSimCmd_Event * data =
      (igris_c_sdk__srv__MujocoSimCmd_Event *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!igris_c_sdk__srv__MujocoSimCmd_Event__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          igris_c_sdk__srv__MujocoSimCmd_Event__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!igris_c_sdk__srv__MujocoSimCmd_Event__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
