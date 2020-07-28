/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <LogMacro.h>
#include <GLDevice.h>
#include <GLExtensions.h>
#include <GLShader.h>
#include <GLSampler.h>
#include <GLTexture.h>
#include <GLDrawList.h>
#include <GLUniformSet.h>
#include <GLArrayObject.h>
#include <GLIndexBuffer.h>
#include <GLFramebuffer.h>
#include <GLVertexBuffer.h>
#include <GLUniformBuffer.h>
#include <GLGraphicsPipeline.h>
#include <GLVertexDeclaration.h>
#include <GLShaderMetaData.h>

#define ABORT_ON_GPU_ERROR(result,message) checkToAbort(result,message);

namespace Berserk {

    GLDevice::GLDevice() : RHIDevice() {
        auto result = gladLoadGL();
        BERSERK_COND_ERROR_FAIL(result != 0, "Failed to load OpenGL functions");
        BERSERK_LOG_INFO("Initialize OpenGL Rendering Device");

        GLExtensions::init();
        GLTexture::createDefaultTextures();

        mSupportedShaderLanguages = { EShaderLanguage::GLSL };
    }


    GLDevice::~GLDevice() {
        GLSampler::clearCachedSamplers();
        GLTexture::releaseDefaultTextures();
        GLVertexDeclaration::clearCachedDeclarations();

        BERSERK_LOG_INFO("Finalize OpenGL Rendering Device");
    }

    TPtrShared<RHIVertexDeclaration> GLDevice::createVertexDeclaration(const RHIVertexDeclarationDesc &vertexDeclarationDesc) {
        return (TPtrShared<RHIVertexDeclaration>) GLVertexDeclaration::createDeclaration(vertexDeclarationDesc);
    }

    TPtrShared <RHIVertexBuffer> GLDevice::createVertexBuffer(uint32 size, EBufferUsage type, const void *data) {
        auto buffer = TPtrShared<GLVertexBuffer>::make();
        auto result = buffer->create(type, size, data);
        ABORT_ON_GPU_ERROR(result,"Failed to create vertex buffer");
        return result ? (TPtrShared<RHIVertexBuffer>) buffer : nullptr;
    }

    TPtrShared <RHIIndexBuffer> GLDevice::createIndexBuffer(uint32 size, EBufferUsage type, const void *data) {
        auto buffer = TPtrShared<GLIndexBuffer>::make();
        auto result = buffer->create(type, size, data);
        ABORT_ON_GPU_ERROR(result,"Failed to create index buffer");
        return result ? (TPtrShared<RHIIndexBuffer>) buffer : nullptr;
    }

    TPtrShared<RHIUniformBuffer> GLDevice::createUniformBuffer(uint32 size, EBufferUsage type, const void *data) {
        auto buffer = TPtrShared<GLUniformBuffer>::make();
        auto result = buffer->create(type, size, data);
        ABORT_ON_GPU_ERROR(result,"Failed to create uniform buffer");
        return result ? (TPtrShared<RHIUniformBuffer>) buffer : nullptr;
    }

    TPtrShared <RHIArrayObject> GLDevice::createArrayObject(const TArrayStatic <TPtrShared<RHIVertexBuffer>> &vertexData, const TPtrShared <RHIIndexBuffer> &indexData, const TPtrShared <RHIVertexDeclaration> &declaration, EPrimitivesType primitivesType) {
        auto object = TPtrShared<GLArrayObject>::make();
        auto result = object->create(vertexData, indexData, declaration, primitivesType);
        ABORT_ON_GPU_ERROR(result,"Failed to create array object");
        return result ? (TPtrShared<RHIArrayObject>) object : nullptr;
    }

    TPtrShared<RHIShader> GLDevice::createShader(EShaderLanguage language, const RHIShaderDesc &modules) {
        BERSERK_COND_ERROR_RET_VALUE(nullptr, language == EShaderLanguage::GLSL, "Unsupported shader language");

        auto shader = TPtrShared<GLShader>::make();
        auto result = shader->create(modules);
        ABORT_ON_GPU_ERROR(result,"Failed to create shader");
        return result ? (TPtrShared<RHIShader>) shader : nullptr;
    }

    TPtrShared<RHIShader> GLDevice::createShader(EShaderLanguage language, const RHIShaderViewDesc &modules) {
        BERSERK_COND_ERROR_RET_VALUE(nullptr, language == EShaderLanguage::GLSL, "Unsupported shader language");

        auto shader = TPtrShared<GLShader>::make();
        auto result = shader->create(modules);
        ABORT_ON_GPU_ERROR(result,"Failed to create shader");
        return result ? (TPtrShared<RHIShader>) shader : nullptr;
    }
    
    TPtrShared<RHIShaderMetaData> GLDevice::createShaderMeta(const TPtrShared<RHIShader> &shader) {
        auto introspection = TPtrShared<GLShaderMetaData>::make();
        auto result = introspection->create(shader);
        ABORT_ON_GPU_ERROR(result,"Failed to create shader introspection info");
        return result ? (TPtrShared<RHIShaderMetaData>) introspection : nullptr;
    }

