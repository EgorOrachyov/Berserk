//
// Created by Egor Orachyov on 2019-08-09.
//

#ifndef BERSERK_TEXTURE2D_H
#define BERSERK_TEXTURE2D_H

#include <Rendering/Texture.h>
#include <Rendering/ImageImportData.h>
#include <Resource/TSharedPtr.h>
#include <RHI/RHITexture2D.h>

namespace Berserk
{

    /** Basic 2D image texture. Common texture type for materials */
    class ENGINE_API Texture2D final : public Texture
    {
    public:

        Texture2D(TSharedPtr<ImageImportData> image, bool genMipMaps = true);

        Texture2D(TSharedPtr<ImageImportData> image, EStorageFormat storageFormat, bool genMipMaps = true);

        ~Texture2D() override = default;

    private:

        TSharedPtr<ImageImportData> mImageData;
        RHITexture2DRef mRHITexture;

    };

} // namespace Berserk

#endif //BERSERK_TEXTURE2D_H