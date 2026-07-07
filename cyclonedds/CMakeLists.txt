# Cyclone DDS-only client examples shipped in the public deliverable.
# Lives at <deliverable>/cyclonedds/ alongside the installed SDK
# (lib/, include/, share/, thirdparty/), so the SDK config sits at
# ./lib/cmake/igris_c_sdk relative to this file.
#
# This file is checked in under release_assets/cyclonedds/ and copied
# verbatim into the deliverable by public.sh. Edit here, not in the
# generated copy.

cmake_minimum_required(VERSION 3.14)
project(igris_c_sdk_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE)
  add_link_options("-Wl,--no-as-needed")
endif()

# SDK install is a sibling tree inside the same deliverable folder.
if(NOT igris_c_sdk_DIR)
  set(igris_c_sdk_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake/igris_c_sdk")
endif()
find_package(igris_c_sdk REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)

find_package(OpenGL REQUIRED)
find_package(glfw3 QUIET)

if(NOT TARGET glfw)
  message(STATUS "glfw3 not found. Downloading GLFW 3.4 from GitHub (first run only, ~10-30s)...")
  include(FetchContent)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
  )
  FetchContent_MakeAvailable(glfw)
  message(STATUS "GLFW ready.")
endif()

find_package(OpenCV QUIET)

message(STATUS "Downloading ImGui v1.90.1 from GitHub (first run only, ~10-30s)...")
include(FetchContent)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.90.1
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(imgui)
message(STATUS "ImGui ready.")

add_library(
  imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC OpenGL::GL glfw)

# Helper: builds an example executable that links the SDK + sets the
# runtime path so it can find libigris_c_sdk siblings at ../lib/.
function(add_sdk_example name)
  cmake_parse_arguments(ARG "" "" "EXTRA_LIBS" ${ARGN})
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} igris_c_sdk::igris_c_sdk ${ARG_EXTRA_LIBS})
  set_target_properties(${name} PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
    BUILD_RPATH_USE_ORIGIN ON)
endfunction()

add_sdk_example(cyclonedds_gui_client          EXTRA_LIBS imgui OpenGL::GL glfw)
add_sdk_example(cyclonedds_low_level_control)
add_sdk_example(cyclonedds_service)
add_sdk_example(cyclonedds_twist_publisher)
add_sdk_example(cyclonedds_hand_example)
add_sdk_example(cyclonedds_hand_gui_client     EXTRA_LIBS imgui OpenGL::GL glfw)
add_sdk_example(cyclonedds_metaquest)
add_sdk_example(cyclonedds_sensor_config)

if(OpenCV_FOUND)
  add_sdk_example(cyclonedds_sensor_viewer EXTRA_LIBS ${OpenCV_LIBS})
else()
  message(WARNING "OpenCV not found. 'cyclonedds_sensor_viewer' target will be skipped.")
endif()

message(STATUS "Examples configured:")
message(STATUS "  - cyclonedds_gui_client:         Full-featured GUI client")
message(STATUS "  - cyclonedds_low_level_control:  Pub/Sub low-level control")
message(STATUS "  - cyclonedds_service:            Service API")
message(STATUS "  - cyclonedds_twist_publisher:    Walk velocity (Twist) publisher")
message(STATUS "  - cyclonedds_hand_example:       Headless hand (end-effector) control")
message(STATUS "  - cyclonedds_hand_gui_client:    Hand control GUI client")
message(STATUS "  - cyclonedds_metaquest:          MetaQuest raw data table")
message(STATUS "  - cyclonedds_sensor_config:      Sensor configuration")
if(OpenCV_FOUND)
  message(STATUS "  - cyclonedds_sensor_viewer:      Sensor data visualization")
else()
  message(STATUS "  - cyclonedds_sensor_viewer:      skipped (OpenCV not found)")
endif()
