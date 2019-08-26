//
// Created by Egor Orachyov on 2019-08-26.
//

#ifndef BERSERK_TEXTUREIMPORTOPTIONS_H
#define BERSERK_TEXTUREIMPORTOPTIONS_H

#include <Importers/ImportOptions.h>
#include <Rendering/Definitions.h>

namespace Berserk
{

    /** Import options to load an arbitrary texture */
    class ENGINE_API TextureImportOptions final : public ImportOptions
    {
    public:

        TextureImportOptions() = default;

        ~TextureImportOptions() override = default;

        /** Set prefered interanl texture storage format */
        void setStorageFormat(EStorageFormat format) { mStorageFormat = format; }

        /** Set caching texture pixel data on CPU RAM side. Duplicates texture data. */
        void setCacheOnCPU(bool cache) { mCacheOnCPU = cache; }

        /** Set gen mip maps for the texture */
        void setGenMipmaps(bool gen) { mGenMipmaps = gen; }

        /** @return Texture storage format */
        EStorageFormat getStorageFormat() const { return mStorageFormat; }

        /** @return Cache data or not on CPU side */
        bool getCacheOnCPU() const { return mCacheOnCPU; }

        /** @return Gen mip maps for the texture */
        bool getGenMipmap() const { return mGenMipmaps; }

    private:

        EStorageFormat mStorageFormat = EStorageFormat::SF_RGBA8;
        bool mCacheOnCPU = false;
        bool mGenMipmaps = true;

    };

} // namespace Berserk

#endif //BERSERK_TEXTUREIMPORTOPTIONS_H