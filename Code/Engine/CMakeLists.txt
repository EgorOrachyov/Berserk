cmake_minimum_required(VERSION 3.11)
project(BerserkEngine VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
    Resources/Mesh.h
    Resources/Texture.h
    Resources/Font.h
    Rendering/ShaderDefsMacro.h
    Rendering/VertexPolicy.h
    Rendering/RenderModule.h
    Rendering/IRenderTarget.h
    Rendering/IRenderUpdate.h
    Rendering/RenderTargetScreen.h
    Rendering/RenderTargetProxy.h
    Rendering/RenderView.h
    Rendering/RenderCanvas.h
    Rendering/RenderModuleConfig.h
    Rendering/Aux/GeometryGenerator.h
    Rendering/Shaders/ShaderCache.h
    Rendering/Shaders/ShaderProfile.h
    Rendering/Shaders/ShaderManager.h
    Rendering/Resources/IRenderResource.h
    Rendering/Resources/Shader.h
    Rendering/Resources/IndexBuffer.h
    Rendering/Resources/VertexBuffer.h
    Rendering/Resources/IndexBufferUint32Static.h
    Rendering/Resources/UniformBuffer.h
    Rendering/Resources/VertexBufferCached.h
    Rendering/Resources/GraphicsPipeline.h

        Rendering/VertexPolicyBuilder.h)

set(Target_SOURCES
    Resources/Mesh.cpp
    Resources/Texture.cpp
    Resources/Font.cpp
    Rendering/ShaderDefsMacro.cpp
    Rendering/VertexPolicy.cpp
    Rendering/RenderModule.cpp
    Rendering/IRenderTarget.cpp
    Rendering/RenderTargetScreen.cpp
    Rendering/RenderTargetProxy.cpp
    Rendering/RenderView.cpp
    Rendering/RenderCanvas.cpp
    Rendering/RenderModuleConfig.cpp
    Rendering/Aux/GeometryGenerator.cpp
    Rendering/Shaders/ShaderCache.cpp
    Rendering/Shaders/ShaderProfile.cpp
    Rendering/Shaders/ShaderManager.cpp
    Rendering/Resources/IRenderResource.cpp
    Rendering/Resources/Shader.cpp
    Rendering/Resources/IndexBufferUint32Static.cpp
    Rendering/Resources/UniformBuffer.cpp
    Rendering/Resources/VertexBufferCached.cpp
    Rendering/Resources/GraphicsPipeline.cpp
        )


add_library(BerserkEngine SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkEngine PUBLIC BerserkCore)
target_compile_features(BerserkEngine PUBLIC cxx_std_11)

set_target_properties(BerserkEngine PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkEngine PROPERTIES CXX_STANDARD_REQUIRED YES)