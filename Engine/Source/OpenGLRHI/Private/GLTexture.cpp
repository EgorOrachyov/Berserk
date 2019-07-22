//
// Created by Egor Orachyov on 2019-07-15.
//

#include <GLResources.h>
#include <GLEnums.h>
#include <GLSizes.h>

namespace Berserk
{

    GLTexture2D::GLTexture2D(Berserk::uint32 width, Berserk::uint32 height, Berserk::EStorageFormat storageFormat)

         : GLTexture2D(
                 width,
                 height,
                 storageFormat,
                 PF_RGBA,
                 DT_UnsignedByte,
                 nullptr,
                 false,
                 GLEnums::StorageFormat(storageFormat),
                 GL_RGBA,
                 GL_UNSIGNED_BYTE)
    {

    }

    GLTexture2D::GLTexture2D(
            Berserk::uint32 width,
            Berserk::uint32 height,
            Berserk::EStorageFormat storageFormat,
            Berserk::EPixelFormat pixelFormat,
            Berserk::EDataType dataType,
            uint8* data,
            bool genMipMaps,
            GLenum gl_storageFormat,
            GLenum gl_pixelFormat,
            GLenum gl_dataType)

         : mWidth(width),
           mHeight(height)
    {
        mIsMipmapsUsed = genMipMaps;
        mStorageFormat = storageFormat;

        glGenTextures(1, &mResourceID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(TEXTURE_TYPE, mResourceID);
        glTexImage2D(TEXTURE_TYPE, 0, gl_storageFormat, width, height, 0, gl_pixelFormat, gl_dataType, data);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        if (genMipMaps) glGenerateMipmap(TEXTURE_TYPE);
        glBindTexture(TEXTURE_TYPE, 0);
    }

    GLTexture2D::~GLTexture2D()
    {
        if (mResourceID)
        {
            glDeleteTextures(1, &mResourceID);
            mResourceID = 0;
        }
    }

    void GLTexture2D::readData(EPixelFormat format, EDataType type, uint32 bufferSize, uint8 *destBuffer) const
    {
        readData(format, type, 0, bufferSize, destBuffer);
    }

    void GLTexture2D::readData(EPixelFormat format, EDataType type, uint32 lod, uint32 bufferSize, uint8 *destBuffer) const
    {
        auto gl_format = GLEnums::PixelFormat(format);
        auto gl_type = GLEnums::DataType(type);

        glBindTexture(TEXTURE_TYPE, mResourceID);
        glGetnTexImage(TEXTURE_TYPE, lod, gl_format, gl_type, bufferSize, destBuffer);
        glBindTexture(TEXTURE_TYPE, 0);
    }

    uint32 GLTexture2D::getWidth() const
    {
        return mWidth;
    }

    uint32 GLTexture2D::getHeight() const
    {
        return mHeight;
    }

    uint32 GLTexture2D::getMemoryUsage() const {
        return sizeof(GLTexture2D);
    }

    uint32 GLTexture2D::getMemoryUsage_GPU() const
    {
        float32 factor = (mIsMipmapsUsed ? 1.34f : 1.0f);
        uint32 size = GLSizes::getComponentSize(mStorageFormat);

        return (uint32) (factor * (float32) (mWidth * mHeight * size));
    }

} // namespace Berserk