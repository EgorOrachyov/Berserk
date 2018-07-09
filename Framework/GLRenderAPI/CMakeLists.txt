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
        Source/GLRenderAPI.cpp
        GLRenderAPI.h
        Source/GLGPUProgram.cpp
        GPUProgram/GLGPUProgram.h
        Source/GLWindowManager.cpp
        Context/GLWindowManager.h
        Source/GLTexture.cpp
        GLTexture.h
        Source/GLTextureManager.cpp
        GLTextureManager.h
        Debug/GLClassTesting.h
        Essential/GLInclude.h
        Source/GLUniformBufferObject.cpp
        Buffers/GLUniformBufferObject.h
        Essential/GLShaderType.h
        Essential/GLDataType.h
        Essential/GLNormalization.h
        Essential/GLPrimitiveMode.h
        Debug/Sphere.cpp
        Debug/Sphere.h Debug/Torus.cpp Debug/Torus.h Source/GLGPUBuffer.cpp Buffers/GLGPUBuffer.h)

# Create static Core Subsystem library

add_library(BerserkGLRenderAPI STATIC ${SOURCE_LIB})