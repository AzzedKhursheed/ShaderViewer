# macOS-specific configuration

message(STATUS "Configuring for macOS platform")

# Find GLFW using find_package on macOS (assumes Homebrew or manual installation)
find_package(glfw3 CONFIG REQUIRED)

# macOS-specific compiler flags
function(configure_macos_compiler_flags)
    # Clang flags for macOS
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic" PARENT_SCOPE)

    # macOS-specific optimizations
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG" PARENT_SCOPE)
endfunction()

# macOS-specific linking
function(configure_macos_linking target_name)
    target_link_libraries(${target_name} PRIVATE
        glfw
    )

    # Link macOS frameworks
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(${target_name} PRIVATE
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endfunction()

# macOS-specific compiler selection
function(configure_macos_compiler)
    # On macOS, prefer system Clang
    message(STATUS "Using system Clang on macOS")
endfunction()
