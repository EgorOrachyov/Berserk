//
// Created by Egor Orachyov on 2019-08-12.
//

#ifndef BERSERK_SERIALIZETEST_H
#define BERSERK_SERIALIZETEST_H

#include <FreeTypeImporter.h>
#include <Rendering/FontSerializerXML.h>

using namespace Berserk;

class SerializeTest
{
public:

    static void FontSerializeTest()
    {
        IAllocator& allocator = Allocator::get();
        FreeTypeImporter fontImporter(allocator);
        auto data = fontImporter.load("../Resources/Fonts/Arial.ttf", "Arial", 40);

        Font font(data->getFontName(), data->getWidth(), data->getWidth(), data->getCharData(), data->getBitmap());

        FontSerializerXML serializerXml;
        XMLDocument document;

        serializerXml.serialize(document, font);

        document.save("Arial.xml");
    }

    static void run()
    {
        FontSerializeTest();
    }


};


#endif //BERSERK_SERIALIZETEST_H
