//
// Created by Egor Orachyov on 2019-07-15.
//

#include <GLSampler.h>

namespace Berserk
{

    GLSampler::GLSampler(
            ESamplerFilter min,
            ESamplerFilter mag,
            ESamplerWrapMode wrapMode,
            const Vec4f &borderColor,
            GLint min_gl,
            GLint mag_gl,
            GLint wrapMode_gl)
        : mMinFilter(min),
          mMagFilter(mag),
          mWrapMode(wrapMode),
          mBorderColor(borderColor)
    {
        glGenSamplers(1, &mResourceID);

        glSamplerParameteri(mResourceID, GL_TEXTURE_MIN_FILTER, min_gl);
        glSamplerParameteri(mResourceID, GL_TEXTURE_MAG_FILTER, mag_gl);

        glSamplerParameteri(mResourceID, GL_TEXTURE_WRAP_S, wrapMode_gl);
        glSamplerParameteri(mResourceID, GL_TEXTURE_WRAP_T, wrapMode_gl);
        glSamplerParameteri(mResourceID, GL_TEXTURE_WRAP_R, wrapMode_gl);

        glSamplerParameterfv(mResourceID, GL_TEXTURE_BORDER_COLOR, (GLfloat*) &mBorderColor);
    }

    GLSampler::~GLSampler()
    {
        if (mResourceID)
        {
            glDeleteSamplers(1, &mResourceID);
            mResourceID = 0;
        }
    }

    void GLSampler::bind(uint32 textureSlot) const
    {
        glBindSampler(textureSlot, mResourceID);
    }

    ESamplerFilter GLSampler::getMinFilter() const
    {
        return mMinFilter;
    }

    ESamplerFilter GLSampler::getMagFilter() const
    {
        return mMagFilter;
    }

    ESamplerWrapMode GLSampler::getWrapMode() const
    {
        return mWrapMode;
    }

    const Vec4f & GLSampler::getBorderColor() const
    {
        return mBorderColor;
    }

} // namespace Berserk