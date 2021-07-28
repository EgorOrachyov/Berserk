cmake_minimum_required(VERSION 3.11)
project(BerserkPlatform LANGUAGES CXX)

set(BerserkPlatform_SOURCES)
set(BerserkPlatform_GLFW_SOURCES)
set(BerserkPlatform_UNIX_SOURCES)
set(BerserkPlatform_WINDOWS_SOURCES)

set(BerserkPlatform_SOURCES
        BerserkPlatform/PlatformEntryPoint.cpp
        BerserkPlatform/PlatformFile.cpp
        BerserkPlatform/PlatformFile.hpp
        BerserkPlatform/PlatformConsole.cpp
        BerserkPlatform/PlatformConsole.hpp
        BerserkPlatform/PlatformStringTable.hpp
        BerserkPlatform/PlatformThread.cpp
        BerserkPlatform/PlatformThread.hpp
        BerserkPlatform/PlatformThreadManager.cpp
        BerserkPlatform/PlatformThreadManager.hpp
        BerserkPlatform/PlatformDialogs.hpp
        BerserkPlatform/PlatformTaskManager.hpp)

if (BERSERK_WITH_GLFW)
set(BerserkPlatform_GLFW_SOURCES
        BerserkPlatform/Glfw/GlfwContext.cpp
        BerserkPlatform/Glfw/GlfwContext.hpp
        BerserkPlatform/Glfw/GlfwWindow.cpp
        BerserkPlatform/Glfw/GlfwWindow.hpp
        BerserkPlatform/Glfw/GlfwWindowManager.cpp
        BerserkPlatform/Glfw/GlfwWindowManager.hpp)
endif ()

if (BERSERK_TARGET_LINUX OR BERSERK_TARGET_MACOS)
set(BerserkPlatform_UNIX_SOURCES
        BerserkPlatform/Unix/UnixSystem.cpp
        BerserkPlatform/Unix/UnixSystem.hpp
        BerserkPlatform/Unix/UnixFileSystem.cpp
        BerserkPlatform/Unix/UnixFileSystem.hpp)
endif()

if (BERSERK_TARGET_WINDOWS)
set(BerserkPlatform_WINDOWS_SOURCES
        BerserkPlatform/Windows/WindowsFileSystem.cpp
        BerserkPlatform/Windows/WindowsFileSystem.hpp
        BerserkPlatform/Windows/WindowsSystem.cpp
        BerserkPlatform/Windows/WindowsSystem.hpp)
endif()

add_library(BerserkPlatform STATIC
        ${BerserkPlatform_SOURCES}
        ${BerserkPlatform_GLFW_SOURCES}
        ${BerserkPlatform_UNIX_SOURCES}
        ${BerserkPlatform_WINDOWS_SOURCES})

target_include_directories(BerserkPlatform PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BerserkPlatform PUBLIC BerserkCore)
target_link_libraries(BerserkPlatform PUBLIC glfw)
target_link_libraries(BerserkPlatform PRIVATE WhereAmI)

if (BERSERK_WITH_OPENGL)
target_link_libraries(BerserkPlatform PRIVATE BerserkOpenGL)
target_compile_definitions(BerserkPlatform PRIVATE BERSERK_WITH_OPENGL)
endif()

if (BERSERK_WITH_VULKAN)
target_link_libraries(BerserkPlatform PRIVATE BerserkVulkan)
target_compile_definitions(BerserkPlatform PRIVATE BERSERK_WITH_VULKAN)
endif()

target_compile_features(BerserkPlatform PUBLIC cxx_std_11)

set_target_properties(BerserkPlatform PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkPlatform PROPERTIES CXX_STANDARD_REQUIRED YES)