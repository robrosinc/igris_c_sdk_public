// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from igris_c_sdk:msg/BmsState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/bms_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__STRUCT_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__STRUCT_HPP_

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
# define DEPRECATED__igris_c_sdk__msg__BmsState __attribute__((deprecated))
#else
# define DEPRECATED__igris_c_sdk__msg__BmsState __declspec(deprecated)
#endif

namespace igris_c_sdk
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct BmsState_
{
  using Type = BmsState_<ContainerAllocator>;

  explicit BmsState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->body_power = 0;
      this->legs_power = 0;
      this->estop = 0;
      this->connect = 0;
      this->battery = 0.0f;
      this->bms_init_state = 0;
    }
  }

  explicit BmsState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->body_power = 0;
      this->legs_power = 0;
      this->estop = 0;
      this->connect = 0;
      this->battery = 0.0f;
      this->bms_init_state = 0;
    }
  }

  // field types and members
  using _header_type =
    igris_c_sdk::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _body_power_type =
    uint8_t;
  _body_power_type body_power;
  using _legs_power_type =
    uint8_t;
  _legs_power_type legs_power;
  using _estop_type =
    uint8_t;
  _estop_type estop;
  using _connect_type =
    uint8_t;
  _connect_type connect;
  using _battery_type =
    float;
  _battery_type battery;
  using _bms_init_state_type =
    uint8_t;
  _bms_init_state_type bms_init_state;

  // setters for named parameter idiom
  Type & set__header(
    const igris_c_sdk::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__body_power(
    const uint8_t & _arg)
  {
    this->body_power = _arg;
    return *this;
  }
  Type & set__legs_power(
    const uint8_t & _arg)
  {
    this->legs_power = _arg;
    return *this;
  }
  Type & set__estop(
    const uint8_t & _arg)
  {
    this->estop = _arg;
    return *this;
  }
  Type & set__connect(
    const uint8_t & _arg)
  {
    this->connect = _arg;
    return *this;
  }
  Type & set__battery(
    const float & _arg)
  {
    this->battery = _arg;
    return *this;
  }
  Type & set__bms_init_state(
    const uint8_t & _arg)
  {
    this->bms_init_state = _arg;
    return *this;
  }

  // constant declarations
  static constexpr uint8_t RELAY_OFF =
    0u;
  static constexpr uint8_t RELAY_ON =
    1u;
  static constexpr uint8_t ESTOP_RELEASED =
    0u;
  static constexpr uint8_t ESTOP_PRESSED =
    1u;
  static constexpr uint8_t BMS_DISCONNECTED =
    0u;
  static constexpr uint8_t BMS_CONNECTED =
    1u;
  static constexpr uint8_t BMS_NOT_INITIALIZED =
    0u;
  static constexpr uint8_t BMS_INITIALIZED =
    1u;
  static constexpr uint8_t MOTOR_INITIALIZED =
    2u;
  static constexpr uint8_t BOTH_INITIALIZED =
    3u;

  // pointer types
  using RawPtr =
    igris_c_sdk::msg::BmsState_<ContainerAllocator> *;
  using ConstRawPtr =
    const igris_c_sdk::msg::BmsState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<igris_c_sdk::msg::BmsState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<igris_c_sdk::msg::BmsState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::BmsState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::BmsState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::BmsState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::BmsState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<igris_c_sdk::msg::BmsState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<igris_c_sdk::msg::BmsState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__igris_c_sdk__msg__BmsState
    std::shared_ptr<igris_c_sdk::msg::BmsState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__igris_c_sdk__msg__BmsState
    std::shared_ptr<igris_c_sdk::msg::BmsState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const BmsState_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->body_power != other.body_power) {
      return false;
    }
    if (this->legs_power != other.legs_power) {
      return false;
    }
    if (this->estop != other.estop) {
      return false;
    }
    if (this->connect != other.connect) {
      return false;
    }
    if (this->battery != other.battery) {
      return false;
    }
    if (this->bms_init_state != other.bms_init_state) {
      return false;
    }
    return true;
  }
  bool operator!=(const BmsState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct BmsState_

// alias to use template instance with default allocator
using BmsState =
  igris_c_sdk::msg::BmsState_<std::allocator<void>>;

// constant definitions
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t BmsState_<ContainerAllocator>::RELAY_OFF;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t BmsState_<ContainerAllocator>::RELAY_ON;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t BmsState_<ContainerAllocator>::ESTOP_RELEASED;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t BmsState_<ContainerAllocator>::ESTOP_PRESSED;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t BmsState_<ContainerAllocator>::BMS_DISCONNECTED;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t BmsState_<ContainerAllocator>::BMS_CONNECTED;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t BmsState_<ContainerAllocator>::BMS_NOT_INITIALIZED;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t BmsState_<ContainerAllocator>::BMS_INITIALIZED;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t BmsState_<ContainerAllocator>::MOTOR_INITIALIZED;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t BmsState_<ContainerAllocator>::BOTH_INITIALIZED;
#endif  // __cplusplus < 201703L

}  // namespace msg

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__STRUCT_HPP_
