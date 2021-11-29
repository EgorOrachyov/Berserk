####################################################################
## BERSERK Engine project

cmake_minimum_required(VERSION 3.11)
project(Berserk VERSION 1.0)

####################################################################
## Build options

option(BERSERK_EDITOR "Build engine sources for use with editor" NO)
option(BERSERK_BUILD_TESTS "Build test folder with modules tests" YES)
option(BERSERK_STATIC_BUILD "Build static runtime library" NO)
option(BERSERK_DYNAMIC_BUILD "Build dynamic runtime library" YES)

####################################################################
## Internal options (for modules build: set automatically)

option(BERSERK_WITH_GLFW "Use GLFW Window system" NO)
option(BERSERK_WITH_GLEW "Use GLEW as gl loader" NO)
option(BERSERK_WITH_STB_IMAGE "Use STB Image for image importing" YES)
option(BERSERK_WITH_OPENGL "Use OpenGL-based as rendering backend" NO)
option(BERSERK_WITH_VULKAN "Use Vulkan-based as rendering backend" NO)
option(BERSERK_WITH_VMA "Use vma library for vulkan GPU memory allocation" NO)
option(BERSERK_WITH_GLSLANG "Use glslang for runtime glsl to spir-v compilation" NO)
option(BERSERK_WITH_WHEREAMI "Use WhereAmI to detect executable path" YES)
option(BERSERK_WITH_YAMC "Use Yamc as cross-platform locks provider" YES)
option(BERSERK_WITH_FREETYPE "Use freetype2 as font importing library" NO)
option(BERSERK_WITH_TINYOBJ "Tiny obj loader to import simple meshes" NO)
option(BERSERK_WITH_PFD "Use portable file dialogs lib for dialogs creation" YES)
option(BERSERK_WITH_STB_IMAGE "Use stb image utility for image processing" YES)

####################################################################
## Internal flags

set(BERSERK_DEFINES)
set(BERSERK_DEBUG NO)
set(BERSERK_RELEASE NO)
set(BERSERK_TARGET_MACOS NO)
set(BERSERK_TARGET_LINUX NO)
set(BERSERK_TARGET_WINDOWS NO)
set(BERSERK_ARCH)

####################################################################
## Define platform
##  - WINDOWS = Windows Desktop
##  - MACOSX  = MacOS X
##  - LINUX   = Linux

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(BERSERK_TARGET_WINDOWS YES)
#    set(BERSERK_WITH_GLFW YES)
#    set(BERSERK_WITH_GLEW YES)
#    set(BERSERK_WITH_OPENGL YES)
#    set(BERSERK_WITH_VULKAN YES)
    set(BERSERK_ARCH "x64")
    list(APPEND BERSERK_DEFINES BERSERK_TARGET_WINDOWS)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(BERSERK_TARGET_LINUX YES)
