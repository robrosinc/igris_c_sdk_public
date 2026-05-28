// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from igris_c_sdk:msg/BmsState.idl
// generated code does not contain a copyright notice
#include "igris_c_sdk/msg/detail/bms_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "igris_c_sdk/msg/detail/header__functions.h"

bool
igris_c_sdk__msg__BmsState__init(igris_c_sdk__msg__BmsState * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!igris_c_sdk__msg__Header__init(&msg->header)) {
    igris_c_sdk__msg__BmsState__fini(msg);
    return false;
  }
  // body_power
  // legs_power
  // estop
  // connect
  // battery
  // bms_init_state
  return true;
}

void
igris_c_sdk__msg__BmsState__fini(igris_c_sdk__msg__BmsState * msg)
{
  if (!msg) {
    return;
  }
  // header
  igris_c_sdk__msg__Header__fini(&msg->header);
  // body_power
  // legs_power
  // estop
  // connect
  // battery
  // bms_init_state
}

bool
igris_c_sdk__msg__BmsState__are_equal(const igris_c_sdk__msg__BmsState * lhs, const igris_c_sdk__msg__BmsState * rhs)
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
  // body_power
  if (lhs->body_power != rhs->body_power) {
    return false;
  }
  // legs_power
  if (lhs->legs_power != rhs->legs_power) {
    return false;
  }
  // estop
  if (lhs->estop != rhs->estop) {
    return false;
  }
  // connect
  if (lhs->connect != rhs->connect) {
    return false;
  }
  // battery
  if (lhs->battery != rhs->battery) {
    return false;
  }
  // bms_init_state
  if (lhs->bms_init_state != rhs->bms_init_state) {
    return false;
  }
  return true;
}

bool
igris_c_sdk__msg__BmsState__copy(
  const igris_c_sdk__msg__BmsState * input,
  igris_c_sdk__msg__BmsState * output)
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
  // body_power
  output->body_power = input->body_power;
  // legs_power
  output->legs_power = input->legs_power;
  // estop
  output->estop = input->estop;
  // connect
  output->connect = input->connect;
  // battery
  output->battery = input->battery;
  // bms_init_state
  output->bms_init_state = input->bms_init_state;
  return true;
}

igris_c_sdk__msg__BmsState *
igris_c_sdk__msg__BmsState__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__BmsState * msg = (igris_c_sdk__msg__BmsState *)allocator.allocate(sizeof(igris_c_sdk__msg__BmsState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(igris_c_sdk__msg__BmsState));
  bool success = igris_c_sdk__msg__BmsState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
igris_c_sdk__msg__BmsState__destroy(igris_c_sdk__msg__BmsState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    igris_c_sdk__msg__BmsState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
igris_c_sdk__msg__BmsState__Sequence__init(igris_c_sdk__msg__BmsState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__BmsState * data = NULL;

  if (size) {
    data = (igris_c_sdk__msg__BmsState *)allocator.zero_allocate(size, sizeof(igris_c_sdk__msg__BmsState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = igris_c_sdk__msg__BmsState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        igris_c_sdk__msg__BmsState__fini(&data[i - 1]);
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
igris_c_sdk__msg__BmsState__Sequence__fini(igris_c_sdk__msg__BmsState__Sequence * array)
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
      igris_c_sdk__msg__BmsState__fini(&array->data[i]);
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

igris_c_sdk__msg__BmsState__Sequence *
igris_c_sdk__msg__BmsState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  igris_c_sdk__msg__BmsState__Sequence * array = (igris_c_sdk__msg__BmsState__Sequence *)allocator.allocate(sizeof(igris_c_sdk__msg__BmsState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = igris_c_sdk__msg__BmsState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
igris_c_sdk__msg__BmsState__Sequence__destroy(igris_c_sdk__msg__BmsState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    igris_c_sdk__msg__BmsState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
igris_c_sdk__msg__BmsState__Sequence__are_equal(const igris_c_sdk__msg__BmsState__Sequence * lhs, const igris_c_sdk__msg__BmsState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!igris_c_sdk__msg__BmsState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
igris_c_sdk__msg__BmsState__Sequence__copy(
  const igris_c_sdk__msg__BmsState__Sequence * input,
  igris_c_sdk__msg__BmsState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(igris_c_sdk__msg__BmsState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    igris_c_sdk__msg__BmsState * data =
      (igris_c_sdk__msg__BmsState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!igris_c_sdk__msg__BmsState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          igris_c_sdk__msg__BmsState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!igris_c_sdk__msg__BmsState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
