cmake_minimum_required(VERSION 3.9)
project(BerserkRenderCore)

# Render Core files

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

        Public/Managers/ITextureManager.h
        Public/Managers/IBufferManager.h
        Public/Managers/IShaderManager.h
        Public/Managers/IMaterialManager.h
        Public/Managers/IDebugRenderManager.h

        Public/Importers/IModelImporter.h
        Public/Importers/IImageImporter.h
        Private/Helpers/ShaderManagerHelper.cpp
        Public/Helpers/ShaderManagerHelper.h

        Public/Base/IMaterial.h
        Public/Base/RenderBase.h
        Public/Base/IRenderSystem.h
        Public/Base/IRenderNode.h
        )

# Create static Render Core library

add_library(BerserkRenderCore STATIC ${SOURCE_LIB})