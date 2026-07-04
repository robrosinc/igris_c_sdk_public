// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from igris_c_sdk:msg/ServiceResponse.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/service_response.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__SERVICE_RESPONSE__STRUCT_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__SERVICE_RESPONSE__STRUCT_HPP_

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
# define DEPRECATED__igris_c_sdk__msg__ServiceResponse __attribute__((deprecated))
#else
# define DEPRECATED__igris_c_sdk__msg__ServiceResponse __declspec(deprecated)
#endif

namespace igris_c_sdk
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ServiceResponse_
{
  using Type = ServiceResponse_<ContainerAllocator>;

  explicit ServiceResponse_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->request_id = "";
      this->success = false;
      this->message = "";
      this->error_code = 0l;
    }
  }

  explicit ServiceResponse_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    request_id(_alloc),
    message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->request_id = "";
      this->success = false;
      this->message = "";
      this->error_code = 0l;
    }
  }

  // field types and members
  using _header_type =
    igris_c_sdk::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _request_id_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _request_id_type request_id;
  using _success_type =
    bool;
  _success_type success;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;
  using _error_code_type =
    int32_t;
  _error_code_type error_code;

  // setters for named parameter idiom
  Type & set__header(
    const igris_c_sdk::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__request_id(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->request_id = _arg;
    return *this;
  }
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }
  Type & set__error_code(
    const int32_t & _arg)
  {
    this->error_code = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    igris_c_sdk::msg::ServiceResponse_<ContainerAllocator> *;
  using ConstRawPtr =
    const igris_c_sdk::msg::ServiceResponse_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<igris_c_sdk::msg::ServiceResponse_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<igris_c_sdk::msg::ServiceResponse_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::ServiceResponse_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::ServiceResponse_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      igris_c_sdk::msg::ServiceResponse_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<igris_c_sdk::msg::ServiceResponse_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<igris_c_sdk::msg::ServiceResponse_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<igris_c_sdk::msg::ServiceResponse_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__igris_c_sdk__msg__ServiceResponse
    std::shared_ptr<igris_c_sdk::msg::ServiceResponse_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__igris_c_sdk__msg__ServiceResponse
    std::shared_ptr<igris_c_sdk::msg::ServiceResponse_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ServiceResponse_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->request_id != other.request_id) {
      return false;
    }
    if (this->success != other.success) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    if (this->error_code != other.error_code) {
      return false;
    }
    return true;
  }
  bool operator!=(const ServiceResponse_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ServiceResponse_

// alias to use template instance with default allocator
using ServiceResponse =
  igris_c_sdk::msg::ServiceResponse_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace igris_c_sdk

#endif  // IGRIS_C_SDK__MSG__DETAIL__SERVICE_RESPONSE__STRUCT_HPP_
