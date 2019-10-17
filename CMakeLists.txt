#--------------------------------------------------------------------------------#
# This file is part of:                                                          #
# Berserk Engine project                                                         #
# https://github.com/EgorOrachyov/Berserk                                        #
#--------------------------------------------------------------------------------#
# MIT License                                                                    #
#                                                                                #
# Copyright (c) 2018-2019 Egor Orachyov                                          #
#                                                                                #
# Permission is hereby granted, free of charge, to any person obtaining a copy   #
# of this software and associated documentation files (the "Software"), to deal  #
# in the Software without restriction, including without limitation the rights   #
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      #
# copies of the Software, and to permit persons to whom the Software is          #
# furnished to do so, subject to the following conditions:                       #
#                                                                                #
# The above copyright notice and this permission notice shall be included in all #
# copies or substantial portions of the Software.                                #
#                                                                                #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    #
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  #
# SOFTWARE.                                                                      #
#--------------------------------------------------------------------------------#

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

set(MODE_DEBUG 1)
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
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wnon-virtual-dtor -pedantic")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v")
    endif()
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