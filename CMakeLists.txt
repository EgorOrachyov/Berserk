cmake_minimum_required(VERSION 3.9)
project(Berserk)

# Engine Version Specification

set (BERSERK_VERSION \"2.0\")
set (BERSERK_VERSION_MAJOR 2)
set (BERSERK_VERSION_MINOR 0)

# Rendering Engine Options

option (USE_METAL "Use for rendering MacOS GPU Driver" OFF)
option (USE_OPEN_GL "Use for rendering Open GL driver" OFF)
option (USE_DIRECT_3D "Use for rendering MS DirectX 3D driver" OFF)

# Audio Engine Options

option (USE_OPEN_AL "Use for audio Open AL driver" OFF)
option (USE_DIRECT_AUDIO "Use for rendering MS DirectX Audio driver" OFF)

# Configure files

configure_file (
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/Info/Version.h.in"
        "${PROJECT_BINARY_DIR}/Engine/Source/Core/Public/Info/Version.h"
)

configure_file (
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/Info/VideoDriver.h.in"
        "${PROJECT_BINARY_DIR}/Engine/Source/Core/Public/Info/VideoDriver.h"
)

configure_file (
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/Info/AudioDriver.h.in"
        "${PROJECT_BINARY_DIR}/Engine/Source/Core/Public/Info/AudioDriver.h"
)

#

set (TARGET Berserk)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

include_directories(${PROJECT_BINARY_DIR})
include_directories("${PROJECT_SOURCE_DIR}/Engine/ThirdParty")

include_directories("${PROJECT_SOURCE_DIR}/Engine/Source/Core")
add_subdirectory ("${PROJECT_SOURCE_DIR}/Engine/Source/Core")


set (SOURCES
        Engine/Source/Debug/main.cpp
        Engine/Source/Debug/CoreTest.h
        Engine/Source/Debug/ThirdPartyTest.h)

add_executable (${TARGET} ${SOURCES})
target_link_libraries (Berserk BerserkCore)