#!/bin/bash

# compile-wasm.sh
# Script to compile WebAssembly C files using Emscripten

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}🔨 Starting WebAssembly Compilation${NC}"
echo "======================================"

# Check if emcc is installed
if ! command -v emcc &> /dev/null; then
    echo -e "${RED}❌ Error: emcc (Emscripten) is not installed or not in PATH${NC}"
    echo "Please install Emscripten: https://emscripten.org/docs/getting_started/downloads.html"
    exit 1
fi

# Display Emscripten version
echo -e "${GREEN}✓${NC} Emscripten found: $(emcc --version | head -n 1)"

# Change to WebAssembly directory
cd "$(dirname "$0")/WebAssembly"

# Run make
echo ""
echo -e "${YELLOW}Running make...${NC}"
make clean
make all

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}✅ WebAssembly compilation successful!${NC}"
    
    # Copy fuente.ttf to public/wasm
    echo ""
    echo -e "${YELLOW}📋 Copying fuente.ttf to public/wasm...${NC}"
    cp fuente.ttf ../public/wasm/fuente.ttf
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓${NC} Font file copied successfully"
    else
        echo -e "${RED}⚠️  Warning: Could not copy font file${NC}"
    fi
    
    echo ""
    echo "Generated files:"
    ls -lh ../public/wasm/simple.* ../public/wasm/fuente.ttf 2>/dev/null || echo "  (files in ../public/wasm/)"
    echo ""
else
    echo -e "${RED}❌ Compilation failed${NC}"
    exit 1
fi
