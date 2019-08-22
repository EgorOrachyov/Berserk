//
// Created by Egor Orachyov on 2019-08-12.
//

#ifndef BERSERK_FONTSERIALIZERXML_H
#define BERSERK_FONTSERIALIZERXML_H

#include <Rendering/FontSerializer.h>

namespace Berserk
{

    /** XML format file font serializer */
    class ENGINE_API FontSerializerXML final : public FontSerializer
    {
    public:

        ~FontSerializerXML() override = default;

        void serialize(XMLDocument &document, Font &font, const String &bitmapFilename) override;

    };

} // namespace Berserk

#endif //BERSERK_FONTSERIALIZERXML_H