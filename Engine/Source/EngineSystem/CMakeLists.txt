cmake_minimum_required(VERSION 3.9)
project(BerserkEngineSystem)

# Engine System files

set(
        SOURCES

        Public/RHI/RHISupport.h
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

        Private/Importers/XMLShaderDataImporter.cpp
        Private/Importers/TextureImporter.cpp
        Public/Importers/IShaderDataImporter.h
        Public/Importers/XMLShaderDataImporter.h
        Public/Importers/IPixelDataImporter.h
        Public/Importers/IFontImporter.h
        Public/Importers/ImportOptions.h
        Public/Importers/IResourceImporter.h
        Public/Importers/TextureImporter.h

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
        Private/Rendering/Material.cpp
        Private/Rendering/Mesh.cpp
        Private/Rendering/MeshFactory.cpp
        Private/Rendering/RenderQueue.cpp
        Private/Rendering/Texture2D.cpp
        Private/Rendering/FontSerializerXML.cpp
        Private/Rendering/GpuProgram.cpp
        Private/Rendering/GpuProgramData.cpp
        Private/Rendering/GpuProgramManager.cpp
        Public/Rendering/VertexTypes.h
        Public/Rendering/Definitions.h
        Public/Rendering/IRenderScene.h
        Public/Rendering/PixelData.h
        Public/Rendering/Font.h
        Public/Rendering/ShaderImportData.h
        Public/Rendering/ShaderInitializer.h
        Public/Rendering/Image.h
        Public/Rendering/RenderView.h
        Public/Rendering/Mesh.h
        Public/Rendering/MeshNode.h
        Public/Rendering/MeshFactory.h
        Public/Rendering/MeshUtils.h
        Public/Rendering/Renderable.h
        Public/Rendering/Material.h
        Public/Rendering/RenderQueue.h
        Public/Rendering/RenderElement.h
        Public/Rendering/FontImportData.h
        Public/Rendering/Texture.h
        Public/Rendering/Texture2D.h
        Public/Rendering/RenderSceneBase.h
        Public/Rendering/FontSerializer.h
        Public/Rendering/FontSerializerXML.h
        Public/Rendering/RenderLayer.h
        Public/Rendering/IRenderSystem.h
        Public/Rendering/RenderTarget.h
        Public/Rendering/TextureImportOptions.h
        Public/Rendering/GpuProgram.h
        Public/Rendering/GpuProgramData.h
        Public/Rendering/GpuProgramManager.h

        Private/Object/Reflectable.cpp
        Private/Object/Object.cpp
        Public/Object/Object.h
        Public/Object/Reflectable.h
        Public/Object/UUID.h
        Public/Object/ObjectID.h
        Public/Object/IObjectManager.h
        Public/Object/ObjectInitializer.h
        Public/Object/ObjectHandle.h

        Private/Components/Component.cpp
        Private/Components/LightComponent.cpp
        Private/Components/PointLightComponent.cpp
        Private/Components/SpotLightComponent.cpp
        Private/Components/DirectionalLightComponent.cpp
        Private/Components/CameraComponent.cpp
        Private/Components/RenderComponent.cpp
        Private/Components/MeshComponent.cpp
        Private/Components/StaticMeshComponent.cpp
        Public/Components/Component.h
        Public/Components/LightComponent.h
        Public/Components/PointLightComponent.h
        Public/Components/SpotLightComponent.h
        Public/Components/DirectionalLightComponent.h
        Public/Components/CameraComponent.h
        Public/Components/RenderComponent.h
        Public/Components/MeshComponent.h
        Public/Components/StaticMeshComponent.h

        Public/Engine/IEnvironment.h
        Public/Engine/EngineCommon.h
        Public/Engine/EngineResources.h
        Public/Engine/EngineObjects.h
        Public/Engine/EngineUtils.h

        Private/Resource/ResourceCache.cpp
        Private/Resource/Resource.cpp
        Private/Resource/ResourceManager.cpp
        Public/Resource/Resource.h
        Public/Resource/ResourceHandle.h
        Public/Resource/IResourceCache.h
        Public/Resource/ResourceCache.h
        Public/Resource/ResourceManager.h

        Public/Rendering/GpuProgramImportData.h)

# Create static Engine Subsystem library

add_library(BerserkEngineSystem STATIC ${SOURCES})