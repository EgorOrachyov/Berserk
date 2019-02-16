//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Public/GLTexture.h"
#include "Public/GLInclude.h"
#include "Public/Logging/LogMacros.h"

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

    void GLTexture::create(uint32 width, uint32 height, StorageFormat storage)
    {
        create(width, height, storage, nullptr, RGB, UNSIGNED_INT, false);
    }

    void GLTexture::create(uint32 width, uint32 height,
                           StorageFormat storage,
                           void* data,
                           PixelFormat format, PixelType pixelType,
                           bool genMipMaps)
    {
        auto trg = getTextureType(TEXTURE_2D);
        auto str = getStorageFormat(storage);
        auto pxf = getPixelFormat(format);
        auto pxt = getPixelType(pixelType);

        glGenTextures(1, &mTextureID);
        glBindTexture(trg, mTextureID);

        glTexImage2D(trg, 0, str, width, height, 0, pxf, pxt, data);

        if (genMipMaps) glGenerateMipmap(trg);

        mWidth = width;
        mHeight = height;
        mGenMipMaps = genMipMaps;

        mTextureType = TEXTURE_2D;
        mStorageFormat = storage;
        mPixelFormat = format;

        glBindTexture(trg, 0);
    }

    void GLTexture::bind(ISampler *sampler)
    {
        mSampler = sampler;
    }

    void GLTexture::bind(uint32 textureSlot)
    {
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(getTextureType(mTextureType), mTextureID);
    }

    void GLTexture::getData(uint32 depth, uint32 size, PixelType destination, void* data)
    {
        glGetnTexImage(getTextureType(mTextureType),
                       depth,
                       getPixelFormat(mPixelFormat),
                       getPixelType(destination),
                       size, data);
    }

    ITexture::TargetType GLTexture::getTargetType()
    {
        return mTargetType;
    }

    ITexture::PixelFormat GLTexture::getPixelFormat()
    {
        return mPixelFormat;
    }

    ITexture::TextureType GLTexture::getTextureType()
    {
        return mTextureType;
    }

    ITexture::StorageFormat GLTexture::getStorageFormat()
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

    uint32 GLTexture::getPixelFormat(PixelFormat format)
    {
        switch (format)
        {
            case R:
                return GL_R;

            case RG:
                return GL_RG;

            case RGB:
                return GL_RGB;

            case BGR:
                return GL_BGR;

            case RGBA:
                return GL_RGBA;

            case ABGR:
                return GL_ABGR_EXT;

            case DEPTH:
                return GL_DEPTH_COMPONENT;

            case DEPTH_AND_STENCIL:
                return GL_DEPTH_STENCIL;

            default:
                WARNING("Invalid argument value");
        }

        return 0;
    }

    uint32 GLTexture::getTextureType(TextureType type)
    {
        switch (type)
        {
            case TEXTURE_1D:
                return GL_TEXTURE_1D;

            case TEXTURE_2D:
                return GL_TEXTURE_2D;

            case TEXTURE_3D:
                return GL_TEXTURE_3D;

            default:
                WARNING("Invalid argument value");
        }

        return 0;
    }

    uint32 GLTexture::getStorageFormat(StorageFormat format)
    {
        switch (format)
        {
            case RGB8:
                return GL_RGB8;

            case RGBA8:
                return GL_RGBA8;

            case RGB32F:
                return GL_RGB32F;

            case DEPTH24:
                return GL_DEPTH;

            case DEPTH24_STENCIL8:
                return GL_DEPTH24_STENCIL8;

            default:
                WARNING("Invalid argument value");
        }

        return 0;
    }

    uint32 GLTexture::getPixelType(PixelType type)
    {
        switch (type)
        {
            case INT:
                return GL_INT;

            case BYTE:
                return GL_BYTE;

            case SHORT:
                return GL_SHORT;

            case FLOAT:
                return GL_FLOAT;

            case HALF_FLOAT:
                return GL_HALF_FLOAT;

            case UNSIGNED_INT:
                return GL_UNSIGNED_INT;

            case UNSIGNED_BYTE:
                return GL_UNSIGNED_BYTE;

            case UNSIGNED_SHORT:
                return GL_UNSIGNED_SHORT;

            default:
                WARNING("Invalid argument value");
        }

        return 0;
    }

} // namespace Berserk