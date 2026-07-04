// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from igris_c_sdk:msg/RobotState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/robot_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__STRUCT_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__igris_c_sdk__msg__RobotState __attribute__((deprecated))
#else
# define DEPRECATED__igris_c_sdk__msg__RobotState __declspec(deprecated)
#endif

namespace igris_c_sdk
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct RobotState_
{
  using Type = RobotState_<ContainerAllocator>;

  explicit RobotState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->state = 0;
      this->environment = 0;
    }
  }

  explicit RobotState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->state = 0;
      this->environment = 0;
    }
  }

  // field types and members
  using _header_type =
    igris_c_sdk::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _state_type =
    uint8_t;
  _state_type state;
  using _environment_type =
    uint8_t;
  _environment_type environment;

  // setters for named parameter idiom
  Type & set__header(
    const igris_c_sdk::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__state(
    const uint8_t & _arg)
  {
    this->state = _arg;
    return *this;
  }
  Type & set__environment(
    const uint8_t & _arg)
  {
    this->environment = _arg;
    return *this;
  }

  // constant declarations
  static constexpr uint8_t ROBOT_STATE_NOT_READY =
    0u;
  static constexpr uint8_t ROBOT_STATE_IDLE =
    1u;
  static constexpr uint8_t ROBOT_STATE_STOP =
    2u;
  static constexpr uint8_t ROBOT_STATE_LOW =
    3u;
  static constexpr uint8_t ROBOT_STATE_HIGH_MOTION_ACTIVE =
    4u;
  static constexpr uint8_t ROBOT_STATE_HIGH_MOTION_STANDBY =
    5u;
  static constexpr uint8_t ROBOT_STATE_WALK_LOW =
    6u;
  static constexpr uint8_t ROBOT_STATE_WALK_HIGH_MOTION_ACTIVE =
    7u;
  static constexpr uint8_t ROBOT_STATE_WALK_HIGH_MOTION_STANDBY =
    8u;
  static constexpr uint8_t ROBOT_STATE_WHOLE_BODY_CONTROL =
    9u;
  static constexpr uint8_t ROBOT_ENV_REAL =
    0u;
  static constexpr uint8_t ROBOT_ENV_SIM =
    1u;

  // pointer types
  using RawPtr =
    igris_c_sdk::msg::RobotState_<ContainerAllocator> *;
  using ConstRawPtr =
    const igris_c_sdk::msg::RobotState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<igris_c_sdk::msg::RobotState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<igris_c_sdk::msg::RobotState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::RobotState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::RobotState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::RobotState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::RobotState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<igris_c_sdk::msg::RobotState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<igris_c_sdk::msg::RobotState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__igris_c_sdk__msg__RobotState
    std::shared_ptr<igris_c_sdk::msg::RobotState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__igris_c_sdk__msg__RobotState
    std::shared_ptr<igris_c_sdk::msg::RobotState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const RobotState_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->state != other.state) {
      return false;
    }
    if (this->environment != other.environment) {
      return false;
    }
    return true;
  }
  bool operator!=(const RobotState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct RobotState_

// alias to use template instance with default allocator
using RobotState =
  igris_c_sdk::msg::RobotState_<std::allocator<void>>;

// constant definitions
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_STATE_NOT_READY;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_STATE_IDLE;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_STATE_STOP;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_STATE_LOW;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_STATE_HIGH_MOTION_ACTIVE;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_STATE_HIGH_MOTION_STANDBY;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_STATE_WALK_LOW;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_STATE_WALK_HIGH_MOTION_ACTIVE;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_STATE_WALK_HIGH_MOTION_STANDBY;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_STATE_WHOLE_BODY_CONTROL;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_ENV_REAL;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t RobotState_<ContainerAllocator>::ROBOT_ENV_SIM;
#endif  // __cplusplus < 201703L

}  // namespace msg

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__STRUCT_HPP_
