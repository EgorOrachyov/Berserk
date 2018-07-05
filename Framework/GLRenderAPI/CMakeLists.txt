cmake_minimum_required(VERSION 3.9)
project(BerserkGLRenderAPI)

# Core Subsystem files

set(SOURCE_LIB
        Source/GLDataBufferPacker.cpp
        GLDataBufferPacker.h
        Source/GLVertexArrayObject.cpp
        GLVertexArrayObject.h
        Source/GLContext.cpp
        GLContext.h
        Source/GLWindow.cpp
        GLWindow.h
        Source/GLRenderAPI.cpp
        GLRenderAPI.h
        Source/GLGPUProgram.cpp
        GLGPUProgram.h
        Source/GLWindowManager.cpp
        GLWindowManager.h
        Source/GLTexture.cpp
        GLTexture.h
        Source/GLTextureManager.cpp
        GLTextureManager.h Debug/GLClassTesting.h GLInclude.h)

# Create static Core Subsystem library

add_library(BerserkGLRenderAPI STATIC ${SOURCE_LIB})