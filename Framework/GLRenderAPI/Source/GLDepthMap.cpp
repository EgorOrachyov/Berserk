//
// Created by Egor Orachyov on 12.08.2018.
//

#include "Buffers/GLDepthMap.h"
#include "Essential/GLInclude.h"
#include "Texture/GLTextureFiltering.h"

namespace Berserk
{

    void GLDepthMap::create(UINT32 width, UINT32 height, UINT32 textureSlot)
    {
        mWidth = width;
        mHeight = height;
        mTextureSlot = textureSlot;

        glGenFramebuffers(1, &mFBOHandle);

        FLOAT32 borderColor[] = {1.0f, 0.0f, 0.0f, 0.0f};

        glGenTextures(1, &mShadowMap);
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, mShadowMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLFiltering::GLF_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLFiltering::GLF_LINEAR);

        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_FUNC, GL_LESS);

        //glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLDepthMap::destroy()
    {
        if (mShadowMap) glDeleteTextures(1, &mShadowMap);
        if (mFBOHandle) glDeleteFramebuffers(1, &mFBOHandle);

        mWidth = 0;
        mHeight = 0;
        mFBOHandle = 0;
        mShadowMap = 0;
        mTextureSlot = 0;
    }

    void GLDepthMap::useAsFBO()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
    }

    void GLDepthMap::useAsUniform()
    {
        glActiveTexture(GL_TEXTURE0 + mTextureSlot);
        glBindTexture(GL_TEXTURE_2D, mShadowMap);
    }

    UINT32 GLDepthMap::getWidth() const
    {
        return mWidth;
    }

    UINT32 GLDepthMap::getHeight() const
    {
        return mHeight;
    }

} // namespace Berserk