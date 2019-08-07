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

        Public/Application/IWindow.h
        Public/Application/IWindowManager.h
        Public/Application/IApplication.h

        Private/Importers/XMLShaderImporter.cpp
        Public/Importers/IShaderImporter.h
        Public/Importers/XMLShaderImporter.h
        Public/Importers/IImageImporter.h
        Public/Importers/IFontImporter.h

        Public/System/ISystem.h
        Public/System/ICommandQueue.h

        Private/Config/XMLConfigLoader.cpp
        Private/Config/ConfigTable.cpp
        Public/Config/IConfigTable.h
        Public/Config/ConfigTable.h
        Public/Config/XMLConfigLoader.h

        Private/Rendering/ShaderImportData.cpp
        Private/Rendering/Font.cpp
        Private/Rendering/Image.cpp
        Public/Rendering/VertexTypes.h
        Public/Rendering/RenderingCommon.h
        Public/Rendering/Definitions.h
        Public/Rendering/IRenderScene.h
        Public/Rendering/ImageData.h
        Public/Rendering/Font.h
        Public/Rendering/ShaderImportData.h
        Public/Rendering/ShaderInitializer.h
        Public/Rendering/Image.h
        Public/Rendering/RenderView.h

        Private/Object/Reflectable.cpp
        Private/Object/Object.cpp
        Public/Object/Allocatable.h
        Public/Object/Object.h
        Public/Object/Reflectable.h
        Public/Object/UUID.h
        Public/Object/ObjectID.h
        Public/Object/IObjectManager.h
        Public/Object/ObjectInitializer.h

        Private/Components/Component.cpp
        Private/Components/LightBase.cpp
        Private/Components/PointLightComponent.cpp
        Private/Components/SpotLightComponent.cpp
        Private/Components/DirectionalLightComponent.cpp
        Private/Components/CameraComponent.cpp
        Private/Components/CameraBase.cpp
        Public/Components/Component.h
        Public/Components/LightBase.h
        Public/Components/PointLightComponent.h
        Public/Components/SpotLightComponent.h
        Public/Components/DirectionalLightComponent.h
        Public/Components/CameraBase.h
        Public/Components/CameraComponent.h

        Public/Engine/IEnvironment.h
        Public/Engine/EngineCommon.h

        Private/Resource/ResourceManager.cpp
        Private/Resource/IResource.cpp
        Public/Resource/IResource.h
        Public/Resource/ResourceHandle.h
        Public/Resource/IResourceManager.h
        Public/Resource/ResourceManager.h

        Public/Rendering/RenderSceneObjects.h Public/Object/ObjectHandle.h Public/Rendering/Mesh.h Public/Rendering/MeshNode.h Private/Rendering/Material.cpp Public/Rendering/Material.h Private/Rendering/MeshFactory.cpp Public/Rendering/MeshFactory.h)

# Create static Engine Subsystem library

add_library(BerserkEngineSystem STATIC ${SOURCES})