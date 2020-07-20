cmake_minimum_required(VERSION 3.11)
project(BerserkOpenGL VERSION 1.0 LANGUAGES CXX)

set(Target_SOURCES
        GLDevice.cpp
        GLDevice.h
        GLDefinitions.h
        GLErrors.h
        GLVertexBuffer.h
        GLIndexBuffer.h
        GLUniformBuffer.h
        GLShader.h
        GLDrawList.h
        GLVertexDeclaration.cpp
        GLVertexDeclaration.h
        GLArrayObject.h
        GLTexture.cpp
        GLTexture.h
        GLSampler.cpp
        GLSampler.h
        GLGraphicsPipeline.cpp
        GLGraphicsPipeline.h
        GLShaderMetaData.h
        GLUniformSet.h
        GLFramebuffer.h
        GLExtensions.cpp
        GLExtensions.h
        )

add_library(BerserkOpenGL SHARED ${Target_SOURCES})
target_include_directories(BerserkOpenGL PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BerserkOpenGL PUBLIC BerserkCore)
target_link_libraries(BerserkOpenGL PUBLIC OpenGL)
target_compile_features(BerserkOpenGL PUBLIC cxx_std_11)

set_target_properties(BerserkOpenGL PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkOpenGL PROPERTIES CXX_STANDARD_REQUIRED YES)