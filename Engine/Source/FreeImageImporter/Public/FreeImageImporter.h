//
// Created by Egor Orachyov on 22.02.2019.
//

#ifndef BERSERK_FREEIMAGEIMPORTER_H
#define BERSERK_FREEIMAGEIMPORTER_H

#include "Importers/IImageImporter.h"
#include "FreeImage/FreeImage.h"

namespace Berserk::Importers
{

    /**
     * Importer implementation that handles various import
     * for various image formats using the FreeImage library.
     */
    class IO_API FreeImageImporter : public IImageImporter
    {
    public:

        /** Want to manually allocate this manager */
        GENARATE_NEW_DELETE(FreeImageImporter);

        /** Initialize internal importer core */
        FreeImageImporter();

        /** Release importer and free resources if needed */
        ~FreeImageImporter() override;

        /** @return True if extension is supported by importer */
        bool isReadingSupported(const char *name) override;

        /** @return True if extension is supported by importer */
        bool isWritingSupported(const char *name) override;

        /** @return True if image successfully loaded and writes its data in image data structure */
        bool import(const char* name, ImageData& data) override;

        /** @return True if successfully save image from data (images should be in RGBA UBYTE format) */
        bool save(const char *name, const ImageSave &data) override;

        /** Unload all the internal data (if it was allocated by importer) */
        void unload() override;

        /** @return Memory cost of this resource (on CPU side only) */
        uint32 getMemoryUsage() override;

    private:

        FIBITMAP* mBitmap;

    };

} // namespace Berserk::Importers

#endif //BERSERK_FREEIMAGEIMPORTER_H