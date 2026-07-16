// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from igris_c_sdk:msg/LowState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/low_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__STRUCT_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__STRUCT_HPP_

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
// Member 'imu_state'
#include "igris_c_sdk/msg/detail/imu_state__struct.hpp"
// Member 'motor_state'
#include "igris_c_sdk/msg/detail/motor_state__struct.hpp"
// Member 'joint_state'
#include "igris_c_sdk/msg/detail/joint_state__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__igris_c_sdk__msg__LowState __attribute__((deprecated))
#else
# define DEPRECATED__igris_c_sdk__msg__LowState __declspec(deprecated)
#endif

namespace igris_c_sdk
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct LowState_
{
  using Type = LowState_<ContainerAllocator>;

  explicit LowState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    imu_state(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->motor_state.fill(igris_c_sdk::msg::MotorState_<ContainerAllocator>{_init});
      this->joint_state.fill(igris_c_sdk::msg::JointState_<ContainerAllocator>{_init});
    }
  }

  explicit LowState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    imu_state(_alloc, _init),
    motor_state(_alloc),
    joint_state(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->motor_state.fill(igris_c_sdk::msg::MotorState_<ContainerAllocator>{_alloc, _init});
      this->joint_state.fill(igris_c_sdk::msg::JointState_<ContainerAllocator>{_alloc, _init});
    }
  }

  // field types and members
  using _header_type =
    igris_c_sdk::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _imu_state_type =
    igris_c_sdk::msg::IMUState_<ContainerAllocator>;
  _imu_state_type imu_state;
  using _motor_state_type =
    std::array<igris_c_sdk::msg::MotorState_<ContainerAllocator>, 31>;
  _motor_state_type motor_state;
  using _joint_state_type =
    std::array<igris_c_sdk::msg::JointState_<ContainerAllocator>, 31>;
  _joint_state_type joint_state;

  // setters for named parameter idiom
  Type & set__header(
    const igris_c_sdk::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__imu_state(
    const igris_c_sdk::msg::IMUState_<ContainerAllocator> & _arg)
  {
    this->imu_state = _arg;
    return *this;
  }
  Type & set__motor_state(
    const std::array<igris_c_sdk::msg::MotorState_<ContainerAllocator>, 31> & _arg)
  {
    this->motor_state = _arg;
    return *this;
  }
  Type & set__joint_state(
    const std::array<igris_c_sdk::msg::JointState_<ContainerAllocator>, 31> & _arg)
  {
    this->joint_state = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    igris_c_sdk::msg::LowState_<ContainerAllocator> *;
  using ConstRawPtr =
    const igris_c_sdk::msg::LowState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<igris_c_sdk::msg::LowState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<igris_c_sdk::msg::LowState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::LowState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::LowState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::LowState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::LowState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<igris_c_sdk::msg::LowState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<igris_c_sdk::msg::LowState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__igris_c_sdk__msg__LowState
    std::shared_ptr<igris_c_sdk::msg::LowState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__igris_c_sdk__msg__LowState
    std::shared_ptr<igris_c_sdk::msg::LowState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const LowState_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->imu_state != other.imu_state) {
      return false;
    }
    if (this->motor_state != other.motor_state) {
      return false;
    }
    if (this->joint_state != other.joint_state) {
      return false;
    }
    return true;
  }
  bool operator!=(const LowState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct LowState_

// alias to use template instance with default allocator
using LowState =
  igris_c_sdk::msg::LowState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__STRUCT_HPP_
