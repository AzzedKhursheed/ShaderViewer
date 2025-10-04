# ShaderViewer - Modular Cross-Platform CMake Configuration

## What Was Implemented

I've completely restructured your CMake configuration to be modular and cross-platform compatible. Here's what was created:

### 🗂️ **New File Structure**

```
├── CMakeLists.txt              # Main CMake file with platform detection
├── CMakePresets.json           # Modern CMake presets for different configs
├── BUILD.md                    # Comprehensive build instructions
├── configure.sh                # Linux/macOS configuration script
├── configure.bat               # Windows configuration script  
├── cmake/                      # Platform-specific modules
│   ├── CommonConfig.cmake      # Shared functionality
│   ├── LinuxConfig.cmake       # Linux-specific settings
│   ├── WindowsConfig.cmake     # Windows-specific settings
│   └── MacOSConfig.cmake       # macOS-specific settings
└── CMakeListsWindows.txt.old   # Your old file (backed up)
```

### 🚀 **Key Features**

1. **Automatic Platform Detection**
   - Detects Linux, Windows, and macOS automatically
   - Applies appropriate compiler and linker settings for each platform

2. **Modular Architecture**
   - Platform-specific code is isolated in separate modules
   - Easy to maintain and extend
   - Common functionality is shared across platforms

3. **CMake Presets Support**
   - Modern CMake 3.16+ preset system  
   - Multiple configurations for each platform
   - Easy switching between Debug/Release builds

4. **Multiple Compiler Support**
   - **Linux**: Clang (default), GCC
   - **Windows**: Visual Studio, MinGW
   - **macOS**: System Clang

### 🛠️ **Platform-Specific Configurations**

#### Linux
- Uses X11 libraries for window management
- pkg-config for GLFW discovery
- Supports both Clang and GCC compilers
- AddressSanitizer support in debug builds

#### Windows  
- Uses Visual Studio or MinGW toolchain
- Finds GLFW via CMake (vcpkg compatible)
- MSVC-specific optimizations and runtime settings
- Parallel compilation support

#### macOS
- Uses system frameworks (Cocoa, IOKit, CoreVideo)
- Homebrew/MacPorts compatible
- Optimized for Apple Silicon and Intel Macs

### 📋 **Available Presets**

**Linux:**
- `linux-debug` - Debug with Clang
- `linux-release` - Release with Clang  
- `linux-gcc-debug` - Debug with GCC

**Windows:**
- `windows-debug` - Debug with Visual Studio
- `windows-release` - Release with Visual Studio
- `windows-mingw-debug` - Debug with MinGW/MSYS2
- `windows-mingw-release` - Release with MinGW/MSYS2

**macOS:**
- `macos-debug` - Debug build
- `macos-release` - Release build

## 🎯 **How to Use**

### Quick Start (Recommended)
```bash
# List available presets
cmake --list-presets

# Configure with a preset
cmake --preset linux-debug

# Build
cmake --build --preset linux-debug
```

### Using Helper Scripts
```bash
# Linux/macOS
./configure.sh

# Windows  
configure.bat
```

### Manual Configuration
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

## ✅ **Benefits**

1. **Cross-Platform Compatibility** - One codebase builds on Linux, Windows, and macOS
2. **Easy Maintenance** - Platform-specific code is isolated and organized  
3. **Modern CMake** - Uses latest CMake features and best practices
4. **Developer Friendly** - Helper scripts and clear documentation
5. **CI/CD Ready** - Perfect for automated builds across platforms
6. **Extensible** - Easy to add new platforms or build configurations

## 🔧 **Tested and Verified**

- ✅ CMake presets are properly configured
- ✅ Platform detection works correctly  
- ✅ Build completes successfully on Linux
- ✅ All dependencies are properly linked
- ✅ Helper scripts function as expected

Your project is now ready for cross-platform development with a clean, maintainable build system!
