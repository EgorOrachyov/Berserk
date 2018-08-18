cmake_minimum_required(VERSION 3.9)
project(BerserkCore)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

set(SOURCE_LIB
        Source/ApplicationContext.cpp
        Context/ApplicationContext.h)

add_library(BerserkApplication STATIC ${SOURCE_LIB})