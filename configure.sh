#!/bin/bash

# Build configuration script for ShaderViewer
# This script helps you configure and build the project for your platform

set -e

echo "=== ShaderViewer Build Configuration ==="
echo

# Detect platform
case "$(uname -s)" in
    Linux*)
        PLATFORM="Linux"
        echo "Detected platform: Linux"
        ;;
    Darwin*)
        PLATFORM="macOS"
        echo "Detected platform: macOS"
        ;;
    MINGW*|MSYS*|CYGWIN*)
        PLATFORM="Windows"
        echo "Detected platform: Windows (MinGW/MSYS)"
        ;;
    *)
        echo "Unknown platform: $(uname -s)"
        exit 1
        ;;
esac

echo

# Show available presets for this platform
echo "Available build configurations:"
case "$PLATFORM" in
    Linux)
        echo "  1) linux-debug        - Debug build with Clang"
        echo "  2) linux-release      - Release build with Clang"
        echo "  3) linux-gcc-debug    - Debug build with GCC"
        echo "  4) custom             - Manual configuration"
        ;;
    macOS)
        echo "  1) macos-debug        - Debug build"
        echo "  2) macos-release      - Release build"
        echo "  3) custom             - Manual configuration"
        ;;
    Windows)
        echo "  1) windows-mingw-debug - Debug build with MinGW"
        echo "  2) custom              - Manual configuration"
        echo "Note: For Visual Studio, use CMakePresets.json directly"
        ;;
esac

echo
read -p "Choose configuration (1-4): " choice

# Set preset based on choice
case "$PLATFORM-$choice" in
    Linux-1)
        PRESET="linux-debug"
        ;;
    Linux-2)
        PRESET="linux-release"
        ;;
    Linux-3)
        PRESET="linux-gcc-debug"
        ;;
    macOS-1)
        PRESET="macos-debug"
        ;;
    macOS-2)
        PRESET="macos-release"
        ;;
    Windows-1)
        PRESET="windows-mingw-debug"
        ;;
    *-4|*-3)
        PRESET="custom"
        ;;
    *)
        echo "Invalid choice"
        exit 1
        ;;
esac

echo

if [ "$PRESET" = "custom" ]; then
    echo "Manual configuration selected."
    echo "Create a build directory and run cmake manually:"
    echo "  mkdir build && cd build"
    echo "  cmake .. -DCMAKE_BUILD_TYPE=Debug"
    echo "  make -j\$(nproc)"
    exit 0
fi

# Configure using preset
echo "Configuring with preset: $PRESET"
cmake --preset "$PRESET"

echo
echo "Configuration complete!"
echo "To build, run:"
echo "  cmake --build --preset $PRESET"
echo
echo "Or:"
echo "  cmake --build build/$(echo $PRESET | tr '-' '/')"
