# IGRIS-C SDK

![GitHub release (latest by date)](https://img.shields.io/github/v/release/robrosinc/igris_c_sdk_public)

C++ + Python SDK for the IGRIS-C robot, layered on top of [Cyclone DDS](https://github.com/eclipse-cyclonedds/cyclonedds). This repository ships the public release artifacts: prebuilt static library, headers, Python wheel, and runnable examples.

## Requirements

| | Version |
|---|---|
| OS | Linux x86_64 (Ubuntu 22.04 / 24.04 tested) |
| CMake | ≥ 3.14 |
| Compiler | C++17 (GCC 11+ / Clang 14+) |
| Python | 3.12 (only for Python bindings) |

### Runtime / build dependencies

```bash
sudo apt install libssl-dev                            # required
sudo apt install libglfw3-dev libgl1-mesa-dev          # for the GUI example
```

## Layout

```
igris_c_sdk_public/
├── include/igris_c_sdk/   # SDK headers (public API)
├── lib/
│   ├── libigris_c_sdk.a   # Static library (Cyclone DDS bundled)
│   └── cmake/             # CMake package config (find_package targets)
├── thirdparty/include/    # Bundled Cyclone DDS headers
├── examples/              # Example sources + CMakeLists
├── dist/                  # Python wheel (igris_c_sdk-*.whl)
├── licenses/              # Third-party license texts
├── pixi.toml              # User-side pixi environment (Python 3.12 + pip)
├── pixi.lock              # Locked dependency resolution
├── LICENSE
└── NOTICE
```

## Quick start

### C++ (CMake)

```cmake
# In your project's CMakeLists.txt
set(igris_c_sdk_DIR "${CMAKE_SOURCE_DIR}/igris_c_sdk_public/lib/cmake/igris_c_sdk")
find_package(igris_c_sdk REQUIRED)

target_link_libraries(your_target PRIVATE igris_c_sdk::igris_c_sdk)
```

### Python wheel — quickest

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install dist/igris_c_sdk-*.whl
```

### Python wheel — with pixi (reproducible)

```bash
# Install pixi once: https://pixi.sh/latest/
pixi install
pixi run pip install dist/igris_c_sdk-*.whl
pixi shell        # activate the env
```

## Examples

```bash
cd examples
./build.sh         # configures + builds all example binaries via CMake
./build/sdk_gui_client 0 igris_c_<your_robot_id>
```

The first arg is the DDS domain id; the second is the namespace prefix the bridge advertises (matches the robot's bridge config `user_suffix`).

## Versioning

Releases follow PEP 440. Tags on the upstream repository's `main` branch trigger a CI mirror to this public repo's `main` branch, and the tag is mirrored 1:1.

Wheel filenames carry the version, including dev/local segments when built off-tag (e.g., `igris_c_sdk-1.0.4rc1.dev3+gabcdef012-py3-none-any.whl`).

## License

The SDK source is licensed under the terms in [LICENSE](LICENSE). This release bundles Cyclone DDS, Fast CDR, and foonathan_memory as static libraries — see [licenses/](licenses/) and [NOTICE](NOTICE) for upstream license texts.
