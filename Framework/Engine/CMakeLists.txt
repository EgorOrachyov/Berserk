cmake_minimum_required(VERSION 3.9)
project(BerserkEngine)

# Core Subsystem files

set(SOURCE_LIB
        System/System.h
        System/RenderSystem.h)

# Create static Core Subsystem library

add_library(BerserkEngine STATIC ${SOURCE_LIB})