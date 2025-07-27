# Cross-Platform Build System

This project now uses a modular CMake configuration that automatically detects your platform and applies the appropriate settings.

## Platform Support

- **Linux**: Uses X11, pkg-config for GLFW, supports GCC and Clang
- **Windows**: Uses Visual Studio or MinGW, finds GLFW via CMake
- **macOS**: Uses system frameworks, finds GLFW via CMake

## Quick Start

### Using CMake Presets (Recommended)

List available presets:
```bash
cmake --list-presets
```

Configure for your platform:
```bash
# Linux (Debug with Clang)
cmake --preset linux-debug

# Linux (Debug with GCC)
cmake --preset linux-gcc-debug

# Linux (Release)
cmake --preset linux-release

# Windows (Visual Studio)
cmake --preset windows-debug
cmake --preset windows-release

# Windows (MinGW/MSYS2)
cmake --preset windows-mingw-debug
cmake --preset windows-mingw-release

# macOS
cmake --preset macos-debug
cmake --preset macos-release
```

Build:
```bash
# Build using preset name
cmake --build --preset linux-debug

# Or build using build directory
cmake --build build/linux-debug
```

### Manual Configuration

If you prefer manual configuration:

**Linux:**
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

**Windows (Visual Studio):**
```cmd
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Debug
```

**Windows (MinGW):**
```cmd
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
mingw32-make -j4
```

**macOS:**
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(sysctl -n hw.ncpu)
```

## Project Structure

```
├── CMakeLists.txt          # Main CMake file with platform detection
├── CMakePresets.json       # CMake presets for different configurations
├── cmake/                  # Platform-specific configurations
│   ├── CommonConfig.cmake  # Shared configuration
│   ├── LinuxConfig.cmake   # Linux-specific settings
│   ├── WindowsConfig.cmake # Windows-specific settings
│   └── MacOSConfig.cmake   # macOS-specific settings
├── src/                    # Source files
├── include/                # Header files
├── shaders/                # Shader files
├── assets/                 # 3D models and textures
└── external/               # Third-party libraries
```

## Dependencies

### Linux
Install required packages:
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

# Arch Linux
sudo pacman -S base-devel cmake glfw-x11 mesa glu libx11 libxrandr libxinerama libxcursor libxi

# Fedora
sudo dnf install gcc-c++ cmake glfw-devel mesa-libGL-devel mesa-libGLU-devel libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel
```

### Windows
- Install Visual Studio 2019/2022 with C++ development tools
- Or install MinGW-w64
- Install GLFW manually or use vcpkg:
  ```cmd
  vcpkg install glfw3:x64-windows
  ```

### macOS
```bash
# Using Homebrew
brew install cmake glfw

# Using MacPorts
sudo port install cmake glfw
```

## Customization

You can customize the build by:

1. **Modifying platform-specific files** in the `cmake/` directory
2. **Adding new presets** in `CMakePresets.json`
3. **Setting environment variables** like `CC` and `CXX` to override compiler selection

## Troubleshooting

### Linux
- If GLFW is not found, install the development packages
- For Clang, make sure both `clang` and `clang++` are installed

### Windows
- Make sure GLFW is properly installed and CMake can find it
- For Visual Studio, ensure the C++ development workload is installed
- For MinGW, ensure it's in your PATH

### macOS
- Install Xcode command line tools: `xcode-select --install`
- Use Homebrew or MacPorts to install dependencies
