// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from igris_c_sdk:msg/LowState.idl
// generated code does not contain a copyright notice
#include "igris_c_sdk/msg/detail/low_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "igris_c_sdk/msg/detail/header__functions.h"
// Member `imu_state`
#include "igris_c_sdk/msg/detail/imu_state__functions.h"
// Member `motor_state`
#include "igris_c_sdk/msg/detail/motor_state__functions.h"
// Member `joint_state`
#include "igris_c_sdk/msg/detail/joint_state__functions.h"

bool
igris_c_sdk__msg__LowState__init(igris_c_sdk__msg__LowState * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!igris_c_sdk__msg__Header__init(&msg->header)) {
    igris_c_sdk__msg__LowState__fini(msg);
    return false;
  }
  // imu_state
  if (!igris_c_sdk__msg__IMUState__init(&msg->imu_state)) {
    igris_c_sdk__msg__LowState__fini(msg);
    return false;
  }
  // motor_state
  for (size_t i = 0; i < 31; ++i) {
    if (!igris_c_sdk__msg__MotorState__init(&msg->motor_state[i])) {
      igris_c_sdk__msg__LowState__fini(msg);
      return false;
    }
  }
  // joint_state
  for (size_t i = 0; i < 31; ++i) {
    if (!igris_c_sdk__msg__JointState__init(&msg->joint_state[i])) {
      igris_c_sdk__msg__LowState__fini(msg);
      return false;
    }
  }
  return true;
}

void
igris_c_sdk__msg__LowState__fini(igris_c_sdk__msg__LowState * msg)
{
  if (!msg) {
    return;
  }
  // header
  igris_c_sdk__msg__Header__fini(&msg->header);
  // imu_state
  igris_c_sdk__msg__IMUState__fini(&msg->imu_state);
  // motor_state
  for (size_t i = 0; i < 31; ++i) {
    igris_c_sdk__msg__MotorState__fini(&msg->motor_state[i]);
  }
  // joint_state
  for (size_t i = 0; i < 31; ++i) {
    igris_c_sdk__msg__JointState__fini(&msg->joint_state[i]);
  }
}

bool
igris_c_sdk__msg__LowState__are_equal(const igris_c_sdk__msg__LowState * lhs, const igris_c_sdk__msg__LowState * rhs)
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
  // imu_state
  if (!igris_c_sdk__msg__IMUState__are_equal(
      &(lhs->imu_state), &(rhs->imu_state)))
  {
    return false;
  }
  // motor_state
  for (size_t i = 0; i < 31; ++i) {
    if (!igris_c_sdk__msg__MotorState__are_equal(
        &(lhs->motor_state[i]), &(rhs->motor_state[i])))
    {
      return false;
    }
  }
  // joint_state
  for (size_t i = 0; i < 31; ++i) {
    if (!igris_c_sdk__msg__JointState__are_equal(
        &(lhs->joint_state[i]), &(rhs->joint_state[i])))
    {
      return false;
    }
  }
  return true;
}

bool
igris_c_sdk__msg__LowState__copy(
  const igris_c_sdk__msg__LowState * input,
  igris_c_sdk__msg__LowState * output)
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
  // imu_state
  if (!igris_c_sdk__msg__IMUState__copy(
      &(input->imu_state), &(output->imu_state)))
  {
    return false;
  }
  // motor_state
  for (size_t i = 0; i < 31; ++i) {
    if (!igris_c_sdk__msg__MotorState__copy(
        &(input->motor_state[i]), &(output->motor_state[i])))
    {
      return false;
    }
  }
  // joint_state
  for (size_t i = 0; i < 31; ++i) {
    if (!igris_c_sdk__msg__JointState__copy(
        &(input->joint_state[i]), &(output->joint_state[i])))
    {
      return false;
    }
  }
  return true;
}

igris_c_sdk__msg__LowState *
igris_c_sdk__msg__LowState__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__LowState * msg = (igris_c_sdk__msg__LowState *)allocator.allocate(sizeof(igris_c_sdk__msg__LowState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(igris_c_sdk__msg__LowState));
  bool success = igris_c_sdk__msg__LowState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
igris_c_sdk__msg__LowState__destroy(igris_c_sdk__msg__LowState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    igris_c_sdk__msg__LowState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
igris_c_sdk__msg__LowState__Sequence__init(igris_c_sdk__msg__LowState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__LowState * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(igris_c_sdk__msg__LowState)) {
      return false;
    }
    data = (igris_c_sdk__msg__LowState *)allocator.zero_allocate(size, sizeof(igris_c_sdk__msg__LowState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = igris_c_sdk__msg__LowState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        igris_c_sdk__msg__LowState__fini(&data[i - 1]);
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
igris_c_sdk__msg__LowState__Sequence__fini(igris_c_sdk__msg__LowState__Sequence * array)
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
      igris_c_sdk__msg__LowState__fini(&array->data[i]);
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

igris_c_sdk__msg__LowState__Sequence *
igris_c_sdk__msg__LowState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__LowState__Sequence * array = (igris_c_sdk__msg__LowState__Sequence *)allocator.allocate(sizeof(igris_c_sdk__msg__LowState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = igris_c_sdk__msg__LowState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
igris_c_sdk__msg__LowState__Sequence__destroy(igris_c_sdk__msg__LowState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    igris_c_sdk__msg__LowState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
igris_c_sdk__msg__LowState__Sequence__are_equal(const igris_c_sdk__msg__LowState__Sequence * lhs, const igris_c_sdk__msg__LowState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!igris_c_sdk__msg__LowState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
igris_c_sdk__msg__LowState__Sequence__copy(
  const igris_c_sdk__msg__LowState__Sequence * input,
  igris_c_sdk__msg__LowState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(igris_c_sdk__msg__LowState)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(igris_c_sdk__msg__LowState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    igris_c_sdk__msg__LowState * data =
      (igris_c_sdk__msg__LowState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!igris_c_sdk__msg__LowState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          igris_c_sdk__msg__LowState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!igris_c_sdk__msg__LowState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
