cmake_minimum_required(VERSION 3.11)
project(BerserkRender VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
        ContextUniformData.h
        ContextShaderDefs.h
        RenderModule.h
        ShaderDefsMacro.h
        RenderResources/RenderResource.h
        RenderResources/UniformBuffer.h
        RenderResources/Texture.h
        ShaderCore/Shader.h
        ShaderCore/ShaderFile.h
        ShaderCore/ShaderProgram.h
        ShaderCore/ShaderProgramCompiler.h
        ShaderCore/ShaderProgramCache.h
        ShaderCore/ShaderUniformBindings.h
        )

set(Target_SOURCES
        ContextUniformData.cpp
        ContextShaderDefs.cpp
        RenderModule.cpp
        ShaderDefsMacro.cpp
        RenderResources/RenderResource.cpp
        RenderResources/UniformBuffer.cpp
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