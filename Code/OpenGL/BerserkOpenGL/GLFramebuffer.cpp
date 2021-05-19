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

#include <BerserkOpenGL/GLFramebuffer.hpp>
#include <BerserkOpenGL/GLTexture.hpp>
#include <BerserkOpenGL/GLDefs.hpp>
#include <BerserkCore/Image/PixelUtil.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>

namespace Berserk {
    namespace RHI {

        GLFramebuffer::GLFramebuffer(const Desc &desc) {
            mDesc = desc;
        }

        GLFramebuffer::~GLFramebuffer() {
            if (mHandle) {
                glDeleteFramebuffers(1, &mHandle);
                BERSERK_GL_CATCH_ERRORS();

                mHandle = 0;
                mHasDepthBuffer = false;
                mHasStencilBuffer = false;

                BERSERK_GL_LOG_INFO("Release framebuffer: thread=\"{0}\"", ThreadManager::GetCurrentThread()->GetName());
            }
        }

        void GLFramebuffer::Initialize() {
            // Currently we support only 2d frame buffers (cube wile be added later)
            Initialize2d();
        }

        void GLFramebuffer::Initialize2d() {
            uint32 width = mDesc.width;
            uint32 height = mDesc.height;
            auto& colors = mDesc.colorTargets;
            auto& depthStencil = mDesc.depthStencilTarget;

            assert(width > 0);
            assert(height > 0);
            assert(colors.IsNotEmpty() || depthStencil.target.IsNotNull());

            for (auto& attachment: colors) {
                auto target = (GLTexture*) attachment.target.Get();
                auto size = PixelUtil::GetMipSize(attachment.mipLevel, target->GetWidth(), target->GetHeight());

                assert(target);
                assert(target->GetTextureType() == TextureType::Texture2d);
                assert(target->UsageColorAttachment());
                assert(width == size.x());
                assert(height == size.y());
                assert(attachment.mipLevel < target->GetMipsCount());
            }

            if (depthStencil.target) {
                auto target = (GLTexture*) depthStencil.target.Get();
                auto size = PixelUtil::GetMipSize(depthStencil.mipLevel, target->GetWidth(), target->GetHeight());

                assert(target);
                assert(target->GetTextureType() == TextureType::Texture2d);
                assert(target->UsageDepthStencilAttachment());
                assert(width == size.x());
                assert(height == size.y());
                assert(depthStencil.mipLevel < target->GetMipsCount());

                auto isSuitable = GLDefs::IsSuitableForDepthStencil(target->GetTextureFormat(), mHasDepthBuffer, mHasStencilBuffer);

                assert(isSuitable);
            }

            glGenFramebuffers(1, &mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            for (size_t i = 0; i < colors.GetSize(); i++) {
                auto& attachment = colors[i];
                auto target = (GLTexture*) attachment.target.Get();

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, target->GetHandle(), attachment.mipLevel);
                BERSERK_GL_CATCH_ERRORS();
            }

            if (depthStencil.target) {
                auto target = (GLTexture*) depthStencil.target.Get();

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, target->GetHandle(), depthStencil.mipLevel);
                BERSERK_GL_CATCH_ERRORS();
            }

            assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
            BERSERK_GL_CATCH_ERRORS();

            uint32 attachmentsCount = colors.GetSize();
            GLenum drawBuffers[Limits::MAX_COLOR_ATTACHMENTS];

            for (uint32 i = 0; i < attachmentsCount; i++) {
                drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
            }

            if (attachmentsCount > 0) {
                glDrawBuffers(attachmentsCount, drawBuffers);
                BERSERK_GL_CATCH_ERRORS();
            }
            else {
                glDrawBuffer(GL_NONE);
                BERSERK_GL_CATCH_ERRORS();
            }

            // Activate later if required read
            glReadBuffer(GL_NONE);
            BERSERK_GL_CATCH_ERRORS();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO("Create framebuffer 2d: thread=\"{0}\"", ThreadManager::GetCurrentThread()->GetName());
        }
    }
}
