//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_ITEXTURE_H
#define BERSERK_ITEXTURE_H

#include "Public/Misc/Bits.h"
#include "Public/Misc/Types.h"
#include "Public/Platform/ISampler.h"
#include "Public/Resource/IResource.h"
#include "Public/Misc/UsageDescriptors.h"

namespace Berserk
{

    class GRAPHICS_API ITexture : public IResource
    {
    public:

        enum TargetType : uint32
        {
            NORMAL_MAP          = SHIFT(0),
            AMBIENT_MAP         = SHIFT(1),
            DIFFUSE_MAP         = SHIFT(2),
            SPECULAR_MAP        = SHIFT(3),
            REFLECTION_MAP      = SHIFT(4),
            DISPLACEMENT_MAP    = SHIFT(5)
        };

        enum TextureType : uint32
        {
            TEXTURE_1D,
            TEXTURE_2D,
            TEXTURE_3D
        };

        enum PixelFormat : uint32
        {
            R,
            RG,
            RGB,
            BGR,
            RGBA,
            ABGR,
            DEPTH,
            DEPTH_AND_STENCIL
        };

        enum PixelType : uint32
        {
            INT,
            BYTE,
            SHORT,
            FLOAT,
            HALF_FLOAT,
            UNSIGNED_INT,
            UNSIGNED_BYTE,
            UNSIGNED_SHORT
        };

        enum StorageFormat : uint32
        {
            RGB8                = SHIFT(0),
            RGBA8               = SHIFT(1),
            RGB32F              = SHIFT(2),
            RGBA32F             = SHIFT(3),
            DEPTH24             = SHIFT(4),
            DEPTH24_STENCIL8    = SHIFT(5),

            BYTE_SIZE_32  = RGB8 | RGBA8 | DEPTH24 | DEPTH24_STENCIL8,
            BYTE_SIZE_96  = RGB32F,
            BYTE_SIZE_128 = RGBA32F,
        };

    public:

        virtual void create(uint32 width, uint32 height, StorageFormat storage) = 0;

        virtual void create(uint32 width, uint32 height,
                            StorageFormat storage,
                            void* data,
                            PixelFormat format, PixelType pixelType,
                            bool genMipMaps) = 0;

        // virtual void bind(ISampler* sampler) = 0;

        virtual void bind(uint32 textureSlot) = 0;

        virtual void getData(uint32 depth, uint32 size, PixelType destination, void* data) = 0;

        virtual TargetType getTargetType() = 0;

        virtual PixelFormat getPixelFormat() = 0;

        virtual TextureType getTextureType() = 0;

        virtual StorageFormat getStorageFormat() = 0;

        virtual bool getMipMapsGen() = 0;

        virtual uint32 getHandle() = 0;

        virtual uint32 getWidth() = 0;

        virtual uint32 getHeight() = 0;

        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_ITEXTURE_H