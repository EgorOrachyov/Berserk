//
// Created by Egor Orachyov on 17.04.2019.
//

#ifndef BERSERK_FREETYPEIMPORTER_H
#define BERSERK_FREETYPEIMPORTER_H

#include <Importers/IFontImporter.h>    // Note to add include_directories for freetype
#include <ft2build.h>                   // because of relative include problems in frre type lig:
#include FT_FREETYPE_H                  // <freetype/...> - so: folder freetype should be included in search paths

namespace Berserk::Importers
{

    /**
     * FreeType library based font importer
     */
    class IO_API FreeTypeImporter : public IFontImporter
    {
        /** Want to manually allocate this manager */
        GENARATE_NEW_DELETE(FreeImageImporter);

        /** Initialize free type library */
        FreeTypeImporter();

        /** @copydoc IFontImporter::IFontImporter() */
        ~FreeTypeImporter() override;

        /** @copydoc IFontImporter::import() */
        bool import(const char* name, Resources::IFont* font) override;

        /** @copydoc IFontImporter::getMemoryUsage() */
        uint32 getMemoryUsage() override;
        
    };

} // namespace Berserk::Importers

#endif //BERSERK_FREETYPEIMPORTER_H