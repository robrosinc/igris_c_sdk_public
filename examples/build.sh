#!/bin/bash

# Simple build script for examples

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo -e "${YELLOW}=== Building IGRIS SDK Examples ===${NC}"

# Auto-install required dependencies on macOS.
if [[ "$(uname -s)" == "Darwin" ]]; then
    if command -v brew >/dev/null 2>&1; then
        for pkg in glfw; do
            if ! brew list --versions "${pkg}" >/dev/null 2>&1; then
                echo -e "${YELLOW}Installing missing dependency: ${pkg}${NC}"
                brew install "${pkg}"
                if [ $? -ne 0 ]; then
                    echo -e "${RED}Failed to install ${pkg} with Homebrew.${NC}"
                    exit 1
                fi
            fi
        done
    else
        echo -e "${RED}Homebrew not found. Please install Homebrew first: https://brew.sh${NC}"
        exit 1
    fi
fi

# Create build directory
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# Prefer local SDK config to avoid stale CMake cache entries.
SDK_CMAKE_DIR=""
if [ -f "${SCRIPT_DIR}/../build/igris_c_sdk-config.cmake" ]; then
    SDK_CMAKE_DIR="${SCRIPT_DIR}/../build"
elif [ -f "${SCRIPT_DIR}/../lib/cmake/igris_c_sdk/igris_c_sdk-config.cmake" ]; then
    SDK_CMAKE_DIR="${SCRIPT_DIR}/../lib/cmake/igris_c_sdk"
fi

# Configure
echo -e "${YELLOW}Configuring...${NC}"
if [ -n "${SDK_CMAKE_DIR}" ]; then
    cmake .. -DCMAKE_BUILD_TYPE=Release -DFETCHCONTENT_QUIET=OFF -Digris_c_sdk_DIR="${SDK_CMAKE_DIR}"
else
    cmake .. -DCMAKE_BUILD_TYPE=Release -DFETCHCONTENT_QUIET=OFF
fi

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed!${NC}"
    exit 1
fi

# Build
echo -e "${YELLOW}Building...${NC}"
if command -v nproc >/dev/null 2>&1; then
    JOBS=$(nproc)
elif [[ "$(uname -s)" == "Darwin" ]]; then
    JOBS=$(sysctl -n hw.ncpu)
else
    JOBS=4
fi

make -j"${JOBS}"

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}=== Build completed ===${NC}"
echo -e "${GREEN}Executables:${NC}"
echo -e "  ${BUILD_DIR}/sdk_gui_client"
echo -e "  ${BUILD_DIR}/lowlevel_example"
echo -e "  ${BUILD_DIR}/service_example"
echo -e "  ${BUILD_DIR}/hand_gui_client"
echo -e "  ${BUILD_DIR}/metaquest"
echo -e "  ${BUILD_DIR}/sensor_configuration"
if [ -f "${BUILD_DIR}/sensor_viewer" ]; then
    echo -e "  ${BUILD_DIR}/sensor_viewer"
fi
echo ""
echo -e "${YELLOW}Usage:${NC}"
echo -e "  ./service_example [domain_id]    - Service API (menu-based)"
echo -e "  ./lowlevel_example [domain_id]   - Pub/Sub low-level control"
echo -e "  ./sdk_gui_client <domain_id> <namespace>  - Full GUI client"
echo -e "  ./hand_gui_client [domain_id]    - Hand control GUI client"
echo -e "  ./metaquest [domain_id] [topic]  - MetaQuest raw data table viewer"
echo -e "  ./sensor_configuration [domain_id] [timeout_ms] - Sensor configuration example"
if [ -f "${BUILD_DIR}/sensor_viewer" ]; then
    echo -e "  ./sensor_viewer [domain_id]      - Sensor data visualization"
fi
