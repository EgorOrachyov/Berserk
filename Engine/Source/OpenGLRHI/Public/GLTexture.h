//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_GLTEXTURE_H
#define BERSERK_GLTEXTURE_H

#include <RHI/RHITexture2D.h>
#include <GLCommon.h>
#include <GLEnums.h>

namespace Berserk
{

    template <typename RHIBaseType, GLenum textureType>
    class GLTexture : public RHIBaseType
    {
    public:

        void bind(uint32 textureSlot) const override
        {
            // !!! WARNING !!!
            //
            // glBindSampler(textureSlot, 0)
            //
            // Unbind any attached sampler object to this
            // texture slot. If there was any sampler binded,
            // our color layer would get wrong filtration properties,
            // therefore sampler2D would not work properly in shader.

            glBindSampler(textureSlot, 0);
            glActiveTexture(GL_TEXTURE0 + textureSlot);
            glBindTexture(textureType, mResourceID);
        }

        void bind(uint32 textureSlot, const RHISamplerRef &sampler) const override
        {
            sampler->bind(textureSlot);
            glActiveTexture(GL_TEXTURE0 + textureSlot);
            glBindTexture(textureType, mResourceID);
        }

        bool isMipmapsUsed() const override
        {
            return mIsMipmapsUsed;
        }

        EStorageFormat getStorageFormat() const override
        {
            return mStorageFormat;
        }

        template <typename RHIBaseTypeRef, typename GLType>
        static GLuint getResourceID(const RHIBaseTypeRef& ref)
        {
            GLuint id = ((GLType*)ref.pointer())->mResourceID;
            return id;
        }

    protected:

        static const GLenum TEXTURE_TYPE = textureType;

        GLuint mResourceID = 0;
        bool mIsMipmapsUsed;
        EStorageFormat mStorageFormat;

    };

    class GLTexture2D : public GLTexture<RHITexture2D, GL_TEXTURE_2D>
    {
    public:

        GLTexture2D(uint32 width,
                    uint32 height,
                    EStorageFormat storageFormat);

        GLTexture2D(uint32 width,
                    uint32 height,
                    EStorageFormat storageFormat,
                    EPixelFormat pixelFormat,
                    EDataType dataType,
                    uint8* data,
                    bool genMipMaps,
                    GLenum gl_storageFormat,
                    GLenum gl_pixelFormat,
                    GLenum gl_dataType);

        ~GLTexture2D() override;

        void readData(EPixelFormat format, EDataType type, uint8 *destBuffer) const override;

        void readData(EPixelFormat format, EDataType type, uint32 lod, uint8 *destBuffer) const override;

        uint32 getWidth() const override;

        uint32 getHeight() const override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

    private:

        uint32 mWidth;
        uint32 mHeight;

    };

} // namespace Berserk

#endif //BERSERK_GLTEXTURE_H
