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

    /**
     * Abstract texture class to handle gpu texture resources
     */
    class GRAPHICS_API ITexture : public IResource
    {
    public:

        /** Specify material layer */
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

        /**
         * Creates empty texture buffer
         * @param width Buffer width
         * @param height Buffer height
         * @param storageFormat Internal format for storing one pixel of texture
         */
        virtual void create(uint32 width,
                            uint32 height,
                            IRenderDriver::StorageFormat storageFormat) = 0;

        /**
         * Creates texture from pointer to data array with texture pixels
         * @param width Buffer width
         * @param height Buffer height
         * @param storageFormat Internal format for storing one pixel of texture
         * @param data Pointer to texture image pixels data
         * @param pixelFormat Format of pixels (order of color chanel)
         * @param pixelType Type of values in the array (common is unsigned byte)
         * @param genMipMaps Set in true to generate textures of different quality
         */
        virtual void create(uint32 width,
                            uint32 height,
                            IRenderDriver::StorageFormat storageFormat,
                            void *data,
                            IRenderDriver::PixelFormat pixelFormat,
                            IRenderDriver::PixelType pixelType,
                            bool genMipMaps) = 0;

        /** Attach samplet to the texture unit */
        virtual void bind(ISampler* sampler) = 0;

        /** Bind this texture to chosen texture slot */
        virtual void bind(uint32 textureSlot) = 0;

        /** Get image data in RGBA format for unsignet byte array */
        virtual void getData(uint32 depth, IRenderDriver::PixelFormat format, uint8 *data) = 0;

        /** Get texture default width and height for 0 level mip map */
        virtual void getSize(uint32& width, uint32& height) = 0;

        /** @return True if mip maps were generated */
        virtual bool getMipMapsGen() = 0;

        /** @return Get material target layer */
        virtual TargetType getTargetType() = 0;

        /** @return GPU handle for texture */
        virtual uint32 getHandle() = 0;

        /** @return Texture width */
        virtual uint32 getWidth() = 0;

        /** @return Texture height */
        virtual uint32 getHeight() = 0;

        /** @return Memory used at gpu side */
        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_ITEXTURE_H