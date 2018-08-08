//
// Created by Egor Orachyov on 16.07.2018.
//

#include "Texture/GLSampler.h"
#include "Essential/Assert.h"
#include "Logging/LogMessages.h"

namespace Berserk
{

    GLSampler::GLSampler()
    {
        mHandle = 0;
    }

    GLSampler::~GLSampler()
    {
        destroy();
    }

    UINT32 GLSampler::getMemoryUsage() const
    {
        return 0 /*todo*/;
    }

    void GLSampler::addReference()
    {
        mReferenceCount += 1;
    }

    void GLSampler::release()
    {
        if (mReferenceCount > 0) mReferenceCount -= 1;
        if (mReferenceCount == 0) destroy();
    }

    UINT32 GLSampler::getReferences() const
    {
        return mReferenceCount;
    }

    void GLSampler::init()
    {
        if (mHandle)
        {
            WARNING("Sampler has been created");
            return;
        }

        glGenSamplers(1, &mHandle);
    }

    void GLSampler::init(GLWrapping wrapping, GLFiltering filtering)
    {
        init();
        setWrapping(wrapping, wrapping);
        setFiltering(filtering, filtering);
    }

    void GLSampler::destroy()
    {
        if (mHandle)
        {
            glDeleteSamplers(1, &mHandle);
        }
        mHandle = 0;
    }

    void GLSampler::use(UINT32 targetTextureSlot) const
    {
        ASSERT(mHandle, "Sampler object is not initialized");
        glBindSampler(targetTextureSlot, mHandle);
    }

    void GLSampler::setWrapping(GLWrapping s, GLWrapping t)
    {
        ASSERT(mHandle, "Sampler object is not initialized");
        glSamplerParameteri(mHandle, GL_TEXTURE_WRAP_S, s);
        glSamplerParameteri(mHandle, GL_TEXTURE_WRAP_T, t);
    }

    void GLSampler::setFiltering(GLFiltering min, GLFiltering mag)
    {
        ASSERT(mHandle, "Sampler object is not initialized");
        glSamplerParameteri(mHandle, GL_TEXTURE_MIN_FILTER, min);
        glSamplerParameteri(mHandle, GL_TEXTURE_MAG_FILTER, mag);
    }

    UINT32 GLSampler::getHandle() const
    {
        return mHandle;
    }

    const bool GLSampler::operator==(const GLSampler &sampler) const
    {
        return (mHandle == sampler.mHandle);
    }

} // namespace Berserk