/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GLDevice.h>
#include <GLShader.h>
#include <GLDrawList.h>
#include <GLIndexBuffer.h>
#include <GLVertexBuffer.h>
#include <GLUniformBuffer.h>
#include <GLVertexDeclaration.h>

namespace Berserk {

    GLDevice::GLDevice() : RHIDevice() {
        auto result = gladLoadGL();
        BERSERK_COND_ERROR_FAIL(result != 0, "Failed to load OpenGL functions");
    }

    GLDevice::~GLDevice() {
        GLVertexDeclaration::clearCachedDeclarations();
    }

    TPtrShared<RHIVertexDeclaration> GLDevice::createVertexDeclaration(const RHIVertexDeclarationDesc &vertexDeclarationDesc) {
        return (TPtrShared<RHIVertexDeclaration>) GLVertexDeclaration::createDeclaration(vertexDeclarationDesc);
    }

    TPtrShared<RHIVertexBuffer> GLDevice::createVertexBuffer(uint32 size, EMemoryType type) {
        auto buffer = TPtrShared<GLVertexBuffer>::make();
        buffer->create(type, size);
        return (TPtrShared<RHIVertexBuffer>) buffer;
    }

    TPtrShared<RHIIndexBuffer> GLDevice::createIndexBuffer(uint32 size, EMemoryType type) {
        auto buffer = TPtrShared<GLIndexBuffer>::make();
        buffer->create(type, size);
        return (TPtrShared<RHIIndexBuffer>) buffer;
    }

    TPtrShared<RHIUniformBuffer> GLDevice::createUniformBuffer(uint32 size, EMemoryType type) {
        auto buffer = TPtrShared<GLUniformBuffer>::make();
        buffer->create(type, size);
        return (TPtrShared<RHIUniformBuffer>) buffer;
    }

    TPtrShared<RHIShader> GLDevice::createShader(EShaderLanguage language, const RHIShaderDesc &modules) {
        BERSERK_COND_ERROR_FAIL(language == EShaderLanguage::GLSL, "Unsupported shader language");

        auto shader = TPtrShared<GLShader>::make();
        shader->create(modules);
        return (TPtrShared<RHIShader>) shader;
    }

    TPtrShared<RHITexture> GLDevice::createTexture2D(bool useMipMaps, const Image &image) {
        return TPtrShared<RHITexture>();
    }

    TPtrShared<RHITexture>
    GLDevice::createTexture2D(uint32 width, uint32 height, EMemoryType memoryType, EPixelFormat format, bool useMipMaps) {
        return TPtrShared<RHITexture>();
    }

    TPtrShared<RHISampler> GLDevice::createSampler(const RHISamplerDesc &samplerDesc) {
        return TPtrShared<RHISampler>();
    }

    TPtrShared<RHIUniformSet> GLDevice::createUniformSet(const TArray<RHIUniformTextureDesc> &textures,
                                                         const TArray<RHIUniformBufferDesc> &buffers) {
        return TPtrShared<RHIUniformSet>();
    }

    TPtrShared<RHIFramebuffer> GLDevice::createFramebuffer() {
        return TPtrShared<RHIFramebuffer>();
    }

    TPtrShared<RHIDrawList> GLDevice::createDrawList() {
        return TPtrShared<RHIDrawList>();
    }

    void GLDevice::beginRenderFrame() {

    }

    void GLDevice::endRenderFrame() {

    }

    ERenderDeviceType GLDevice::getDeviceType() const {
        return ERenderDeviceType::OpenGL;
    }

    const TArray<EPixelFormat> &GLDevice::getSupportedTextureFormats() const {
        return mSupportedTextureFormats;
    }

    const TArray<EShaderLanguage> &GLDevice::getSupportedShaderLanguages() const {
        return mSupportedShaderLanguages;
    }

    void GLDevice::setWindowBindFunction(ISystem::WINDOW_ID window, const GLBindWindwoFunc &function) {
        mWindowBindFunctions.add(window, function);
    }

    GLBindWindwoFunc& GLDevice::getWindowBindFunction(ISystem::WINDOW_ID window) {
        return mWindowBindFunctions[window];
    }

    TArray<EPixelFormat> GLDevice::mSupportedTextureFormats;
    TArray<EShaderLanguage> GLDevice::mSupportedShaderLanguages;
    TMap<ISystem::WINDOW_ID, GLBindWindwoFunc, THashRaw<ISystem::WINDOW_ID>> GLDevice::mWindowBindFunctions;

}