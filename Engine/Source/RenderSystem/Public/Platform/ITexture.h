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

        /**
         * Creates empty texture buffer
         * @param width Buffer width
         * @param height Buffer height
         * @param storageFormat Internal format for storing one pixel of texture
         */
        virtual void create(uint32 width,
                            uint32 height,
                            IRenderDriver::StorageFormat storageFormat,
                            IRenderDriver::DataType dataType) = 0;

        /**
         * Creates texture from pointer to data array with texture pixels
         * @param width Buffer width
         * @param height Buffer height
         * @param storageFormat Internal format for storing one pixel of texture
         * @param pixelFormat Format of pixels (order of color chanel)
         * @param pixelType Type of values in the array (common is unsigned byte)
         * @param data Pointer to texture image pixels data
         * @param genMipMaps Set in true to generate textures of different quality
         */
        virtual void create(uint32 width,
                            uint32 height,
                            IRenderDriver::StorageFormat storageFormat,
                            IRenderDriver::PixelFormat pixelFormat,
                            IRenderDriver::DataType pixelType,
                            void *data,
                            bool genMipMaps) = 0;

        /** Attach samplet to the texture unit */
        virtual void bind(ISampler* sampler) = 0;

        /** Bind this texture to chosen texture slot */
        virtual void bind(uint32 textureSlot) = 0;

        /** Get image data in RGBA format for unsigned byte array */
        virtual void getData(uint32 depth,
                             IRenderDriver::PixelFormat format,
                             IRenderDriver::DataType type,
                             void *data) = 0;

        /** Get texture default width and height for 0 level mip map */
        virtual void getSize(uint32& width, uint32& height) = 0;

        /** !Only for frame buffers! Set filtering mode */
        virtual void setFiltering(IRenderDriver::SamplerFilter min, IRenderDriver::SamplerFilter mag) = 0;

        /** !Only for frame buffers! Set wrapping mode */
        virtual void setWrapping(IRenderDriver::SamplerWrapMode wrap) = 0;

        /** !Only for frame buffers! Set border color */
        virtual void setBorderColor(const Vec4f& color) = 0;

        /** @return True if mip maps were generated */
        virtual bool getMipMapsGen() = 0;

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