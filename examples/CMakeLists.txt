cmake_minimum_required(VERSION 3.14)
project(igris_c_sdk_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE)
  add_link_options("-Wl,--no-as-needed")
endif()

# Find the installed IGRIS SDK
# Auto-detect: deployed SDK (../lib/) or development environment (colcon)
if(NOT igris_c_sdk_DIR)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../lib/cmake/igris_c_sdk/igris_c_sdk-config.cmake")
    # Deployed SDK structure (igris_c_sdk_public/examples/)
    set(igris_c_sdk_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../lib/cmake/igris_c_sdk")
  elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../build/igris_c_sdk-config.cmake"
         AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../build/igris_c_sdk-targets.cmake"
  )
    # Source tree development build (igris_c_sdk/examples/)
    set(igris_c_sdk_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../build")
  elseif(DEFINED ENV{COLCON_PREFIX_PATH})
    # Colcon workspace - use install directory
    set(igris_c_sdk_DIR "$ENV{COLCON_PREFIX_PATH}/igris_c_sdk/lib/cmake/igris_c_sdk")
  elseif(
    EXISTS
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../install/igris_c_sdk/lib/cmake/igris_c_sdk/igris_c_sdk-config.cmake"
  )
    # Development environment: colcon workspace (ros2_ws/src/igris_c_sdk/examples/)
    set(igris_c_sdk_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../install/igris_c_sdk/lib/cmake/igris_c_sdk"
    )
  elseif(
    EXISTS
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../install/igris_c_sdk/lib/cmake/igris_c_sdk/igris_c_sdk-config.cmake"
  )
    # Nested colcon workspace (ws/src/<group>/igris_c_sdk/examples/)
    set(igris_c_sdk_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../install/igris_c_sdk/lib/cmake/igris_c_sdk"
    )
  endif()
endif()
find_package(igris_c_sdk REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)

# Find required packages for GUI
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

# Fetch ImGui
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

# Create ImGui library
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
target_link_libraries(
  imgui
  PUBLIC OpenGL::GL glfw
)

# SDK GUI Client (full-featured example)
add_executable(sdk_gui_client sdk_gui_client.cpp)
target_link_libraries(
  sdk_gui_client igris_c_sdk::igris_c_sdk imgui OpenGL::GL glfw
)

set_target_properties(
  sdk_gui_client PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" BUILD_RPATH_USE_ORIGIN ON
)

# Low-Level Example (Pub/Sub)
add_executable(lowlevel_example lowlevel_example.cpp)
target_link_libraries(
  lowlevel_example igris_c_sdk::igris_c_sdk
)

set_target_properties(
  lowlevel_example PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" BUILD_RPATH_USE_ORIGIN ON
)

# Service Example (Service API)
add_executable(service_example service_example.cpp)
target_link_libraries(
  service_example igris_c_sdk::igris_c_sdk
)

set_target_properties(
  service_example PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" BUILD_RPATH_USE_ORIGIN ON
)

# Hand GUI Client (hand control example)
add_executable(hand_gui_client hand_gui_client.cpp)
target_link_libraries(
  hand_gui_client igris_c_sdk::igris_c_sdk imgui OpenGL::GL glfw
)

set_target_properties(
  hand_gui_client PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" BUILD_RPATH_USE_ORIGIN ON
)

# Sensor viewer example (sensor data visualization example)
if(OpenCV_FOUND)
  add_executable(sensor_viewer sensor_viewer.cpp)
  target_link_libraries(sensor_viewer igris_c_sdk::igris_c_sdk ${OpenCV_LIBS})

  set_target_properties(
    sensor_viewer PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" BUILD_RPATH_USE_ORIGIN ON
  )
else()
  message(WARNING "OpenCV not found. 'sensor_viewer' target will be skipped.")
endif()

# MetaQuest raw viewer example
add_executable(metaquest metaquest.cpp)
target_link_libraries(
  metaquest igris_c_sdk::igris_c_sdk
)

set_target_properties(metaquest PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" BUILD_RPATH_USE_ORIGIN ON)

# Sensor configuration example (sensor data visualization example)
add_executable(sensor_configuration sensor_config.cpp)
target_link_libraries(
  sensor_configuration igris_c_sdk::igris_c_sdk
)

set_target_properties(
  sensor_configuration PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" BUILD_RPATH_USE_ORIGIN ON
)

message(STATUS "Examples configured:")
message(STATUS "  - sdk_gui_client: Full-featured GUI client example")
message(STATUS "  - lowlevel_example: Pub/Sub low-level control example")
message(STATUS "  - service_example: Service API example")
message(STATUS "  - hand_gui_client: Hand control GUI client example")
if(OpenCV_FOUND)
  message(STATUS "  - sensor_viewer: Sensor data visualization example")
else()
  message(STATUS "  - sensor_viewer: skipped (OpenCV not found)")
endif()
message(STATUS "  - metaquest: MetaQuest raw data table example")
message(STATUS "  - sensor_configuration: Sensor configuration example")
