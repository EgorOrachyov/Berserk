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

#include <core/image/ImageUtil.hpp>
#include <rhi/opengl/GLFramebuffer.hpp>
#include <rhi/opengl/GLTexture.hpp>

BRK_NS_BEGIN

GLFramebuffer::GLFramebuffer(const RHIFramebufferDesc &desc) {
    mDesc = desc;
}

GLFramebuffer::~GLFramebuffer() {
    if (mHandle) {
        glDeleteFramebuffers(1, &mHandle);
        BRK_GL_CATCH_ERR();

        mHandle = 0;
        mHasDepthBuffer = false;
        mHasStencilBuffer = false;
    }
}

void GLFramebuffer::Initialize() {
    // Currently we support only 2d frame buffers (cube wile be added later)
    Initialize2d();
}

void GLFramebuffer::Initialize2d() {
    uint32 width = mDesc.width;
    uint32 height = mDesc.height;
    auto &colors = mDesc.colorTargets;
    auto &depthStencil = mDesc.depthStencilTarget;

    assert(width > 0);
    assert(height > 0);
    assert(!colors.empty() || depthStencil.target.IsNotNull());

    auto validate = [&](const RHIAttachmentDesc &attachment, bool color, bool depthStencil) {
        auto target = attachment.target.Cast<GLTexture>();
        auto size = ImageUtil::GetMipSize(attachment.mipLevel, target->GetWidth(), target->GetHeight());

        assert(target);
        assert(target->GetTextureType() == RHITextureType::Texture2d || target->GetTextureType() == RHITextureType::TextureCube || target->GetTextureType() == RHITextureType::Texture2dArray);
        assert(!color || target->UsageColorAttachment());
        assert(!depthStencil || target->UsageDepthStencilAttachment());
        assert(width == size.x());
        assert(height == size.y());
        assert(attachment.mipLevel < target->GetMipsCount());
        assert(attachment.arraySlice < target->GetArraySlices());
        assert(!depthStencil || GLDefs::IsSuitableForDepthStencil(target->GetTextureFormat(), mHasDepthBuffer, mHasStencilBuffer));
    };

    for (auto &attachment : colors) {
        validate(attachment, true, false);
    }

    if (depthStencil.target) {
        validate(depthStencil, false, true);
    }

    glGenFramebuffers(1, &mHandle);
    BRK_GL_CATCH_ERR();

    glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
    BRK_GL_CATCH_ERR();

    auto setAttachment = [](const RHIAttachmentDesc &attachment, GLenum attachmentType, GLuint i) {
        auto target = attachment.target.Cast<GLTexture>();
        auto type = target->GetTextureType();

        switch (type) {
            case RHITextureType::Texture2d:
                glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType + i, GL_TEXTURE_2D,
                                       target->GetHandle(), static_cast<GLint>(attachment.mipLevel));
                break;
            case RHITextureType::Texture2dArray:
                glFramebufferTextureLayer(GL_FRAMEBUFFER, attachmentType + i,
                                          target->GetHandle(), static_cast<GLint>(attachment.mipLevel),
                                          static_cast<GLint>(attachment.arraySlice));
                break;
            case RHITextureType::TextureCube:
                glFramebufferTextureLayer(GL_FRAMEBUFFER, attachmentType + i,
                                          target->GetHandle(), static_cast<GLint>(attachment.mipLevel),
                                          static_cast<GLint>(attachment.face));
                break;
            default:
                break;
        }

        BRK_GL_CATCH_ERR();
    };

    for (uint32 i = 0; i < static_cast<uint32>(colors.size()); i++) {
        auto &attachment = colors[i];
        setAttachment(attachment, GL_COLOR_ATTACHMENT0, i);
    }

    if (depthStencil.target) {
        GLenum attachmentType;

        if (mHasDepthBuffer && mHasStencilBuffer) {
            attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
        } else if (mHasDepthBuffer) {
            attachmentType = GL_DEPTH_ATTACHMENT;
        } else {
            attachmentType = GL_STENCIL_ATTACHMENT;
        }

        setAttachment(depthStencil, attachmentType, 0);
    }

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    BRK_GL_CATCH_ERR();

    auto attachmentsCount = static_cast<uint32>(colors.size());
    GLenum drawBuffers[RHILimits::MAX_COLOR_ATTACHMENTS];

    for (uint32 i = 0; i < attachmentsCount; i++) {
        drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    if (attachmentsCount > 0) {
        glDrawBuffers(static_cast<GLsizei>(attachmentsCount), drawBuffers);
        BRK_GL_CATCH_ERR();
    } else {
        glDrawBuffer(GL_NONE);
        BRK_GL_CATCH_ERR();
    }

    // Activate later if required read
    glReadBuffer(GL_NONE);
    BRK_GL_CATCH_ERR();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    BRK_GL_CATCH_ERR();
}

BRK_NS_END