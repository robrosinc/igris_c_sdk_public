# IGRIS-C SDK — Native C++ (Cyclone DDS)

Prebuilt C++ SDK that talks DDS directly via the bundled Cyclone DDS
runtime — no ROS 2 install needed.

## Layout

```text
cyclonedds/
├── include/igris_c_sdk/    Public headers
├── lib/                    libigris_c_sdk.a + CMake package config
├── thirdparty/             Bundled Cyclone DDS headers + static libs
├── licenses/, LICENSE, NOTICE
├── CMakeLists.txt          Builds the example clients below
└── *.cpp + *.md            Example sources + per-example notes
```

## Build the examples

```bash
cd cyclonedds
mkdir build && cd build
cmake ..
make -j"$(nproc)"
```

`CMakeLists.txt` defaults `igris_c_sdk_DIR` to
`${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake/igris_c_sdk`, so no extra flags
are needed when the SDK sits alongside `CMakeLists.txt` (which is the
shipped layout). The example executables are written to
`cyclonedds/build/<exe>`.

### Override SDK path

If you stage the SDK somewhere else, pass `-Digris_c_sdk_DIR` (standard
CMake `find_package` knob):

```bash
cmake -Digris_c_sdk_DIR=/custom/prefix/lib/cmake/igris_c_sdk ..
```

## Run

Every example takes `<domain_id> <namespace>` (mirrors `ROS_DOMAIN_ID`
and the bridge's `user_suffix`):

| Executable | Purpose |
| --- | --- |
| `cyclonedds_low_level_control` | Pub/Sub low-level control loop |
| `cyclonedds_service` | Service API — init / torque / control-mode (menu-based) |
| `cyclonedds_hand_example` | Headless end-effector (hand) control via `rt/handcmd` + `hand_init` |
| `cyclonedds_twist_publisher` | Walk-velocity (`Twist`) publisher on `rt/cmd_vel` |
| `cyclonedds_gui_client` | Full-featured GUI client (Dear ImGui) |
| `cyclonedds_hand_gui_client` | Hand control GUI client |
| `cyclonedds_metaquest` | MetaQuest raw data table viewer |
| `cyclonedds_sensor_config` | Sensor configuration RPC example |
| `cyclonedds_sensor_viewer` | Sensor visualization (built only when OpenCV is present) |

```bash
./build/cyclonedds_low_level_control 0 igris_c_<your_robot_id>
./build/cyclonedds_gui_client        0 igris_c_<your_robot_id>
```

## Consume the SDK from your own CMake project

```cmake
set(igris_c_sdk_DIR "/path/to/cyclonedds/lib/cmake/igris_c_sdk")
find_package(igris_c_sdk REQUIRED)
target_link_libraries(your_target PRIVATE igris_c_sdk::igris_c_sdk)
```

The exported target carries the SDK include dirs and links the bundled
Cyclone DDS static libs and OpenSSL automatically.

## System dependencies

```bash
sudo apt install libssl-dev libglfw3-dev libgl1-mesa-dev
# OpenCV is optional; install it to enable cyclonedds_sensor_viewer:
sudo apt install libopencv-dev
```

`glfw` and `imgui` are fetched automatically on first configure if they
are not already on the system.
