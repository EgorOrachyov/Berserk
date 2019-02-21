cmake_minimum_required(VERSION 3.9)
project(Berserk)

# Engine Version Specification

set (BERSERK_VERSION \"0.3\")
set (BERSERK_VERSION_MAJOR 0)
set (BERSERK_VERSION_MINOR 3)

# Rendering Engine Options

option (USE_METAL "Use for rendering MacOS GPU Driver" OFF)
option (USE_OPEN_GL "Use for rendering Open GL driver" ON)
option (USE_DIRECT_3D "Use for rendering MS DirectX 3D driver" OFF)

# Audio Engine Options

option (USE_OPEN_AL "Use for audio Open AL driver" OFF)
option (USE_DIRECT_AUDIO "Use for rendering MS DirectX Audio driver" OFF)

# Configure files

configure_file (
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/Info/Version.h.in"
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/Info/Version.h"
)

configure_file (
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/Info/VideoDriver.h.in"
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/Info/VideoDriver.h"
)

configure_file (
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/Info/AudioDriver.h.in"
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/Info/AudioDriver.h"
)

#

set (TARGET Berserk)
set (THIRD_PARTY)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse -mmmx -msse -msse2 -msse3 -msse4")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

include_directories("${PROJECT_SOURCE_DIR}/Engine/ThirdParty")
include_directories("${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public")
include_directories("${PROJECT_SOURCE_DIR}/Engine/Source/Engine/Public")
include_directories("${PROJECT_SOURCE_DIR}/Engine/Source/Render/Public")

add_subdirectory ("${PROJECT_SOURCE_DIR}/Engine/Source/Core")

#

if (USE_OPEN_GL)

    include (Engine/Source/OpenGL/CMakeInclude.txt)
    include_directories ("${PROJECT_SOURCE_DIR}/Engine/Source/OpenGL/Public")
    add_subdirectory ("${PROJECT_SOURCE_DIR}/Engine/Source/OpenGL")
    set (THIRD_PARTY ${THIRD_PARTY} BerserkOpenGL)
    message ("-- Use for rendering Open GL driver")

endif (USE_OPEN_GL)

#

set (SOURCES

        Engine/Source/Engine/Public/Resource/IResource.h

        Engine/Source/Render/Public/Platform/IShader.h
        Engine/Source/Render/Public/Platform/IGPUBuffer.h

        Engine/Source/Debug/main.cpp
        Engine/Source/Debug/CoreTest.h
        Engine/Source/Debug/ThirdPartyTest.h
        Engine/Source/Debug/FrustumCullingTest.h
        Engine/Source/Debug/OpenGLStartTest.h

        )


add_executable (${TARGET} ${SOURCES})
target_link_libraries (Berserk BerserkCore ${THIRD_PARTY})