//
// Created by Egor Orachyov on 2019-08-26.
//

#ifndef BERSERK_TEXTUREIMPORTER_H
#define BERSERK_TEXTUREIMPORTER_H

#include <Importers/IResourceImporter.h>
#include <Importers/IPixelDataImporter.h>
#include <RHI/RHIDriver.h>

namespace Berserk
{

    /** Import Texture2D resource based on pixel data from pixel data importer */
    class ENGINE_API TextureImporter final : public IResourceImporter
    {
    public:

        /**
         * Initialize texture importer
         * @param driver Driver to create texture 2D RHI resource
         * @param importer To load raw pixel data
         * @param allocator To allocate textures
         */
        explicit TextureImporter(RHIDriver &driver, IPixelDataImporter &importer,
                                 IAllocator &allocator = Allocator::get());

        ~TextureImporter() override;

        /** @copydoc IResourceImporter::isExtensionSupported() */
        bool isExtensionSupported(const char *extension) const override;

        /** @copydoc IResourceImporter::import() */
        TSharedPtr<Resource> import(const String &path, const ImportOptions &options) override;

    private:

        typedef const TArray<String> ExtensionsList;

        RHIDriver& mDriver;
        IAllocator& mAllocator;
        ExtensionsList& mExtensions;
        IPixelDataImporter& mImporter;

    };

} // namespace Berserk

#endif //BERSERK_TEXTUREIMPORTER_H