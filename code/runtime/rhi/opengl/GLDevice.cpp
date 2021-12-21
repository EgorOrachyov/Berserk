/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <core/Engine.hpp>
#include <core/math/MathUtils3d.hpp>
#include <rhi/opengl/GLBuffer.hpp>
#include <rhi/opengl/GLDevice.hpp>
#include <rhi/opengl/GLFramebuffer.hpp>
#include <rhi/opengl/GLGraphicsPipeline.hpp>
#include <rhi/opengl/GLRenderPass.hpp>
#include <rhi/opengl/GLResourceSet.hpp>
#include <rhi/opengl/GLSampler.hpp>
#include <rhi/opengl/GLTexture.hpp>
#include <rhi/opengl/GLVertexDeclaration.hpp>

#include <cstring>
#include <utility>

BRK_NS_BEGIN

namespace {
    /** Check if extension with specified name is supported */
    bool IsExtensionSupported(const char *name) {
        GLint n = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &n);

        for (GLint i = 0; i < n; i++) {
            const char *extension = reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i));
            if (!std::strcmp(name, extension))
                return true;
        }

        return false;
    }
}// namespace

GLDevice::GLDevice(MakeContextCurrentFunc makeCurrentFunc, SwapBuffersFunc swapBuffersFunc)
    : mMakeCurrentFunc(std::move(makeCurrentFunc)),
      mSwapBuffersFunc(std::move(swapBuffersFunc)) {
    mSupportedShaderLanguages.push_back(RHIShaderLanguage::GLSL410GL);

#if defined(BERSERK_TARGET_LINUX) || defined(BERSERK_TARGET_WINDOWS)
    mSupportedShaderLanguages.push_back(RHIShaderLanguage::GLSL450GL);
#endif

    mSupportedTextureFormats.push_back(RHITextureFormat::R8);
    mSupportedTextureFormats.push_back(RHITextureFormat::R8_SNORM);
    mSupportedTextureFormats.push_back(RHITextureFormat::R16);
    mSupportedTextureFormats.push_back(RHITextureFormat::R16_SNORM);
    mSupportedTextureFormats.push_back(RHITextureFormat::RG8);
    mSupportedTextureFormats.push_back(RHITextureFormat::RG8_SNORM);
    mSupportedTextureFormats.push_back(RHITextureFormat::RG16);
    mSupportedTextureFormats.push_back(RHITextureFormat::RG16_SNORM);
    mSupportedTextureFormats.push_back(RHITextureFormat::RGB8);
    mSupportedTextureFormats.push_back(RHITextureFormat::RGB8_SNORM);
    mSupportedTextureFormats.push_back(RHITextureFormat::RGB16_SNORM);
    mSupportedTextureFormats.push_back(RHITextureFormat::RGBA8);
    mSupportedTextureFormats.push_back(RHITextureFormat::RGBA8_SNORM);
    mSupportedTextureFormats.push_back(RHITextureFormat::RGBA16);
    mSupportedTextureFormats.push_back(RHITextureFormat::SRGB8);
    mSupportedTextureFormats.push_back(RHITextureFormat::SRGB8_ALPHA8);
    mSupportedTextureFormats.push_back(RHITextureFormat::R16F);
    mSupportedTextureFormats.push_back(RHITextureFormat::RG16F);
    mSupportedTextureFormats.push_back(RHITextureFormat::RGB16F);
    mSupportedTextureFormats.push_back(RHITextureFormat::RGBA16F);
    mSupportedTextureFormats.push_back(RHITextureFormat::R32F);
    mSupportedTextureFormats.push_back(RHITextureFormat::RG32F);
    mSupportedTextureFormats.push_back(RHITextureFormat::RGB32F);
    mSupportedTextureFormats.push_back(RHITextureFormat::RGBA32F);
    mSupportedTextureFormats.push_back(RHITextureFormat::DEPTH32F);
    mSupportedTextureFormats.push_back(RHITextureFormat::DEPTH32F_STENCIL8);
    mSupportedTextureFormats.push_back(RHITextureFormat::DEPTH24_STENCIL8);

    GLint maxVertexAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
    BRK_GL_CATCH_ERR();
    mCaps.maxVertexAttributes = (uint32) maxVertexAttributes;

    GLint maxCombinedUniformBlocks;
    glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &maxCombinedUniformBlocks);
    BRK_GL_CATCH_ERR();
    mCaps.maxCombinedUniformBlocks = (uint32) maxCombinedUniformBlocks;

    GLint maxTextureArrayLayers;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTextureArrayLayers);
    BRK_GL_CATCH_ERR();
    mCaps.maxTextureArrayLayers = (uint32) maxTextureArrayLayers;

    GLint maxTexture3dSize;
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &maxTexture3dSize);
    BRK_GL_CATCH_ERR();
    mCaps.maxTexture3dSize = (uint32) maxTexture3dSize;

    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    BRK_GL_CATCH_ERR();
    mCaps.maxTextureSize = (uint32) maxTextureSize;

    GLint maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    BRK_GL_CATCH_ERR();
    mCaps.maxTextureUnits = (uint32) maxTextureUnits;

    GLint maxColorAttachments;
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxColorAttachments);
    BRK_GL_CATCH_ERR();
    mCaps.maxColorAttachments = (uint32) maxColorAttachments;

    GLint maxFramebufferWidth;
    glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &maxFramebufferWidth);
    BRK_GL_CATCH_ERR();
    mCaps.maxFramebufferWidth = (uint32) maxFramebufferWidth;

    GLint maxFramebufferHeight;
    glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &maxFramebufferHeight);
    BRK_GL_CATCH_ERR();
    mCaps.maxFramebufferHeight = (uint32) maxFramebufferHeight;

    GLint uniformBlockOffsetAlignment;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniformBlockOffsetAlignment);
    BRK_GL_CATCH_ERR();
    mCaps.uniformBlockOffsetAlignment = (uint32) uniformBlockOffsetAlignment;

    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
    BRK_GL_CATCH_ERR();
    mCaps.maxAnisotropy = maxAnisotropy;
    mCaps.supportAnisotropy = IsExtensionSupported("GL_EXT_texture_filter_anisotropic");

    mClipMatrix = MathUtils3d::IdentityMatrix();
    mType = RHIType::OpenGL;
    mRHIThread = &Engine::Instance().GetRHIThread();
}

