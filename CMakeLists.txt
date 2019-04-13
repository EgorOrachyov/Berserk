cmake_minimum_required(VERSION 3.9)
project(Berserk)

# Required engine linking variables

set (TARGET Berserk)
set (CMAKE_CXX_STANDARD 11)

include ("${PROJECT_SOURCE_DIR}/Engine/Source/CMakeLists.txt")

# Additional compilation sources

set (
    SOURCES

        Engine/Debug/main.cpp
        Engine/Debug/CoreTest.h
        Engine/Debug/OpenGLDriverTest.h
        Engine/Debug/RenderingSystemTest.h
        Engine/Debug/EntitySystemTest.h
)

add_executable (${TARGET} ${SOURCES})
target_link_libraries (Berserk ${BERSERK_LINK_COMMON} ${BERSERK_LINK_THIRD_PARTY})