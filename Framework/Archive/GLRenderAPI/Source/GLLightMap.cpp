//
// Created by Egor Orachyov on 26.08.2018.
//

#include "Buffers/GLLightMap.h"
#include "Essential/GLInclude.h"

namespace Berserk
{

    void GLLightMap::create(UINT32 width, UINT32 height)
    {
        glGenFramebuffers(1, &mFBOHandle);

        glGenTextures(1, &mTextureHandle);
        glBindTexture(GL_TEXTURE_2D, mTextureHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureHandle, 0);

        UINT32 attachments[] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, attachments);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLLightMap::destroy()
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

    void GLLightMap::useAsFBO()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
    }

    void GLLightMap::useAsUniform(UINT32 binding)
    {
        glActiveTexture(GL_TEXTURE0 + binding);
        glBindTexture(GL_TEXTURE_2D, mTextureHandle);
    }

} // namespace Berserk