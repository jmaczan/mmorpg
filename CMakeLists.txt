cmake_minimum_required(VERSION 3.20)
project(mmorpg LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(SDL3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

add_library(wgpu-native SHARED IMPORTED)

set_target_properties(wgpu-native PROPERTIES
     IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/external/wgpu-native/lib/libwgpu_native.so"
     INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/external/wgpu-native/include"
)

add_subdirectory(external/sdl3webgpu)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(mmorpg ${SRC_FILES})

target_link_libraries(mmorpg PRIVATE
    SDL3::SDL3
    Boost::system
    wgpu-native
    sdl3webgpu
)

if (MSVC)
    target_compile_options(mmorpg PRIVATE /W4 /permissive-)
else()
    target_compile_options(mmorpg PRIVATE -Wall -Wextra -Wpedantic)
endif()