GLDevice::~GLDevice() {
    BRK_INFO(BRK_TEXT("Finalize RHI Device"));
}

#define BRK_GL_CREATE_RESOURCE(type, gl_type, ...)                  \
    Ref<gl_type> object(new gl_type(__VA_ARGS__));                  \
    if (mRHIThread->OnThread())                                     \
        object->Initialize();                                       \
    else                                                            \
        mRHIThread->EnqueueBefore([=]() { object->Initialize(); }); \
    return object.Cast<type>()

Ref<RHIVertexDeclaration> GLDevice::CreateVertexDeclaration(const RHIVertexDeclarationDesc &desc) {
    return Ref<RHIVertexDeclaration>(new GLVertexDeclaration(desc));
}

Ref<RHIVertexBuffer> GLDevice::CreateVertexBuffer(const RHIBufferDesc &desc) {
    BRK_GL_CREATE_RESOURCE(RHIVertexBuffer, GLVertexBuffer, desc);
}

Ref<RHIIndexBuffer> GLDevice::CreateIndexBuffer(const RHIBufferDesc &desc) {
    BRK_GL_CREATE_RESOURCE(RHIIndexBuffer, GLIndexBuffer, desc);
}

Ref<RHIUniformBuffer> GLDevice::CreateUniformBuffer(const RHIBufferDesc &desc) {
    BRK_GL_CREATE_RESOURCE(RHIUniformBuffer, GLUniformBuffer, desc);
}

Ref<RHISampler> GLDevice::CreateSampler(const RHISamplerDesc &desc) {
    BRK_GL_CREATE_RESOURCE(RHISampler, GLSampler, desc);
}

Ref<RHITexture> GLDevice::CreateTexture(const RHITextureDesc &desc) {
    BRK_GL_CREATE_RESOURCE(RHITexture, GLTexture, desc);
}

Ref<RHIResourceSet> GLDevice::CreateResourceSet(const RHIResourceSetDesc &desc) {
    return Ref<RHIResourceSet>(new GLResourceSet(desc));
}

Ref<RHIFramebuffer> GLDevice::CreateFramebuffer(const RHIFramebufferDesc &desc) {
    BRK_GL_CREATE_RESOURCE(RHIFramebuffer, GLFramebuffer, desc);
}

Ref<RHIShader> GLDevice::CreateShader(const RHIShaderDesc &desc) {
    BRK_GL_CREATE_RESOURCE(RHIShader, GLShader, desc);
}

Ref<RHIRenderPass> GLDevice::CreateRenderPass(const RHIRenderPassDesc &desc) {
    return Ref<RHIRenderPass>(new GLRenderPass(desc));
}

Ref<RHIGraphicsPipeline> GLDevice::CreateGraphicsPipeline(const RHIGraphicsPipelineDesc &desc) {
    return Ref<RHIGraphicsPipeline>(new GLGraphicsPipeline(desc));
}

Ref<RHICommandList> GLDevice::GetCoreCommandList() {
    return Ref<RHICommandList>();
}

#undef BRK_GL_CREATE_RESOURCE

GLDevice::MakeContextCurrentFunc &GLDevice::GetContextFunc() {
    return mMakeCurrentFunc;
}

GLDevice::SwapBuffersFunc &GLDevice::GetSwapFunc() {
    return mSwapBuffersFunc;
}

std::shared_ptr<GLDevice> GLDevice::Make(MakeContextCurrentFunc makeCurrentFunc, SwapBuffersFunc swapBuffersFunc) {
    GLenum error = glewInit();

    if (error != GLEW_OK) {
        BRK_ERROR("[GL] Failed to initialize glew library `glewInit`. Why: " << glewGetErrorString(error));
        return nullptr;
    }

    return std::make_shared<GLDevice>(std::move(makeCurrentFunc), std::move(swapBuffersFunc));
}

BRK_NS_END
