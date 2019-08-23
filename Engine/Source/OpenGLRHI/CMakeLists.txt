cmake_minimum_required(VERSION 3.9)
project(BerserkOpenGLRHI)

# OpenGLRHI Subsystem files

set(
        SOURCES

        Private/GLDriver.cpp
        Private/GLTexture.cpp
        Private/GLSampler.cpp
        Private/GLStates.cpp
        Private/GLIndexedGeometryBuffer.cpp
        Private/GLUniformBuffer.cpp
        Private/GLShaderProgram.cpp
        Private/GLFrameBuffer.cpp

        Public/GLCommon.h
        Public/GLEnums.h
        Public/GLSizes.h
        Public/GLDriver.h
        Public/GLResources.h
        Public/GLShader.h
        Public/GLProgram.h
        Public/GLUniformBuffer.h
        Public/GLSampler.h
        Public/GLStates.h
        Public/GLTexture.h
        Public/GLFrameBuffer.h
        Public/GLBuffers.h
        Public/GLSupport.h
)

# Create static OpenGLRHI Subsystem library

add_library(BerserkOpenGLRHI STATIC ${SOURCES})