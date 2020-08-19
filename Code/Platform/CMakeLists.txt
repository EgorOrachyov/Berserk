cmake_minimum_required(VERSION 3.11)
project(BerserkPlatform VERSION 1.0 LANGUAGES CXX)

file(GLOB_RECURSE GlfwSystem_SOURCES RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "GlfwSystem/*.cpp" "GlfwSystem/*.h")
file(GLOB_RECURSE Std_SOURCES RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "Std/*.cpp" "Std/*.h")
file(GLOB_RECURSE Unix_SOURCES RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "Unix/*.cpp" "Unix/*.h")
file(GLOB_RECURSE macOS_SOURCES RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "macOS/*.cpp" "macOS/*.h")

if(BERSERK_PLATFORM_MACOS)
    set(Target_SOURCES
        ${macOS_SOURCES}
        ${Std_SOURCES}
        ${Unix_SOURCES}
        ${GlfwSystem_SOURCES}
        )
endif()

add_library(BerserkPlatform SHARED ${Target_SOURCES})
target_include_directories(BerserkPlatform PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BerserkPlatform PUBLIC BerserkCore)
target_compile_features(BerserkPlatform PUBLIC cxx_std_11)

if (BERSERK_WITH_GLFW)
    target_link_libraries(BerserkPlatform PRIVATE glfw)
endif()

if (BERSERK_WITH_OPENGL)
    target_link_libraries(BerserkPlatform PRIVATE BerserkOpenGL)
endif()

if (BERSERK_WITH_WHEREAMI)
    target_link_libraries(BerserkPlatform PRIVATE WhereAmI)
endif()

set_target_properties(BerserkPlatform PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkPlatform PROPERTIES CXX_STANDARD_REQUIRED YES)
