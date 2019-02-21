//
// Created by Egor Orachyov on 07.02.2019.
//

#include "GLTexture.h"
#include "GLInclude.h"
#include "GLRenderDriver.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    void GLTexture::initialize(const char* name)
    {
        mWidth = 0;
        mHeight = 0;
        mGenMipMaps = false;

        mTextureID = 0;
        mReferenceCount = 0;

        mSampler = nullptr;

        mResourceName = name;
    }

    void GLTexture::addReference()
    {
        mReferenceCount += 1;
    }

    void GLTexture::release()
    {
        if (mReferenceCount > 0)
        {
            mReferenceCount -= 1;
        }

        if (mReferenceCount == 0 && mTextureID)
        {
            PUSH("GLTexture: delete | name: %s | id: %u", mResourceName.get(), mTextureID);

            glDeleteTextures(1, &mTextureID);
            mTextureID = 0;
        }
    }

    uint32 GLTexture::getReferenceCount()
    {
        return mReferenceCount;
    }

    uint32 GLTexture::getMemoryUsage()
    {
        return sizeof(GLTexture);
    }

    const char* GLTexture::getName()
    {
        return mResourceName.get();
    }

    void GLTexture::create(uint32 width, uint32 height, uint32 storageFormat)
    {
        create(width, height, storageFormat, nullptr, GLRenderDriver::RGB, GLRenderDriver::UNSIGNED_INT, false);
    }

    void GLTexture::create(uint32 width, uint32 height,
                           uint32 storageFormat,
                           void* data,
                           uint32 pixelFormat, uint32 pixelType,
                           bool genMipMaps)
    {
        auto trg = GLRenderDriver::TEXTURE_2D;
        auto str = storageFormat;
        auto pxf = pixelFormat;
        auto pxt = pixelType;

        glGenTextures(1, &mTextureID);
        glBindTexture(trg, mTextureID);

        glTexImage2D(trg, 0, str, width, height, 0, pxf, pxt, data);

        if (genMipMaps) glGenerateMipmap(trg);

        mWidth = width;
        mHeight = height;
        mGenMipMaps = genMipMaps;

        mTextureType = GLRenderDriver::TEXTURE_2D;
        mStorageFormat = storageFormat;
        mPixelFormat = pixelFormat;

        glBindTexture(trg, 0);
    }

    void GLTexture::bind(ISampler *sampler)
    {
        mSampler = sampler;
    }

    void GLTexture::bind(uint32 textureSlot)
    {
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(mTextureType, mTextureID);
    }

    void GLTexture::getData(uint32 depth, uint32 size, uint32 pixelType, void* data)
    {
        glGetnTexImage(mTextureType,
                       depth,
                       mPixelFormat,
                       pixelType,
                       size, data);
    }

    ITexture::TargetType GLTexture::getTargetType()
    {
        return mTargetType;
    }

    uint32 GLTexture::getPixelFormat()
    {
        return mPixelFormat;
    }

    uint32 GLTexture::getTextureType()
    {
        return mTextureType;
    }

    uint32 GLTexture::getStorageFormat()
    {
        return mStorageFormat;
    }

    bool GLTexture::getMipMapsGen()
    {
        return mGenMipMaps;
    }

    uint32 GLTexture::getHandle()
    {
        return mTextureID;
    }

    uint32 GLTexture::getWidth()
    {
        return mWidth;
    }

    uint32 GLTexture::getHeight()
    {
        return mHeight;
    }

    uint32 GLTexture::getGPUMemoryUsage()
    {
        return 0;
    }

} // namespace Berserk