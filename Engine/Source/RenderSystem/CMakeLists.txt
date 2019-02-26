cmake_minimum_required(VERSION 3.9)
project(BerserkRenderSystem)

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
        Public/Platform/IWindow.h

        # Queue submodule's files

        Private/Queue/RenderQueue.cpp
        Public/Queue/RenderQueue.h

        # Image Imporer submodule's files

        Public/ImageImporter/ImageImporter.h

        # System submodule's files

        Private/System/RenderSystem.cpp
        Public/System/RenderSystem.h

        Public/Manager/ITextureManager.h Public/Manager/IBufferManager.h Public/Manager/IShaderManager.h Public/Manager/IMaterialManager.h)

# Create static Render Subsystem library

add_library(BerserkRenderSystem STATIC ${SOURCE_LIB})