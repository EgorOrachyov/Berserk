//
// Created by Egor Orachyov on 16.07.2018.
//

#include "Buffers/GLFrameBufferObject.h"
#include "Essential/Assert.h"
#include "Logging/LogMessages.h""

namespace Berserk
{

    GLFrameBufferObject::GLFrameBufferObject()
    {
        mFBOHandle = 0;
        mRBOHandle = 0;
        mWidth = 0;
        mHeight = 0;
    }

    GLFrameBufferObject::~GLFrameBufferObject()
    {
        destroy();
    }

    void GLFrameBufferObject::init(UINT16 width, UINT16 height)
    {
        if (mFBOHandle)
        {
            WARNING("Frame Buffer Object has been initialized");
            return;
        }
        if (!(width || height))
        {
            WARNING("An attempt to create FBO with illegal size %u x %u", width, height);
            return;
        }

        mWidth = width;
        mHeight = height;

        // Create and bind Frame Buffer Object
        glGenFramebuffers(1, &mFBOHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);

        // Create and bind color attachment
        mTexture.create(width, height); // RGB (8)
        mSampler.init(GLWrapping::GLW_CLAMP_TO_EDGE, GLFiltering::GLF_NEAREST); // todo: init params should be defined from the outside

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture.getHandle(), 0);

        // Create and bind depth (stencil) attachment
        glGenRenderbuffers(1, &mRBOHandle);
        glBindRenderbuffer(GL_RENDERBUFFER, mRBOHandle);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBOHandle);

        // Attach our color layouts with out variables of frag shader (0 - COLOR ATTACHMENT 0)
        GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, drawBuffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            WARNING("Frame Buffer Object %u is not complete", mFBOHandle);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFrameBufferObject::destroy()
    {
        if (mFBOHandle)
        {
            glDeleteFramebuffers(1, &mFBOHandle);
        }

        if (mRBOHandle)
        {
            glDeleteRenderbuffers(1, &mRBOHandle);
        }

        mFBOHandle = 0;
        mRBOHandle = 0;
        mWidth = 0;
        mHeight = 0;
        mTexture.destroy();
        mSampler.destroy();
    }

    void GLFrameBufferObject::use() const
    {
        ASSERT(mFBOHandle, "Frame Buffer Object is not initialized");
        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
        glViewport(0, 0, mWidth, mHeight);
    }

    void GLFrameBufferObject::disable() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    UINT32 GLFrameBufferObject::getFBO() const
    {
        return mFBOHandle;
    }

    UINT32 GLFrameBufferObject::getRBO() const
    {
        return mRBOHandle;
    }

    UINT32 GLFrameBufferObject::getWidth() const
    {
        return mWidth;
    }

    UINT32 GLFrameBufferObject::getHeight() const
    {
        return mHeight;
    }

} // namespace Berserk