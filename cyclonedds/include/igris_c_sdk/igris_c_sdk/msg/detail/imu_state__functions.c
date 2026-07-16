// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from igris_c_sdk:msg/IMUState.idl
// generated code does not contain a copyright notice
#include "igris_c_sdk/msg/detail/imu_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
igris_c_sdk__msg__IMUState__init(igris_c_sdk__msg__IMUState * msg)
{
  if (!msg) {
    return false;
  }
  // quaternion
  // gyroscope
  // accelerometer
  // rpy
  return true;
}

void
igris_c_sdk__msg__IMUState__fini(igris_c_sdk__msg__IMUState * msg)
{
  if (!msg) {
    return;
  }
  // quaternion
  // gyroscope
  // accelerometer
  // rpy
}

bool
igris_c_sdk__msg__IMUState__are_equal(const igris_c_sdk__msg__IMUState * lhs, const igris_c_sdk__msg__IMUState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // quaternion
  for (size_t i = 0; i < 4; ++i) {
    if (lhs->quaternion[i] != rhs->quaternion[i]) {
      return false;
    }
  }
  // gyroscope
  for (size_t i = 0; i < 3; ++i) {
    if (lhs->gyroscope[i] != rhs->gyroscope[i]) {
      return false;
    }
  }
  // accelerometer
  for (size_t i = 0; i < 3; ++i) {
    if (lhs->accelerometer[i] != rhs->accelerometer[i]) {
      return false;
    }
  }
  // rpy
  for (size_t i = 0; i < 3; ++i) {
    if (lhs->rpy[i] != rhs->rpy[i]) {
      return false;
    }
  }
  return true;
}

bool
igris_c_sdk__msg__IMUState__copy(
  const igris_c_sdk__msg__IMUState * input,
  igris_c_sdk__msg__IMUState * output)
{
  if (!input || !output) {
    return false;
  }
  // quaternion
  for (size_t i = 0; i < 4; ++i) {
    output->quaternion[i] = input->quaternion[i];
  }
  // gyroscope
  for (size_t i = 0; i < 3; ++i) {
    output->gyroscope[i] = input->gyroscope[i];
  }
  // accelerometer
  for (size_t i = 0; i < 3; ++i) {
    output->accelerometer[i] = input->accelerometer[i];
  }
  // rpy
  for (size_t i = 0; i < 3; ++i) {
    output->rpy[i] = input->rpy[i];
  }
  return true;
}

igris_c_sdk__msg__IMUState *
igris_c_sdk__msg__IMUState__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__IMUState * msg = (igris_c_sdk__msg__IMUState *)allocator.allocate(sizeof(igris_c_sdk__msg__IMUState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(igris_c_sdk__msg__IMUState));
  bool success = igris_c_sdk__msg__IMUState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
igris_c_sdk__msg__IMUState__destroy(igris_c_sdk__msg__IMUState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    igris_c_sdk__msg__IMUState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
igris_c_sdk__msg__IMUState__Sequence__init(igris_c_sdk__msg__IMUState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__IMUState * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(igris_c_sdk__msg__IMUState)) {
      return false;
    }
    data = (igris_c_sdk__msg__IMUState *)allocator.zero_allocate(size, sizeof(igris_c_sdk__msg__IMUState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = igris_c_sdk__msg__IMUState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        igris_c_sdk__msg__IMUState__fini(&data[i - 1]);
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
igris_c_sdk__msg__IMUState__Sequence__fini(igris_c_sdk__msg__IMUState__Sequence * array)
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
      igris_c_sdk__msg__IMUState__fini(&array->data[i]);
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

igris_c_sdk__msg__IMUState__Sequence *
igris_c_sdk__msg__IMUState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__IMUState__Sequence * array = (igris_c_sdk__msg__IMUState__Sequence *)allocator.allocate(sizeof(igris_c_sdk__msg__IMUState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = igris_c_sdk__msg__IMUState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
igris_c_sdk__msg__IMUState__Sequence__destroy(igris_c_sdk__msg__IMUState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    igris_c_sdk__msg__IMUState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
igris_c_sdk__msg__IMUState__Sequence__are_equal(const igris_c_sdk__msg__IMUState__Sequence * lhs, const igris_c_sdk__msg__IMUState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!igris_c_sdk__msg__IMUState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
igris_c_sdk__msg__IMUState__Sequence__copy(
  const igris_c_sdk__msg__IMUState__Sequence * input,
  igris_c_sdk__msg__IMUState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(igris_c_sdk__msg__IMUState)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(igris_c_sdk__msg__IMUState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    igris_c_sdk__msg__IMUState * data =
      (igris_c_sdk__msg__IMUState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!igris_c_sdk__msg__IMUState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          igris_c_sdk__msg__IMUState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!igris_c_sdk__msg__IMUState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
