cmake_minimum_required(VERSION 3.9)
project(Berserk)

# Engine Version Specification

set (BERSERK_VERSION "2.0")
set (BERSERK_VERSION_MAJOR 2)
set (BERSERK_VERSION_MINOR 0)

configure_file (
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/Info/Version.h.in"
        "${PROJECT_BINARY_DIR}/Engine/Source/Core/Public/Info/Version.h"
)

# Rendering Engine Options

option (USE_METAL "Use for rendering MacOS GPU Driver" NO)
option (USE_OPEN_GL "Use for rendering Open GL driver" NO)
option (USE_DIRECT_3D "Use for rendering MS DirectX 3D driver" NO)

# Audio Engine Options

option (USE_OPEN_AL "Use for audio Open AL driver" NO)
option (USE_DIRECT_AUDIO "Use for rendering MS DirectX Audio driver" NO)

set (TARGET Berserk)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

include_directories(${PROJECT_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}/Engine/Source/Core")
add_subdirectory ("${PROJECT_SOURCE_DIR}/Engine/Source/Core")

set (SOURCES
        Engine/Source/Debug/main.cpp
        )

add_executable (${TARGET} ${SOURCES})
target_link_libraries (Berserk BerserkCore)