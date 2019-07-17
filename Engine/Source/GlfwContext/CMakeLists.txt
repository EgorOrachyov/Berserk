cmake_minimum_required(VERSION 3.9)
project(BerserkGlfwContext)

# GLFW context Subsystem files

set(
        SOURCES

        Private/GlfwWindow.cpp
        Public/GlfwWindow.h
        Private/GlfwWindowManager.cpp
        Public/GlfwWindowManager.h

)

# Create static GLFW context Subsystem library

add_library(BerserkGlfwContext STATIC ${SOURCES})