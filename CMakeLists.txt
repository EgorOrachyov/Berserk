#######################################################################################
#                                                                                     #
# BERSERK Engine(2019)                                                               #
# CMake build file                                                                    #
#                                                                                     #
# Created by Orachyov Egor on 16.03.2019                                              #
#                                                                                     #
#######################################################################################

cmake_minimum_required(VERSION 3.1)
project(Berserk)

set(CMAKE_CXX_STANDARD 11)

# Engine defines Specification

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

option(CMODE_DEBUG "Build mode" OFF)
option(CMODE_RELEASE "Build mode" OFF)
option(CMODE_EDITOR "Build mode" OFF)

option(CPLATFORM_MAC "Target OS platform" OFF)
option(CPLATFORM_WIN "Target OS platform" OFF)
option(CPLATFORM_LINUX "Target OS platform" OFF)

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

set(BERSERK_LINK_COMMON)
set(BERSERK_LINK_THIRD_PARTY)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse -mmmx -msse -msse2 -msse3 -msse4")

if(CMODE_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")
else(CMODE_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif(CMODE_DEBUG)

include_directories("${PROJECT_SOURCE_DgIR}/Engine/Source/ThirdParty")
include_directories("${PROJECT_SOURCE_DIR}/Engine/Source/Core/Public")

add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Source/Core")

#######################################################################################
#                                                                                     #
# Successful engine linking                                                           #
#                                                                                     #
#######################################################################################

message(STATUS "Use 'BERSERK_LINK_COMMON' to link default engine components")
message(STATUS "Use 'BERSERK_LINK_THIRD_PARTY' to link third-party platform-based engine components")

#######################################################################################
#                                                                                     #
# Build tests and other things from here                                              #
#                                                                                     #
#######################################################################################

add_subdirectory("${PROJECT_SOURCE_DIR}/Tests")