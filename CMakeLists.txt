####################################################################
## BERSERK Engine project

cmake_minimum_required(VERSION 3.11)
project(Berserk VERSION 1.0)

####################################################################
## Build options

option(BERSERK_EDITOR "Build engine sources for use with editor" NO)
option(BERSERK_BUILD_TESTS "Build test folder with modules tests" YES)

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
option(BERSERK_WITH_TFD "Use tiny file dialog lib for dialogs creation" NO)
option(BERSERK_WITH_PFD "Use portable file dialogs lib for dialogs creation" YES)
option(BERSERK_WITH_STB_IMAGE "Use stb image utility for image processing" YES)

####################################################################
## Internal flags

set(BERSERK_DEFINES)
set(BERSERK_DEBUG NO)
set(BERSERK_RELEASE NO)
option(BERSERK_TARGET_MACOS NO)
option(BERSERK_TARGET_LINUX NO)
option(BERSERK_TARGET_WINDOWS NO)

####################################################################
## Define platform

if(APPLE)
    set(BERSERK_TARGET_MACOS YES)
    set(BERSERK_WITH_GLFW YES)
    set(BERSERK_WITH_GLEW YES)
    set(BERSERK_WITH_OPENGL YES)
    list(APPEND BERSERK_DEFINES BERSERK_TARGET_MACOS)
elseif(UNIX)
    set(BERSERK_TARGET_LINUX YES)
    set(BERSERK_WITH_GLFW YES)
    set(BERSERK_WITH_GLEW YES)
    set(BERSERK_WITH_OPENGL YES)
    set(BERSERK_WITH_VULKAN YES)
    list(APPEND BERSERK_DEFINES BERSERK_TARGET_LINUX)
else()
    message(FATAL_ERROR "Platform not specified")
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
else()
    set(BERSERK_RELEASE YES)
    list(APPEND BERSERK_DEFINES BERSERK_RELEASE)
    message(STATUS "Build berserk in release mode (default, not specified)")
endif()

##################################################################
## Third-party setup

if (BERSERK_WITH_GLFW)
    message(STATUS "Add GLFW as window system API")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(Code/BerserkThirdparty/glfw)
endif()

if (BERSERK_WITH_GLEW)
    message(STATUS "Add GLEW as gl loader")
    set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(glew-cmake_BUILD_STATIC ON CACHE BOOL "" FORCE)
    set(USE_GLU OFF CACHE BOOL "" FORCE)
    set(PKG_CONFIG_REPRESENTATIVE_TARGET OFF CACHE BOOL "" FORCE)
    set(ONLY_LIBS ON CACHE BOOL "" FORCE)
    add_subdirectory(Code/BerserkThirdparty/glew)
endif()

if (BERSERK_WITH_VULKAN)
    find_package(Vulkan REQUIRED FATAL_ERROR)

    if (NOT Vulkan_FOUND)
        message(STATUS "Vulkan not found")
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
    target_include_directories(Vma INTERFACE Code/BerserkThirdparty/vma/include)
endif()

if (BERSERK_WITH_GLSLANG)
    message(STATUS "Add glslang for runtime glsl to spir-v compilation")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(BUILD_EXTERNAL OFF CACHE BOOL "" FORCE)
    set(ENABLE_SPVREMAPPER OFF CACHE BOOL "" FORCE)
    set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE)
    set(ENABLE_GLSLANG_JS OFF CACHE BOOL "" FORCE)
    add_subdirectory(Code/BerserkThirdparty/glslang)
endif()

if (BERSERK_WITH_STB_IMAGE)
    message(STATUS "Add stb image utility for image processing")
    add_subdirectory(Code/BerserkThirdparty/stbimage)
endif()

if (BERSERK_WITH_WHEREAMI)
    message(STATUS "Add WhereAmI as executable path utility")
    add_subdirectory(Code/BerserkThirdparty/whereami)
endif()

if (BERSERK_WITH_YAMC)
    message(STATUS "Add yamc as c++ locks implementation")
    add_subdirectory(Code/BerserkThirdparty/yamc)
endif()

if (BERSERK_WITH_TFD)
    message(STATUS "Add tfd as native file dialog lib for dialogs creation")
    add_subdirectory(Code/BerserkThirdparty/tinyfiledialogs)
endif()

if (BERSERK_WITH_PFD)
    message(STATUS "Add pfd as native file dialog lib for dialogs creation")
    add_subdirectory(Code/BerserkThirdparty/portablefiledialogs)
endif()


#if (BERSERK_WITH_FREETYPE)
#    message(STATUS "Add freetype2 as font import tool")
#    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
#    set(SKIP_INSTALL_HEADERS ON CACHE BOOL "" FORCE)
#    set(SKIP_INSTALL_LIBRARIES ON CACHE BOOL "" FORCE)
#    set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
#    set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz ON CACHE BOOL "" FORCE)
#    add_subdirectory(Code/BerserkThirdparty/freetype2)
#endif()

#if (BERSERK_WITH_TINYOBJ)
#    message(STATUS "Add tinyobjloader to import mesh data")
#    add_subdirectory(Code/BerserkThirdparty/tinyobjloader)
#endif()

if (BERSERK_BUILD_TESTS)
    message(STATUS "Add GTest as unit test utility")
    add_subdirectory(Code/BerserkThirdparty/googletest)
endif()

####################################################################
## Add engine modules here

# Engine modules
add_subdirectory(Code)

# Tests for development
if (BERSERK_BUILD_TESTS)
    add_subdirectory(Tests)
endif()

####################################################################
## Copy resources/scripts inside build directory

message(STATUS "Copy Scrips into binary directory")
file(COPY Scripts DESTINATION ${CMAKE_BINARY_DIR}/)

message(STATUS "Copy Engine resource into binary directory")
file(COPY Engine DESTINATION ${CMAKE_BINARY_DIR}/)