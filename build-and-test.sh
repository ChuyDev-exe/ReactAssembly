#!/bin/bash

# build-and-test.sh
# Script to build and test the Vite application

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${YELLOW}🚀 Vite Application Build & Test${NC}"
echo "======================================"

# Change to project root
cd "$(dirname "$0")"

# Check if node is installed
if ! command -v node &> /dev/null; then
    echo -e "${RED}❌ Error: Node.js is not installed${NC}"
    exit 1
fi

# Check if npm is installed
if ! command -v npm &> /dev/null; then
    echo -e "${RED}❌ Error: npm is not installed${NC}"
    exit 1
fi

echo -e "${GREEN}✓${NC} Node.js: $(node --version)"
echo -e "${GREEN}✓${NC} npm: $(npm --version)"
echo ""

# Check if node_modules exists
if [ ! -d "node_modules" ]; then
    echo -e "${YELLOW}📦 Installing dependencies...${NC}"
    npm install
    echo ""
fi

# Run linting
echo -e "${BLUE}🔍 Running ESLint...${NC}"
if npm run lint; then
    echo -e "${GREEN}✅ Linting passed${NC}"
else
    echo -e "${YELLOW}⚠️  Linting found issues (continuing anyway)${NC}"
fi
echo ""

# Run tests if available
echo -e "${BLUE}🧪 Running tests...${NC}"
if npm test -- --run 2>/dev/null; then
    echo -e "${GREEN}✅ Tests passed${NC}"
else
    # Check if test script exists
    if grep -q '"test"' package.json; then
        echo -e "${YELLOW}⚠️  Tests failed or not configured (continuing anyway)${NC}"
    else
        echo -e "${YELLOW}ℹ️  No tests configured${NC}"
    fi
fi
echo ""

# Build the application
echo -e "${BLUE}🔨 Building Vite application...${NC}"
npm run build

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Build successful!${NC}"
    echo ""
    
    # Show build output
    if [ -d "dist" ]; then
        echo "Build output in dist/:"
        du -sh dist/
        echo ""
    fi
else
    echo -e "${RED}❌ Build failed${NC}"
    exit 1
fi

# Preview the build
echo -e "${YELLOW}📋 Options:${NC}"
echo "  1. Preview build (npm run preview)"
echo "  2. Run dev server (npm run dev)"
echo "  3. Exit"
echo ""
read -p "Select option (1-3): " choice

case $choice in
    1)
        echo -e "${BLUE}🌐 Starting preview server...${NC}"
        npm run preview
        ;;
    2)
        echo -e "${BLUE}🌐 Starting dev server...${NC}"
        npm run dev
        ;;
    3)
        echo -e "${GREEN}✅ Done!${NC}"
        ;;
    *)
        echo -e "${YELLOW}Invalid option. Exiting.${NC}"
        ;;
esac
