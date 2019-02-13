//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLTEXTURE_H
#define BERSERK_GLTEXTURE_H

#include "Public/Platform/ITexture.h"
#include "Public/Resource/IResource.h"
#include "Public/Strings/StaticString.h"

namespace Berserk
{

    class GLTexture : public ITexture, public IResource
    {
    public:

        ~GLTexture() override = default;

        void initialize() override;

        void addReference() override;

        void release() override;

        uint32 getReferenceCount() override;

        uint32 getMemoryUsage() override;

        const char* getName() override;

    public:

        void   create(uint32 width, uint32 height, StorageFormat storage) override;

        void   create(uint32 width, uint32 height, StorageFormat storage, void* data, PixelFormat format, bool genMipMaps) override;

        void   bind(ISampler* sampler, uint32 textureSlot) override;

        void   getData(uint32 depth, uint32 size, PixelType destination, void* data) override;

        uint32 getTargetType() override;

        uint32 getPixelFormat() override;

        uint32 getTextureType() override;

        uint32 getStorageFormat() override;

        bool   getMipMapsGen() override;

        virtual uint32 getHandle() override;

        uint32 getWidth() override;

        uint32 getHeight() override;

        uint32 getGPUMemoryUsage() override;

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

        CName mResourceName;

    };

} // namespace Berserk

#endif //BERSERK_GLTEXTURE_H