//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_XMLDOCUMENT_H
#define BERSERK_XMLDOCUMENT_H

#include "Public/Misc/Assert.h"
#include "Public/Misc/Include.h"
#include "Public/XML/XMLNode.h"
#include "Public/XML/XMLUtility.h"
#include "RapidXML/rapidxml_utils.hpp"

namespace Berserk
{

    class XMLDocument
    {
    public:

        explicit XMLDocument(const char* filename);

        ~XMLDocument() = default;

        XMLNode getNode(const char *name);

        XMLNode getFirst();

        const char* getData();

    private:

        rapidxml::xml_document<> mDocument;

    };

    XMLDocument::XMLDocument(const char *filename)
    {
        ASSERT(XML::exist(filename), "Cannot open file with name %s", filename);
        ASSERT(XML::extension(filename), "File with name %s has no .xml extension", filename);

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

#endif //BERSERK_XMLDOCUMENT_H
