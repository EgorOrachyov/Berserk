//
// Created by Egor Orachyov on 17.04.2019.
//

#ifndef BERSERK_IFONTIMPORTER_H
#define BERSERK_IFONTIMPORTER_H

#include <Misc/Types.h>
#include <Foundation/IFont.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk::Importers
{

#ifndef PROFILE_FONT_IMPORTER
    #define PROFILE_FONT_IMPORTER 0
#endif

    /**
     * Font importer for importing TrueType font into engine in runtime mode
     */
    class IO_API IFontImporter
    {
    public:

        /** Release importer and free resources if needed */
        virtual ~IFontImporter() = default;

        /**
         * Loads font into font resource
         * @param name Full file name to the font file
         * @param font Pointer to allocated font to load
         *
         * @return True if font successfully loaded
         */
        virtual bool import(const char *name, uint32 pixelSize, Resources::IFont *font) = 0;

        /** @return Memory cost of this resource (on CPU side only) */
        virtual uint32 getMemoryUsage() = 0;

    };

} // namespace Berserk::Importers

#endif //BERSERK_IFONTIMPORTER_H