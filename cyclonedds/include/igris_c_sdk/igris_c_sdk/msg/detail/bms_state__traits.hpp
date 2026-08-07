// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from igris_c_sdk:msg/BmsState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/bms_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__TRAITS_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "igris_c_sdk/msg/detail/bms_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__traits.hpp"

namespace igris_c_sdk
{

namespace msg
{

inline void to_flow_style_yaml(
  const BmsState & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: body_power
  {
    out << "body_power: ";
    rosidl_generator_traits::value_to_yaml(msg.body_power, out);
    out << ", ";
  }

  // member: legs_power
  {
    out << "legs_power: ";
    rosidl_generator_traits::value_to_yaml(msg.legs_power, out);
    out << ", ";
  }

  // member: estop
  {
    out << "estop: ";
    rosidl_generator_traits::value_to_yaml(msg.estop, out);
    out << ", ";
  }

  // member: connect
  {
    out << "connect: ";
    rosidl_generator_traits::value_to_yaml(msg.connect, out);
    out << ", ";
  }

  // member: battery
  {
    out << "battery: ";
    rosidl_generator_traits::value_to_yaml(msg.battery, out);
    out << ", ";
  }

  // member: bms_init_state
  {
    out << "bms_init_state: ";
    rosidl_generator_traits::value_to_yaml(msg.bms_init_state, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const BmsState & msg,
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

  // member: body_power
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "body_power: ";
    rosidl_generator_traits::value_to_yaml(msg.body_power, out);
    out << "\n";
  }

  // member: legs_power
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "legs_power: ";
    rosidl_generator_traits::value_to_yaml(msg.legs_power, out);
    out << "\n";
  }

  // member: estop
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "estop: ";
    rosidl_generator_traits::value_to_yaml(msg.estop, out);
    out << "\n";
  }

  // member: connect
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "connect: ";
    rosidl_generator_traits::value_to_yaml(msg.connect, out);
    out << "\n";
  }

  // member: battery
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "battery: ";
    rosidl_generator_traits::value_to_yaml(msg.battery, out);
    out << "\n";
  }

  // member: bms_init_state
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "bms_init_state: ";
    rosidl_generator_traits::value_to_yaml(msg.bms_init_state, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const BmsState & msg, bool use_flow_style = false)
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
  const igris_c_sdk::msg::BmsState & msg,
  std::ostream & out, size_t indentation = 0)
{
  igris_c_sdk::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use igris_c_sdk::msg::to_yaml() instead")]]
inline std::string to_yaml(const igris_c_sdk::msg::BmsState & msg)
{
  return igris_c_sdk::msg::to_yaml(msg);
}

template<>
inline const char * data_type<igris_c_sdk::msg::BmsState>()
{
  return "igris_c_sdk::msg::BmsState";
}

template<>
inline const char * name<igris_c_sdk::msg::BmsState>()
{
  return "igris_c_sdk/msg/BmsState";
}

template<>
struct has_fixed_size<igris_c_sdk::msg::BmsState>
  : std::integral_constant<bool, has_fixed_size<igris_c_sdk::msg::Header>::value> {};

template<>
struct has_bounded_size<igris_c_sdk::msg::BmsState>
  : std::integral_constant<bool, has_bounded_size<igris_c_sdk::msg::Header>::value> {};

template<>
struct is_message<igris_c_sdk::msg::BmsState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // IGRIS_C_SDK__MSG__DETAIL__BMS_STATE__TRAITS_HPP_
