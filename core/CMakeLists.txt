# Define the Engine library target
add_library(Core STATIC)

# Get all .cpp files recursively under Core folder
file(GLOB_RECURSE ALL_ENGINE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Filter out any source files inside a 'vkcore' folder
set(ENGINE_SOURCES "")
foreach(src_file IN LISTS ALL_ENGINE_SOURCES)
    if(NOT src_file MATCHES "/vkcore/")
        list(APPEND ENGINE_SOURCES ${src_file})
    endif()
endforeach()

# Add the filtered source files to the Engine library
target_sources(Core PRIVATE ${ENGINE_SOURCES})

# Specify the precompiled header
target_precompile_headers(Core PRIVATE ${CMAKE_SOURCE_DIR}/pch/pch.h)

# Include directories for the Engine library (Vulkan_INCLUDE_DIRS removed)
target_include_directories(Core PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# Link the Logger library and other dependencies (Vulkan removed)
target_link_libraries(Core PRIVATE Utils glfw glad glm tinygltf yaml-cpp::yaml-cpp EnTT::EnTT)
