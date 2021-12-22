cmake_minimum_required(VERSION 3.11)

# Set project name here
set(APP_NAME example)

project(${APP_NAME} VERSION 1.0)

# Option how to build game (do not touch)
option(GAME_BUILD_EXE "Build game as standalone executable" YES)
option(GAME_BUILD_DYNAMIC "Build game as shared library" NO)

# Internal defines
set(GAME_DEFINES)
set(GAME_DEBUG NO)
set(GAME_RELEASE NO)
set(GAME_TARGET_MACOS NO)
set(GAME_TARGET_LINUX NO)
set(GAME_TARGET_WINDOWS NO)
set(GAME_ARCH)
set(GAME_EXT)

# Path to bin berserk engine directory
set(BERSERK_PATH $ENV{BERSERK_PATH})

if (BERSERK_PATH)
    message(STATUS "Found berserk binary directory ${BERSERK_PATH}")
else ()
    message(FATAL_ERROR "Failed to find berserk path. Path must be specified in env")
endif ()

# Put sources here
set(GAME_HEADERS)
set(GAME_SRC)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(GAME_TARGET_WINDOWS YES)
    set(GAME_EXT "dll")
    set(GAME_ARCH "x64")
    list(APPEND GAME_DEFINES GAME_TARGET_WINDOWS)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(GAME_TARGET_LINUX YES)
    set(GAME_EXT "so")
    set(GAME_ARCH "x64")
    list(APPEND GAME_DEFINES GAME_TARGET_LINUX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(GAME_TARGET_MACOS YES)
    set(GAME_EXT "dylib")
    if (NOT GAME_ARCH)
        if (NOT CMAKE_OSX_ARCHITECTURES)
            if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64")
                set(GAME_ARCH "arm64")
            else ()
                set(GAME_ARCH "x64")
            endif ()
        else ()
            if ("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "arm64")
                set(GAME_ARCH "arm64")
            else ()
                set(GAME_ARCH "x64")
            endif ()
        endif ()
    endif ()
    message(STATUS "Build macos binaries for ${GAME_ARCH} architecture")
    list(APPEND GAME_DEFINES GAME_TARGET_MACOS)
else ()
    message(FATAL_ERROR "Unsupported target platform")
endif ()

# Entry point of the application
list(APPEND GAME_SRC main/Main.cpp)

# Head files
list(APPEND GAME_HEADERS
        code/GameApplication.hpp
        code/TestRHISimpleQuad.hpp)

# Source file
list(APPEND GAME_SRC
        code/GameApplication.cpp)

# All game sources
set(GAME_ALL_SOURCES ${GAME_HEADERS} ${GAME_SRC})

if (GAME_BUILD_EXE)
    add_executable(${APP_NAME} ${GAME_ALL_SOURCES})
    target_include_directories(${APP_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/code)
    target_link_libraries(${APP_NAME} PRIVATE berserk_runtime_dynamic)
endif ()