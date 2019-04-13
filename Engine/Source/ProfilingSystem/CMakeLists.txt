cmake_minimum_required(VERSION 3.9)
project(BerserkProfilingSystem)

# Profiling System files

set(SOURCE_LIB

        Private/MemorySizer.cpp
        Public/MemorySizer.h

        )

# Create static Profiling Subsystem library

add_library(BerserkProfilingSystem STATIC ${SOURCE_LIB})