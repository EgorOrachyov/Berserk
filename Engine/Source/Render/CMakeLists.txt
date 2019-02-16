cmake_minimum_required(VERSION 3.9)
project(BerserkRender)

# Render Subsystem files

set(SOURCE_LIB

        # Platform submodule's files

        Public/Platform/IRenderDriver.h
        Public/Platform/ITexture.h
        Public/Platform/ISampler.h
        Public/Platform/IShader.h
        Public/Platform/IGPUBuffer.h
        Public/Platform/IUniformBuffer.h
        Public/Platform/IFrameBuffer.h
        Public/Platform/VertexTypes.h

        )

# Create static Render Subsystem library

add_library(BerserkRender STATIC ${SOURCE_LIB})