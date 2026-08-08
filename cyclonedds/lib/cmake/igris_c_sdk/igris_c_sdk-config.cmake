
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was igris_c_sdk-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

# Exported targets already carry the include directories and thirdparty link
# dependencies needed by consumers. We only need OpenSSL for the bundled
# Cyclone DDS targets referenced by the exported interface.
find_package(OpenSSL REQUIRED)

# When installed via ament/colcon, delegate to the ament-generated
# <pkg>Config.cmake under share/<pkg>/cmake/. It registers all the
# rosidl typesupport Export targets, generator Export targets, and ament
# extras (export_dependencies / export_targets / rosidl_cmake / ...)
# that downstream consumers need to use rosidl_get_typesupport_target()
# or to link igris_c_sdk__rosidl_typesupport_cpp directly. Because this
# custom config (in lib/cmake/<pkg>/) wins over the ament one during
# find_package, the only way for downstream to see those targets is for
# us to include it here.
#
# Gated on rosidl_cmake being available so that standalone (non-ROS)
# consumers of the public deliverable — which now ships share/ too — do
# not fail when the ament config calls find_dependency(rosidl_*). On
# such systems the gate skips delegation entirely and downstream only
# sees the igris_c_sdk::igris_c_sdk static lib + Cyclone DDS bundle via
# the targets file below.
# Load the DDS SDK static lib target first so igris_c_sdk::igris_c_sdk is
# registered with the correct INTERFACE_INCLUDE_DIRECTORIES before the ament
# rosidl_cmake_aggregate_target-extras runs its "if(NOT TARGET ...)" guard.
include("${CMAKE_CURRENT_LIST_DIR}/igris_c_sdk-targets.cmake")

find_package(rosidl_cmake QUIET CONFIG)
if(rosidl_cmake_FOUND)
  get_filename_component(_igris_c_sdk_prefix "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)
  set(_igris_c_sdk_ament_dir "${_igris_c_sdk_prefix}/share/igris_c_sdk/cmake")
  if(EXISTS "${_igris_c_sdk_ament_dir}/igris_c_sdkConfig.cmake")
    # ament's auto-config resolves its extras files via ${igris_c_sdk_DIR},
    # which find_package() set to *this* custom config's dir (lib/cmake/...).
    # Redirect it to the ament cmake dir during the include so the extras
    # (rosidl_cmake-extras, ament_cmake_export_*-extras, typesupport
    # Export.cmake, ...) resolve correctly, then restore.
    set(_igris_c_sdk_DIR_saved "${igris_c_sdk_DIR}")
    set(igris_c_sdk_DIR "${_igris_c_sdk_ament_dir}")
    include("${_igris_c_sdk_ament_dir}/igris_c_sdkConfig.cmake")
    set(igris_c_sdk_DIR "${_igris_c_sdk_DIR_saved}")
    unset(_igris_c_sdk_DIR_saved)
  endif()
  unset(_igris_c_sdk_ament_dir)
  unset(_igris_c_sdk_prefix)
endif()

check_required_components(igris_c_sdk)
