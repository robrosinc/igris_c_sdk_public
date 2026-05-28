// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from igris_c_sdk:msg/MotorState.idl
// generated code does not contain a copyright notice
#include "igris_c_sdk/msg/detail/motor_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
igris_c_sdk__msg__MotorState__init(igris_c_sdk__msg__MotorState * msg)
{
  if (!msg) {
    return false;
  }
  // q
  // dq
  // tau_est
  // temperature
  // status_bits
  return true;
}

void
igris_c_sdk__msg__MotorState__fini(igris_c_sdk__msg__MotorState * msg)
{
  if (!msg) {
    return;
  }
  // q
  // dq
  // tau_est
  // temperature
  // status_bits
}

bool
igris_c_sdk__msg__MotorState__are_equal(const igris_c_sdk__msg__MotorState * lhs, const igris_c_sdk__msg__MotorState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // q
  if (lhs->q != rhs->q) {
    return false;
  }
  // dq
  if (lhs->dq != rhs->dq) {
    return false;
  }
  // tau_est
  if (lhs->tau_est != rhs->tau_est) {
    return false;
  }
  // temperature
  if (lhs->temperature != rhs->temperature) {
    return false;
  }
  // status_bits
  if (lhs->status_bits != rhs->status_bits) {
    return false;
  }
  return true;
}

bool
igris_c_sdk__msg__MotorState__copy(
  const igris_c_sdk__msg__MotorState * input,
  igris_c_sdk__msg__MotorState * output)
{
  if (!input || !output) {
    return false;
  }
  // q
  output->q = input->q;
  // dq
  output->dq = input->dq;
  // tau_est
  output->tau_est = input->tau_est;
  // temperature
  output->temperature = input->temperature;
  // status_bits
  output->status_bits = input->status_bits;
  return true;
}

igris_c_sdk__msg__MotorState *
igris_c_sdk__msg__MotorState__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__MotorState * msg = (igris_c_sdk__msg__MotorState *)allocator.allocate(sizeof(igris_c_sdk__msg__MotorState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(igris_c_sdk__msg__MotorState));
  bool success = igris_c_sdk__msg__MotorState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
igris_c_sdk__msg__MotorState__destroy(igris_c_sdk__msg__MotorState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    igris_c_sdk__msg__MotorState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
igris_c_sdk__msg__MotorState__Sequence__init(igris_c_sdk__msg__MotorState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__MotorState * data = NULL;

  if (size) {
    data = (igris_c_sdk__msg__MotorState *)allocator.zero_allocate(size, sizeof(igris_c_sdk__msg__MotorState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = igris_c_sdk__msg__MotorState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        igris_c_sdk__msg__MotorState__fini(&data[i - 1]);
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
igris_c_sdk__msg__MotorState__Sequence__fini(igris_c_sdk__msg__MotorState__Sequence * array)
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
      igris_c_sdk__msg__MotorState__fini(&array->data[i]);
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

igris_c_sdk__msg__MotorState__Sequence *
igris_c_sdk__msg__MotorState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__MotorState__Sequence * array = (igris_c_sdk__msg__MotorState__Sequence *)allocator.allocate(sizeof(igris_c_sdk__msg__MotorState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = igris_c_sdk__msg__MotorState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
igris_c_sdk__msg__MotorState__Sequence__destroy(igris_c_sdk__msg__MotorState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    igris_c_sdk__msg__MotorState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
igris_c_sdk__msg__MotorState__Sequence__are_equal(const igris_c_sdk__msg__MotorState__Sequence * lhs, const igris_c_sdk__msg__MotorState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!igris_c_sdk__msg__MotorState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
igris_c_sdk__msg__MotorState__Sequence__copy(
  const igris_c_sdk__msg__MotorState__Sequence * input,
  igris_c_sdk__msg__MotorState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(igris_c_sdk__msg__MotorState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    igris_c_sdk__msg__MotorState * data =
      (igris_c_sdk__msg__MotorState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!igris_c_sdk__msg__MotorState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          igris_c_sdk__msg__MotorState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!igris_c_sdk__msg__MotorState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
