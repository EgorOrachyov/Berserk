//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLTEXTURE_H
#define BERSERK_GLTEXTURE_H

#include "Platform/ITexture.h"
#include "Strings/StaticString.h"

namespace Berserk
{

    /**
     * OpengGL implementation for texture class
     */
    class GRAPHICS_API GLTexture : public ITexture
    {
    public:

        ~GLTexture() = default;

        /** @copydoc IResource::initialize() */
        void initialize(const char* name) override;

        /** @copydoc IResource::addReference() */
        void addReference() override;

        /** @copydoc IResource::release() */
        void release() override;

        /** @copydoc IResource::getReferenceCount() */
        uint32 getReferenceCount() override;

        /** @copydoc IResource::getMemoryUsage() */
        uint32 getMemoryUsage() override;

        /** @copydoc IResource::getName() */
        const char* getName() override;

    public:

        /** @copydoc ITexture::create(width,height,storageFormat) */
        void create(uint32 width,
                    uint32 height,
                    IRenderDriver::StorageFormat storageFormat) override;

        /** @copydoc ITexture::create(width,height,storageFormat,data,pixelFormat,pixelType,genMipMaps) */
        void create(uint32 width,
                    uint32 height,
                    IRenderDriver::StorageFormat storageFormat,
                    void *data,
                    IRenderDriver::PixelFormat pixelFormat,
                    IRenderDriver::PixelType pixelType,
                    bool genMipMaps) override;

        /** @copydoc ITexture::bind(sampler) */
        void bind(ISampler* sampler) override;

        /** @copydoc ITexture::bind(textureSlot) */
        void bind(uint32 textureSlot) override;

        /** @copydoc ITexture::getData() */
        void getData(uint32 depth, IRenderDriver::PixelFormat format, uint8 *data) override;

        /** @copydoc ITexture::getSize() */
        void getSize(uint32& width, uint32& height) override;

        /** @copydoc ITexture::getMipMapsGen() */
        bool getMipMapsGen() override;

        /** @copydoc ITexture::getTargetType() */
        TargetType getTargetType() override;

        /** @copydoc ITexture::getHandle() */
        uint32 getHandle() override;

        /** @copydoc ITexture::getWidth() */
        uint32 getWidth() override;

        /** @copydoc ITexture::getHeight() */
        uint32 getHeight() override;

        /** @copydoc ITexture::getGPUMemoryUsage() */
        uint32 getGPUMemoryUsage() override;

    private:

        TargetType mTargetType;
        uint32 mPixelFormat;
        uint32 mTextureType;
        uint32 mStorageFormat;

        uint32 mWidth;
        uint32 mHeight;
        bool mGenMipMaps;

        uint32 mTextureID;
        uint32 mReferenceCount;

        CName mResourceName;

    };

} // namespace Berserk

#endif //BERSERK_GLTEXTURE_H