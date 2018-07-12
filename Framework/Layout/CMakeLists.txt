cmake_minimum_required(VERSION 3.9)
project(BerserkLayout)

# Core Subsystem files

set(SOURCE_LIB
        Source/Some.cpp
        System/System.h
        System/RenderSystem.h)

# Create static Core Subsystem library

add_library(BerserkLayout STATIC ${SOURCE_LIB})