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
        Public/Platform/IDepthBuffer.h
        Public/Platform/IUniformBuffer.h
        Public/Platform/IFrameBuffer.h
        Public/Platform/IWindow.h
        Public/Platform/VertexTypes.h

        Public/Manager/ITextureManager.h
        Public/Manager/IBufferManager.h
        Public/Manager/IShaderManager.h
        Public/Manager/IMaterialManager.h
        Public/Manager/IDebugRenderManager.h

        Public/ModelImporter/IModelImporter.h
        Public/ImageImporter/IImageImporter.h
        Private/ManagerHelpers/ShaderManagerHelper.cpp
        Public/ManagerHelpers/ShaderManagerHelper.h

        Public/Base/IMaterial.h
        Public/Base/RenderBase.h
        Public/Base/IRenderSystem.h
        Public/Base/IRenderNode.h

        )

# Create static Render Subsystem library

add_library(BerserkRenderSystem STATIC ${SOURCE_LIB})