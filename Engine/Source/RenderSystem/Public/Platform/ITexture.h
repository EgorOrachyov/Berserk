//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_ITEXTURE_H
#define BERSERK_ITEXTURE_H

#include "Misc/Bits.h"
#include "Misc/Types.h"
#include "Platform/ISampler.h"
#include "Resource/IResource.h"
#include "Misc/UsageDescriptors.h"

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

    public:

        virtual void create(uint32 width, uint32 height, uint32 storageFormat) = 0;

        virtual void create(uint32 width, uint32 height,
                            uint32 storageFormat,
                            void* data,
                            uint32 pixelFormat, uint32 pixelType,
                            bool genMipMaps) = 0;

        virtual void bind(ISampler* sampler) = 0;

        virtual void bind(uint32 textureSlot) = 0;

        virtual void getData(uint32 depth, uint32 size, uint32 pixelType, void* data) = 0;

    public:

        virtual TargetType getTargetType() = 0;

        virtual uint32 getPixelFormat() = 0;

        virtual uint32 getTextureType() = 0;

        virtual uint32 getStorageFormat() = 0;

        virtual bool getMipMapsGen() = 0;

    public:

        virtual uint32 getHandle() = 0;

        virtual uint32 getWidth() = 0;

        virtual uint32 getHeight() = 0;

        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_ITEXTURE_H