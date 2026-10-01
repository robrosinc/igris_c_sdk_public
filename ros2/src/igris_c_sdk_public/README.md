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

Terminal-based teleop that publishes `geometry_msgs/msg/Twist` on the resolved `/.../cmd_vel` topic at 20 Hz. The bridge subscribes to `cmd_vel` and maps `linear.x` / `linear.y` / `angular.z` into the controller's joystick SHM buffer.

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

- Subscribes to the resolved `/.../lowstate` and `/.../robotstate`
- Publishes low-level motor/parallel-joint commands to the resolved `/.../lowcmd`
- Provides the same low-command sliders, gain editing, publish status tables, service buttons, and response log layout as the SDK GUI
- Calls ROS 2 services under the resolved `/.../service/*` namespace for PDU, torque, control mode, and sim commands

Usage:

```bash
ros2 run igris_c_sdk ros_gui_client
```

The GUI behavior follows the SDK GUI; only the communication layer is ROS 2 instead of the SDK DDS client.

### `ros_lowcmd_publisher`

Continuously publishes a zeroed `LowCmd` at 50 Hz on the resolved `/.../lowcmd` topic.

Usage:

```bash
ros2 run igris_c_sdk ros_lowcmd_publisher
```

Useful as a minimal low-level publisher or as a starting point for scripted command generation.

### `ros_lowstate_listener`

Subscribes to the resolved `/.../lowstate` topic and prints incoming robot state messages.

Usage:

```bash
ros2 run igris_c_sdk ros_lowstate_listener
```

### `ros_service`

Interactive menu for all public `igris_c_sdk` services: PDU/Motor init, torque on/off, hand init, and every `ControlModeCommandRequest` command type (including motion presets `HOME` / `HI_WAVE`, walkmode, joint-position hold, and custom modes).

Usage:

```bash
ros2 run igris_c_sdk ros_service
```

### `ros_hand_example`

Headless end-effector (hand) control, mirroring `cyclonedds/cyclonedds_hand_example.cpp`: calls the
`service/hand_init` service, then publishes normalized `HandCmd` targets on `handcmd`
(open → close → pinch → open) while printing `HandState` from `handstate`.

Usage:

```bash
ros2 run igris_c_sdk ros_hand_example
```

### `ros_hand_gui_client`

Open a ROS 2 GUI that mirrors the SDK hand GUI layout from `igris_c_sdk/examples/cyclonedds/cyclonedds_hand_gui_client.cpp`.

Features:

- Subscribes to the resolved `/.../handstate`
- Publishes hand motor commands to the resolved `/.../handcmd` at ~20 Hz from 12 sliders (ids 11-16, 21-26)
- Calls the resolved `/.../service/hand_init` service for hand initialization
- Provides the same slider layout, motor-state table, and response log as the SDK hand GUI

Usage:

```bash
ros2 run igris_c_sdk ros_hand_gui_client
```

### `ros_sensor_listener`

Text probe for the camera lane of `igris_c_camera`. Subscribes to every stream the camera can
publish and prints a per-second table of rate, throughput and the message descriptor, so an
absent row means "not published" rather than "subscription failed".

Unlike the other examples this one consumes **standard `sensor_msgs`**, not SDK interfaces: the
camera's ROS lane publishes `sensor_msgs/Image` for a raw stream and
`sensor_msgs/CompressedImage` on `<topic>/compressed` for an encoded one (jpeg color, png16
depth). Both are subscribed per topic and the `detail` column shows which arrived — the quickest
way to confirm what a given camera config is actually emitting.

Usage:

```bash
ROS_DOMAIN_ID=5 ros2 run igris_c_sdk ros_sensor_listener \
    --ros-args -p robot_namespace:=robot1
```

### `ros_sensor_viewer`

Same streams as `ros_sensor_listener`, drawn in an OpenCV window per stream with an FPS/throughput
overlay, plus the same once-per-second terminal report. The ROS counterpart of
`cyclonedds/cyclonedds_sensor_viewer.cpp`. 16UC1 depth is min/max normalised and colour-mapped;
`rgb8` is converted to BGR for display. Press `q` or `ESC` in a window to quit.

Requires OpenCV **with highgui**. The build skips this target when highgui is missing (the
igris-c-core env ships only core/imgproc/imgcodecs), and says so; `ros_sensor_listener` still
builds there.

Usage:

```bash
ROS_DOMAIN_ID=5 ros2 run igris_c_sdk ros_sensor_viewer \
    --ros-args -p robot_namespace:=robot1
```

## Notes

- This package depends on `igris_c_sdk`, `rclcpp`, and common ROS 2 message libraries.
- It does not include the `igris_c_bridge` implementation headers.
- The robot namespace is read from the `robot_namespace` ROS parameter on each example node; pass `--ros-args -p robot_namespace:=<ns>` at launch or set it in a launch file. Default is empty (no namespace).
