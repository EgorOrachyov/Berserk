cmake_minimum_required(VERSION 3.9)
project(BerserkEngineSystem)

# Engine System files

set(
        SOURCES

        Public/RHI/RHIDriver.h
        Public/RHI/RHITexture.h
        Public/RHI/RHITexture2D.h
        Public/RHI/RHITextureCube.h
        Public/RHI/RHIUniformBuffer.h
        Public/RHI/RHIFrameBuffer.h
        Public/RHI/RHIVertexBuffer.h
        Public/RHI/RHIIndexBuffer.h
        Public/RHI/RHIShader.h
        Public/RHI/RHIResources.h
        Public/RHI/RHIResource.h
        Public/RHI/RHIGeometryBuffer.h
        Public/RHI/RHIShaderProgram.h
        Public/RHI/RHISampler.h
        Public/RHI/RHIDepthTestState.h
        Public/RHI/RHIBlendState.h
        Public/RHI/RHIStencilTestState.h
        Public/RHI/RHIFaceCullingState.h

        Public/Rendering/VertexTypes.h
        Public/Rendering/RenderingCommon.h
        Public/Rendering/Definitions.h

        Public/Application/IWindow.h
        Public/Application/IWindowManager.h
        Public/Application/IApplication.h

        Public/Importers/IShaderImporter.h
        Private/Importers/XMLShaderImporter.cpp
        Public/Importers/XMLShaderImporter.h
        Public/Importers/IImageImporter.h
        Public/Importers/IFontImporter.h

        Public/System/ISystem.h

        Public/Resource/IResource.h

        Public/Config/IConfigTable.h
        Private/Config/ConfigTable.cpp
        Public/Config/ConfigTable.h
        Private/Config/XMLConfigLoader.cpp
        Public/Config/XMLConfigLoader.h
        Public/System/ICommandQueue.h

        Public/Rendering/ImageData.h Private/Rendering/Font.cpp Public/Rendering/Font.h Public/Rendering/ShaderImportData.h Public/Object/Allocatable.h Private/Rendering/ShaderImportData.cpp Private/Object/Object.cpp Public/Object/Object.h Private/Components/Component.cpp Public/Components/Component.h Private/Components/LightBase.cpp Public/Components/LightBase.h Private/Components/PointLightComponent.cpp Public/Components/PointLightComponent.h Private/Components/SpotLightComponent.cpp Public/Components/SpotLightComponent.h Private/Components/DirectionalLightComponent.cpp Public/Components/DirectionalLightComponent.h Public/Object/Reflectable.h Public/Engine/IEnvironment.h)

# Create static Engine Subsystem library

add_library(BerserkEngineSystem STATIC ${SOURCES})