# Merged ROS 2 package shipped in the public deliverable:
# rosidl-generates the SDK interfaces (msg/srv) AND builds the example
# client nodes that consume them in one shot. Project name `igris_c_sdk`
# matches the SDK bridge so DDS wire types (igris_c_sdk::msg::*)
# interoperate.
#
# This file is checked in under release_assets/ros2/ros2_pkg_template/ and copied
# verbatim into the deliverable by public.sh. Edit here, not in the
# generated copy.

cmake_minimum_required(VERSION 3.16)
project(igris_c_sdk)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED) # camera lane publishes standard sensor_msgs
find_package(OpenGL REQUIRED)
find_package(glfw3 QUIET)

# imgui + glfw fetched on demand for the GUI example.
include(FetchContent)
if(NOT TARGET glfw)
  FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
  )
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(glfw)
endif()

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.90.1
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC OpenGL::GL glfw)

# rosidl interfaces — enumerated from the on-disk msg/ and srv/ trees
# so adding a new .msg/.srv requires no CMake edit. CONFIGURE_DEPENDS
# makes the glob re-evaluate on every build (CMake 3.12+).
file(GLOB MSG_FILES CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "msg/*.msg")
file(GLOB SRV_FILES CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "srv/*.srv")
# DEPENDENCIES lists the packages the .msg/.srv files THEMSELVES reference, which today is
# std_msgs (Header, 19x) and sensor_msgs (Imu 3x, Joy 1x). It must match the SDK's own
# rosidl_generate_interfaces or the customer's build dies in the type-description generator
# with KeyError on the missing one - it did: this said `geometry_msgs` while the contract had
# moved to sensor_msgs/Imu, and geometry_msgs is not referenced by any .msg at all.
#
# geometry_msgs stays a package dependency regardless: ros_twist_publisher uses
# geometry_msgs/TwistStamped from C++. That belongs in ament_target_dependencies, not here.
rosidl_generate_interfaces(${PROJECT_NAME}
  ${MSG_FILES}
  ${SRV_FILES}
  DEPENDENCIES std_msgs sensor_msgs
)

# In-package C++ typesupport target — produced by
# rosidl_generate_interfaces above. Each example executable links it
# so the rosidl-generated headers / .so files are wired automatically.
# Uses rosidl_get_typesupport_target (Jazzy preferred API) instead of
# the deprecated rosidl_target_interfaces.
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")

# Example executables.
set(EXAMPLE_NODES
  ros_lowstate_listener
  ros_lowcmd_publisher
  ros_service
  ros_hand_example
)
foreach(exe ${EXAMPLE_NODES})
  add_executable(${exe} src/${exe}.cpp)
  ament_target_dependencies(${exe} rclcpp std_msgs)
  target_link_libraries(${exe} "${cpp_typesupport_target}")
  install(TARGETS ${exe} DESTINATION lib/${PROJECT_NAME})
endforeach()

# Sensor listener is the one example that consumes NO SDK interface: it subscribes to
# standard sensor_msgs published by the camera lane, so it needs sensor_msgs and must NOT
# be folded into EXAMPLE_NODES above (that loop links std_msgs + the typesupport target).
add_executable(ros_sensor_listener src/ros_sensor_listener.cpp)
ament_target_dependencies(ros_sensor_listener rclcpp sensor_msgs)
install(TARGETS ros_sensor_listener DESTINATION lib/${PROJECT_NAME})

option(IGRIS_C_BUILD_VIEWERS "Build the sensor viewer example (requires OpenCV)" ON)
if(IGRIS_C_BUILD_VIEWERS)
  find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)
  add_executable(ros_sensor_viewer src/ros_sensor_viewer.cpp)
  ament_target_dependencies(ros_sensor_viewer rclcpp sensor_msgs)
  target_link_libraries(ros_sensor_viewer imgui OpenGL::GL glfw ${OpenCV_LIBRARIES})
  install(TARGETS ros_sensor_viewer DESTINATION lib/${PROJECT_NAME})
else()
  message(STATUS "ros_sensor_viewer off (-DIGRIS_C_BUILD_VIEWERS=OFF); ros_sensor_listener still builds.")
endif()

# Twist teleop publisher uses geometry_msgs.
add_executable(ros_twist_publisher src/ros_twist_publisher.cpp)
ament_target_dependencies(ros_twist_publisher rclcpp geometry_msgs std_msgs)
target_link_libraries(ros_twist_publisher "${cpp_typesupport_target}")
install(TARGETS ros_twist_publisher DESTINATION lib/${PROJECT_NAME})

# GUI example pulls in imgui / OpenGL / glfw on top of the standard deps.
# Also subscribes to odom (nav_msgs) and cmd_vel (geometry_msgs).
add_executable(ros_gui_client src/ros_gui_client.cpp)
ament_target_dependencies(ros_gui_client rclcpp std_msgs geometry_msgs nav_msgs)
target_link_libraries(ros_gui_client
  imgui OpenGL::GL glfw "${cpp_typesupport_target}")
install(TARGETS ros_gui_client DESTINATION lib/${PROJECT_NAME})

# Hand GUI example also uses imgui / OpenGL / glfw.
add_executable(ros_hand_gui_client src/ros_hand_gui_client.cpp)
ament_target_dependencies(ros_hand_gui_client rclcpp std_msgs)
target_link_libraries(ros_hand_gui_client
  imgui OpenGL::GL glfw "${cpp_typesupport_target}")
install(TARGETS ros_hand_gui_client DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch/ DESTINATION share/${PROJECT_NAME}/launch OPTIONAL)
install(FILES README.md DESTINATION share/${PROJECT_NAME} OPTIONAL)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
