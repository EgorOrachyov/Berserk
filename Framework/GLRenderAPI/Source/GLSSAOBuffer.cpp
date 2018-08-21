//
// Created by Egor Orachyov on 21.08.2018.
//

#include "Buffers/GLSSAOBuffer.h"
#include "Essential/GLInclude.h"

namespace Berserk
{

    void GLSSAOBuffer::create(UINT32 width, UINT32 height)
    {
        glGenFramebuffers(1, &mFBOHandle);

        glGenTextures(1, &mTextureHandle);
        glBindTexture(GL_TEXTURE_2D, mTextureHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureHandle, 0);

        UINT32 attachments[] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, attachments);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLSSAOBuffer::destroy()
    {
        if (mFBOHandle)
        {
            glDeleteFramebuffers(1, &mFBOHandle);
            mFBOHandle = 0;
        }
        if (mTextureHandle)
        {
            glDeleteTextures(1, &mTextureHandle);
            mTextureHandle = 0;
        }
    }

    void GLSSAOBuffer::useAsFBO()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
    }

    void GLSSAOBuffer::useAsUniform(UINT32 textureSlot)
    {
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, mTextureHandle);
    }
    
} // namespace Berserk