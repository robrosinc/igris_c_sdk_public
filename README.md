# IGRIS C SDK
![GitHub release (latest by date)](https://img.shields.io/github/v/release/robrosinc/igris_c_sdk_public)

Communication SDK using cyclonedds for IGRIS-C

## Requirements

- **OS**: Linux (Ubuntu 24.04)
- **CMake**: 3.14 or higher
- **Compiler**: C++17 compatible (GCC 9+, Clang 10+)
- **Python**: 3.12 (when using Python bindings)

### Required Dependencies

```bash
# Ubuntu/Debian
sudo apt install libssl-dev
```

### Additional Dependencies for Building Examples

```bash
# Ubuntu/Debian (required for building examples)
sudo apt install libglfw3-dev libgl1-mesa-dev
```

## Directory Structure

```
igris_c_sdk_public/
├── include/igris_sdk/     # SDK header files
├── lib/
│   ├── libigris_sdk.a     # Static library
│   └── cmake/             # CMake configuration files
├── thirdparty/
│   └── include/           # Cyclone DDS headers
├── examples/              # Example code
├── dist/                  # Python wheel packages
├── licenses/              # Third-party licenses
├── LICENSE
└── NOTICE
```

## Using in C++ Projects

### CMake Configuration

```cmake
# Set SDK path (modify according to igris_c_sdk_public location)
set(igris_sdk_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../igris_c_sdk_public/lib/cmake/igris_sdk")
find_package(igris_sdk REQUIRED)

# Link to target
target_link_libraries(your_target igris_sdk::igris_sdk)
```

## Using Python Bindings

### Installation

```bash
# Create and activate virtual environment
python3 -m venv venv
source venv/bin/activate

# Install wheel package
pip install dist/igris_c_sdk-*.whl
```

## Building and Running Examples

```bash
cd examples
./build.sh
```

## License

This SDK includes the Cyclone DDS library statically linked. Refer to the `licenses/` directory for third-party licenses.