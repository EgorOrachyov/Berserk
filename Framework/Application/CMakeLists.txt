cmake_minimum_required(VERSION 3.9)
project(BerserkCore)

# Core Subsystem files

set(SOURCE_LIB
        Source/ApplicationContext.cpp
        ApplicationContext.h)

# Create static Core Subsystem library

add_library(BerserkApplication STATIC ${SOURCE_LIB})