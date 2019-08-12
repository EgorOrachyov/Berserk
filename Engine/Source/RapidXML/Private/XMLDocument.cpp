//
// Created by Egor Orachyov on 08.03.2019.
//

#include "XMLDocument.h"
#include <Misc/Assert.h>
#include <IO/PlatformFile.h>
#include <RapidXML/rapidxml_print.hpp>
#include <RapidXML/rapidxml_utils.hpp>

#include <iostream>

namespace Berserk
{

    XMLDocument::XMLDocument(const char *filename)
        : mDocumentName(filename)
    {
        assertion(filename);

        PlatformFile file(filename);
        uint32 size = file.size() + 1;
        mBuffer = (char*) Allocator::get().allocate(size);
        file.read(mBuffer, size);
        mBuffer[size] = '\0';

        mDocument.parse<0>(mBuffer);
    }

    XMLDocument::XMLDocument(const char *filename, char *fileContent)
        : mDocumentName(filename)
    {
        assertion(filename);
        assertion(fileContent);

        mDocument.parse<0>(fileContent);
    }

    XMLDocument::~XMLDocument()
    {
        if (mBuffer)
        {
            Allocator::get().free(mBuffer);
            mBuffer = nullptr;
        }
    }

    XMLDocument::XMLDocument()
    {

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

    XMLNode XMLDocument::createComment(const char *comment)
    {
        char* commentStr = mDocument.allocate_string(comment);
        auto node = mDocument.allocate_node(rapidxml::node_comment, commentStr);

        return XMLNode(node, &mDocument);
    }

    XMLNode XMLDocument::createDeclaration(const char *version, const char *encoding)
    {
        auto node = mDocument.allocate_node(rapidxml::node_declaration);
        auto versionNode = this->createAttribute("version", version);
        auto encodingNode = this->createAttribute("encoding", encoding);

        XMLNode declaration(node, &mDocument);
        declaration.append(versionNode);
        declaration.append(encodingNode);

        return declaration;
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
    void XMLDocument::save(const char *filename)
    {
        std::ofstream file(filename);
        assertion_msg(file.is_open(), "XMLDocument: cannot open [filename: '%s']", filename);
        file << mDocument;
    }

} // namespace Berserk
