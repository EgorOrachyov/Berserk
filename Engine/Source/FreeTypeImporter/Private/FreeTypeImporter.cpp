//
// Created by Egor Orachyov on 17.04.2019.
//

#include "FreeTypeImporter.h"

namespace Berserk::Importers
{

    FreeTypeImporter::FreeTypeImporter()
    {
        auto error = FT_Init_FreeType(&mLibrary);

        if (error)
        {
            FAIL(false, "Cannot initialize FreeType library");
        }
    }

    FreeTypeImporter::~FreeTypeImporter()
    {
        FT_Done_FreeType(mLibrary);
    }

    bool FreeTypeImporter::import(const char *name, Resources::IFont *font)
    {

    }

    uint32 FreeTypeImporter::getMemoryUsage()
    {
        return 0;
    }

} // namespace Berserk::Importers