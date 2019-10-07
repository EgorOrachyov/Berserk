#######################################################################################
#                                                                                     #
# BERSERK Engine(2019)                                                               #
# CMake build file                                                                    #
#                                                                                     #
# Created by Orachyov Egor on 16.03.2019                                              #
#                                                                                     #
#######################################################################################

cmake_minimum_required(VERSION 3.1)

# Engine Version Specification

set(BERSERK_VERSION \"0.3\")
set(BERSERK_VERSION_MAJOR 0)
set(BERSERK_VERSION_MINOR 3)

#######################################################################################
#                                                                                     #
# Configure compilation options for chosen third-party dependencies and additional    #
# macro extra data(version, etc.)                                                    #
#                                                                                     #
#######################################################################################

# Compile options for third-party based engine modules

option(USE_METAL "Use for rendering MacOS GPU Driver" OFF)
option(USE_OPEN_GL "Use for rendering OpenGL driver" ON)
option(USE_DIRECT_3D "Use for rendering MS DirectX 3D driver" OFF)
option(USE_OPEN_AL "Use for audio Open AL driver" OFF)
option(USE_DIRECT_AUDIO "Use for rendering MS DirectX Audio driver" OFF)
option(USE_FREE_IMAGE "Use FreeImage library for importing and exporting images" ON)
option(USE_FREE_TYPE "Use FreeType library for importing fonts" ON)
option(USE_GLFW_SYSTEM "Use GLFW Window managers to access graphics features" ON)

# Configure files

configure_file(
        "${PROJECT_SOURCE_DIR}/Engine/Source/CoreSystem/Public/CMakeOptions/CMakeOptionsList.h.in"
        "${PROJECT_SOURCE_DIR}/Engine/Source/CoreSystem/Public/CMakeOptions/CMakeOptionsList.h"
)

#######################################################################################
#                                                                                     #
# Configure common engine modules with default compilation params                     #
#                                                                                     #
#######################################################################################

# Extra compilation flags to support hardware extensions and compilation modes

set(BERSERK_LINK_COMMON)
set(BERSERK_LINK_THIRD_PARTY)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse -mmmx -msse -msse2 -msse3 -msse4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

include_directories("${PROJECT_SOURCE_DIR}/Engine/Source/ThirdParty")
include_directories("${PROJECT_SOURCE_DIR}/Engine/Source/CoreSystem/Public")
include_directories("${PROJECT_SOURCE_DIR}/Engine/Source/RapidXML/Public")

add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Source/CoreSystem")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Source/RapidXML")

#######################################################################################
#                                                                                     #
# Successful engine linking                                                           #
#                                                                                     #
#######################################################################################

message(STATUS "Use to configure BERSERK Engine(CMakeInclude.txt)")
message(STATUS "Use 'BERSERK_LINK_COMMON' to link default engine components")
message(STATUS "Use 'BERSERK_LINK_THIRD_PARTY' to link third-party platform-based engine components")