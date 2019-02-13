cmake_minimum_required(VERSION 3.9)
project(BerserkOpenGL)

# Render OpenGL files

set(SOURCE_LIB

        Private/GLRenderDriver.cpp
        Private/GLTexture.cpp
        Private/GLSampler.cpp
        Private/GLShader.cpp
        Private/GLGPUBuffer.cpp
        Private/GLUniformBuffer.cpp
        Private/GLFrameBuffer.cpp
        Private/GLDepthBuffer.cpp

        Public/GLRenderDriver.h
        Public/GLTexture.h
        Public/GLSampler.h
        Public/GLShader.h
        Public/GLGPUBuffer.h
        Public/GLUniformBuffer.h
        Public/GLFrameBuffer.h
        Public/GLDepthBuffer.h

        )

# Create static OpenGL Subsystem library

add_library(BerserkOpenGL STATIC ${SOURCE_LIB})