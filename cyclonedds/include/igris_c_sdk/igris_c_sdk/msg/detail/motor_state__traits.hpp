// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from igris_c_sdk:msg/MotorState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/msg/motor_state.hpp"


#ifndef IGRIS_C_SDK__MSG__DETAIL__MOTOR_STATE__TRAITS_HPP_
#define IGRIS_C_SDK__MSG__DETAIL__MOTOR_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "igris_c_sdk/msg/detail/motor_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace igris_c_sdk
{

namespace msg
{

inline void to_flow_style_yaml(
  const MotorState & msg,
  std::ostream & out)
{
  out << "{";
  // member: q
  {
    out << "q: ";
    rosidl_generator_traits::value_to_yaml(msg.q, out);
    out << ", ";
  }

  // member: dq
  {
    out << "dq: ";
    rosidl_generator_traits::value_to_yaml(msg.dq, out);
    out << ", ";
  }

  // member: tau_est
  {
    out << "tau_est: ";
    rosidl_generator_traits::value_to_yaml(msg.tau_est, out);
    out << ", ";
  }

  // member: temperature
  {
    out << "temperature: ";
    rosidl_generator_traits::value_to_yaml(msg.temperature, out);
    out << ", ";
  }

  // member: status_bits
  {
    out << "status_bits: ";
    rosidl_generator_traits::value_to_yaml(msg.status_bits, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MotorState & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: q
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "q: ";
    rosidl_generator_traits::value_to_yaml(msg.q, out);
    out << "\n";
  }

  // member: dq
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "dq: ";
    rosidl_generator_traits::value_to_yaml(msg.dq, out);
    out << "\n";
  }

  // member: tau_est
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "tau_est: ";
    rosidl_generator_traits::value_to_yaml(msg.tau_est, out);
    out << "\n";
  }

  // member: temperature
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "temperature: ";
    rosidl_generator_traits::value_to_yaml(msg.temperature, out);
    out << "\n";
  }

  // member: status_bits
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "status_bits: ";
    rosidl_generator_traits::value_to_yaml(msg.status_bits, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MotorState & msg, bool use_flow_style = false)
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
  const igris_c_sdk::msg::MotorState & msg,
  std::ostream & out, size_t indentation = 0)
{
  igris_c_sdk::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use igris_c_sdk::msg::to_yaml() instead")]]
inline std::string to_yaml(const igris_c_sdk::msg::MotorState & msg)
{
  return igris_c_sdk::msg::to_yaml(msg);
}

template<>
inline const char * data_type<igris_c_sdk::msg::MotorState>()
{
  return "igris_c_sdk::msg::MotorState";
}

template<>
inline const char * name<igris_c_sdk::msg::MotorState>()
{
  return "igris_c_sdk/msg/MotorState";
}

template<>
struct has_fixed_size<igris_c_sdk::msg::MotorState>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<igris_c_sdk::msg::MotorState>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<igris_c_sdk::msg::MotorState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // IGRIS_C_SDK__MSG__DETAIL__MOTOR_STATE__TRAITS_HPP_
