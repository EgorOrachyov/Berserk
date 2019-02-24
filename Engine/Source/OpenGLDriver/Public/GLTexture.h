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

        void create(uint32 width,
                    uint32 height,
                    IRenderDriver::StorageFormat storageFormat) override;

        void create(uint32 width,
                    uint32 height,
                    IRenderDriver::StorageFormat storageFormat,
                    void *data,
                    IRenderDriver::PixelFormat pixelFormat,
                    IRenderDriver::PixelType pixelType,
                    bool genMipMaps) override;

        void bind(ISampler* sampler) override;

        void bind(uint32 textureSlot) override;

        void getData(uint32 depth, IRenderDriver::PixelFormat format, uint8 *data) override;

    public:

        TargetType getTargetType() override;

        bool   getMipMapsGen() override;

    public:

        uint32 getHandle() override;

        uint32 getWidth() override;

        uint32 getHeight() override;

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

        ISampler* mSampler;

        CName mResourceName;

    };

} // namespace Berserk

#endif //BERSERK_GLTEXTURE_H