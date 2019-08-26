//
// Created by Egor Orachyov on 2019-07-23.
//

#ifndef BERSERK_FREEIMAGEIMPORTER_H
#define BERSERK_FREEIMAGEIMPORTER_H

#include <Importers/IPixelDataImporter.h>

namespace Berserk
{

    /** FreeImage load/save image importer implementation */
    class ENGINE_API FreeImageImporter final : public IPixelDataImporter
    {
    public:

        explicit FreeImageImporter(IAllocator& allocator = Allocator::get());

        ~FreeImageImporter() override;

        /** @copydoc IPixelDataImporter::importRaw() */
        TSharedPtr<PixelData> importRaw(const char *filename) override;

        /** @copydoc IPixelDataImporter::exportRaw() */
        bool exportRaw(const char *filename, const PixelData &image) override;

        /** @copydoc IPixelDataImporter::isReadingSupported() */
        bool isReadingSupported(const char *filename) override;

        /** @copydoc IPixelDataImporter::isWritingSupported() */
        bool isWritingSupported(const char *filename) override;

        /** @copydoc IPixelDataImporter::getSupportedReadingExtensions() */
        const TArray<String> &getSupportedReadingExtensions() const override;

    private:

        /** Internal allocator to allocate image data */
        IAllocator& mAllocator;

        /** Supported read extension */
        TArray<String> mExtensions;

    };

} // namespace Berserk

#endif //BERSERK_FREEIMAGEIMPORTER_H