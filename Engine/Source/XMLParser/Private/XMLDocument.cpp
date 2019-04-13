//
// Created by Egor Orachyov on 08.03.2019.
//

#include "XMLDocument.h"

namespace Berserk
{

    XMLDocument::XMLDocument(const char *filename, const char* ext)
    {
        FAIL(XML::exist(filename), "Cannot open file with name %s", filename);
        FAIL(XML::extension(filename, ext), "File with name %s has no .xml extension", filename);

        rapidxml::file<> xmlFile(filename);
        mDocument.parse<0>(xmlFile.data());
    }

    XMLNode XMLDocument::getNode(const char *name)
    {
        return XMLNode(mDocument.first_node(name));
    }

    XMLNode XMLDocument::getFirst()
    {
        return XMLNode(mDocument.first_node());
    }

    const char* XMLDocument::getData()
    {
        return mDocument.value();
    }

} // namespace Berserk
