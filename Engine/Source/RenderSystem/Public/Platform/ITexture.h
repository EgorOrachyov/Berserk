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
#include "Platform/IRenderDriver.h"

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

        virtual void create(uint32 width,
                            uint32 height,
                            IRenderDriver::StorageFormat storageFormat) = 0;

        virtual void create(uint32 width,
                            uint32 height,
                            IRenderDriver::StorageFormat storageFormat,
                            void *data,
                            IRenderDriver::PixelFormat pixelFormat,
                            IRenderDriver::PixelType pixelType,
                            bool genMipMaps) = 0;

        virtual void bind(ISampler* sampler) = 0;

        virtual void bind(uint32 textureSlot) = 0;

        /** Get image data in RGBA format for unsignet byte array */
        virtual void getData(uint32 depth, IRenderDriver::PixelFormat format, uint8 *data) = 0;

        virtual TargetType getTargetType() = 0;

        virtual bool getMipMapsGen() = 0;

    public:

        virtual uint32 getHandle() = 0;

        virtual uint32 getWidth() = 0;

        virtual uint32 getHeight() = 0;

        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_ITEXTURE_H