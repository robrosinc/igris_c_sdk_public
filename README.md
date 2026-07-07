# IGRIS-C SDK

![GitHub release (latest by date)](https://img.shields.io/github/v/release/robrosinc/igris_c_sdk_public)

C++ + Python SDK for the IGRIS-C robot, layered on top of [Cyclone DDS](https://github.com/eclipse-cyclonedds/cyclonedds). This repository ships the public release artifacts split by language / runtime so each use case stays self-contained.

## Requirements

| Tool | Version |
| --- | --- |
| OS | Linux x86_64 (Ubuntu 24.04, ROS 2 Jazzy) |
| CMake | ≥ 3.14 |
| Compiler | C++17 (GCC 11+ / Clang 14+) |
| Python | 3.12 (Python bindings + ROS 2 nodes) |

### Runtime / build dependencies

```bash
sudo apt install libssl-dev                            # required
sudo apt install libglfw3-dev libgl1-mesa-dev          # for the GUI examples
```

## Layout

```text
igris_c_sdk_public/
├── README.md
├── NOTICE
├── dist/                       # Python wheel (igris_c_sdk-*.whl)
├── cyclonedds/                 # Native C++ SDK (uses bundled Cyclone DDS directly)
│   ├── include/igris_c_sdk/    # Public headers
│   ├── lib/                    # libigris_c_sdk.a + CMake package config
│   ├── thirdparty/             # Bundled Cyclone DDS headers + static libs
│   ├── licenses/, LICENSE, NOTICE
│   ├── CMakeLists.txt          # Examples CMake project (links the SDK)
│   ├── *.cpp + *.md            # Example sources + per-example notes
│   └── imgui.ini
├── python/                     # Python examples + reproducible env
│   ├── pixi.toml, pixi.lock    # User-side pixi environment (Python 3.12 + pip)
│   └── *.py, client.ipynb
└── ros2/                       # ROS 2 ament package (msg/srv + example nodes)
    ├── README.md
    └── src/
        └── igris_c_sdk_public/ # Drop this folder into your colcon workspace src/
```

Each top-level subtree is independent — pick the one matching your runtime.

## Connecting to the robot

Before running any example, the SDK client must join the robot's DDS network. Two things must match the robot.

### Robot namespace

Every DDS topic is resolved under a per-unit **robot namespace** of the form `igris_c_<robot_id>` — the fixed
prefix `igris_c_` plus the robot's configured suffix (e.g. `igris_c_IG01`). Topics written as `rt/...` resolve to
`<namespace>/rt/...`. Pass the namespace (and the DDS domain id, which mirrors `ROS_DOMAIN_ID`) to the client:

```cpp
ChannelFactory::Instance()->Init(domain_id, "igris_c_<robot_id>");   // C++
```

```python
igc_sdk.ChannelFactory.Instance().Init(domain_id, "igris_c_<robot_id>")  # Python
```

The C++ examples take both as arguments: `./cyclonedds_low_level_control <domain_id> igris_c_<robot_id>`.
The development PC and the robot **must use the same namespace + domain id**.

### CYCLONEDDS_URI (network interface)

DDS discovery needs a CycloneDDS XML describing the network interface. A sample **`cyclonedds.xml` is bundled at
the deliverable root**. Edit `<NetworkInterface name="...">` to the interface on THIS machine that reaches the
robot (list with `ip link`, or set `autodetermine="true"`), then export it before running anything:

```bash
# from the deliverable root (where cyclonedds.xml lives)
export CYCLONEDDS_URI="file://$PWD/cyclonedds.xml"
export ROS_DOMAIN_ID=0          # for ROS 2 nodes (match the domain)
```

If `CYCLONEDDS_URI` is unset or the interface is wrong, participant discovery fails and no topics appear.

## Quick start

### Native C++ (Cyclone DDS directly)

```bash
cd cyclonedds
mkdir build && cd build
cmake .. && make -j"$(nproc)"
./cyclonedds_low_level_control 0 igris_c_<your_robot_id>
```

See [cyclonedds/README.md](cyclonedds/README.md) for the full list
of examples and how to override the SDK path.

To consume the SDK from your own CMake project, point `find_package`
at the bundled config:

```cmake
set(igris_c_sdk_DIR "<deliverable>/cyclonedds/lib/cmake/igris_c_sdk")
find_package(igris_c_sdk REQUIRED)
target_link_libraries(your_target PRIVATE igris_c_sdk::igris_c_sdk)
```

### Python wheel — quickest

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install dist/igris_c_sdk-*.whl
```

### Python wheel — reproducible env via pixi

```bash
# Install pixi once: https://pixi.sh/latest/
cd python
pixi install
pixi run pip install ../dist/igris_c_sdk-*.whl
pixi shell        # activate the env
```

### ROS 2 (colcon workspace)

Drop the package folder into your own workspace `src/`; the package
name (`igris_c_sdk`) matches what the SDK bridge publishes, so DDS
type ids interoperate without renaming.

```bash
cp -r ros2/src/igris_c_sdk_public ~/my_ws/src/
cd ~/my_ws && colcon build
source install/setup.bash
ROS_DOMAIN_ID=5 ros2 run igris_c_sdk ros_lowstate_listener \
    --ros-args -p robot_namespace:=igris_c_<your_robot_id>
```

See [ros2/README.md](ros2/README.md) for the full ROS 2 walkthrough,
including using the SDK types from your own nodes.

The first arg to the C++ examples is the DDS domain id (mirrors
`ROS_DOMAIN_ID`); the namespace prefix matches the robot's bridge
config `user_suffix`.

## Examples

All examples take the DDS domain id and robot namespace (C++/ROS 2 as args, Python via `--domain-id` /
`--namespace`). Set `CYCLONEDDS_URI` first (see [Connecting to the robot](#connecting-to-the-robot)).

Each row is a capability; the cell shows the example for that interface, or `—` if not provided.

| Capability | C++ (`cyclonedds/`) | Python (`python/`) | ROS 2 (`ros2/`) |
| --- | --- | --- | --- |
| Low-level control (`LowCmd`/`LowState`) | `cyclonedds_low_level_control` | `client.py` | `ros_lowcmd_publisher`, `ros_lowstate_listener` |
| Service API (init / torque / control-mode) | `cyclonedds_service` | `client.py` | `ros_service` |
| End-effector (hand) | `cyclonedds_hand_example` | `hand_example.py` | `ros_hand_init_client` (init only) |
| Walk velocity (`Twist`) | `cyclonedds_twist_publisher` | `twist_publisher.py` | `ros_twist_publisher` |
| GUI client | `cyclonedds_gui_client`, `cyclonedds_hand_gui_client` | `sdk_gui_client.py` | `ros_gui_client` |
| Sensor viewer | `cyclonedds_sensor_viewer` ¹ | `sensor_viewer.py` | — |
| Sensor config | `cyclonedds_sensor_config` | — | — |
| MetaQuest viewer | `cyclonedds_metaquest` | `metaquest.py` | — |
| Notebook walkthrough | — | `client.ipynb` | — |

¹ built only when OpenCV is present.

## API reference (summary)

The SDK communicates over DDS in two patterns: **Publish/Subscribe** (continuous state/command streams) and
**Request/Response** (services). All topics resolve under the robot namespace described above.

### High-level services — `IgrisC_Client`

| Function | Method | Service topic |
| --- | --- | --- |
| BMS / motor init | `InitBms(BmsInitType, timeout_ms)` | `rt/service/bms_init` |
| Torque on/off | `SetTorque(TorqueType, timeout_ms)` | `rt/service/torque` |
| End-effector (hand) init | `InitHand(timeout_ms)` | `rt/service/hand_init` |
| Control-mode switch | `SendControlModeCommand(type, preset_id, is_cyclic, timeout_ms)` | `rt/service/control_mode` |

Each service is a `<topic>/request` + `<topic>/response` pair, matched by `request_id`; responses carry
`success` / `message` / `error_code` (0 = success).

- `BmsInitType`: `BMS_INIT_NONE` (power **OFF**) · `BMS_INIT` (power ON) · `MOTOR_INIT` · `BMS_AND_MOTOR_INIT` · `BMS_OFF` (power **OFF**, same as `BMS_INIT_NONE`)
- `TorqueType`: `TORQUE_ON` · `TORQUE_OFF`

### Control modes — `ControlModeCommandType`

`MOTION_PRESET` (needs `preset_id`; HIGH_LEVEL only) · `MOTION_PRESET_CYCLIC_TOGGLE` · `JOINT_POSITION_HOLD` ·
`MOTION_STOP` (HIGH_LEVEL only) · `WALKMODE_ON` · `LOW_LEVEL_JOINT_CONTROL` · `LOW_LEVEL_WALKMODE_ON` ·
`HIGH_LEVEL_JOINT_CONTROL` · `HIGH_LEVEL_WALKMODE_ON`. (`CUSTOM_MODE_*` are internal — not for SDK use.)

Cyclic repeat of a preset is typically requested via the `is_cyclic` flag on `MOTION_PRESET` (as the examples do);
`MOTION_PRESET_CYCLIC_TOGGLE` is the dedicated toggle command.

> A mode switch is accepted only when **body power + torque are ON and E-Stop is released**; it is blocked while
> the robot is in the stopped state (run the `HOME` motion preset to exit).

### Low-level control

Stream targets on `rt/lowcmd` (`LowCmd`) and read state on `rt/lowstate` (`LowState`). `MotorCmd` carries
`q`/`dq`/`tau`/`kp`/`kd` per joint (31 DOF, HYBRID mode); `LowCmd.kinematic_modes[5]` selects MS/PJS per
parallel-link group. See the `cyclonedds/` and `python/` low-level examples.

### End effector (hand)

A single DDS interface (`rt/handcmd` / `rt/handstate` / `rt/service/hand_init`) drives whichever end effector
the robot is built with — 5-finger hand (ids **11–16 right / 21–26 left**), 1-DoF gripper, or magnet.
`HandCmd.motor_cmd[].q` is normalized 0–1. Initialize via `InitHand()` (or a `rt/service/hand_init` request).

## License

The SDK source is licensed under the terms in [cyclonedds/LICENSE](cyclonedds/LICENSE). This release bundles Cyclone DDS, Fast CDR, and foonathan_memory as static libraries — see [cyclonedds/licenses/](cyclonedds/licenses/) and [cyclonedds/NOTICE](cyclonedds/NOTICE) for upstream license texts.
