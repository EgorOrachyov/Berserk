cmake_minimum_required(VERSION 3.9)
project(BerserkGLRenderAPI)

# Core Subsystem files

set(SOURCE_LIB
        Source/GLDataBufferPacker.cpp
        Buffers/GLDataBufferPacker.h
        Source/GLRenderSystem.cpp
        Render/GLRenderSystem.h
        Source/GLGPUProgram.cpp
        GPUProgram/GLGPUProgram.h
        Source/GLTexture.cpp
        Texture/GLTexture.h
        Source/GLTextureManager.cpp
        Managers/GLTextureManager.h
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
        Texture/GLTextureType.h
        Debug/GLSizeTesting.h
        Pipeline/GLRenderPipeline.h
        Source/GLFragmentLightning.cpp
        Pipeline/GLFragmentLightning.h
        Uniform/LightStructures.h
        Uniform/MaterialStructures.h
        Uniform/TransformationStructures.h
        Source/GLTonemapBloom.cpp
        Pipeline/GLTonemapBloom.h
        Uniform/PostProcessStructures.h
        Source/GLMaterial.cpp
        Render/GLMaterial.h
        Source/GLMaterialManager.cpp
        Managers/GLMaterialManager.h
        Source/GLRenderNode.cpp
        Render/GLRenderNode.h
        Source/GLSamplerManager.cpp
        Managers/GLSamplerManager.h
        Source/GLRenderMesh.cpp
        Render/GLRenderMesh.h
        Source/GLRenderMeshManager.cpp
        Managers/GLRenderMeshManager.h
        Source/GLScreenPlane.cpp
        Render/GLScreenPlane.h
        Source/GLToneMap.cpp
        Pipeline/GLToneMap.h
        Source/GLGaussianBloom.cpp
        Pipeline/GLGaussianBloom.h
        Source/GLLensOptics.cpp
        Pipeline/GLLensOptics.h
        Source/GLMoutionBlur.cpp
        Pipeline/GLMoutionBlur.h
        Source/GLDepthOfField.cpp
        Pipeline/GLDepthOfField.h
        Source/GLDeferredShading.cpp
        Pipeline/GLDeferredShading.h
        Uniform/UniformInfo.h
        Source/GLRenderDriver.cpp
        Render/GLRenderDriver.h
        Source/GLScreenRender.cpp
        Pipeline/GLScreenRender.h
        Source/GLPhongModel.cpp
        Pipeline/GLPhongModel.h)

# Create static Core Subsystem library

add_library(BerserkGLRenderAPI STATIC ${SOURCE_LIB})