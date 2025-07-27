# Windows-specific configuration

message(STATUS "Configuring for Windows platform")

# Find GLFW using find_package on Windows (assumes vcpkg or manual installation)
find_package(glfw3 CONFIG REQUIRED)

# Windows-specific compiler flags
function(configure_windows_compiler_flags)
    if(MSVC)
        # MSVC-specific flags
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4" PARENT_SCOPE)

        # Enable parallel compilation
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" PARENT_SCOPE)

        # Runtime library
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" PARENT_SCOPE)
    else()
        # MinGW or other Windows compilers
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic" PARENT_SCOPE)
    endif()
endfunction()

# Windows-specific linking
function(configure_windows_linking target_name)
    target_link_libraries(${target_name} PRIVATE
        glfw
    )

    # Link Windows-specific libraries
    if(MSVC)
        target_link_libraries(${target_name} PRIVATE
            opengl32
            gdi32
            user32
            kernel32
            shell32
        )
    endif()
endfunction()

# Windows-specific compiler selection
function(configure_windows_compiler)
    # On Windows, let CMake handle compiler detection
    # Users can specify -G "Visual Studio 16 2019" or similar
    message(STATUS "Using default Windows compiler detection")
endfunction()
