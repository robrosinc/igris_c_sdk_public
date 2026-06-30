// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from igris_c_sdk:msg/MotorState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/motor_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__MOTOR_STATE__STRUCT_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__MOTOR_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__igris_c_sdk__msg__MotorState __attribute__((deprecated))
#else
# define DEPRECATED__igris_c_sdk__msg__MotorState __declspec(deprecated)
#endif

namespace igris_c_sdk
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct MotorState_
{
  using Type = MotorState_<ContainerAllocator>;

  explicit MotorState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->q = 0.0f;
      this->dq = 0.0f;
      this->tau_est = 0.0f;
      this->temperature = 0;
      this->status_bits = 0ul;
    }
  }

  explicit MotorState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->q = 0.0f;
      this->dq = 0.0f;
      this->tau_est = 0.0f;
      this->temperature = 0;
      this->status_bits = 0ul;
    }
  }

  // field types and members
  using _q_type =
    float;
  _q_type q;
  using _dq_type =
    float;
  _dq_type dq;
  using _tau_est_type =
    float;
  _tau_est_type tau_est;
  using _temperature_type =
    int16_t;
  _temperature_type temperature;
  using _status_bits_type =
    uint32_t;
  _status_bits_type status_bits;

  // setters for named parameter idiom
  Type & set__q(
    const float & _arg)
  {
    this->q = _arg;
    return *this;
  }
  Type & set__dq(
    const float & _arg)
  {
    this->dq = _arg;
    return *this;
  }
  Type & set__tau_est(
    const float & _arg)
  {
    this->tau_est = _arg;
    return *this;
  }
  Type & set__temperature(
    const int16_t & _arg)
  {
    this->temperature = _arg;
    return *this;
  }
  Type & set__status_bits(
    const uint32_t & _arg)
  {
    this->status_bits = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    igris_c_sdk::msg::MotorState_<ContainerAllocator> *;
  using ConstRawPtr =
    const igris_c_sdk::msg::MotorState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<igris_c_sdk::msg::MotorState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<igris_c_sdk::msg::MotorState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::MotorState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::MotorState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::MotorState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::MotorState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<igris_c_sdk::msg::MotorState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<igris_c_sdk::msg::MotorState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__igris_c_sdk__msg__MotorState
    std::shared_ptr<igris_c_sdk::msg::MotorState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__igris_c_sdk__msg__MotorState
    std::shared_ptr<igris_c_sdk::msg::MotorState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MotorState_ & other) const
  {
    if (this->q != other.q) {
      return false;
    }
    if (this->dq != other.dq) {
      return false;
    }
    if (this->tau_est != other.tau_est) {
      return false;
    }
    if (this->temperature != other.temperature) {
      return false;
    }
    if (this->status_bits != other.status_bits) {
      return false;
    }
    return true;
  }
  bool operator!=(const MotorState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MotorState_

// alias to use template instance with default allocator
using MotorState =
  igris_c_sdk::msg::MotorState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__MOTOR_STATE__STRUCT_HPP_
