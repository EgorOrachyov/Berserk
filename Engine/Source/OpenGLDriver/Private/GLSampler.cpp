//
// Created by Egor Orachyov on 07.02.2019.
//

#include "GLSampler.h"
#include "GLInclude.h"
#include "GLRenderDriver.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    void GLSampler::initialize(const char *name)
    {
        mSamplerId = 0;
        mFilteringMin = 0;
        mFilteringMag = 0;
        mWrapping = 0;
        mBorderColor = Vec4f(0);
        mReferenceCount = 0;
        mResourceName = name;
    }

    void GLSampler::addReference()
    {
        mReferenceCount += 1;
    }

    void GLSampler::release()
    {
        if (mReferenceCount > 0)
        {
            mReferenceCount -= 1;
        }

        if (mReferenceCount == 0 && mSamplerId)
        {
            PUSH("GLSampler: delete | name: %s | id: %u", mResourceName.get(), mSamplerId);

            glDeleteSamplers(1, &mSamplerId);
            mSamplerId = 0;
        }
    }

    uint32 GLSampler::getReferenceCount()
    {
        return mReferenceCount;
    }

    uint32 GLSampler::getMemoryUsage()
    {
        return sizeof(GLSampler);
    }

    const char* GLSampler::getName()
    {
        return mResourceName.get();
    }

    void GLSampler::create(IRenderDriver::SamplerFilter min, IRenderDriver::SamplerFilter mag,
                           IRenderDriver::SamplerWrapMode wrap)
    {
        auto minification = GLRenderDriver::getSamplerFilter(min);
        auto magnification = GLRenderDriver::getSamplerFilter(mag);
        auto wrapping = GLRenderDriver::getSamplerWrapMode(wrap);

        glGenSamplers(1, &mSamplerId);
        glSamplerParameteri(mSamplerId, GL_TEXTURE_MAG_FILTER, magnification);
        glSamplerParameteri(mSamplerId, GL_TEXTURE_MIN_FILTER, minification);
        glSamplerParameteri(mSamplerId, GL_TEXTURE_WRAP_S, wrapping);
        glSamplerParameteri(mSamplerId, GL_TEXTURE_WRAP_T, wrapping);
        glSamplerParameteri(mSamplerId, GL_TEXTURE_WRAP_R, wrapping);
    }

    void GLSampler::bind(uint32 textureSlot)
    {
        glBindSampler(textureSlot, mSamplerId);
    }

    void GLSampler::setFiltering(IRenderDriver::SamplerFilter min, IRenderDriver::SamplerFilter mag)
    {
        auto minification = GLRenderDriver::getSamplerFilter(min);
        auto magnification = GLRenderDriver::getSamplerFilter(mag);

        glSamplerParameteri(mSamplerId, GL_TEXTURE_MAG_FILTER, magnification);
        glSamplerParameteri(mSamplerId, GL_TEXTURE_MIN_FILTER, minification);
    }

    void GLSampler::setWrapping(IRenderDriver::SamplerWrapMode wrap)
    {
        auto wrapping = GLRenderDriver::getSamplerWrapMode(wrap);

        glSamplerParameteri(mSamplerId, GL_TEXTURE_WRAP_S, wrapping);
        glSamplerParameteri(mSamplerId, GL_TEXTURE_WRAP_T, wrapping);
        glSamplerParameteri(mSamplerId, GL_TEXTURE_WRAP_R, wrapping);
    }

    void GLSampler::setBorderColor(const Vec4f &color)
    {
        mBorderColor = color;
        glSamplerParameterfv(mSamplerId, GL_TEXTURE_BORDER_COLOR, (float32*)&color);
    }

} // namespace Berserk