//
// Created by Egor Orachyov on 2019-07-23.
//

#ifndef BERSERK_FREEIMAGEIMPORTER_H
#define BERSERK_FREEIMAGEIMPORTER_H

#include <Importers/IImageImporter.h>

namespace Berserk
{

    /** FreeImage load/save image importer implementation */
    class FreeImageImporter final : public IImageImporter
    {
    public:

        explicit FreeImageImporter(IAllocator& allocator = Allocator::get());

        ~FreeImageImporter() override;

        TSharedPtr<ImageData> load(const char *filename) override;

        bool save(const char *filename, const ImageData &image) override;

        bool isReadingSupported(const char *filename) override;

        bool isWritingSupported(const char *filename) override;

    private:

        /** Internal allocator to allocate image data */
        IAllocator& mAllocator;

    };

} // namespace Berserk

#endif //BERSERK_FREEIMAGEIMPORTER_H