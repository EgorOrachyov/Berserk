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

    class IO_API FreeTypeImporter
    {

    };

} // namespace Berserk::Importers

#endif //BERSERK_FREETYPEIMPORTER_H