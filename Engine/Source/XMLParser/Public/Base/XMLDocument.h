//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_XMLDOCUMENT_H
#define BERSERK_XMLDOCUMENT_H

#include "Misc/Assert.h"
#include "Misc/Include.h"
#include "XMLNode.h"
#include "Utility/XMLUtility.h"
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
    private:

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

        /**
         * Creates and parses xml document from the source string
         * @param source String with xml data
         * @param size   Length of the string (optional)
         */
        // explicit XMLDocument(const char* source, uint32 size);

        /**
         * Creates nex file with filename and uses this file as nex xml document
         * @param filename Name of (full name) document ot create
         */
        // explicit XMLDocument(const char* filename);

        ~XMLDocument() = default;

        /**
         * @param name Name of node in the document
         * @return Node with name 'name' (need explicit check for isEmpty)
         */
        XMLNode getNode(const char *name);

        /** @return First nod in doc structure tree */
        XMLNode getFirst();

        /** Creates new node for the document */
        // XMLNode createNode(const char* name, const char* value = 0);

        /** Creates new attribute for the document */
        // XMLAttribute createAttribute(const char* name, const char* value);

        /** Inserts node in the end of the document */
        // void insert(XMLNode source);

        /** Inserts node as child for node 'where' */
        // void insert(XMLNode where, XMLNode node);

        /** Inserts attribute as neighbor for attribute 'where'  */
        // void insert(XMLAttribute where, XMLAttribute attribute);

        /** @return Pointer to the content of the parsed document */
        const char* getData();

    private:

        rapidxml::xml_document<> mDocument;

    };

} // namespace Berserk

#endif //BERSERK_XMLDOCUMENT_H
