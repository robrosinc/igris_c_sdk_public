// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from igris_c_sdk:msg/RobotState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/robot_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "igris_c_sdk/msg/detail/robot_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__traits.hpp"

namespace igris_c_sdk
{

namespace msg
{

inline void to_flow_style_yaml(
  const RobotState & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: state
  {
    out << "state: ";
    rosidl_generator_traits::value_to_yaml(msg.state, out);
    out << ", ";
  }

  // member: environment
  {
    out << "environment: ";
    rosidl_generator_traits::value_to_yaml(msg.environment, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const RobotState & msg,
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

  // member: state
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "state: ";
    rosidl_generator_traits::value_to_yaml(msg.state, out);
    out << "\n";
  }

  // member: environment
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "environment: ";
    rosidl_generator_traits::value_to_yaml(msg.environment, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const RobotState & msg, bool use_flow_style = false)
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
  const igris_c_sdk::msg::RobotState & msg,
  std::ostream & out, size_t indentation = 0)
{
  igris_c_sdk::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use igris_c_sdk::msg::to_yaml() instead")]]
inline std::string to_yaml(const igris_c_sdk::msg::RobotState & msg)
{
  return igris_c_sdk::msg::to_yaml(msg);
}

template<>
inline const char * data_type<igris_c_sdk::msg::RobotState>()
{
  return "igris_c_sdk::msg::RobotState";
}

template<>
inline const char * name<igris_c_sdk::msg::RobotState>()
{
  return "igris_c_sdk/msg/RobotState";
}

template<>
struct has_fixed_size<igris_c_sdk::msg::RobotState>
  : std::integral_constant<bool, has_fixed_size<igris_c_sdk::msg::Header>::value> {};

template<>
struct has_bounded_size<igris_c_sdk::msg::RobotState>
  : std::integral_constant<bool, has_bounded_size<igris_c_sdk::msg::Header>::value> {};

template<>
struct is_message<igris_c_sdk::msg::RobotState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // IGRIS_C_SDK__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_
