####################################################################
## BERSERK Engine project

cmake_minimum_required(VERSION 3.11)
project(Berserk VERSION 1.0)

####################################################################
## Define platform

option(BERSERK_PLATFORM_MACOS "Target platform to build/run" NO)

if(APPLE)
    set(BERSERK_PLATFORM_MACOS YES)
else()
    message(FATAL_ERROR "Platform not specified")
endif()

####################################################################
## Build options

option(BERSERK_DEBUG "Build engine sources in debug mode" NO)
option(BERSERK_RELEASE "Build engine sources in release mode" NO)
option(BERSERK_EDITOR "Build engine sources for use with editor" NO)

option(BERSERK_BUILD_TESTS "Build test folder with modules tests" YES)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(BERSERK_DEBUG YES)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(BERSERK_DEBUG YES)
endif()

configure_file(
    ${CMAKE_SOURCE_DIR}/Code/Core/BuildOptions.h.in
    ${CMAKE_BINARY_DIR}/Code/Core/BuildOptions.h)

####################################################################
## Add engine modules here

add_subdirectory(Code/Core)
add_subdirectory(Code/Platform)

if(BERSERK_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()