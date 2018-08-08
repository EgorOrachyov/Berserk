//
// Created by Egor Orachyov on 04.07.2018.
//

#include "Texture/GLTexture.h"
#include "Essential/Assert.h"
#include "Logging/LogMessages.h"

namespace Berserk
{

    GLTexture::GLTexture()
    {
        mWidth = 0;
        mHeight = 0;
        mHandle = 0;
        mReferenceCount = 0;

        mType = TextureType::TT_NO_TYPE;
        mFormat = GLInternalTextureFormat::GLTF_RGB8;

        mGenMipMaps = false;
        mSampler = nullptr;
    }

    void GLTexture::destroy()
    {
        if (mHandle)
        {
            glDeleteTextures(1, &mHandle);
        }
        if (mSampler)
        {

        }

        mHandle = 0;
        mSampler = nullptr;
    }

    UINT32 GLTexture::getMemoryUsage() const
    {
        return sizeof(GLTexture);
    }

    void   GLTexture::addReference()
    {
        mReferenceCount += 1;
    }

    void   GLTexture::release()
    {
        if (mReferenceCount > 0) mReferenceCount -= 1;
        if (mReferenceCount == 0) destroy();
    }

    UINT32 GLTexture::getReferences() const
    {
        return mReferenceCount;
    }

    void GLTexture::setName(const CStaticString& name)
    {
        mName = name;
    }

    const CStaticString& GLTexture::getName() const
    {
        return mName;
    }

    void GLTexture::setType(TextureType type)
    {
        mType = type;
    }

    TextureType GLTexture::getType() const
    {
        return mType;
    }

    void GLTexture::create(UINT32 width, UINT32 height)
    {
        create(width, height, GLInternalTextureFormat::GLTF_RGB8);
    }

    void GLTexture::create(UINT32 width, UINT32 height, GLInternalTextureFormat format)
    {
        if (mHandle)
        {
            WARNING("Texture with %u handle has been already created", mHandle);
            return;
        }

        glGenTextures(1, &mHandle);
        glBindTexture(GL_TEXTURE_2D, mHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GLImageFormat::GLTF_RGB, GLDataType::GLPT_UINT, NULL);

        mWidth = width;
        mHeight = height;
        mFormat = format;

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLTexture::create(UINT32 width, UINT32 height, GLInternalTextureFormat format,
                           GLImageFormat source, GLDataType actualType, void* data, GLMipmaps gen)
    {
        if (mHandle)
        {
            WARNING("Texture with %u handle has been already created", mHandle);
            return;
        }
        if (!data)
        {
            WARNING("Null pointer data in texture (id = %u) creating function", mHandle);
            return;
        }

        glGenTextures(1, &mHandle);
        glBindTexture(GL_TEXTURE_2D, mHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, source, actualType, data);

        if (gen == GLMipmaps::GLM_USE)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
            mGenMipMaps = true;
        }

        mWidth = width;
        mHeight = height;
        mFormat = format;

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLTexture::use(UINT32 textureSlot) const
    {
        ASSERT(mHandle, "Texture is not created");

        if (mSampler)
        {
            mSampler->use(textureSlot);
        }

        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, mHandle);
    }

    void GLTexture::setSampler(GLSampler* sampler)
    {
        ASSERT(sampler, "GLTexture: Attempt to pass NULL sampler to texture %s", mName.getChars());
        mSampler = sampler;
    }

    GLSampler* GLTexture::getSampler() const
    {
        return mSampler;
    }

    UINT32 GLTexture::getHandle() const
    {
        return mHandle;
    }

    UINT32 GLTexture::getWidth() const
    {
        return mWidth;
    }

    UINT32 GLTexture::getHeight() const
    {
        return mHeight;
    }

    void   GLTexture::getSize(UINT32& width, UINT32& height) const
    {
        width = mWidth;
        height = mHeight;
    }

    bool GLTexture::isMipMapsGenerated() const
    {
        return mGenMipMaps;
    }

    bool GLTexture::isLoaded() const
    {
        return (mHandle != 0);
    }

    const bool GLTexture::operator == (const GLTexture &texture) const
    {
        return mName == texture.mName;
    }

} // namespace Berserk