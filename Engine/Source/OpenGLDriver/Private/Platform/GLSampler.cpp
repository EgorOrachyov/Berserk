//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Platform/GLSampler.h"
#include "Platform/GLInclude.h"
#include "Platform/GLRenderDriver.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    void GLSampler::initialize(const char *name)
    {
        mSamplerID = 0;
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

        if (mReferenceCount == 0 && mSamplerID)
        {
            PUSH("GLSampler: delete | name: %s | id: %u", mResourceName.get(), mSamplerID);

            glDeleteSamplers(1, &mSamplerID);
            mSamplerID = 0;
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
        mFilteringMin = GLRenderDriver::getSamplerFilter(min);
        mFilteringMag = GLRenderDriver::getSamplerFilter(mag);
        mWrapping = GLRenderDriver::getSamplerWrapMode(wrap);

        glGenSamplers(1, &mSamplerID);
        glSamplerParameteri(mSamplerID, GL_TEXTURE_MIN_FILTER, mFilteringMin);
        glSamplerParameteri(mSamplerID, GL_TEXTURE_MAG_FILTER, mFilteringMag);
        glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_S, mWrapping);
        glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_T, mWrapping);
        glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_R, mWrapping);
    }

    void GLSampler::bind(uint32 textureSlot)
    {
        glBindSampler(textureSlot, mSamplerID);
    }

    void GLSampler::setFiltering(IRenderDriver::SamplerFilter min, IRenderDriver::SamplerFilter mag)
    {
        mFilteringMin = GLRenderDriver::getSamplerFilter(min);
        mFilteringMag = GLRenderDriver::getSamplerFilter(mag);

        glSamplerParameteri(mSamplerID, GL_TEXTURE_MIN_FILTER, mFilteringMin);
        glSamplerParameteri(mSamplerID, GL_TEXTURE_MAG_FILTER, mFilteringMag);
    }

    void GLSampler::setWrapping(IRenderDriver::SamplerWrapMode wrap)
    {
        mWrapping = GLRenderDriver::getSamplerWrapMode(wrap);

        glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_S, mWrapping);
        glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_T, mWrapping);
        glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_R, mWrapping);
    }

    void GLSampler::setBorderColor(const Vec4f &color)
    {
        mBorderColor = color;
        glSamplerParameterfv(mSamplerID, GL_TEXTURE_BORDER_COLOR, (float32*)&color);
    }

    uint32 GLSampler::getGPUMemoryUsage()
    {
        return 0;
    }

} // namespace Berserk