    TPtrShared<RHIShaderMetaData> GLDevice::createShaderMeta() {
        auto introspection = TPtrShared<GLShaderMetaData>::make();
        return (TPtrShared<RHIShaderMetaData>) introspection;
    }

    TPtrShared <RHITexture> GLDevice::createTexture2D(EBufferUsage memoryType, bool useMipMaps, const Image &image) {
        auto texture = TPtrShared<GLTexture>::make();
        auto result = texture->create2d(memoryType, useMipMaps, image);
        ABORT_ON_GPU_ERROR(result,"Failed to create texture2D");
        return result ? (TPtrShared<RHITexture>) texture : nullptr;
    }

    TPtrShared<RHITexture> GLDevice::createTexture2D(uint32 width, uint32 height, EBufferUsage memoryType, EPixelFormat format, bool useMipMaps) {
        auto texture = TPtrShared<GLTexture>::make();
        auto result = texture->create2d(width, height, memoryType, format, useMipMaps);
        ABORT_ON_GPU_ERROR(result,"Failed to create texture2D");
        return result ? (TPtrShared<RHITexture>) texture : nullptr;
    }

    TPtrShared<RHISampler> GLDevice::createSampler(const RHISamplerDesc &samplerDesc) {
        return (TPtrShared<RHISampler>) GLSampler::createSampler(samplerDesc);
    }

    TPtrShared<RHIUniformSet> GLDevice::createUniformSet(const TArray<RHIUniformTextureDesc> &textures, const TArray<RHIUniformBlockDesc> &uniformBlocks) {
        auto set = TPtrShared<GLUniformSet>::make();
        auto result = set->create(textures, uniformBlocks);
        ABORT_ON_GPU_ERROR(result,"Failed to create uniform set");
        return result ? (TPtrShared<RHIUniformSet>) set : nullptr;
    }

    TPtrShared<RHIFramebuffer> GLDevice::createFramebuffer(const TArrayStatic<TPtrShared<RHITexture>,  RHIConst::MAX_COLOR_ATTACHMENTS> &colors, const TPtrShared<RHITexture> &depthStencil) {
        auto framebuffer = TPtrShared<GLFramebuffer>::make();
        auto result = framebuffer->create(colors, depthStencil);
        ABORT_ON_GPU_ERROR(result,"Failed to create framebuffer");
        return result ? (TPtrShared<RHIFramebuffer>) framebuffer : nullptr;
    }

    TPtrShared<RHIDrawList> GLDevice::createDrawList() {
        auto list = TPtrShared<GLDrawList>::make();
        auto result = list->create();
        ABORT_ON_GPU_ERROR(result,"Failed to create draw list");
        return result ? (TPtrShared<RHIDrawList>) list : nullptr;
    }

    const TPtrShared<RHITexture> &GLDevice::getWhiteTexture() {
        return GLTexture::getDefaultWhiteTexture();
    }

    const TPtrShared<RHITexture> &GLDevice::getBlackTexture() {
        return GLTexture::getDefaultBlackTexture();
    }

    void GLDevice::beginRenderFrame() {
        // For OpenGL empty
    }

