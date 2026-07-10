# igris_c_sdk

ROS 2 client examples for the public `igris_c_sdk` API.

## Build

```bash
colcon build --packages-select igris_c_sdk
source install/setup.bash
```

The GUI example pulls `glfw` and `imgui` at configure time if they are not already available on the system.
The examples read the robot namespace from the ROS parameter `robot_namespace` and the DDS domain id from the `ROS_DOMAIN_ID` environment variable (the standard ROS 2 mechanism — domain id is bound at participant creation time so it cannot be a node parameter):

```bash
ROS_DOMAIN_ID=5 ros2 run igris_c_sdk ros_lowstate_listener \
    --ros-args -p robot_namespace:=<ns>
```

Both default to empty / 0 if omitted.

## Examples

### `ros_twist_publisher`

Terminal-based teleop that publishes `geometry_msgs/msg/Twist` on the resolved `/.../rt/cmd_vel` topic at 20 Hz. The bridge subscribes to `rt/cmd_vel` and maps `linear.x` / `linear.y` / `angular.z` into the controller's joystick SHM buffer.

Switch the robot into walkmode first (e.g. `ros_service` option 14) before running this example.

Keyboard controls:

- `w` / `s`: increase or decrease `linear.x` (forward)
- `a` / `d`: increase or decrease `linear.y` (lateral)
- `q` / `e`: increase or decrease `angular.z` (yaw)
- `space`: zero all velocity targets
- `x`: exit

Usage:

```bash
ros2 run igris_c_sdk ros_twist_publisher
```

### `ros_gui_client`

Open a ROS 2 GUI that mirrors the SDK dev-tool layout from `igris_c_sdk/examples/cyclonedds_gui_client.cpp`.

Features:

- Subscribes to the resolved `/.../rt/lowstate` and `/.../rt/robotstate`
- Publishes low-level motor/parallel-joint commands to the resolved `/.../rt/lowcmd`
- Provides the same low-command sliders, gain editing, publish status tables, service buttons, and response log layout as the SDK GUI
- Calls ROS 2 services under the resolved `/.../rt/service/*` namespace for BMS, torque, control mode, and sim commands

Usage:

```bash
ros2 run igris_c_sdk ros_gui_client
```

The GUI behavior follows the SDK GUI; only the communication layer is ROS 2 instead of the SDK DDS client.

### `ros_lowcmd_publisher`

Continuously publishes a zeroed `LowCmd` at 50 Hz on the resolved `/.../rt/lowcmd` topic.

Usage:

```bash
ros2 run igris_c_sdk ros_lowcmd_publisher
```

Useful as a minimal low-level publisher or as a starting point for scripted command generation.

### `ros_lowstate_listener`

Subscribes to the resolved `/.../rt/lowstate` topic and prints incoming robot state messages.

Usage:

```bash
ros2 run igris_c_sdk ros_lowstate_listener
```

### `ros_service`

Interactive menu for all public `igris_c_sdk` services: BMS/Motor init, torque on/off, hand init, and every `ControlModeCommandRequest` command type (including motion presets `HOME` / `HI_WAVE`, walkmode, joint-position hold, and custom modes).

Usage:

```bash
ros2 run igris_c_sdk ros_service
```

### `ros_hand_init_client`

Calls the hand initialization service.

Usage:

```bash
ros2 run igris_c_sdk ros_hand_init_client
```

## Notes

- This package depends on `igris_c_sdk`, `rclcpp`, and common ROS 2 message libraries.
- It does not include the `igris_c_bridge` implementation headers.
- The robot namespace is read from the `robot_namespace` ROS parameter on each example node; pass `--ros-args -p robot_namespace:=<ns>` at launch or set it in a launch file. Default is empty (no namespace).
