//
// Created by Egor Orachyov on 2019-07-30.
//

#ifndef BERSERK_FREETYPEIMPORTER_H
#define BERSERK_FREETYPEIMPORTER_H

#include <Rendering/Font.h>
#include <Importers/IFontImporter.h>    // Note to add include_directories for freetype
#include <ft2build.h>                   // because of relative include problems in frre type lig:
#include FT_FREETYPE_H                  // <freetype/...> - so: folder freetype should be included in search paths

namespace Berserk
{

    /** FreeType lib base font importer implementation */
    class FreeTypeImporter final : public IFontImporter
    {
    public:

        /**
         * Initialize importer [and third-party lib]
         * @param allocator To allocate imported fonts and its internal data
         */
        FreeTypeImporter(IAllocator& allocator);

        ~FreeTypeImporter() override;

        TSharedPtr<Font> load(const char *filename, const char *fontname, uint32 fontSize) override;

    private:

        /** Allocate imported fonts */
        IAllocator& mAllocator;

        /** Private lib instance */
        FT_Library mLibrary;

    };

} // namespace Berserk

#endif //BERSERK_FREETYPEIMPORTER_H