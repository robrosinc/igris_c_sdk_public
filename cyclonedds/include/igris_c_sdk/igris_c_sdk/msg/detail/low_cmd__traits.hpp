// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from igris_c_sdk:msg/LowCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/low_cmd.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__TRAITS_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "igris_c_sdk/msg/detail/low_cmd__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__traits.hpp"
// Member 'motors'
#include "igris_c_sdk/msg/detail/motor_cmd__traits.hpp"

namespace igris_c_sdk
{

namespace msg
{

inline void to_flow_style_yaml(
  const LowCmd & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: kinematic_modes
  {
    if (msg.kinematic_modes.size() == 0) {
      out << "kinematic_modes: []";
    } else {
      out << "kinematic_modes: [";
      size_t pending_items = msg.kinematic_modes.size();
      for (auto item : msg.kinematic_modes) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: motors
  {
    if (msg.motors.size() == 0) {
      out << "motors: []";
    } else {
      out << "motors: [";
      size_t pending_items = msg.motors.size();
      for (auto item : msg.motors) {
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
  const LowCmd & msg,
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

  // member: kinematic_modes
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.kinematic_modes.size() == 0) {
      out << "kinematic_modes: []\n";
    } else {
      out << "kinematic_modes:\n";
      for (auto item : msg.kinematic_modes) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: motors
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.motors.size() == 0) {
      out << "motors: []\n";
    } else {
      out << "motors:\n";
      for (auto item : msg.motors) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const LowCmd & msg, bool use_flow_style = false)
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
  const igris_c_sdk::msg::LowCmd & msg,
  std::ostream & out, size_t indentation = 0)
{
  igris_c_sdk::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use igris_c_sdk::msg::to_yaml() instead")]]
inline std::string to_yaml(const igris_c_sdk::msg::LowCmd & msg)
{
  return igris_c_sdk::msg::to_yaml(msg);
}

template<>
inline const char * data_type<igris_c_sdk::msg::LowCmd>()
{
  return "igris_c_sdk::msg::LowCmd";
}

template<>
inline const char * name<igris_c_sdk::msg::LowCmd>()
{
  return "igris_c_sdk/msg/LowCmd";
}

template<>
struct has_fixed_size<igris_c_sdk::msg::LowCmd>
  : std::integral_constant<bool, has_fixed_size<igris_c_sdk::msg::Header>::value && has_fixed_size<igris_c_sdk::msg::MotorCmd>::value> {};

template<>
struct has_bounded_size<igris_c_sdk::msg::LowCmd>
  : std::integral_constant<bool, has_bounded_size<igris_c_sdk::msg::Header>::value && has_bounded_size<igris_c_sdk::msg::MotorCmd>::value> {};

template<>
struct is_message<igris_c_sdk::msg::LowCmd>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // IGRIS_C_SDK__MSG__DETAIL__LOW_CMD__TRAITS_HPP_
