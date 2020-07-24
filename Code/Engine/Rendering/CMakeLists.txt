cmake_minimum_required(VERSION 3.11)
project(BerserkRendering VERSION 1.0 LANGUAGES CXX)

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

add_library(BerserkRendering SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkRendering PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkRendering PUBLIC BerserkCore)
target_compile_features(BerserkRendering PUBLIC cxx_std_11)

set_target_properties(BerserkRendering PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkRendering PROPERTIES CXX_STANDARD_REQUIRED YES)