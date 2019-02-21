//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLTEXTURE_H
#define BERSERK_GLTEXTURE_H

#include "Platform/ITexture.h"
#include "Strings/StaticString.h"

namespace Berserk
{

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

        void create(uint32 width, uint32 height, StorageFormat storage) override;

        void create(uint32 width, uint32 height,
                    StorageFormat storage,
                    void* data,
                    PixelFormat format, PixelType pixelType,
                    bool genMipMaps) override;

        void bind(ISampler* sampler) override;

        void bind(uint32 textureSlot) override;

        void getData(uint32 depth, uint32 size, PixelType destination, void* data) override;

        TargetType getTargetType() override;

        PixelFormat getPixelFormat() override;

        TextureType getTextureType() override;

        StorageFormat getStorageFormat() override;

        bool   getMipMapsGen() override;

        uint32 getHandle() override;

        uint32 getWidth() override;

        uint32 getHeight() override;

        uint32 getGPUMemoryUsage() override;

    public:

        static uint32 getPixelFormat(PixelFormat format);

        static uint32 getTextureType(TextureType type);

        static uint32 getStorageFormat(StorageFormat format);

        static uint32 getPixelType(PixelType type);

    private:

        TargetType mTargetType;
        PixelFormat mPixelFormat;
        TextureType mTextureType;
        StorageFormat mStorageFormat;

        uint32 mWidth;
        uint32 mHeight;
        bool mGenMipMaps;

        uint32 mTextureID;
        uint32 mReferenceCount;

        ISampler* mSampler;

        CName mResourceName;

    };

} // namespace Berserk

#endif //BERSERK_GLTEXTURE_H