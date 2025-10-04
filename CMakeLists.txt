cmake_minimum_required(VERSION 3.16)
cmake_policy(VERSION 3.16)
project(ShaderViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add cmake modules to path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include common configuration
include(CommonConfig)

# Platform detection and configuration
message(STATUS "Detected platform: ${CMAKE_SYSTEM_NAME}")

if(WIN32)
    include(WindowsConfig)
    configure_windows_compiler()
    configure_windows_compiler_flags()
elseif(UNIX AND NOT APPLE)
    include(LinuxConfig)
    configure_linux_compiler()
    configure_linux_compiler_flags()
elseif(APPLE)
    include(MacOSConfig)
    configure_macos_compiler()
    configure_macos_compiler_flags()
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# Find common packages
find_package(OpenGL REQUIRED)

# Create GLAD library
add_library(glad STATIC external/glad/gl.c)
target_include_directories(glad PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# Add source files
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Shader.cpp
    src/ObjModel.cpp
    src/Camera.cpp
)

# Configure common includes and linking
configure_common_includes(${PROJECT_NAME})
configure_common_linking(${PROJECT_NAME})

# Configure platform-specific linking
if(WIN32)
    configure_windows_linking(${PROJECT_NAME})
elseif(UNIX AND NOT APPLE)
    configure_linux_linking(${PROJECT_NAME})
elseif(APPLE)
    configure_macos_linking(${PROJECT_NAME})
endif()

# Configure ImGui
configure_imgui(${PROJECT_NAME})

# Copy resources
copy_resources()
