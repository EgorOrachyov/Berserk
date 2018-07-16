cmake_minimum_required(VERSION 3.9)
project(BerserkGLRenderAPI)

# Core Subsystem files

set(SOURCE_LIB
        Source/GLDataBufferPacker.cpp
        Buffers/GLDataBufferPacker.h
        Source/GLContext.cpp
        Context/GLContext.h
        Source/GLWindow.cpp
        Context/GLWindow.h
        Source/GLRenderSystem.cpp
        GLRenderSystem.h
        Source/GLGPUProgram.cpp
        GPUProgram/GLGPUProgram.h
        Source/GLTexture.cpp
        Texture/GLTexture.h
        Source/GLTextureManager.cpp
        GLTextureManager.h
        Debug/GLClassTesting.h
        Essential/GLInclude.h
        Source/GLUniformBufferObject.cpp
        Buffers/GLUniformBufferObject.h
        GPUProgram/GLShaderType.h
        Essential/GLDataType.h
        GPUProgram/GLNormalization.h
        Essential/GLPrimitiveMode.h
        Source/GLGPUBuffer.cpp
        Buffers/GLGPUBuffer.h
        Source/GLFrameBufferObject.cpp
        Buffers/GLFrameBufferObject.h
        Texture/GLTextureWrapping.h
        Texture/GLTextureFiltering.h
        Texture/GLTextureMipmaps.h
        Source/GLSampler.cpp
        Texture/GLSampler.h
        Texture/GLTextureFormat.h
        Texture/GLTextureType.h Debug/GLSizeTesting.h)

# Create static Core Subsystem library

add_library(BerserkGLRenderAPI STATIC ${SOURCE_LIB})