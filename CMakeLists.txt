cmake_minimum_required(VERSION 3.9)
project(Berserk)

set (TARGET Berserk)
set (CMAKE_CXX_STANDARD 11)

include ("${PROJECT_SOURCE_DIR}/Engine/Source/CMakeLists.txt")
include_directories("${PROJECT_SOURCE_DIR}/Test")

set (
    SOURCES
        Test/main.cpp
        Test/CoreSystem/ArrayTest.h
        Test/CoreSystem/LinkedListTest.h
        Test/Utils/Profiling.h
        Test/CoreSystem/FilesTest.h
        Test/CoreSystem/LoggingTest.h
        Test/CoreSystem/ResourcesTest.h)

add_executable (${TARGET} ${SOURCES})
target_link_libraries (Berserk ${BERSERK_LINK_COMMON} ${BERSERK_LINK_THIRD_PARTY})