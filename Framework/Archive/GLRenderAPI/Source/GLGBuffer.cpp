//
// Created by Egor Orachyov on 18.08.2018.
//

#include "Buffers/GLGBuffer.h"
#include "Essential/GLInclude.h"
#include "Misc/Assert.h"

namespace Berserk
{

    void GLGBuffer::init(UINT32 width, UINT32 height)
    {
        mWidht = width;
        mHeight = height;

        glGenFramebuffers(1, &mFBOHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);

        INT32 internalFormat[]  = {GL_RGB32F, GL_RGB32F, GL_RGB8, GL_RGBA8, GL_RGB16F};
        UINT32 format[]         = {GL_RGB, GL_RGB, GL_RGB, GL_RGBA, GL_RGB};
        UINT32 type[]           = {GL_FLOAT, GL_FLOAT, GL_UNSIGNED_INT, GL_UNSIGNED_INT, GL_FLOAT};

        for (UINT32 i = 0; i < GBufferInfo::GBI_SUPPORTED_LAYOUTS; ++i)
        {
            mTexture[i].mSlot = i;
            glGenTextures(1, &mTexture[i].mHandle);
            glActiveTexture(GL_TEXTURE0 + mTexture[i].mSlot);
            glBindTexture(GL_TEXTURE_2D, mTexture[i].mHandle);

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat[i], width, height, 0, format[i], type[i], NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTexture[i].mHandle, 0);
        }

        glGenTextures(1, &mDepthHandle);
        glBindTexture(GL_TEXTURE_2D, mDepthHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthHandle, 0);

        UINT32 attachments[] = {GL_COLOR_ATTACHMENT0,
                                GL_COLOR_ATTACHMENT1,
                                GL_COLOR_ATTACHMENT2,
                                GL_COLOR_ATTACHMENT3,
                                GL_COLOR_ATTACHMENT4};

        glDrawBuffers(GBufferInfo::GBI_SUPPORTED_LAYOUTS, attachments);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLGBuffer::destroy()
    {
        if (mFBOHandle)
        {
            glDeleteFramebuffers(1, &mFBOHandle);
            mFBOHandle = 0;
        }
        if (mDepthHandle)
        {
            glDeleteTextures(1, &mDepthHandle);
            mDepthHandle = 0;
        }
        for (UINT32 i = 0; i < GBufferInfo::GBI_SUPPORTED_LAYOUTS; ++i)
        {
            if (mTexture[i].mHandle)
            {
                glDeleteTextures(1, &mTexture[i].mHandle);
                mTexture[i].mHandle = 0;
            }
        }
    }

    void GLGBuffer::useAsFBO()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
    }

    void GLGBuffer::useAsUniform()
    {
        for (UINT32 i = 0; i < GBufferInfo::GBI_SUPPORTED_LAYOUTS; ++i)
        {
            glActiveTexture(GL_TEXTURE0 + mTexture[i].mSlot);
            glBindTexture(GL_TEXTURE_2D, mTexture[i].mHandle);
        }
    }

    void GLGBuffer::useAsUniformLayout(UINT32 index, UINT32 binding)
    {
        ASSERT(index < GBufferInfo::GBI_SUPPORTED_LAYOUTS, "GLGBuffer: Index out of range");
        glActiveTexture(GL_TEXTURE0 + binding);
        glBindTexture(GL_TEXTURE_2D, mTexture[index].mHandle);
    }

    void GLGBuffer::useAsUniformDepthBuffer(UINT32 binding)
    {
        glActiveTexture(GL_TEXTURE0 + binding);
        glBindTexture(GL_TEXTURE_2D, mDepthHandle);
    }

    UINT32 GLGBuffer::getWidht()
    {
        return mWidht;
    }

    UINT32 GLGBuffer::getHeight()
    {
        return mHeight;
    }

} // namespace Berserk