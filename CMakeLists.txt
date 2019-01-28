cmake_minimum_required(VERSION 3.9)
project(Berserk)

set (BERSERK_VERSION_MAJOR 2)
set (BERSERK_VERSION_MINOR 0)

configure_file (
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/Misc/Version.h.in"
        "${PROJECT_BINARY_DIR}/Engine/Source/Core/Public/Misc/Version.h"
)

set (TARGET Berserk)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

include_directories(${PROJECT_BINARY_DIR})
include_directories(Engine/Source/Core)

set (SOURCES
        Engine/Source/Debug/main.cpp
        )

add_executable (${TARGET} ${SOURCES})

add_subdirectory (Engine/Source/Core)

target_link_libraries (Berserk BerserkCore)