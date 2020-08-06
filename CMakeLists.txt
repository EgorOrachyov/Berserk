####################################################################
## BERSERK Engine project

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
option(BERSERK_WITH_STB_IMAGE "Use STB Image for image importing" YES)
option(BERSERK_WITH_OPENGL "Use OpenGL as rendering API" YES)
option(BERSERK_WITH_WHEREAMI "Use WhereAmI to detect executable path" YES)
option(BERSERK_WITH_FREETYPE "Use freetype2 as font importing library" YES)

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

if (BERSERK_WITH_STB_IMAGE)
    message(STATUS "Add STB Image as image utility")
    add_subdirectory(Code/Thirdparty/stb_image)
    add_subdirectory(Code/Thirdparty/stb_image_resize)
endif()

if (BERSERK_WITH_WHEREAMI)
    message(STATUS "Add WhereAmI as executable path utility")
    add_subdirectory(Code/Thirdparty/whereami)
endif()

if (BERSERK_WITH_FREETYPE)
    message(STATUS "Add freetype2 as font import tool")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(SKIP_INSTALL_HEADERS ON CACHE BOOL "" FORCE)
    set(SKIP_INSTALL_LIBRARIES ON CACHE BOOL "" FORCE)
    set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
    set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz ON CACHE BOOL "" FORCE)
    add_subdirectory(Code/Thirdparty/freetype2)
endif()


####################################################################
## Configure file with options

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/Code/Core/BuildOptions.h.in
    ${CMAKE_CURRENT_LIST_DIR}/Code/Core/BuildOptions.h)

####################################################################
## Add engine modules here

add_subdirectory(Code/Core)
add_subdirectory(Code/Platform)
add_subdirectory(Code/Engine)
add_subdirectory(Code/Importers)
add_subdirectory(Code/Tools)
add_subdirectory(Code/Main)

if(BERSERK_BUILD_TESTS)
    add_subdirectory(Tests)
endif()