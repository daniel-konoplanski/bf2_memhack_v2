# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.22)

# Define the project and specify the languages
project(bf2_memhack VERSION 0.1 LANGUAGES CXX)

set(CMAKE_SYSTEM_NAME Windows)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add the source files for the library

set(SOURCES
    src/main.cpp
    src/helpers/memory_operations.cpp
    src/constants/module_addresses.cpp
    src/features/feature_common.cpp
    src/features/nametags/nametags.cpp
    src/features/nametags/nametags_range.cpp
    src/features/nametags/nametags_display_distance.cpp
    src/features/nametags/nametags_distance_range.cpp
    src/features/nametags/nametags_manager.cpp
    src/features/minimap/minimap_1.cpp
)

# Create the DLL (shared library)
add_library(bf2memhack SHARED ${SOURCES})

set_target_properties(bf2memhack PROPERTIES OUTPUT_NAME "bf2memhack" SUFFIX ".dll" PREFIX "")

# Include MinGW headers
include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
