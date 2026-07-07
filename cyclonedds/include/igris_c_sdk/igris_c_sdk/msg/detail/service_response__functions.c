// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from igris_c_sdk:msg/ServiceResponse.idl
// generated code does not contain a copyright notice
#include "igris_c_sdk/msg/detail/service_response__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "igris_c_sdk/msg/detail/header__functions.h"
// Member `request_id`
// Member `message`
#include "rosidl_runtime_c/string_functions.h"

bool
igris_c_sdk__msg__ServiceResponse__init(igris_c_sdk__msg__ServiceResponse * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!igris_c_sdk__msg__Header__init(&msg->header)) {
    igris_c_sdk__msg__ServiceResponse__fini(msg);
    return false;
  }
  // request_id
  if (!rosidl_runtime_c__String__init(&msg->request_id)) {
    igris_c_sdk__msg__ServiceResponse__fini(msg);
    return false;
  }
  // success
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    igris_c_sdk__msg__ServiceResponse__fini(msg);
    return false;
  }
  // error_code
  return true;
}

void
igris_c_sdk__msg__ServiceResponse__fini(igris_c_sdk__msg__ServiceResponse * msg)
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
igris_c_sdk__msg__ServiceResponse__are_equal(const igris_c_sdk__msg__ServiceResponse * lhs, const igris_c_sdk__msg__ServiceResponse * rhs)
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
igris_c_sdk__msg__ServiceResponse__copy(
  const igris_c_sdk__msg__ServiceResponse * input,
  igris_c_sdk__msg__ServiceResponse * output)
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

igris_c_sdk__msg__ServiceResponse *
igris_c_sdk__msg__ServiceResponse__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__ServiceResponse * msg = (igris_c_sdk__msg__ServiceResponse *)allocator.allocate(sizeof(igris_c_sdk__msg__ServiceResponse), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(igris_c_sdk__msg__ServiceResponse));
  bool success = igris_c_sdk__msg__ServiceResponse__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
igris_c_sdk__msg__ServiceResponse__destroy(igris_c_sdk__msg__ServiceResponse * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    igris_c_sdk__msg__ServiceResponse__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
igris_c_sdk__msg__ServiceResponse__Sequence__init(igris_c_sdk__msg__ServiceResponse__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__ServiceResponse * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(igris_c_sdk__msg__ServiceResponse)) {
      return false;
    }
    data = (igris_c_sdk__msg__ServiceResponse *)allocator.zero_allocate(size, sizeof(igris_c_sdk__msg__ServiceResponse), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = igris_c_sdk__msg__ServiceResponse__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        igris_c_sdk__msg__ServiceResponse__fini(&data[i - 1]);
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
igris_c_sdk__msg__ServiceResponse__Sequence__fini(igris_c_sdk__msg__ServiceResponse__Sequence * array)
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
      igris_c_sdk__msg__ServiceResponse__fini(&array->data[i]);
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

igris_c_sdk__msg__ServiceResponse__Sequence *
igris_c_sdk__msg__ServiceResponse__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__ServiceResponse__Sequence * array = (igris_c_sdk__msg__ServiceResponse__Sequence *)allocator.allocate(sizeof(igris_c_sdk__msg__ServiceResponse__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = igris_c_sdk__msg__ServiceResponse__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
igris_c_sdk__msg__ServiceResponse__Sequence__destroy(igris_c_sdk__msg__ServiceResponse__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    igris_c_sdk__msg__ServiceResponse__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
igris_c_sdk__msg__ServiceResponse__Sequence__are_equal(const igris_c_sdk__msg__ServiceResponse__Sequence * lhs, const igris_c_sdk__msg__ServiceResponse__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!igris_c_sdk__msg__ServiceResponse__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
igris_c_sdk__msg__ServiceResponse__Sequence__copy(
  const igris_c_sdk__msg__ServiceResponse__Sequence * input,
  igris_c_sdk__msg__ServiceResponse__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(igris_c_sdk__msg__ServiceResponse)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(igris_c_sdk__msg__ServiceResponse);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    igris_c_sdk__msg__ServiceResponse * data =
      (igris_c_sdk__msg__ServiceResponse *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!igris_c_sdk__msg__ServiceResponse__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          igris_c_sdk__msg__ServiceResponse__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!igris_c_sdk__msg__ServiceResponse__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
