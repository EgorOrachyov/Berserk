cmake_minimum_required(VERSION 3.11)
project(BerserkPlatform VERSION 1.0 LANGUAGES CXX)

set(GlfwSystem_HEADERS
    GlfwSystem/GlfwInput.h
    GlfwSystem/GlfwWindow.h)

set(Std_HEADERS
    Std/StdFile.h)

set(Unix_HEADERS
    Unix/UnixDirectory.h)

set(macOS_SOURCES
    macOS/macOS.cpp
    macOS/macOS.h)

if(BERSERK_PLATFORM_MACOS)
    set(Target_SOURCES
        ${macOS_SOURCES}
        ${Std_HEADERS}
        ${Unix_HEADERS}
        ${GlfwSystem_HEADERS}
        ${macOS_HEADERS})
endif()

add_library(BerserkPlatform SHARED ${Target_SOURCES})
target_include_directories(BerserkPlatform PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkPlatform PRIVATE glfw)
target_link_libraries(BerserkPlatform PRIVATE BerserkCore)

target_compile_features(BerserkPlatform PUBLIC cxx_std_11)

set_target_properties(BerserkPlatform PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkPlatform PROPERTIES CXX_STANDARD_REQUIRED YES)
