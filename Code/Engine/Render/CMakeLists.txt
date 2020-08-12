cmake_minimum_required(VERSION 3.11)
project(BerserkRender VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
        Shader.h
        ShaderFactory.h
        ShaderFile.h
        ShaderProgram.h
        ShaderProgramCompiler.h
        ShaderProgramCache.h
        ShaderBindings.h
        ShaderManager.h
        ShaderIncludeProcessor.h
        BatchedElements.h
        BatchedElementsRenderer.h
        ConsoleRenderer.h
        ContextShaderDefs.h
        ContextUniformData.h
        GeometryGenerator.h
        Graphics.h
        GraphicsItems.h
        GraphicsRenderer.h
        GraphicsTextRenderer.h
        GraphicsTexturesRenderer.h
        GraphicsPrimitivesRenderer.h
        RenderModule.h
        RenderModuleFwd.h
        RenderUpdate.h
        ShaderDefsMacro.h
        TextureManager.h
        VertexStreams.h
        VertexDeclarationCache.h
        VertexDeclarationLoader.h
        FactoryRegistry.h
        GlobalShaderFactory.h
        RenderTargets/RenderTarget.h
        RenderTargets/WindowTarget.h
        RenderResources/RenderResource.h
        RenderResources/UniformBuffer.h
        RenderResources/UniformBufferWriter.h
        RenderResources/Sampler.h
        RenderResources/Texture.h
        RenderResources/Texture2D.h
        RenderResources/GpuFont.h
        RenderResources/GraphicsPipelineBuilder.h
        RenderResources/VertexDeclaration.h
        RenderResources/VertexDeclarationBuilder.h
        RenderResources/VertexArray.h
        RenderResources/VertexArrayBuilder.h
        RenderResources/VertexArrayData.h
        RenderResources/VertexArrayUpdate.h
        RenderResources/DynamicVertexBuffer.h
        RenderResources/DynamicIndexBuffer.h
        RenderResources/DynamicUniformBuffer.h
        RenderResources/TDynamicBuffer.h
        Scene/Camera.h
        Scene/Scene.h
        Scene/SceneView.h
        Scene/ViewData.h

        )

set(Target_SOURCES
        Shader.cpp
        ShaderFile.cpp
        ShaderProgram.cpp
        ShaderProgramCompiler.cpp
        ShaderProgramCache.cpp
        ShaderBindings.cpp
        ShaderManager.cpp
        ShaderIncludeProcessor.cpp
        BatchedElements.cpp
        BatchedElementsRenderer.cpp
        ConsoleRenderer.cpp
        ContextShaderDefs.cpp
        ContextUniformData.cpp
        GeometryGenerator.cpp
        Graphics.cpp
        GraphicsPen.h
        GraphicsRenderer.cpp
        GraphicsTextRenderer.cpp
        GraphicsTexturesRenderer.cpp
        GraphicsPrimitivesRenderer.cpp
        RenderModule.cpp
        RenderUpdate.cpp
        ShaderDefsMacro.cpp
        TextureManager.cpp
        VertexDeclarationCache.cpp
        FactoryRegistry.cpp
        GlobalShaderFactory.cpp
        RenderTargets/RenderTarget.cpp
        RenderTargets/WindowTarget.cpp
        RenderResources/RenderResource.cpp
        RenderResources/Texture2D.cpp
        RenderResources/UniformBuffer.cpp
        RenderResources/UniformBufferWriter.cpp
        RenderResources/GpuFont.cpp
        RenderResources/GraphicsPipelineBuilder.cpp
        RenderResources/VertexDeclaration.cpp
        RenderResources/VertexDeclarationBuilder.cpp
        RenderResources/VertexArray.cpp
        RenderResources/VertexArrayBuilder.cpp
        RenderResources/VertexArrayUpdate.cpp
        Scene/Camera.cpp
        Scene/Scene.cpp
        Scene/SceneView.cpp
        )

add_library(BerserkRender SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkRender PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkRender PUBLIC BerserkCore)
target_link_libraries(BerserkRender PUBLIC BerserkResources)
target_link_libraries(BerserkRender PUBLIC BerserkImporters)
target_compile_features(BerserkRender PUBLIC cxx_std_11)

set_target_properties(BerserkRender PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkRender PROPERTIES CXX_STANDARD_REQUIRED YES)