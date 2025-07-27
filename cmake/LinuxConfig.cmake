# Linux-specific configuration

message(STATUS "Configuring for Linux platform")

# Find X11 libraries (required for GLFW on Linux)
find_package(X11 REQUIRED)

# Use PkgConfig for GLFW on Linux
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# Linux-specific compiler selection
function(configure_linux_compiler)
    if(DEFINED ENV{CC} AND DEFINED ENV{CXX})
        # Use environment variables if set (e.g., CC=clang CXX=clang++)
        set(CMAKE_C_COMPILER $ENV{CC} PARENT_SCOPE)
        set(CMAKE_CXX_COMPILER $ENV{CXX} PARENT_SCOPE)
        message(STATUS "Using compilers from environment: CC=$ENV{CC}, CXX=$ENV{CXX}")
    elseif(EXISTS "/usr/bin/clang" AND EXISTS "/usr/bin/clang++")
        # Default to Clang if available on Ubuntu
        set(CMAKE_C_COMPILER "clang" PARENT_SCOPE)
        set(CMAKE_CXX_COMPILER "clang++" PARENT_SCOPE)
        message(STATUS "Using Clang compilers")
    endif()
endfunction()

# Linux-specific linking
function(configure_linux_linking target_name)
    target_link_libraries(${target_name} PRIVATE
        ${GLFW_LIBRARIES}
        ${CMAKE_DL_LIBS} # Required for dynamic loading on Linux
        ${X11_LIBRARIES}
        ${X11_Xrandr_LIB}
        ${X11_Xinput_LIB}
        ${X11_Xcursor_LIB}
        ${X11_Xxf86vm_LIB}
        pthread
    )

    target_include_directories(${target_name} PRIVATE
        ${GLFW_INCLUDE_DIRS}
    )
endfunction()

# Linux-specific compiler flags
function(configure_linux_compiler_flags)
    # GCC/Clang flags for Linux
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic" PARENT_SCOPE)

    # Clang-specific optimizations
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Using Clang-specific optimizations")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fsanitize=address" PARENT_SCOPE)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG" PARENT_SCOPE)
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address" PARENT_SCOPE)
    endif()
endfunction()
