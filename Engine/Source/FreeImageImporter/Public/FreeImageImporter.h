//
// Created by Egor Orachyov on 22.02.2019.
//

#ifndef BERSERK_FREEIMAGEIMPORTER_H
#define BERSERK_FREEIMAGEIMPORTER_H

#include "ImageImporter/ImageImporter.h"

namespace Berserk
{

    class FreeImageImporter : public ImageImporter
    {
    public:

        /** Initialize internal importer core */
        void initialize() override;

        /** Release import and free resources if needed */
        void release() override;

        /** @return True if extension is supported by impoter */
        bool isExtensionSupported(const char* extension) override;

        /** @return True if image successfully loaded and writes its data in image data structure */
        bool import(const char* name, ImageData& data) override;

        /** @return True if successfully save image from data */
        bool export(const char* name, const ImageData& data) override;

    private:

        void* mBitmap;

    };

} // namespace Berserk

#endif //BERSERK_FREEIMAGEIMPORTER_H