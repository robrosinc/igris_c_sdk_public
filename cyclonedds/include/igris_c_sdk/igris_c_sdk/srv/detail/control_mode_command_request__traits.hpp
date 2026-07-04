// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from igris_c_sdk:srv/ControlModeCommandRequest.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/srv/control_mode_command_request.hpp"


#ifndef IGRIS_C_SDK__SRV__DETAIL__CONTROL_MODE_COMMAND_REQUEST__TRAITS_HPP_
#define IGRIS_C_SDK__SRV__DETAIL__CONTROL_MODE_COMMAND_REQUEST__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "igris_c_sdk/srv/detail/control_mode_command_request__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__traits.hpp"

namespace igris_c_sdk
{

namespace srv
{

inline void to_flow_style_yaml(
  const ControlModeCommandRequest_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: request_id
  {
    out << "request_id: ";
    rosidl_generator_traits::value_to_yaml(msg.request_id, out);
    out << ", ";
  }

  // member: command_type
  {
    out << "command_type: ";
    rosidl_generator_traits::value_to_yaml(msg.command_type, out);
    out << ", ";
  }

  // member: preset_id
  {
    out << "preset_id: ";
    rosidl_generator_traits::value_to_yaml(msg.preset_id, out);
    out << ", ";
  }

  // member: is_cyclic
  {
    out << "is_cyclic: ";
    rosidl_generator_traits::value_to_yaml(msg.is_cyclic, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ControlModeCommandRequest_Request & msg,
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

  // member: request_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "request_id: ";
    rosidl_generator_traits::value_to_yaml(msg.request_id, out);
    out << "\n";
  }

  // member: command_type
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "command_type: ";
    rosidl_generator_traits::value_to_yaml(msg.command_type, out);
    out << "\n";
  }

  // member: preset_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "preset_id: ";
    rosidl_generator_traits::value_to_yaml(msg.preset_id, out);
    out << "\n";
  }

