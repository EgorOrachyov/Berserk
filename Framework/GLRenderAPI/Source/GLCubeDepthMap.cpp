//
// Created by Egor Orachyov on 12.08.2018.
//

#include "Buffers/GLCubeDepthMap.h"
#include "Essential/GLInclude.h"

namespace Berserk
{

    void GLCubeDepthMap::create(UINT32 size, UINT32 textureSlot)
    {
        mSize = size;
        mTextureSlot = textureSlot;

        glGenFramebuffers(1, &mFBOHandle);

        glGenTextures(1, &mCubeMap);
        glActiveTexture(GL_TEXTURE0 + mTextureSlot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMap);

        for (UINT32 i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mCubeMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLCubeDepthMap::destroy()
    {
        if (mCubeMap) glDeleteTextures(1, &mCubeMap);
        if (mFBOHandle) glDeleteFramebuffers(1, &mFBOHandle);

        mSize = 0;
        mTextureSlot = 0;
        mFBOHandle = 0;
        mCubeMap = 0;
    }

    void GLCubeDepthMap::useAsFBO()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
    }

    void GLCubeDepthMap::useAsUniform()
    {
        glActiveTexture(GL_TEXTURE0 + mTextureSlot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMap);
    }

    UINT32 GLCubeDepthMap::getSize() const
    {
        return mSize;
    }

} // namespace Berserk