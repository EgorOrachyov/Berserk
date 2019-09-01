//
// Created by Egor Orachyov on 2019-08-09.
//

#ifndef BERSERK_TEXTURE2D_H
#define BERSERK_TEXTURE2D_H

#include <Rendering/Texture.h>
#include <Rendering/PixelData.h>
#include <Resource/TSharedPtr.h>
#include <RHI/RHITexture2D.h>
#include <RHI/RHIDriver.h>
#include <Engine/EngineForward.h>

namespace Berserk
{

    /** Basic 2D image texture. Common texture type for materials */
    class ENGINE_API Texture2D final : public Texture
    {
    public:

        /**
         * Creates 2D texture, ready to use by RHI render system
         * @warning For debug only
         *
         * @param driver RHI driver to create actual texture resource for GPU [CPU]
         * @param pixelData Loaded image pixel data
         */
        Texture2D(RHIDriver &driver, const PixelDataRef& pixelData);

        /**
         * Creates 2D texture, ready to use by RHI render system
         * @warning For debug only
         *
         * @param driver RHI driver to create actual texture resource for GPU [CPU]
         * @param pixelData Loaded image pixel data
         * @param format Storage format for RHI driver side
         * @param genMipMaps True, to gen LOD for that texture
         * @param cacheData Copy texture data for CPU side
         */
        Texture2D(RHIDriver &driver, const PixelDataRef &pixelData, EStorageFormat format, bool genMipMaps, bool cacheData);

        ~Texture2D() override = default;

        /**
         * Shows whether pixel data for the texture is replicated (cached) on CPU (RAM) side
         * @return True if data replicated
         */
        bool getCacheOnCPU() const { return mPixelData.isPresent(); }

        /**
         * Image data needed for RHI to create texture reference
         * @return Image data of this texture
         */
        const PixelDataRef &getImageData() const { return mPixelData; }

        /**
         * RHI ready to use texture handler
         * @return Texture handler for RHI
         */
        const RHITexture2DRef &getRHITexture() const { return mResourceRHI; }

    private:

        /** Compute texture memory usage (called in constructor) */
        void _computeMemoryUsage(const PixelDataRef& data);

    private:

        PixelDataRef mPixelData;
        RHITexture2DRef mResourceRHI;

    };

} // namespace Berserk

#endif //BERSERK_TEXTURE2D_H