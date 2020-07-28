/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLFRAMEBUFFER_H
#define BERSERK_GLFRAMEBUFFER_H

#include <RHI/RHIResources.h>
#include <GLDefinitions.h>
#include <GLTexture.h>
#include <GLErrors.h>

namespace Berserk {

    class GLFramebuffer : public RHIFramebuffer {
    public:

        ~GLFramebuffer() override {
            destroy();
        }

        bool create(const TArrayStatic<TPtrShared<RHITexture>, RHIConst::MAX_COLOR_ATTACHMENTS> &colors, const TPtrShared<RHITexture> &depthStencil) {
            uint32 useDepthStencil = (depthStencil.isNotNull() ? 1 : 0);
            uint32 colorAttachments = colors.size();
            uint32 attachmentsCount = useDepthStencil + colorAttachments;

            BERSERK_COND_ERROR_RET_VALUE(false, colorAttachments <= RHIConst::MAX_COLOR_ATTACHMENTS, "Color attachments too much")
            BERSERK_COND_ERROR_RET_VALUE(false, attachmentsCount > 0, "Attempt to create framebuffer without attachments");

            {
                if (useDepthStencil) {
                    mWidth = depthStencil->getWidth();
                    mHeight = depthStencil->getHeight();
                }
                else {
                    mWidth = colors[0]->getWidth();
                    mHeight = colors[0]->getHeight();
                }

                uint32 index = 0;
                for (const auto& attachment: colors) {
                    BERSERK_COND_ERROR_RET_VALUE(false, attachment.isNotNull(), "Attempt to pass null attachment at [%u] index", index);

                    uint32 w = attachment->getWidth();
                    uint32 h = attachment->getHeight();

                    BERSERK_COND_ERROR_RET_VALUE(false, w == mWidth && h == mHeight, "Attachments must be of the same size");

                    index += 1;
                }
            }

            glGenFramebuffers(1, &mHandle);
            glBindFramebuffer(GL_FRAMEBUFFER, mHandle);

            uint32 i = 0;
            for (const auto& attachment: colors) {
                auto& GL_texture = (GLTexture&)*attachment;

                if (GL_texture.getTextureType() == ETextureType::Texture2D) {
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, GL_texture.getTextureHandle(), 0);
                    BERSERK_CATCH_OPENGL_ERRORS();
                }

                i += 1;
            }

            if (useDepthStencil) {
                auto& GL_texture = (GLTexture&)*depthStencil;
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, GL_texture.getTextureHandle(), 0);
                BERSERK_CATCH_OPENGL_ERRORS();
            }

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                BERSERK_ERROR_RET_VALUE(false, "GLFramebuffer has incomplete state");
            }

            uint32 n = colors.size();
            GLenum drawBuffers[RHIConst::MAX_COLOR_ATTACHMENTS];

            for (uint32 i = 0; i < n; i++) {
                drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
            }

            if (n > 0) {
                glDrawBuffers(n, drawBuffers);
            }
            else {
                glDrawBuffer(GL_NONE);
            }
            // Activate later if required read
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            BERSERK_CATCH_OPENGL_ERRORS();

            mColorAttachments = colors;
            mDepthAttachment = depthStencil;
            mClearColors.resize(colors.size(), Color4f(0.0f));

            return true;
        }

        void destroy() {
            if (mHandle) {
                glDeleteFramebuffers(1, &mHandle);
                mHandle = 0;
                mClearColors.clear();
                mColorAttachments.clear();
                mDepthAttachment.free();

                BERSERK_CATCH_OPENGL_ERRORS();
            }
        }

        void clear(const TEnumMask<EClearOption>& clearMask) const {
            if (clearMask.getFlag(EClearOption::Color)) {
                // Disable for some reasons
                glDisable(GL_STENCIL_TEST);

                for (uint32 i = 0; i < mClearColors.size(); i++) {
                    auto& c = mClearColors[i];
                    glColorMaski(i, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                    glClearBufferfv(GL_COLOR, i, (GLfloat*) c.getData());
                }

                BERSERK_CATCH_OPENGL_ERRORS();
            }

            if (useDepthStencil()) {
                // Enable depth writing if it not
                glDepthMask(GL_TRUE);
                // Enable stencil write
                glStencilMask(0xffffffff);

                if (clearMask.getFlag(EClearOption::Depth)) {
                    if (clearMask.getFlag(EClearOption::Stencil)) {
                        // Clear depth and stencil simultaneously
                        glClearBufferfi(GL_DEPTH_STENCIL, 0, mDepthClear, mStencilClear);
                    }
                    else {
                        glClearBufferfv(GL_DEPTH, 0, &mDepthClear);
                    }
                }
                else if (clearMask.getFlag(EClearOption::Stencil)) {
                    glClearBufferiv(GL_STENCIL, 0, &mStencilClear);
                }

                BERSERK_CATCH_OPENGL_ERRORS();
            }
        }

        GLuint getHandle() const {
            return mHandle;
        }

    private:

        GLuint mHandle = 0;

    };

}

#endif //BERSERK_GLFRAMEBUFFER_H
