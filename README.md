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

## License

The SDK source is licensed under the terms in [cyclonedds/LICENSE](cyclonedds/LICENSE). This release bundles Cyclone DDS, Fast CDR, and foonathan_memory as static libraries — see [cyclonedds/licenses/](cyclonedds/licenses/) and [cyclonedds/NOTICE](cyclonedds/NOTICE) for upstream license texts.
