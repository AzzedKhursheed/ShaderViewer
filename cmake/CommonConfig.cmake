# Common configuration shared across all platforms

# Common include directories
function(configure_common_includes target_name)
    target_include_directories(${target_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader
    )

    # Add GLM include directory if found
    if(glm_FOUND)
        target_include_directories(${target_name} PRIVATE ${glm_INCLUDE_DIRS})
    elseif(GLM_INCLUDE_DIR)
        target_include_directories(${target_name} PRIVATE ${GLM_INCLUDE_DIR})
    endif()
endfunction()

# Common libraries that all platforms need
function(configure_common_linking target_name)
    target_link_libraries(${target_name} PRIVATE
        glad
        OpenGL::GL
    )
endfunction()

# Configure ImGui if available
function(configure_imgui target_name)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui.cpp")
        add_library(imgui
            external/imgui/imgui.cpp
            external/imgui/imgui_draw.cpp
            external/imgui/imgui_tables.cpp
            external/imgui/imgui_widgets.cpp
            external/imgui/backends/imgui_impl_glfw.cpp
            external/imgui/backends/imgui_impl_opengl3.cpp
        )
        target_include_directories(imgui PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
            ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends
        )
        target_link_libraries(${target_name} PRIVATE imgui)
        message(STATUS "ImGui found and added to build")
    else()
        message(WARNING "ImGui not found at external/imgui/. Please clone it there.")
    endif()
endfunction()

# Copy resources to build directory
function(copy_resources)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # Only copy tinyobjloader if it exists as a separate directory
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader")
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    endif()
endfunction()
