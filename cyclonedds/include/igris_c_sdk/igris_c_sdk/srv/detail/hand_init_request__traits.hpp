// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from igris_c_sdk:srv/HandInitRequest.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "igris_c_sdk/srv/hand_init_request.hpp"


#ifndef IGRIS_C_SDK__SRV__DETAIL__HAND_INIT_REQUEST__TRAITS_HPP_
#define IGRIS_C_SDK__SRV__DETAIL__HAND_INIT_REQUEST__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "igris_c_sdk/srv/detail/hand_init_request__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "igris_c_sdk/msg/detail/header__traits.hpp"

namespace igris_c_sdk
{

namespace srv
{

inline void to_flow_style_yaml(
  const HandInitRequest_Request & msg,
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
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const HandInitRequest_Request & msg,
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
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const HandInitRequest_Request & msg, bool use_flow_style = false)
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
  const igris_c_sdk::srv::HandInitRequest_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  igris_c_sdk::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use igris_c_sdk::srv::to_yaml() instead")]]
inline std::string to_yaml(const igris_c_sdk::srv::HandInitRequest_Request & msg)
{
  return igris_c_sdk::srv::to_yaml(msg);
}

template<>
inline const char * data_type<igris_c_sdk::srv::HandInitRequest_Request>()
{
  return "igris_c_sdk::srv::HandInitRequest_Request";
}

template<>
inline const char * name<igris_c_sdk::srv::HandInitRequest_Request>()
{
  return "igris_c_sdk/srv/HandInitRequest_Request";
}

template<>
struct has_fixed_size<igris_c_sdk::srv::HandInitRequest_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<igris_c_sdk::srv::HandInitRequest_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<igris_c_sdk::srv::HandInitRequest_Request>
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
  const HandInitRequest_Response & msg,
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
  const HandInitRequest_Response & msg,
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

inline std::string to_yaml(const HandInitRequest_Response & msg, bool use_flow_style = false)
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
  const igris_c_sdk::srv::HandInitRequest_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  igris_c_sdk::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use igris_c_sdk::srv::to_yaml() instead")]]
inline std::string to_yaml(const igris_c_sdk::srv::HandInitRequest_Response & msg)
{
  return igris_c_sdk::srv::to_yaml(msg);
}

template<>
inline const char * data_type<igris_c_sdk::srv::HandInitRequest_Response>()
{
  return "igris_c_sdk::srv::HandInitRequest_Response";
}

template<>
inline const char * name<igris_c_sdk::srv::HandInitRequest_Response>()
{
  return "igris_c_sdk/srv/HandInitRequest_Response";
}

template<>
struct has_fixed_size<igris_c_sdk::srv::HandInitRequest_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<igris_c_sdk::srv::HandInitRequest_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<igris_c_sdk::srv::HandInitRequest_Response>
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
  const HandInitRequest_Event & msg,
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
  const HandInitRequest_Event & msg,
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

inline std::string to_yaml(const HandInitRequest_Event & msg, bool use_flow_style = false)
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
  const igris_c_sdk::srv::HandInitRequest_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  igris_c_sdk::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use igris_c_sdk::srv::to_yaml() instead")]]
inline std::string to_yaml(const igris_c_sdk::srv::HandInitRequest_Event & msg)
{
  return igris_c_sdk::srv::to_yaml(msg);
}

template<>
inline const char * data_type<igris_c_sdk::srv::HandInitRequest_Event>()
{
  return "igris_c_sdk::srv::HandInitRequest_Event";
}

template<>
inline const char * name<igris_c_sdk::srv::HandInitRequest_Event>()
{
  return "igris_c_sdk/srv/HandInitRequest_Event";
}

template<>
struct has_fixed_size<igris_c_sdk::srv::HandInitRequest_Event>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<igris_c_sdk::srv::HandInitRequest_Event>
  : std::integral_constant<bool, has_bounded_size<igris_c_sdk::srv::HandInitRequest_Request>::value && has_bounded_size<igris_c_sdk::srv::HandInitRequest_Response>::value && has_bounded_size<service_msgs::msg::ServiceEventInfo>::value> {};

template<>
struct is_message<igris_c_sdk::srv::HandInitRequest_Event>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<igris_c_sdk::srv::HandInitRequest>()
{
  return "igris_c_sdk::srv::HandInitRequest";
}

template<>
inline const char * name<igris_c_sdk::srv::HandInitRequest>()
{
  return "igris_c_sdk/srv/HandInitRequest";
}

template<>
struct has_fixed_size<igris_c_sdk::srv::HandInitRequest>
  : std::integral_constant<
    bool,
    has_fixed_size<igris_c_sdk::srv::HandInitRequest_Request>::value &&
    has_fixed_size<igris_c_sdk::srv::HandInitRequest_Response>::value
  >
{
};

template<>
struct has_bounded_size<igris_c_sdk::srv::HandInitRequest>
  : std::integral_constant<
    bool,
    has_bounded_size<igris_c_sdk::srv::HandInitRequest_Request>::value &&
    has_bounded_size<igris_c_sdk::srv::HandInitRequest_Response>::value
  >
{
};

template<>
struct is_service<igris_c_sdk::srv::HandInitRequest>
  : std::true_type
{
};

template<>
struct is_service_request<igris_c_sdk::srv::HandInitRequest_Request>
  : std::true_type
{
};

template<>
struct is_service_response<igris_c_sdk::srv::HandInitRequest_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // IGRIS_C_SDK__SRV__DETAIL__HAND_INIT_REQUEST__TRAITS_HPP_
