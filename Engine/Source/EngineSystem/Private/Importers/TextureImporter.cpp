//
// Created by Egor Orachyov on 2019-08-26.
//

#include "Importers/TextureImporter.h"
#include <Logging/DebugLogMacros.h>
#include <Rendering/TextureImportOptions.h>
#include <Rendering/Texture2D.h>

namespace Berserk
{

    TextureImporter::TextureImporter(Berserk::RHIDriver &driver, Berserk::IPixelDataImporter &importer,
                                     Berserk::IAllocator &allocator)
            : mDriver(driver),
              mAllocator(allocator),
              mExtensions(importer.getSupportedReadingExtensions()),
              mImporter(importer)
    {

    }

    TextureImporter::~TextureImporter()
    {
        DEBUG_LOG_DISPLAY("TextureImporter: destroy");
    }

    bool TextureImporter::isExtensionSupported(const char *extension) const
    {
        for (auto str = mExtensions.begin(); str != nullptr; str = mExtensions.next())
        {
            if (*str == extension) return true;
        }

        return false;
    }

    TSharedPtr<Resource> TextureImporter::import(const String &path, const ImportOptions &options)
    {
        auto* texOptions = dynamic_cast<const TextureImportOptions*>(&options);

        if (texOptions == nullptr)
        {
            DEBUG_LOG_ERROR("TextureImporter: invalid import options for [path: %s]", path.get());
            return TSharedPtr<Resource>();
        }

        TSharedPtr<PixelData> pixelData = mImporter.importRaw(path.get());

        auto texture = new(mAllocator.engine_alloc<Texture2D>()) Texture2D(
                    mDriver,
                    pixelData,
                    texOptions->getStorageFormat(),
                    texOptions->getGenMipmap(),
                    texOptions->getCacheOnCPU()
                    );

        texture->setName(path);

        return TSharedPtr<Resource>(texture, &mAllocator);
    }
} // namespace Berserk