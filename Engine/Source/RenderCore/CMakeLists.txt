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

        # Managers submodule's files

        Public/Managers/ITextureManager.h
        Public/Managers/IBufferManager.h
        Public/Managers/IShaderManager.h
        Public/Managers/IMaterialManager.h

        # Imporetrs submodule's files

        Public/Importers/IModelImporter.h
        Public/Importers/IImageImporter.h

        # Helpers submodule's files

        Private/Helpers/MaterialManagerHelper.cpp
        Private/Helpers/ShaderManagerHelper.cpp
        Public/Helpers/MaterialManagerHelper.h
        Public/Helpers/ShaderManagerHelper.h
        Public/Helpers/ProfileHelpers.h

        # Foundation submodule's files

        Public/Foundation/IMaterial.h
        Public/Foundation/RenderBase.h
        Public/Foundation/IRenderSystem.h
        Public/Foundation/RenderPassInfo.h
        Public/Foundation/RenderInfo.h
        Public/Foundation/RenderSettings.h
        Public/Foundation/IPipelineStage.h

        Public/Foundation/IPipelineScheduler.h Private/Foundation/RenderBase.cpp Public/Foundation/IFont.h Public/Managers/IFontManager.h Public/Importers/IFontImporter.h)

# Create static Render Core library

add_library(BerserkRenderCore STATIC ${SOURCE_LIB})