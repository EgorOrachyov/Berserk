//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Public/GLTexture.h"

namespace Berserk
{

    void GLTexture::initialize()
    {
        mWidth = 0;
        mHeight = 0;
        mGenMipMaps = false;

        mTextureID = 0;
        mReferenceCount = 0;

        mResourceName = "";
    }

    void GLTexture::addReference()
    {
        mReferenceCount += 1;
    }

    void GLTexture::release()
    {
        mReferenceCount -= 1;

        if (mReferenceCount == 0)
        {

        }
    }

    uint32 GLTexture::getReferenceCount()
    {
        return mReferenceCount;
    }

    uint32 GLTexture::getMemoryUsage()
    {

    }

    const char* GLTexture::getName()
    {
        return mResourceName.get();
    }

    void GLTexture::create(uint32 width, uint32 height, StorageFormat storage)
    {

    }

    void GLTexture::create(uint32 width, uint32 height, StorageFormat storage, void* data, PixelFormat format, bool genMipMaps)
    {

    }

    void GLTexture::bind(ISampler* sampler, uint32 textureSlot)
    {

    }

    void GLTexture::getData(uint32 depth, uint32 size, PixelType destination, void* data)
    {

    }

    uint32 GLTexture::getTargetType()
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

    }

} // namespace Berserk