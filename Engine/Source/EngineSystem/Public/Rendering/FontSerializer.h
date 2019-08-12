//
// Created by Egor Orachyov on 2019-08-12.
//

#ifndef BERSERK_FONTSERIALIZER_H
#define BERSERK_FONTSERIALIZER_H

#include <Serialization/ArchiveWriter.h>
#include <Rendering/Font.h>
#include <XMLDocument.h>

namespace Berserk
{

    /**
     * Abstract class used for serializing fonts resources
     * to specified binary/readable format.
     *
     * @note Font resource has internal data format and could
     *       be serialized at once for fast loading/reloading in run-time mode.
     *
     * @note Preferred format is binary, however xml files could be
     *       read and observed for some debug purposes.
     */
    class ENGINE_API FontSerializer
    {
    public:

        virtual ~FontSerializer() = default;

        /**
         * Serialize font resource in some output file
         * @param writer Binary archive to write this font resource
         * @param font Font resource to serialize
         */
        virtual void serialize(ArchiveWriter& writer, Font& font) { };

        /**
         * Serialize font resource in some output file
         * @param document XML document to write this font resource
         * @param font Font resource to serialize
         */
        virtual void serialize(XMLDocument& document, Font& font) { };

    };

} // namespace Berserk

#endif //BERSERK_FONTSERIALIZER_H
