cmake_minimum_required(VERSION 3.9)
project(BerserkOpenGLDriver)

# OpenGL driver files

set(SOURCE_LIB

        Private/Platform/GLRenderDriver.cpp
        Private/Platform/GLTexture.cpp
        Private/Platform/GLSampler.cpp
        Private/Platform/GLShader.cpp
        Private/Platform/GLGPUBuffer.cpp
        Private/Platform/GLDepthBuffer.cpp
        Private/Platform/GLUniformBuffer.cpp
        Private/Platform/GLFrameBuffer.cpp
        Private/Platform/GLWindow.cpp

        Public/Platform/GLRenderDriver.h
        Public/Platform/GLTexture.h
        Public/Platform/GLSampler.h
        Public/Platform/GLShader.h
        Public/Platform/GLGPUBuffer.h
        Public/Platform/GLDepthBuffer.h
        Public/Platform/GLUniformBuffer.h
        Public/Platform/GLFrameBuffer.h
        Public/Platform/GLWindow.h

        Public/Platform/GLInclude.h
        Public/Platform/GLProfile.h

        Private/Managers/GLBufferManager.cpp
        Private/Managers/GLShaderManager.cpp
        Private/Managers/GLTextureManager.cpp

        Public/Managers/GLBufferManager.h
        Public/Managers/GLShaderManager.h
        Public/Managers/GLTextureManager.h

        )

# Create static OpenGL Subsystem library

add_library(BerserkOpenGLDriver STATIC ${SOURCE_LIB})