    void GLDevice::submitDrawList(const TPtrShared<RHIDrawList> &drawList) {
        BERSERK_COND_ERROR_RET(drawList->getDrawListState() == EDrawListState::Complete, "Invalid draw list state");

        auto list = (const GLDrawList*) drawList.getPtr();

        auto& cmd = list->getCmdDescriptions();
        auto& cmdBindSurface = list->getCmdBindSurface();
        auto& cmdBindFramebuffer = list->getCmdBindFramebuffer();
        auto& cmdBindPipeline = list->getCmdBindGraphicsPipeline();
        auto& cmdBindArrayObject = list->getCmdBindArrayObject();
        auto& cmdBindUniformSet = list->getCmdBindUniformSet();
        auto& cmdDrawIndexed = list->getCmdDrawIndexed();
        auto& cmdDraw = list->getCmdDraw();

        /** Will be set from pipeline */
        GLenum GL_primitiveType = 0;
        GLShader* GL_shader = nullptr;

        bool GL_surfaceBound = false;
        bool GL_pipelineBound = false;
        bool GL_arrayBound = false;

        for (const auto& c: cmd) {
            switch (c.type) {

                case ECommandType::BindSurface: {
                    GL_surfaceBound = true;
                    GL_pipelineBound = false;

                    auto& desc = cmdBindSurface[c.index];
                    auto& view = desc.viewport;
                    auto& color = desc.clearColor;
                    auto depth = desc.clearDepth;
                    auto stencil = desc.clearStencil;

                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    desc.window->makeRenderContextCurrent();
                    glViewport(view.getX(), view.getY(), view.getW(), view.getH());

                    GLbitfield mask = 0;

                    if (desc.clearOptions.getFlag(EClearOption::Color)) {
                        mask |= GL_COLOR_BUFFER_BIT;
                        glDisable(GL_STENCIL_TEST);
                        glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                        glClearColor(color.getR(), color.getG(), color.getB(), color.getA());
                    }
                    if (desc.clearOptions.getFlag(EClearOption::Depth)) {
                        mask |= GL_DEPTH_BUFFER_BIT;
                        glDepthMask(GL_TRUE);
                        glClearDepthf(depth);
                    }
                    if (desc.clearOptions.getFlag(EClearOption::Stencil)) {
                        mask |= GL_STENCIL_BUFFER_BIT;
                        glStencilMask(0xffffffff);
                        glClearStencil(stencil);
                    }

                    glClear(mask);

                    BERSERK_CATCH_OPENGL_ERRORS();
                }
                break;

                case ECommandType::BindFramebuffer: {
                    GL_surfaceBound = true;
                    GL_pipelineBound = false;

                    auto& desc = cmdBindFramebuffer[c.index];
                    auto& GL_framebuffer = (GLFramebuffer&) *desc.framebuffer;
                    auto& view = desc.viewport;

                    glBindFramebuffer(GL_FRAMEBUFFER, GL_framebuffer.getHandle());
                    glViewport(view.getX(), view.getY(), view.getW(), view.getH());

                    GL_framebuffer.clear();

                    BERSERK_CATCH_OPENGL_ERRORS();
                }

                case ECommandType::BindPipeline: {
                    if (!GL_surfaceBound)
                        continue;

                    auto& desc = cmdBindPipeline[c.index];
                    GLGraphicsPipeline GL_pipeline(desc.pipeline);

                    GL_pipelineBound = true;
                    GL_shader = &GL_pipeline.getShader();
                    GL_pipeline.bind();

                    BERSERK_CATCH_OPENGL_ERRORS();
                }
                break;

                case ECommandType::BindArrayObject: {
                    if (!GL_pipelineBound || !GL_surfaceBound)
                        continue;

                    GL_arrayBound = true;

                    auto& desc = cmdBindArrayObject[c.index];
                    auto& GL_arrayObject = (GLArrayObject&) *desc.arrayObject;
                    GL_primitiveType = GLDefinitions::getPrimitivesType(desc.arrayObject->getPrimitivesType());

                    glBindVertexArray(GL_arrayObject.getObjectHandle());

                    BERSERK_CATCH_OPENGL_ERRORS();
                }
                break;

                case ECommandType::BindUniformSet: {
                    if (!GL_pipelineBound || !GL_surfaceBound)
                        continue;

                    auto& desc = cmdBindUniformSet[c.index];
                    auto& GL_uniformSet = (GLUniformSet&) *desc.uniformSet;

                    GL_uniformSet.bind(*GL_shader);

                    BERSERK_CATCH_OPENGL_ERRORS();
                }
                    break;

                case ECommandType::DrawIndexed: {
                    if (!GL_pipelineBound || !GL_surfaceBound || !GL_arrayBound)
                        continue;

                    auto& desc = cmdDrawIndexed[c.index];
                    auto indexCount = desc.indexCount;
                    auto baseOffset = desc.baseOffset;
                    auto instanceCount = desc.instancesCount;
                    auto GL_indexType = GLDefinitions::getIndexType(desc.indexType);

                    glDrawElementsInstancedBaseVertex(GL_primitiveType, indexCount, GL_indexType, nullptr, instanceCount, baseOffset);

                    BERSERK_CATCH_OPENGL_ERRORS();
                }
                break;

                case ECommandType::Draw: {
                    if (!GL_pipelineBound || !GL_surfaceBound || !GL_arrayBound)
                        continue;

                    auto& desc = cmdDraw[c.index];
                    auto verticesCount = desc.verticesCount;
                    auto instancesCount = desc.instancesCount;
                    auto baseOffset = desc.baseOffset;

                    glDrawArraysInstanced(GL_primitiveType, baseOffset, verticesCount, instancesCount);

                    BERSERK_CATCH_OPENGL_ERRORS();
                }
                break;

                default:
                    BERSERK_ERROR("Unsupported draw list command");
            }
        }
    }

    void GLDevice::endRenderFrame() {
        // For OpenGL empty
    }

    ERenderDeviceType GLDevice::getDeviceType() const {
        return ERenderDeviceType::OpenGL;
    }

    CString GLDevice::getDeviceName() const {
        return "OpenGL";
    }
    
    bool GLDevice::isFormatSupported(EPixelFormat pixelFormat) const {
        return mSupportedTextureFormats.contains(pixelFormat);
    }

    const TArray<EPixelFormat> &GLDevice::getSupportedTextureFormats() const {
        return mSupportedTextureFormats;
    }

    const TArray<EShaderLanguage> &GLDevice::getSupportedShaderLanguages() const {
        return mSupportedShaderLanguages;
    }

    void GLDevice::checkToAbort(bool result, const char* message) const {
        static AutoConsoleVarInt mCVarAbortOnGpuError("r.AbortOnGpuError");

        if (!result && mCVarAbortOnGpuError.get() == 1) {
            BERSERK_ERROR_FAIL("GPU error: %s", message);
        }
    }

}