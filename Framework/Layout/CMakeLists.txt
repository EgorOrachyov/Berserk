cmake_minimum_required(VERSION 3.9)
project(BerserkLayout)

# Core Subsystem files

set(SOURCE_LIB
        System/System.h
        System/RenderSystem.h
        Source/Cube.cpp
        Shapes/Cube.h
        Source/Plane.cpp
        Shapes/Plane.h
        Source/Sphere.cpp
        Shapes/Sphere.h
        Source/Torus.cpp
        Shapes/Torus.h)

# Create static Core Subsystem library

add_library(BerserkLayout STATIC ${SOURCE_LIB})