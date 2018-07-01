cmake_minimum_required(VERSION 3.9)
project(BerserkEngine)

# Compilation properties

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v")

# Engine Subsystmes' include directories

include_directories(Core)

# Debug testing files

set(SOURCES Debug/main.cpp Debug/ClassTesting.h)

add_executable(BerserkEngine ${SOURCES})

# Engine Subsystems

add_subdirectory(Core)

# Link full project

target_link_libraries(BerserkEngine BerserkCore)