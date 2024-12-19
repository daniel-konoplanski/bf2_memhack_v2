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
    src/dllmain.cpp
    src/helpers/module_addresses.cpp
    src/features/feature_common.cpp
    src/features/nametags/nametags.cpp
    src/features/nametags/nametags_range.cpp
    src/features/nametags/nametags_display_distance.cpp
    src/features/nametags/nametags_distance_range.cpp
    src/features/nametags/nametags_manager.cpp
    src/features/minimap/minimap_1.cpp
    src/features/minimap/minimap_4.cpp
    src/features/minimap/minimap_7.cpp
    src/features/minimap/kit_icons.cpp
    src/features/minimap/minimap_manager.cpp
    src/helpers/memory_operations.cpp
    src/helpers/hooking.cpp
    src/helpers/windows.cpp
    src/helpers/bf2_classes.cpp
    src/hooks/original_functions.cpp
    src/hooks/functions/dx9.cpp
    src/hooks/functions/windows.cpp
    src/gui/imgui_manager.cpp
    src/hooks/hook_manager.cpp
    src/worker.cpp
)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

set(IMGUI_BACKENDS
    ${IMGUI_DIR}/backends/imgui_impl_dx9.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
)

set(ALL_SOURCES ${SOURCES} ${IMGUI_SOURCES} ${IMGUI_BACKENDS})

# Create the shared library
add_library(bf2memhack SHARED ${ALL_SOURCES})

set_target_properties(bf2memhack PROPERTIES OUTPUT_NAME "bf2memhack" SUFFIX ".dll" PREFIX "")

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external
)

target_include_directories(bf2memhack PRIVATE ${IMGUI_DIR} ${IMGUI_DIR}/backends)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static -lwinpthread")

add_library(MinHook STATIC IMPORTED)

set_target_properties(MinHook PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/external/minhook/lib/libMinHook.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/external/minhook/include"
)

target_link_libraries(bf2memhack PRIVATE
    MinHook
    d3d9
    gdi32
    dwmapi)
