#######################################################################################
#                                                                                     #
# BERSERK Engine (2018 - 2019)                                                        #
# CMake build file                                                                    #
#                                                                                     #
# Created by Orachyov Egor on 16.03.2019                                              #
#                                                                                     #
#######################################################################################

cmake_minimum_required(VERSION 3.1)
project(Berserk)

set(CMAKE_CXX_STANDARD 11)
set(ENGINE_LIBS)
set(THIRDPARTY_LIBS)

#######################################################################################
#                                                                                     #
# Configure compilation options for chosen third-party dependencies and additional    #
# macro extra data(version, etc.)                                                     #
#                                                                                     #
#######################################################################################

# Engine defines Specification

set(BERSERK_VERSION \"0.4\")
set(BERSERK_VERSION_MAJOR 0)
set(BERSERK_VERSION_MINOR 4)

# Build mode and platform

set(MODE_DEBUG 0)
set(MODE_RELEASE 0)
set(MODE_EDITOR 0)

set(PLATFORM_MAC 0)
set(PLATFORM_WIN 0)
set(PLATFORM_LINUX 0)

if(MODE STREQUAL debug)
    set(MODE_DEBUG 1)
elseif(MODE STREQUAL "release")
    set(MODE_RELEASE 1)
elseif(MODE STREQUAL "editor")
    set(MODE_EDITOR 1)
endif()

if(APPLE)
    set(PLATFORM_MAC 1)
elseif(WIN32)
    set(PLATFORM_WIN 1)
elseif(UNIX)
    set(PLATFORM_LINUX 1)
endif()

# Configure files

configure_file(
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/CMakeOptions/CMakeOptionsList.h.in"
        "${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public/CMakeOptions/CMakeOptionsList.h"
)

#######################################################################################
#                                                                                     #
# Configure common engine modules with default compilation params                     #
#                                                                                     #
#######################################################################################

# Extra compilation flags to support hardware extensions and compilation modes

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse -mmmx -msse -msse2 -msse3 -msse4")

if(MODE_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

include_directories("${PROJECT_SOURCE_DgIR}/Engine/Source/ThirdParty")
include_directories("${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public")

add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Source/Core")

list(APPEND ENGINE_LIBS BerserkCore)

#######################################################################################
#                                                                                     #
# Build tests and other things from here                                              #
#                                                                                     #
#######################################################################################

add_subdirectory("${PROJECT_SOURCE_DIR}/Tests")