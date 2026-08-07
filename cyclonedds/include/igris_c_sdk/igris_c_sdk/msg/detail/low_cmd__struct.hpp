// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from igris_c_sdk:msg/LowCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/low_cmd.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__STRUCT_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__STRUCT_HPP_

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
// Member 'motors'
#include "igris_c_sdk/msg/detail/motor_cmd__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__igris_c_sdk__msg__LowCmd __attribute__((deprecated))
#else
# define DEPRECATED__igris_c_sdk__msg__LowCmd __declspec(deprecated)
#endif

namespace igris_c_sdk
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct LowCmd_
{
  using Type = LowCmd_<ContainerAllocator>;

  explicit LowCmd_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<uint8_t, 5>::iterator, uint8_t>(this->kinematic_modes.begin(), this->kinematic_modes.end(), 0);
      this->motors.fill(igris_c_sdk::msg::MotorCmd_<ContainerAllocator>{_init});
    }
  }

  explicit LowCmd_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    kinematic_modes(_alloc),
    motors(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<uint8_t, 5>::iterator, uint8_t>(this->kinematic_modes.begin(), this->kinematic_modes.end(), 0);
      this->motors.fill(igris_c_sdk::msg::MotorCmd_<ContainerAllocator>{_alloc, _init});
    }
  }

  // field types and members
  using _header_type =
    igris_c_sdk::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _kinematic_modes_type =
    std::array<uint8_t, 5>;
  _kinematic_modes_type kinematic_modes;
  using _motors_type =
    std::array<igris_c_sdk::msg::MotorCmd_<ContainerAllocator>, 31>;
  _motors_type motors;

  // setters for named parameter idiom
  Type & set__header(
    const igris_c_sdk::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__kinematic_modes(
    const std::array<uint8_t, 5> & _arg)
  {
    this->kinematic_modes = _arg;
    return *this;
  }
  Type & set__motors(
    const std::array<igris_c_sdk::msg::MotorCmd_<ContainerAllocator>, 31> & _arg)
  {
    this->motors = _arg;
    return *this;
  }

  // constant declarations
  static constexpr uint8_t KINEMATIC_MODE_MS =
    0u;
  static constexpr uint8_t KINEMATIC_MODE_PJS =
    1u;

  // pointer types
  using RawPtr =
    igris_c_sdk::msg::LowCmd_<ContainerAllocator> *;
  using ConstRawPtr =
    const igris_c_sdk::msg::LowCmd_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<igris_c_sdk::msg::LowCmd_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<igris_c_sdk::msg::LowCmd_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::LowCmd_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::LowCmd_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::LowCmd_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::LowCmd_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<igris_c_sdk::msg::LowCmd_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<igris_c_sdk::msg::LowCmd_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__igris_c_sdk__msg__LowCmd
    std::shared_ptr<igris_c_sdk::msg::LowCmd_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__igris_c_sdk__msg__LowCmd
    std::shared_ptr<igris_c_sdk::msg::LowCmd_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const LowCmd_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->kinematic_modes != other.kinematic_modes) {
      return false;
    }
    if (this->motors != other.motors) {
      return false;
    }
    return true;
  }
  bool operator!=(const LowCmd_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct LowCmd_

// alias to use template instance with default allocator
using LowCmd =
  igris_c_sdk::msg::LowCmd_<std::allocator<void>>;

// constant definitions
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t LowCmd_<ContainerAllocator>::KINEMATIC_MODE_MS;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t LowCmd_<ContainerAllocator>::KINEMATIC_MODE_PJS;
#endif  // __cplusplus < 201703L

}  // namespace msg

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__STRUCT_HPP_
