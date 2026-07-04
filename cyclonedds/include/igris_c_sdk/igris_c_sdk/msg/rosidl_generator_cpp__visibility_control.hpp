// generated from rosidl_generator_cpp/resource/rosidl_generator_cpp__visibility_control.hpp.in
// generated code does not contain a copyright notice

#ifndef IGRIS_C_SDK__MSG__ROSIDL_GENERATOR_CPP__VISIBILITY_CONTROL_HPP_
#define IGRIS_C_SDK__MSG__ROSIDL_GENERATOR_CPP__VISIBILITY_CONTROL_HPP_

#ifdef __cplusplus
extern "C"
{
#endif

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define ROSIDL_GENERATOR_CPP_EXPORT_igris_c_sdk __attribute__ ((dllexport))
    #define ROSIDL_GENERATOR_CPP_IMPORT_igris_c_sdk __attribute__ ((dllimport))
  #else
    #define ROSIDL_GENERATOR_CPP_EXPORT_igris_c_sdk __declspec(dllexport)
    #define ROSIDL_GENERATOR_CPP_IMPORT_igris_c_sdk __declspec(dllimport)
  #endif
  #ifdef ROSIDL_GENERATOR_CPP_BUILDING_DLL_igris_c_sdk
    #define ROSIDL_GENERATOR_CPP_PUBLIC_igris_c_sdk ROSIDL_GENERATOR_CPP_EXPORT_igris_c_sdk
  #else
    #define ROSIDL_GENERATOR_CPP_PUBLIC_igris_c_sdk ROSIDL_GENERATOR_CPP_IMPORT_igris_c_sdk
  #endif
#else
  #define ROSIDL_GENERATOR_CPP_EXPORT_igris_c_sdk __attribute__ ((visibility("default")))
  #define ROSIDL_GENERATOR_CPP_IMPORT_igris_c_sdk
  #if __GNUC__ >= 4
    #define ROSIDL_GENERATOR_CPP_PUBLIC_igris_c_sdk __attribute__ ((visibility("default")))
  #else
    #define ROSIDL_GENERATOR_CPP_PUBLIC_igris_c_sdk
  #endif
#endif

#ifdef __cplusplus
}
#endif

#endif  // IGRIS_C_SDK__MSG__ROSIDL_GENERATOR_CPP__VISIBILITY_CONTROL_HPP_
