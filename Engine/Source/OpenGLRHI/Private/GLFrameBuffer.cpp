//
// Created by Egor Orachyov on 2019-07-22.
//

#include <GLResources.h>

namespace Berserk
{

    GLFrameBufferTarget::GLFrameBufferTarget(const RHITexture2DRef &color, const RHITexture2DRef &depth)
            : mColorBuffer(color), mDepthStencil(depth)
    {
        GLuint textureID = GLTexture2D::getResourceID<RHITexture2DRef, GLTexture2D>(color);
        GLuint depthID = GLTexture2D::getResourceID<RHITexture2DRef, GLTexture2D>(depth);

        glGenFramebuffers(1, &mResourceID);
        assertion_dev(glGetError() == GL_NO_ERROR);
        glBindFramebuffer(GL_FRAMEBUFFER, mResourceID);
        assertion_dev(glGetError() == GL_NO_ERROR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
        assertion_dev(glGetError() == GL_NO_ERROR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthID, 0);
        assertion_dev(glGetError() == GL_NO_ERROR);
        glBindFramebuffer(GL_FRAMEBUFFER, mResourceID);
        assertion_dev(glGetError() == GL_NO_ERROR);
        GLenum attachments[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, attachments);
        assertion_dev(glGetError() == GL_NO_ERROR);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            DEBUG_LOG_ERROR("GLFrameBufferTarget: incomplete");
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLFrameBufferTarget::~GLFrameBufferTarget()
    {
        if (mResourceID)
        {
            OutputDevice::printf("Delete fb\n");
            glDeleteFramebuffers(1, &mResourceID);
            mResourceID = 0;
        }
    }

    void GLFrameBufferTarget::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mResourceID);
    }

    uint32 GLFrameBufferTarget::getColorAttachmentsCount() const
    {
        return 1;
    }

    RHITextureRef GLFrameBufferTarget::getDepthAttachment() const
    {
        return mDepthStencil.cast<RHITexture>();
    }

    RHITextureRef GLFrameBufferTarget::getColorAttachment(uint32 layer) const
    {
        return mColorBuffer.cast<RHITexture>();
    }

} // namespace Berserk