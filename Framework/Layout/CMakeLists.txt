cmake_minimum_required(VERSION 3.9)
project(BerserkLayout)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

set(SOURCE_LIB
        System/System.h
        Render/RenderSystem.h
        Source/Cube.cpp
        Shapes/Cube.h
        Source/Plane.cpp
        Shapes/Plane.h
        Source/Sphere.cpp
        Shapes/Sphere.h
        Source/Torus.cpp
        Shapes/Torus.h
        Source/Object.cpp
        Objects/Object.h
        Source/Actor.cpp
        Objects/Actor.h
        Debug/LayoutClassTesting.h
        Source/ObjectManager.cpp
        Managers/ObjectManager.h
        Source/Light.cpp
        Objects/Lights/Light.h
        Source/PointLight.cpp
        Objects/Lights/PointLight.h
        Source/DirectionalLight.cpp
        Objects/Lights/DirectionalLight.h
        Source/SpotLight.cpp
        Objects/Lights/SpotLight.h
        Source/Camera.cpp
        Objects/Cameras/Camera.h
        Source/SceneManager.cpp
        Managers/SceneManager.h
        Managers/Manager.h
        Components/CameraComponent.h
        Components/PointLightComponent.h
        Components/SpotLightComponent.h
        Components/DirectionalLightComponent.h
        Source/GPUBuffer.cpp
        Render/GPUBuffer.h
        Components/RenderComponent.h
        Components/BaseMaterialComponent.h
        Components/VertexComponents.h
        Source/Scene.cpp
        Objects/Scene/Scene.h
        Objects/Resource.h
        Source/ResourceManager.cpp
        Managers/ResourceManager.h
        Source/Resource.cpp
        Source/RenderSystem.cpp
        Render/PipelineStage.h
        Render/RenderNode.h
        Render/Material.h
        Render/Texture.h
        Render/TextureManager.h
        Render/MaterialManager.h
        Render/RenderResource.h
        Render/RenderMesh.h
        Render/RenderMeshManager.h
        Render/RenderDriver.h
        Source/RenderDriver.cpp
        Render/DepthMap.h
        Render/CubeDepthMap.h
        Components/ShadowCasterComponent.h
        Components/PointShadowCasterComponent.h
        Render/GBuffer.h
        Render/TextureImporter.h Render/SSAONoise.h Render/SSAOBuffer.h Source/GlobalLight.cpp Objects/Lights/GlobalLight.h Render/LightMap.h)

add_library(BerserkLayout STATIC ${SOURCE_LIB})