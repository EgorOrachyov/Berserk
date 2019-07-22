cmake_minimum_required(VERSION 3.9)
project(BerserkOpenGLRHI)

# OpenGLRHI Subsystem files

set(
        SOURCES

        Private/GLDriver.cpp
        Public/GLDriver.h
        Public/GLResources.h
        Private/GLTexture.cpp
        Private/GLSampler.cpp Private/GLStates.cpp Private/GLIndexedGeometryBuffer.cpp Private/GLUniformBuffer.cpp Private/GLShaderProgram.cpp Public/GLCommon.h Public/GLEnums.h Private/GLFrameBuffer.cpp)

# Create static OpenGLRHI Subsystem library

add_library(BerserkOpenGLRHI STATIC ${SOURCES})