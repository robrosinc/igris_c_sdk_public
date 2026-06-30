// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from igris_c_sdk:msg/LowState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/low_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__TRAITS_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "igris_c_sdk/msg/detail/low_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__traits.hpp"
// Member 'imu_state'
#include "igris_c_sdk/msg/detail/imu_state__traits.hpp"
// Member 'motor_state'
#include "igris_c_sdk/msg/detail/motor_state__traits.hpp"
// Member 'joint_state'
#include "igris_c_sdk/msg/detail/joint_state__traits.hpp"

namespace igris_c_sdk
{

namespace msg
{

inline void to_flow_style_yaml(
  const LowState & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: imu_state
  {
    out << "imu_state: ";
    to_flow_style_yaml(msg.imu_state, out);
    out << ", ";
  }

  // member: motor_state
  {
    if (msg.motor_state.size() == 0) {
      out << "motor_state: []";
    } else {
      out << "motor_state: [";
      size_t pending_items = msg.motor_state.size();
      for (auto item : msg.motor_state) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: joint_state
  {
    if (msg.joint_state.size() == 0) {
      out << "joint_state: []";
    } else {
      out << "joint_state: [";
      size_t pending_items = msg.joint_state.size();
      for (auto item : msg.joint_state) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const LowState & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: imu_state
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "imu_state:\n";
    to_block_style_yaml(msg.imu_state, out, indentation + 2);
  }

  // member: motor_state
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.motor_state.size() == 0) {
      out << "motor_state: []\n";
    } else {
      out << "motor_state:\n";
      for (auto item : msg.motor_state) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: joint_state
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.joint_state.size() == 0) {
      out << "joint_state: []\n";
    } else {
      out << "joint_state:\n";
      for (auto item : msg.joint_state) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const LowState & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace igris_c_sdk

namespace rosidl_generator_traits
{

[[deprecated("use igris_c_sdk::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const igris_c_sdk::msg::LowState & msg,
  std::ostream & out, size_t indentation = 0)
{
  igris_c_sdk::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use igris_c_sdk::msg::to_yaml() instead")]]
inline std::string to_yaml(const igris_c_sdk::msg::LowState & msg)
{
  return igris_c_sdk::msg::to_yaml(msg);
}

template<>
inline const char * data_type<igris_c_sdk::msg::LowState>()
{
  return "igris_c_sdk::msg::LowState";
}

template<>
inline const char * name<igris_c_sdk::msg::LowState>()
{
  return "igris_c_sdk/msg/LowState";
}

template<>
struct has_fixed_size<igris_c_sdk::msg::LowState>
  : std::integral_constant<bool, has_fixed_size<igris_c_sdk::msg::Header>::value && has_fixed_size<igris_c_sdk::msg::IMUState>::value && has_fixed_size<igris_c_sdk::msg::JointState>::value && has_fixed_size<igris_c_sdk::msg::MotorState>::value> {};

template<>
struct has_bounded_size<igris_c_sdk::msg::LowState>
  : std::integral_constant<bool, has_bounded_size<igris_c_sdk::msg::Header>::value && has_bounded_size<igris_c_sdk::msg::IMUState>::value && has_bounded_size<igris_c_sdk::msg::JointState>::value && has_bounded_size<igris_c_sdk::msg::MotorState>::value> {};

template<>
struct is_message<igris_c_sdk::msg::LowState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // IGRIS_C_SDK__MSG__DETAIL__LOW_STATE__TRAITS_HPP_
