//
// Created by Egor Orachyov on 08.03.2019.
//

#include "XMLDocument.h"
#include <Misc/Assert.h>

namespace Berserk
{

    XMLDocument::XMLDocument(const char *filename)
    : mDocumentName(filename)
    {
        assertion(filename);

        rapidxml::file<> xmlFile(filename);
        mDocument.parse<0>(xmlFile.data());
    }

    XMLNode XMLDocument::getNode(const char *name)
    {
        return XMLNode(mDocument.first_node(name), &mDocument);
    }

    XMLNode XMLDocument::getFirst()
    {
        return XMLNode(mDocument.first_node(), &mDocument);
    }

    XMLNode XMLDocument::createNode(const char *name)
    {
        char* nodeName = mDocument.allocate_string(name);
        auto node = mDocument.allocate_node(rapidxml::node_element, nodeName);

        return XMLNode(node, &mDocument);
    }

    XMLAttribute XMLDocument::createAttribute(const char *name, const char *value)
    {
        char* attributeName = mDocument.allocate_string(name);
        char* attributeValue = mDocument.allocate_string(value);
        auto attribute = mDocument.allocate_attribute(attributeName, attributeValue);

        return XMLAttribute(attribute, &mDocument);
    }

    bool XMLDocument::contains(const char *nodeName)
    {
        return mDocument.first_node(nodeName) != nullptr;
    }

    void XMLDocument::append(const Berserk::XMLNode &node)
    {
        assertion(node.mDocument == &mDocument);
        mDocument.append_node(node.mNode);
    }

} // namespace Berserk
