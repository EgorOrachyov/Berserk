cmake_minimum_required(VERSION 3.11)
project(BerserkRender VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
        BatchedElements.h
        BatchedElementsRenderer.h
        ContextShaderDefs.h
        ContextUniformData.h
        GeometryGenerator.h
        RenderModule.h
        ShaderDefsMacro.h
        VertexArrayData.h
        VertexDeclarationCache.h
        VertexStreams.h
        RenderTargets/RenderTarget.h
        RenderTargets/WindowTarget.h
        RenderResources/RenderResource.h
        RenderResources/UniformBuffer.h
        RenderResources/Sampler.h
        RenderResources/Texture.h
        RenderResources/Texture2D.h
        RenderResources/GraphicsPipeline.h
        RenderResources/GraphicsPipelineBuilder.h
        RenderResources/VertexDeclaration.h
        RenderResources/VertexDeclarationBuilder.h
        RenderResources/VertexArray.h
        RenderResources/VertexArrayBuilder.h
        RenderResources/VertexArrayUpdate.h
        ShaderCore/Shader.h
        ShaderCore/ShaderFile.h
        ShaderCore/ShaderProgram.h
        ShaderCore/ShaderProgramCompiler.h
        ShaderCore/ShaderProgramCache.h
        ShaderCore/ShaderUniformBindings.h
        )

set(Target_SOURCES
        BatchedElements.cpp
        BatchedElementsRenderer.cpp
        ContextShaderDefs.cpp
        ContextUniformData.cpp
        GeometryGenerator.cpp
        RenderModule.cpp
        ShaderDefsMacro.cpp
        VertexDeclarationCache.cpp
        RenderTargets/RenderTarget.cpp
        RenderTargets/WindowTarget.cpp
        RenderResources/RenderResource.cpp
        RenderResources/Texture2D.cpp
        RenderResources/UniformBuffer.cpp
        RenderResources/GraphicsPipeline.cpp
        RenderResources/GraphicsPipelineBuilder.cpp
        RenderResources/VertexDeclaration.cpp
        RenderResources/VertexDeclarationBuilder.cpp
        RenderResources/VertexArray.cpp
        RenderResources/VertexArrayBuilder.cpp
        RenderResources/VertexArrayUpdate.cpp
        ShaderCore/Shader.cpp
        ShaderCore/ShaderFile.cpp
        ShaderCore/ShaderProgram.cpp
        ShaderCore/ShaderProgramCompiler.cpp
        ShaderCore/ShaderProgramCache.cpp
        ShaderCore/ShaderUniformBindings.cpp
        )

add_library(BerserkRender SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkRender PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkRender PUBLIC BerserkCore)
target_compile_features(BerserkRender PUBLIC cxx_std_11)

set_target_properties(BerserkRender PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkRender PROPERTIES CXX_STANDARD_REQUIRED YES)