  // member: is_cyclic
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "is_cyclic: ";
    rosidl_generator_traits::value_to_yaml(msg.is_cyclic, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ControlModeCommandRequest_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace igris_c_sdk

namespace rosidl_generator_traits
{

[[deprecated("use igris_c_sdk::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const igris_c_sdk::srv::ControlModeCommandRequest_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  igris_c_sdk::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use igris_c_sdk::srv::to_yaml() instead")]]
inline std::string to_yaml(const igris_c_sdk::srv::ControlModeCommandRequest_Request & msg)
{
  return igris_c_sdk::srv::to_yaml(msg);
}

template<>
inline const char * data_type<igris_c_sdk::srv::ControlModeCommandRequest_Request>()
{
  return "igris_c_sdk::srv::ControlModeCommandRequest_Request";
}

template<>
inline const char * name<igris_c_sdk::srv::ControlModeCommandRequest_Request>()
{
  return "igris_c_sdk/srv/ControlModeCommandRequest_Request";
}

template<>
struct has_fixed_size<igris_c_sdk::srv::ControlModeCommandRequest_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<igris_c_sdk::srv::ControlModeCommandRequest_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<igris_c_sdk::srv::ControlModeCommandRequest_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'header'
// already included above
// #include "igris_c_sdk/msg/detail/header__traits.hpp"

namespace igris_c_sdk
{

namespace srv
{

inline void to_flow_style_yaml(
  const ControlModeCommandRequest_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: request_id
  {
    out << "request_id: ";
    rosidl_generator_traits::value_to_yaml(msg.request_id, out);
    out << ", ";
  }

  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << ", ";
  }

  // member: error_code
  {
    out << "error_code: ";
    rosidl_generator_traits::value_to_yaml(msg.error_code, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ControlModeCommandRequest_Response & msg,
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

  // member: request_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "request_id: ";
    rosidl_generator_traits::value_to_yaml(msg.request_id, out);
    out << "\n";
  }

  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }

  // member: error_code
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "error_code: ";
    rosidl_generator_traits::value_to_yaml(msg.error_code, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ControlModeCommandRequest_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace igris_c_sdk

namespace rosidl_generator_traits
{

[[deprecated("use igris_c_sdk::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const igris_c_sdk::srv::ControlModeCommandRequest_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  igris_c_sdk::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use igris_c_sdk::srv::to_yaml() instead")]]
inline std::string to_yaml(const igris_c_sdk::srv::ControlModeCommandRequest_Response & msg)
{
  return igris_c_sdk::srv::to_yaml(msg);
}

template<>
inline const char * data_type<igris_c_sdk::srv::ControlModeCommandRequest_Response>()
{
  return "igris_c_sdk::srv::ControlModeCommandRequest_Response";
}

template<>
inline const char * name<igris_c_sdk::srv::ControlModeCommandRequest_Response>()
{
  return "igris_c_sdk/srv/ControlModeCommandRequest_Response";
}

template<>
struct has_fixed_size<igris_c_sdk::srv::ControlModeCommandRequest_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<igris_c_sdk::srv::ControlModeCommandRequest_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<igris_c_sdk::srv::ControlModeCommandRequest_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__traits.hpp"

namespace igris_c_sdk
{

namespace srv
{

inline void to_flow_style_yaml(
  const ControlModeCommandRequest_Event & msg,
  std::ostream & out)
{
  out << "{";
  // member: info
  {
    out << "info: ";
    to_flow_style_yaml(msg.info, out);
    out << ", ";
  }

  // member: request
  {
    if (msg.request.size() == 0) {
      out << "request: []";
    } else {
      out << "request: [";
      size_t pending_items = msg.request.size();
      for (auto item : msg.request) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: response
  {
    if (msg.response.size() == 0) {
      out << "response: []";
    } else {
      out << "response: [";
      size_t pending_items = msg.response.size();
      for (auto item : msg.response) {
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
  const ControlModeCommandRequest_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: info
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "info:\n";
    to_block_style_yaml(msg.info, out, indentation + 2);
  }

  // member: request
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.request.size() == 0) {
      out << "request: []\n";
    } else {
      out << "request:\n";
      for (auto item : msg.request) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: response
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.response.size() == 0) {
      out << "response: []\n";
    } else {
      out << "response:\n";
      for (auto item : msg.response) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ControlModeCommandRequest_Event & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace igris_c_sdk

namespace rosidl_generator_traits
{

[[deprecated("use igris_c_sdk::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const igris_c_sdk::srv::ControlModeCommandRequest_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  igris_c_sdk::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use igris_c_sdk::srv::to_yaml() instead")]]
inline std::string to_yaml(const igris_c_sdk::srv::ControlModeCommandRequest_Event & msg)
{
  return igris_c_sdk::srv::to_yaml(msg);
}

template<>
inline const char * data_type<igris_c_sdk::srv::ControlModeCommandRequest_Event>()
{
  return "igris_c_sdk::srv::ControlModeCommandRequest_Event";
}

template<>
inline const char * name<igris_c_sdk::srv::ControlModeCommandRequest_Event>()
{
  return "igris_c_sdk/srv/ControlModeCommandRequest_Event";
}

template<>
struct has_fixed_size<igris_c_sdk::srv::ControlModeCommandRequest_Event>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<igris_c_sdk::srv::ControlModeCommandRequest_Event>
  : std::integral_constant<bool, has_bounded_size<igris_c_sdk::srv::ControlModeCommandRequest_Request>::value && has_bounded_size<igris_c_sdk::srv::ControlModeCommandRequest_Response>::value && has_bounded_size<service_msgs::msg::ServiceEventInfo>::value> {};

template<>
struct is_message<igris_c_sdk::srv::ControlModeCommandRequest_Event>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<igris_c_sdk::srv::ControlModeCommandRequest>()
{
  return "igris_c_sdk::srv::ControlModeCommandRequest";
}

template<>
inline const char * name<igris_c_sdk::srv::ControlModeCommandRequest>()
{
  return "igris_c_sdk/srv/ControlModeCommandRequest";
}

template<>
struct has_fixed_size<igris_c_sdk::srv::ControlModeCommandRequest>
  : std::integral_constant<
    bool,
    has_fixed_size<igris_c_sdk::srv::ControlModeCommandRequest_Request>::value &&
    has_fixed_size<igris_c_sdk::srv::ControlModeCommandRequest_Response>::value
  >
{
};

template<>
struct has_bounded_size<igris_c_sdk::srv::ControlModeCommandRequest>
  : std::integral_constant<
    bool,
    has_bounded_size<igris_c_sdk::srv::ControlModeCommandRequest_Request>::value &&
    has_bounded_size<igris_c_sdk::srv::ControlModeCommandRequest_Response>::value
  >
{
};

template<>
struct is_service<igris_c_sdk::srv::ControlModeCommandRequest>
  : std::true_type
{
};

template<>
struct is_service_request<igris_c_sdk::srv::ControlModeCommandRequest_Request>
  : std::true_type
{
};

template<>
struct is_service_response<igris_c_sdk::srv::ControlModeCommandRequest_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // IGRIS_C_SDK__SRV__DETAIL__CONTROL_MODE_COMMAND_REQUEST__TRAITS_HPP_
