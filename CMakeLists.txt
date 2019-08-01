cmake_minimum_required(VERSION 3.9)
project(Berserk)

set (TARGET Berserk)
set (CMAKE_CXX_STANDARD 11)

include ("${PROJECT_SOURCE_DIR}/Engine/Source/CMakeLists.txt")
include_directories("${PROJECT_SOURCE_DIR}/Tests")

set (
        SOURCES

        Tests/main.cpp
        Tests/CoreSystem/ArrayTest.h
        Tests/CoreSystem/LinkedListTest.h
        Tests/Utils/Profiling.h
        Tests/CoreSystem/FilesTest.h
        Tests/CoreSystem/LoggingTest.h
        Tests/CoreSystem/ResourcesTest.h
        Tests/CoreSystem/MemoryTest.h
        Tests/CoreSystem/StringsTest.h
        Tests/CoreSystem/ContainersTest.h
        Tests/CoreSystem/ThreadingTest.h
        Tests/CoreSystem/ConsoleTest.h
        Tests/CoreSystem/MathTest.h
        Tests/RapidXMLTest/RapidXMLTest.h
        Tests/EngineSystem/GlfwWindowSystemTest.h
        Tests/OpenGL/GLRHITest.h

        Tests/EngineSystem/ObjectsTest.h)

add_executable (${TARGET} ${SOURCES})
target_link_libraries (Berserk ${BERSERK_LINK_COMMON} ${BERSERK_LINK_THIRD_PARTY})