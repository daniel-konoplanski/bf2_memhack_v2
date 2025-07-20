cmake_minimum_required(VERSION 3.22)

project(bf2_memhack VERSION 1.0 LANGUAGES CXX)

set(CMAKE_SYSTEM_NAME Windows)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(imgui CONFIG REQUIRED)
find_package(minhook CONFIG REQUIRED)

file(
    GLOB_RECURSE
    SOURCE
    CONFIGURE_DEPENDS
    RELATIVE "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

message(STATUS "All source files:\n  ${SOURCE}")

add_library(bf2memhack SHARED ${SOURCE})

set_target_properties(bf2memhack PROPERTIES OUTPUT_NAME "bf2memhack" SUFFIX ".dll" PREFIX "")

include_directories(${CMAKE_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -lwinpthread")

target_link_libraries(bf2memhack PRIVATE
    imgui::imgui
    minhook::minhook
    gdi32
    dwmapi
    user32
    shell32)

message("VCPKG_BUILD_TYPE is ${VCPKG_BUILD_TYPE}")
