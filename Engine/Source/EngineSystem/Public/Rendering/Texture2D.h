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
         * @param format Storage format for RHI driver side
         * @param genMipMaps True, to gen LOD for that texture
         */
        Texture2D(RHIDriverRef &driver, PixelDataRef pixelData, EStorageFormat format = SF_RGBA8, bool genMipMaps = true);

        ~Texture2D() override = default;

        /**
         * Image data needed for RHI to create texture reference
         * @return Image data of this texture
         */
        const PixelDataRef &getImageData() const { return mImageData; }

        /**
         * RHI ready to use texture handler
         * @return Texture handler for RHI
         */
        const RHITexture2DRef &getRHITexture() const { return mRHITexture; }

    private:

        PixelDataRef mImageData;
        RHITexture2DRef mRHITexture;

    };

} // namespace Berserk

#endif //BERSERK_TEXTURE2D_H