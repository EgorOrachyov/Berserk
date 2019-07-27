//
// Created by Egor Orachyov on 2019-07-23.
//

#ifndef BERSERK_IFONTIMPORTER_H
#define BERSERK_IFONTIMPORTER_H

#include <Rendering/Font.h>

namespace Berserk
{

    /** Loads various TTF fonts in runtime mode */
    class IFontImporter
    {
    public:

        virtual ~IFontImporter() = default;

        /**
         * Import single font of specified size from TTF file
         * @param filename Full file name and OS path
         * @param fontSize Size of the font (height)
         * @return Loaded font object or null, if there is some kind of error
         */
        virtual TSharedPtr<Font> load(const char* filename, uint32 fontSize) = 0;

    };

} // namespace Berserk

#endif //BERSERK_IFONTIMPORTER_H