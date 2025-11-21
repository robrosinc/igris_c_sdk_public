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

# Create build directory
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# Configure
echo -e "${YELLOW}Configuring...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed!${NC}"
    exit 1
fi

# Build
echo -e "${YELLOW}Building...${NC}"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}=== Build completed ===${NC}"
echo -e "${GREEN}Executables:${NC}"
echo -e "  ${BUILD_DIR}/sdk_gui_client"
echo -e "  ${BUILD_DIR}/lowlevel_example"
echo -e "  ${BUILD_DIR}/service_example"
echo ""
echo -e "${YELLOW}Usage:${NC}"
echo -e "  ./service_example [domain_id]    - Service API (menu-based)"
echo -e "  ./lowlevel_example [domain_id]   - Pub/Sub low-level control"
echo -e "  ./sdk_gui_client [domain_id]     - Full GUI client"
