cmake_minimum_required(VERSION 3.14)
project(igris_sdk_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_link_options("-Wl,--no-as-needed")

# Find the installed IGRIS SDK
# This CMakeLists.txt is designed to be used in the deployed SDK
# Assumes this file is in igris-sdk-deploy/examples/
set(igris_sdk_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../lib/cmake/igris_sdk")
find_package(igris_sdk REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)

# Find required packages for GUI
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Fetch ImGui
include(FetchContent)
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG v1.90.1)
FetchContent_MakeAvailable(imgui)

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
  sdk_gui_client igris_sdk::igris_sdk imgui OpenGL::GL glfw
)

set_target_properties(
  sdk_gui_client PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" BUILD_RPATH_USE_ORIGIN ON
)

# Low-Level Example (Pub/Sub)
add_executable(lowlevel_example lowlevel_example.cpp)
target_link_libraries(
  lowlevel_example igris_sdk::igris_sdk
)

set_target_properties(
  lowlevel_example PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" BUILD_RPATH_USE_ORIGIN ON
)

# Service Example (Service API)
add_executable(service_example service_example.cpp)
target_link_libraries(
  service_example igris_sdk::igris_sdk
)

set_target_properties(
  service_example PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" BUILD_RPATH_USE_ORIGIN ON
)

message(STATUS "Examples configured:")
message(STATUS "  - sdk_gui_client: Full-featured GUI client example")
message(STATUS "  - lowlevel_example: Pub/Sub low-level control example")
message(STATUS "  - service_example: Service API example")
