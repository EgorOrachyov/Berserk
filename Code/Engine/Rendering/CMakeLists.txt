cmake_minimum_required(VERSION 3.11)
project(BerserkRendering VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
        ShaderDefsMacro.h
        VertexPolicy.h
        RenderModule.h
        RenderTarget.h
        IRenderUpdate.h
        RenderTargetScreen.h
        RenderTargetProxy.h
        RenderView.h
        RenderModuleConfig.h
        Util/GeometryGenerator.h
        Resources/RenderResource.h
        Resources/Shader.h
        Resources/IndexBuffer.h
        Resources/VertexBuffer.h
        Resources/IndexBufferUint32Static.h
        Resources/UniformBuffer.h
        Resources/VertexBufferCached.h
        Resources/GraphicsPipeline.h
        Resources/Texture.h

        VertexPolicyBuilder.h

        ShaderCore/ShaderFile.h
        ShaderCore/ShaderProgram.h
        ShaderCore/ShaderProgramCompiler.h
        ShaderCore/ShaderProgramCache.h
        )

set(Target_SOURCES
        ShaderDefsMacro.cpp
        VertexPolicy.cpp
        RenderModule.cpp
        RenderTarget.cpp
        RenderTargetScreen.cpp
        RenderTargetProxy.cpp
        RenderView.cpp
        RenderModuleConfig.cpp
        Util/GeometryGenerator.cpp
        Resources/RenderResource.cpp
        Resources/Shader.cpp
        Resources/IndexBufferUint32Static.cpp
        Resources/UniformBuffer.cpp
        Resources/VertexBufferCached.cpp
        Resources/GraphicsPipeline.cpp

        ShaderCore/ShaderFile.cpp
        ShaderCore/ShaderProgram.cpp
        ShaderCore/ShaderProgramCompiler.cpp
        ShaderCore/ShaderProgramCache.cpp
        )

add_library(BerserkRendering SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkRendering PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkRendering PUBLIC BerserkCore)
target_compile_features(BerserkRendering PUBLIC cxx_std_11)

set_target_properties(BerserkRendering PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkRendering PROPERTIES CXX_STANDARD_REQUIRED YES)