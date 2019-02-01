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

    /**
     * Wrapper for RapidXML lib
     * Provides mechanism for work with xml files
     * @warning Does not allows to word more than 1 XMLDocument class object
     *          with one chosen xml file, because in time of parsing XMLDocument
     *          object modifies opened source file
     */
    class XMLDocument
    {
    public:

        enum XMLType
        {
            Node = rapidxml::node_type::node_element,
            Document = rapidxml::node_type::node_document
        };

    public:

        /**
         * Creates, opens, and parses xml file with name
         * @warning Requires file to has correct extension (ASSERT fail)
         * @warning Requires file to be (ASSERT fail)
         * @param filename Name of file with xml content
         * @param ext Extension of the file (example: '.xml')
         */
        explicit XMLDocument(const char* filename, const char* ext);

        explicit XMLDocument(const char* source, uint32 size);

        explicit XMLDocument(const char* filename);

        ~XMLDocument() = default;

        /**
         * @param name Name of node in the document
         * @return Node with name 'name' (need explicit check for isEmpty)
         */
        XMLNode getNode(const char *name);

        /** @return First nod in doc structure tree */
        XMLNode getFirst();

        XMLNode createNode(XMLType node, const char* name, const char* value);

        XMLAttribute createAttribute(const char* name, const char* value);

        void insert(XMLNode source);

        void insert(XMLNode where, XMLNode node);

        void insert(XMLAttribute where, XMLAttribute attribute);

        /** @return Pointer to the content of the parsed document */
        const char* getData();

    private:

        rapidxml::xml_document<> mDocument;

    };

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

#endif //BERSERK_XMLDOCUMENT_H
