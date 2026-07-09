// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from igris_c_sdk:msg/LowCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/low_cmd.h"


#ifndef IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__FUNCTIONS_H_
#define IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/action_type_support_struct.h"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_runtime_c/type_description/type_description__struct.h"
#include "rosidl_runtime_c/type_description/type_source__struct.h"
#include "rosidl_runtime_c/type_hash.h"
#include "rosidl_runtime_c/visibility_control.h"
#include "igris_c_sdk/msg/rosidl_generator_c__visibility_control.h"

#include "igris_c_sdk/msg/detail/low_cmd__struct.h"

/// Initialize msg/LowCmd message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * igris_c_sdk__msg__LowCmd
 * )) before or use
 * igris_c_sdk__msg__LowCmd__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
bool
igris_c_sdk__msg__LowCmd__init(igris_c_sdk__msg__LowCmd * msg);

/// Finalize msg/LowCmd message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
void
igris_c_sdk__msg__LowCmd__fini(igris_c_sdk__msg__LowCmd * msg);

/// Create msg/LowCmd message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * igris_c_sdk__msg__LowCmd__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
igris_c_sdk__msg__LowCmd *
igris_c_sdk__msg__LowCmd__create(void);

/// Destroy msg/LowCmd message.
/**
 * It calls
 * igris_c_sdk__msg__LowCmd__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
void
igris_c_sdk__msg__LowCmd__destroy(igris_c_sdk__msg__LowCmd * msg);

/// Check for msg/LowCmd message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
bool
igris_c_sdk__msg__LowCmd__are_equal(const igris_c_sdk__msg__LowCmd * lhs, const igris_c_sdk__msg__LowCmd * rhs);

/// Copy a msg/LowCmd message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
bool
igris_c_sdk__msg__LowCmd__copy(
  const igris_c_sdk__msg__LowCmd * input,
  igris_c_sdk__msg__LowCmd * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_type_hash_t *
igris_c_sdk__msg__LowCmd__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_runtime_c__type_description__TypeDescription *
igris_c_sdk__msg__LowCmd__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_runtime_c__type_description__TypeSource *
igris_c_sdk__msg__LowCmd__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
const rosidl_runtime_c__type_description__TypeSource__Sequence *
igris_c_sdk__msg__LowCmd__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of msg/LowCmd messages.
/**
 * It allocates the memory for the number of elements and calls
 * igris_c_sdk__msg__LowCmd__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
bool
igris_c_sdk__msg__LowCmd__Sequence__init(igris_c_sdk__msg__LowCmd__Sequence * array, size_t size);

/// Finalize array of msg/LowCmd messages.
/**
 * It calls
 * igris_c_sdk__msg__LowCmd__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
void
igris_c_sdk__msg__LowCmd__Sequence__fini(igris_c_sdk__msg__LowCmd__Sequence * array);

/// Create array of msg/LowCmd messages.
/**
 * It allocates the memory for the array and calls
 * igris_c_sdk__msg__LowCmd__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
igris_c_sdk__msg__LowCmd__Sequence *
igris_c_sdk__msg__LowCmd__Sequence__create(size_t size);

/// Destroy array of msg/LowCmd messages.
/**
 * It calls
 * igris_c_sdk__msg__LowCmd__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
void
igris_c_sdk__msg__LowCmd__Sequence__destroy(igris_c_sdk__msg__LowCmd__Sequence * array);

/// Check for msg/LowCmd message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
bool
igris_c_sdk__msg__LowCmd__Sequence__are_equal(const igris_c_sdk__msg__LowCmd__Sequence * lhs, const igris_c_sdk__msg__LowCmd__Sequence * rhs);

/// Copy an array of msg/LowCmd messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_igris_c_sdk
bool
igris_c_sdk__msg__LowCmd__Sequence__copy(
  const igris_c_sdk__msg__LowCmd__Sequence * input,
  igris_c_sdk__msg__LowCmd__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__FUNCTIONS_H_
