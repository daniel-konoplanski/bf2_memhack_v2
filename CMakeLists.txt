# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.22)

# Define the project and specify the languages
project(bf2_memhack VERSION 0.1 LANGUAGES CXX)

set(CMAKE_SYSTEM_NAME Windows)

# Add the source files for the library
set(SOURCES
    src/main.cpp
)

# Create the DLL (shared library)
add_library(bf2memhack SHARED ${SOURCES})

set_target_properties(bf2memhack PROPERTIES OUTPUT_NAME "bf2memhack" SUFFIX ".dll")

# Include MinGW headers
include_directories(/usr/i686-w64-mingw32/include)

# Link MinGW libraries
link_directories(/usr/i686-w64-mingw32/lib)
