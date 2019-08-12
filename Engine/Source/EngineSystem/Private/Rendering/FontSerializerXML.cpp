//
// Created by Egor Orachyov on 2019-08-12.
//

#include "Rendering/FontSerializerXML.h"

namespace Berserk
{

    void FontSerializerXML::serialize(XMLDocument &document, Font &font)
    {
        auto fontNode = document.createNode("font");

        document.append(fontNode);

        auto fontInfo = document.createNode("info");
        fontNode.append(fontInfo);

        auto fontName = document.createAttribute("name", font.getFontName().get());
        auto fontWidth = document.createAttribute("width", String::toString((int32) font.getWidth()).get());
        auto fontHeight = document.createAttribute("height", String::toString((int32) font.getHeight()).get());

        fontInfo.append(fontName);
        fontInfo.append(fontWidth);
        fontInfo.append(fontHeight);

        auto fontChars = document.createNode("chars");
        fontNode.append(fontChars);

        int32 charsCount = 0;

        for (auto character = font.getCharData().begin(); character != nullptr; character = font.getCharData().next())
        {
            if (character->codePoint >= 0 && character->codePoint <= 33) continue;

            charsCount += 1;

            auto fontChar = document.createNode("char");

            const char code[] = { character->codePoint , '\0' };
            auto charCode = document.createAttribute("code", code);

            auto charWidth = document.createAttribute("width", String::toString((int32) character->width).get());
            auto charHeight = document.createAttribute("height", String::toString((int32) character->height).get());

            auto charAdvanceX = document.createAttribute("advanceX", String::toString((int32) character->advanceX).get());
            auto charAdvanceY = document.createAttribute("advanceY", String::toString((int32) character->advanceY).get());

            auto charTexPosX = document.createAttribute("texX", String::toString((float32) character->texturePos.x).get());
            auto charTexPosY = document.createAttribute("texY", String::toString((float32) character->texturePos.y).get());

            auto charTexPosXW = document.createAttribute("texXW", String::toString((float32) character->texturePos.z).get());
            auto charTexPosYH = document.createAttribute("texXH", String::toString((float32) character->texturePos.w).get());

            fontChar.append(charCode);

            fontChar.append(charWidth);
            fontChar.append(charHeight);

            fontChar.append(charAdvanceX);
            fontChar.append(charAdvanceY);

            fontChar.append(charTexPosX);
            fontChar.append(charTexPosY);

            fontChar.append(charTexPosXW);
            fontChar.append(charTexPosYH);

            fontChars.append(fontChar);
        }

        auto fontCharsCount = document.createAttribute("count", String::toString(charsCount).get());
        fontChars.append(fontCharsCount);
    }
} // namespace Berserk