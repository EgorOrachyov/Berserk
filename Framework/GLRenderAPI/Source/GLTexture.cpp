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
        mHandle = 0;
        mSlot = 0;
        mSampler = 0;
    }

    GLTexture::~GLTexture()
    {
        destroy();
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

        // without mip maps (only for buffered rendering)

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLTexture::create(UINT32 width, UINT32 height, GLInternalTextureFormat target,
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
        glTexImage2D(GL_TEXTURE_2D, 0, target, width, height, 0, source, actualType, data);

        if (gen == GLMipmaps::GLM_USE)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLTexture::destroy()
    {
        if (mHandle)
        {
            glDeleteTextures(1, &mHandle);
        }
        mHandle = 0;
        mSlot = 0;
        mSampler = 0;
    }

    void GLTexture::use() const
    {
        ASSERT(mHandle, "Texture is not created");

        glActiveTexture(GL_TEXTURE0 + mSlot);
        glBindTexture(GL_TEXTURE_2D, mHandle);
    }

    void GLTexture::setSlot(UINT16 slot)
    {
        ASSERT(mHandle, "Texture is not initialized");
        mSlot = slot;
    }

    void GLTexture::setSampler(UINT16 sampler)
    {
        ASSERT(mHandle, "Texture is not initialized");
        mSampler = sampler;
    }

    UINT32 GLTexture::getHandle() const
    {
        return mHandle;
    }

    UINT32 GLTexture::getSlot() const
    {
        return mSlot;
    }

    UINT32 GLTexture::getSampler() const
    {
        return mSampler;
    }


} // namespace Berserk