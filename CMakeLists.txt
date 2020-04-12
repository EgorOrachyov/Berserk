####################################################################
## BERSERK Device project

cmake_minimum_required(VERSION 3.11)
project(Berserk VERSION 1.0)

####################################################################
## Build options

option(BERSERK_DEBUG "Build engine sources in debug mode" NO)
option(BERSERK_RELEASE "Build engine sources in release mode" NO)
option(BERSERK_EDITOR "Build engine sources for use with editor" NO)
option(BERSERK_BUILD_TESTS "Build test folder with modules tests" YES)

option(BERSERK_PLATFORM_MACOS "Target platform to build/run" NO)
option(BERSERK_PLATFORM_WINDOWS "Target platform to build/run" NO)
option(BERSERK_WITH_GLFW "GLFW Window system" NO)
option(BERSERK_WITH_OPENGL "Use OpenGL as rendering API" NO)

####################################################################
## Define platform

if(APPLE)
    set(BERSERK_PLATFORM_MACOS YES)
    set(BERSERK_WITH_GLFW YES)
    set(BERSERK_WITH_OPENGL YES)
else()
    message(FATAL_ERROR "Platform not specified")
endif()

####################################################################
## Define Build type

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(BERSERK_DEBUG YES)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(BERSERK_RELEASE YES)
endif()

##################################################################
## Third-party setup

if (BERSERK_WITH_GLFW)
    message(STATUS "Add GLFW as window system API")
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(Code/Thirdparty/glfw)
endif()

if (BERSERK_PLATFORM_WINDOWS)
    message(STATUS "Add dirent as windows dir access")
    add_subdirectory(Code/Thirdparty/dirent)
endif()

if (BERSERK_WITH_OPENGL)
    message(STATUS "Add OpenGL as rendering API")
    add_subdirectory(Code/Thirdparty/opengl/core/4.1)
    add_subdirectory(Code/Devices/OpenGL)
endif()

####################################################################
## Configure file with options

configure_file(
    ${CMAKE_SOURCE_DIR}/Code/Core/BuildOptions.h.in
    ${CMAKE_BINARY_DIR}/Code/Core/BuildOptions.h)

####################################################################
## Add engine modules here

add_subdirectory(Code/Core)
add_subdirectory(Code/Platform)
add_subdirectory(Code/Engine)

if(BERSERK_BUILD_TESTS)
    add_subdirectory(Tests)
endif()