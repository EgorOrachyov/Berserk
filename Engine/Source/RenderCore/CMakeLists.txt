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

        # Image Imporer submodule's files

        Public/ImageImporter/IImageImporter.h

        # Manager submodule's files

        Public/Manager/ITextureManager.h
        Public/Manager/IBufferManager.h
        Public/Manager/IShaderManager.h
        Public/Manager/IMaterialManager.h

        # Material submodule's files

        Public/Material/IMaterial.h

        # Model Importer submodule's files

        Public/ModelImporter/IModelImporter.h

        # Model ManagersHelpers submodule's files

        Private/ManagerHelpers/ShaderManagerHelper.cpp
        Public/ManagerHelpers/ShaderManagerHelper.h

        )

# Create static Render Subsystem library

add_library(BerserkRenderSystem STATIC ${SOURCE_LIB})