#    set(BERSERK_WITH_GLFW YES)
#    set(BERSERK_WITH_GLEW YES)
#    set(BERSERK_WITH_OPENGL YES)
#    set(BERSERK_WITH_VULKAN YES)
    set(BERSERK_ARCH "x64")
    list(APPEND BERSERK_DEFINES BERSERK_TARGET_LINUX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(BERSERK_TARGET_MACOS YES)
#    set(BERSERK_WITH_GLFW YES)
#    set(BERSERK_WITH_GLEW YES)
#    set(BERSERK_WITH_OPENGL YES)
#    set(BERSERK_WITH_VULKAN YES)
    if (NOT BERSERK_ARCH)
        if (NOT CMAKE_OSX_ARCHITECTURES)
            if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64")
                set(BERSERK_ARCH "arm64")
            else ()
                set(BERSERK_ARCH "x64")
            endif ()
        else ()
            if ("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "arm64")
                set(BERSERK_ARCH "arm64")
            else ()
                set(BERSERK_ARCH "x64")
            endif ()
        endif ()
    endif ()
    message(STATUS "Build macos binaries for ${BERSERK_ARCH} architecture")
    list(APPEND BERSERK_DEFINES BERSERK_TARGET_MACOS)
else()
    message(FATAL_ERROR "Unsupported target platform")
endif()

####################################################################
## Define Build type

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(BERSERK_DEBUG YES)
    list(APPEND BERSERK_DEFINES BERSERK_DEBUG)
    message(STATUS "Build berserk in debug mode (specified)")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(BERSERK_RELEASE YES)
    list(APPEND BERSERK_DEFINES BERSERK_RELEASE)
    message(STATUS "Build berserk in release mode (specified)")
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    set(BERSERK_DEBUG YES)
    set(BERSERK_RELEASE YES)
    list(APPEND BERSERK_DEFINES BERSERK_DEBUG)
    list(APPEND BERSERK_DEFINES BERSERK_RELEASE)
    message(STATUS "Build berserk in release mode with debug info (specified)")
else()
    set(BERSERK_RELEASE YES)
    list(APPEND BERSERK_DEFINES BERSERK_RELEASE)
    message(STATUS "Build berserk in release mode (default, not specified)")
endif()

##################################################################
## Compiler and language specifics

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (BERSERK_TARGET_WINDOWS)
    if (MSVC)
        list(APPEND BERSERK_DEFINES BERSERK_MSVC)
    else()
        message(FATAL_ERROR "Supported only MSVC compiler for Windows platform")
    endif()
endif()

##################################################################
## Target options and defs functions
## (use this functions to configure build targets)

function(berserk_target_compile_definitions target)
    foreach(DEFINITION ${BERSERK_DEFINES})
        target_compile_definitions(${target} PUBLIC ${DEFINITION})
    endforeach()

    if (BERSERK_TARGET_WINDOWS)
        target_compile_definitions(${target}
                PUBLIC WIN32
                PUBLIC _WIN32
                PUBLIC _WINDOWS
                PUBLIC UNICODE
                PUBLIC _UNICODE
                PUBLIC _CRT_SECURE_NO_WARNINGS
                PUBLIC _SCL_SECURE_NO_WARNINGS)
    endif()
endfunction()

function(berserk_target_compile_options target)
    if (MSVC)
        target_compile_options(${target}
                PUBLIC /source-charset:utf-8
                PUBLIC $<$<CONFIG:Debug>: /MDd>
                PUBLIC $<$<CONFIG:Release>: /MD>)
    endif()
endfunction()

function(berserk_target_link_options target)
    if (MSVC)
        target_link_options(${target}
                PRIVATE /INCREMENTAL:NO
                PRIVATE /NODEFAULTLIB:MSVCRT
                PRIVATE /machine:X64)
    endif()
endfunction()

##################################################################
## Dependencies setup

if (BERSERK_WITH_GLFW)
    message(STATUS "Add GLFW as window system API")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(deps/glfw)
endif()

if (BERSERK_WITH_GLEW)
    message(STATUS "Add GLEW as gl loader")
    set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(glew-cmake_BUILD_STATIC ON CACHE BOOL "" FORCE)
    set(USE_GLU OFF CACHE BOOL "" FORCE)
    set(PKG_CONFIG_REPRESENTATIVE_TARGET OFF CACHE BOOL "" FORCE)
    set(ONLY_LIBS ON CACHE BOOL "" FORCE)
    add_subdirectory(Deps/glew)
endif()

if (BERSERK_WITH_VULKAN)
    find_package(Vulkan REQUIRED)

    if (NOT Vulkan_FOUND)
        message(STATUS "Vulkan not found. Make sure that you export VULKAN_SDK environment path.")
        set(BERSERK_WITH_VULKAN NO)
        set(BERSERK_WITH_VMA NO)
        set(BERSERK_WITH_GLSLANG NO)
    else()
        message(STATUS "Add Vulkan SDK for rendering backend impl")
        set(BERSERK_WITH_VMA YES)
        set(BERSERK_WITH_GLSLANG YES)
        add_library(Vulkan INTERFACE)
        target_include_directories(Vulkan INTERFACE ${Vulkan_INCLUDE_DIRS})
        target_link_libraries(Vulkan INTERFACE ${Vulkan_LIBRARIES})
    endif()
endif()

if (BERSERK_WITH_VMA)
    message(STATUS "Add vma library for vulkan memory allocation")
    add_library(Vma INTERFACE)
    target_include_directories(Vma INTERFACE deps/vma/include)
endif()

if (BERSERK_WITH_GLSLANG)
    message(STATUS "Add glslang for runtime glsl to spir-v compilation")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(BUILD_EXTERNAL OFF CACHE BOOL "" FORCE)
    set(ENABLE_SPVREMAPPER OFF CACHE BOOL "" FORCE)
    set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE)
    set(ENABLE_GLSLANG_JS OFF CACHE BOOL "" FORCE)
    add_subdirectory(deps/glslang)
endif()

if (BERSERK_WITH_STB_IMAGE)
    message(STATUS "Add stb image utility for image processing")
    add_subdirectory(deps/stbimage)
endif()

if (BERSERK_WITH_WHEREAMI)
    message(STATUS "Add WhereAmI as executable path utility")
    add_subdirectory(deps/whereami)
endif()

if (BERSERK_WITH_YAMC)
    message(STATUS "Add yamc as c++ locks implementation")
    add_subdirectory(deps/yamc)
endif()

if (BERSERK_WITH_PFD)
    message(STATUS "Add pfd as native file dialog lib for dialogs creation")
    add_subdirectory(deps/pfd)
endif()

if (BERSERK_BUILD_TESTS)
    message(STATUS "Add GTest as unit test utility")
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(gtest_hide_internal_symbols ON CACHE BOOL "" FORCE)
    add_subdirectory(deps/googletest)
endif()

####################################################################
## Add engine modules here

# Engine modules
add_subdirectory(code)

# Tests for development
if (BERSERK_BUILD_TESTS)
    add_subdirectory(tests)
endif()

####################################################################
## Copy resources inside build directory

message(STATUS "Copy Engine resource into binary directory")
file(COPY engine DESTINATION ${CMAKE_BINARY_DIR}/)