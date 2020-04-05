/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GLDevice.h>
#include <GLShader.h>
#include <GLSampler.h>
#include <GLTexture.h>
#include <GLDrawList.h>
#include <GLArrayObject.h>
#include <GLIndexBuffer.h>
#include <GLVertexBuffer.h>
#include <GLUniformBuffer.h>
#include <GLGraphicsPipeline.h>
#include <GLVertexDeclaration.h>
#include <GLShaderIntrospection.h>

namespace Berserk {

    GLDevice::GLDevice() : RHIDevice() {
        auto result = gladLoadGL();
        BERSERK_COND_ERROR_FAIL(result != 0, "Failed to load OpenGL functions");

        GLTexture::createDefaultTextures();
    }

    GLDevice::~GLDevice() {
        GLSampler::clearCachedSamplers();
        GLTexture::releaseDefaultTextures();
        GLVertexDeclaration::clearCachedDeclarations();
        GLGraphicsPipeline::clearCachedGraphicsPipelines();
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

    TPtrShared <RHIArrayObject>
    GLDevice::createArrayObject(const TArrayStatic <TPtrShared<RHIVertexBuffer>> &vertexData,
                                const TPtrShared <RHIIndexBuffer> &indexData,
                                const TPtrShared <RHIVertexDeclaration> &declaration) {
        auto object = TPtrShared<GLArrayObject>::make();
        object->create(vertexData, indexData, declaration);
        return (TPtrShared<RHIArrayObject>) object;
    }

    TPtrShared<RHIShader> GLDevice::createShader(EShaderLanguage language, const RHIShaderDesc &modules) {
        BERSERK_COND_ERROR_FAIL(language == EShaderLanguage::GLSL, "Unsupported shader language");

        auto shader = TPtrShared<GLShader>::make();
        shader->create(modules);
        return (TPtrShared<RHIShader>) shader;
    }

    TPtrShared<RHIShaderIntrospection> GLDevice::createShaderIntrospection(const TPtrShared<RHIShader> &shader) {
        auto introspection = TPtrShared<GLShaderIntrospection>::make();
        introspection->create(shader);
        return (TPtrShared<RHIShaderIntrospection>) introspection;
    }

    TPtrShared<RHITexture> GLDevice::createTexture2D(bool useMipMaps, const Image &image) {
        auto texture = TPtrShared<GLTexture>::make();
        texture->create(useMipMaps, image);
        return (TPtrShared<RHITexture>) texture;
    }

    TPtrShared<RHITexture> GLDevice::createTexture2D(uint32 width, uint32 height, EMemoryType memoryType, EPixelFormat format, bool useMipMaps) {
        auto texture = TPtrShared<GLTexture>::make();
        texture->create(width, height, memoryType, format, useMipMaps);
        return (TPtrShared<RHITexture>) texture;
    }

    TPtrShared<RHISampler> GLDevice::createSampler(const RHISamplerDesc &samplerDesc) {
        return (TPtrShared<RHISampler>) GLSampler::createSampler(samplerDesc);
    }

    TPtrShared<RHIUniformSet> GLDevice::createUniformSet(const TArray<RHIUniformTextureDesc> &textures,
                                                         const TArray<RHIUniformBufferDesc> &buffers) {
        return TPtrShared<RHIUniformSet>();
    }

    TPtrShared<RHIFramebuffer> GLDevice::createFramebuffer() {
        return TPtrShared<RHIFramebuffer>();
    }

    TPtrShared<RHIGraphicsPipeline> GLDevice::createGraphicsPipeline(const RHIGraphicsPipelineDesc &pipelineDesc) {
        return (TPtrShared<RHIGraphicsPipeline>) GLGraphicsPipeline::createPipeline(pipelineDesc);
    }

    TPtrShared<RHIDrawList> GLDevice::createDrawList() {
        auto list = TPtrShared<GLDrawList>::make();
        list->create();
        return (TPtrShared<RHIDrawList>) list;
    }

    const TPtrShared<RHITexture> &GLDevice::getWhiteTexture() {
        return GLTexture::getDefaultWhiteTexture();
    }

    const TPtrShared<RHITexture> &GLDevice::getBlackTexture() {
        return GLTexture::getDefaultBlackTexture();
    }

    void GLDevice::beginRenderFrame() {

    }

    void GLDevice::submitDrawList(const TPtrShared<RHIDrawList> &drawList) {
        BERSERK_COND_ERROR_FAIL(drawList->getDrawListState() == EDrawListState::Complete, "Invalid draw list state");

        auto list = (const GLDrawList*) drawList.getPtr();

        auto& cmd = list->getCmdDescriptions();
        auto& cmdBindSurface = list->getCmdBindSurface();
        auto& cmdBindPipeline = list->getCmdBindGraphicsPipeline();
        auto& cmdBindArrayObject = list->getCmdBindArrayObject();
        auto& cmdDrawIndexed = list->getCmdDrawIndexed();
        auto& wndBindFunction = getWindowBindFunction();

        /** Will be set from pipeline */
        GLenum GL_primitiveType = 0;

        for (const auto& c: cmd) {
            switch (c.type) {

                case ECommandType::BindSurface: {
                    auto& desc = cmdBindSurface[c.index];
                    auto& view = desc.viewport;
                    auto& color = desc.clearColor;

                    getWindowBindFunction()(desc.window);
                    glViewport(view.getX(), view.getY(), view.getW(), view.getH());
                    glClearColor(color.getR(), color.getG(), color.getB(), color.getA());
                    glClear(GL_COLOR_BUFFER_BIT);

                    BERSERK_CATCH_OPENGL_ERRORS();

                }
                break;

                case ECommandType::BindPipeline: {
                    auto& desc = cmdBindPipeline[c.index];
                    auto& GL_pipeline = (GLGraphicsPipeline&) *desc.pipeline;

                    GL_pipeline.bind();
                    GL_primitiveType = GL_pipeline.getPipelineState().primitivesType;

                    BERSERK_CATCH_OPENGL_ERRORS();
                }
                break;

                case ECommandType::BindArrayObject: {
                    auto& desc = cmdBindArrayObject[c.index];
                    auto& GL_arrayObject = (GLArrayObject&) *desc.arrayObject;

                    glBindVertexArray(GL_arrayObject.getObjectHandle());

                    BERSERK_CATCH_OPENGL_ERRORS();
                }
                break;

                case ECommandType::DrawIndexed: {
                    auto& desc = cmdDrawIndexed[c.index];
                    auto indexCount = desc.indexCount;
                    auto baseOffset = desc.baseOffset;
                    auto instanceCount = desc.instancesCount;
                    auto GL_indexType = GLDefinitions::getIndexType(desc.indexType);

                    glDrawElementsInstancedBaseVertex(GL_primitiveType, indexCount, GL_indexType, nullptr, instanceCount, baseOffset);

                    BERSERK_CATCH_OPENGL_ERRORS();
                }
                break;

                default:
                    BERSERK_ERROR_FAIL("Unsupported draw list command");
            }
        }
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

    void GLDevice::setWindowBindFunction(const GLBindWindwoFunc &function) {
        mWindowBindFunction = function;
    }

    GLBindWindwoFunc & GLDevice::getWindowBindFunction() {
        return mWindowBindFunction;
    }

    GLBindWindwoFunc GLDevice::mWindowBindFunction;
    TArray<EPixelFormat> GLDevice::mSupportedTextureFormats;
    TArray<EShaderLanguage> GLDevice::mSupportedShaderLanguages;

}