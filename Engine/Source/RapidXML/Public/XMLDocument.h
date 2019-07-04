//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_XMLDOCUMENT_H
#define BERSERK_XMLDOCUMENT_H


#include <Misc/Include.h>
#include <Strings/String.h>
#include <Containers/TIterator.h>

#include <XMLNode.h>
#include <RapidXML/rapidxml_utils.hpp>

namespace Berserk
{

    /**
     * Wrapper for RapidXML lib
     * Provides mechanism for work with xml files
     * @warning Does not allows to word more than 1 XMLDocument class object
     *          with one chosen xml file, because in time of parsing XMLDocument
     *          object modifies opened source file
     */
    class CORE_EXPORT XMLDocument
    {
    public:

        /**
         * Creates, opens, and parses xml file with name
         * @param filename Name of file with xml content
         */
        explicit XMLDocument(const char* filename);

        ~XMLDocument() = default;

        /** @return Node with name 'name' (need explicit check for isEmpty) */
        XMLNode getNode(const char *name);

        /** @return First nod in doc structure tree */
        XMLNode getFirst();

        /** @return Creates new node for the document */
        XMLNode createNode(const char* name);

        /** @return Creates new attribute for the document */
        XMLAttribute createAttribute(const char* name, const char* value);

        /** @return True whether node with this name presented in this document hierarchy */
        bool contains(const char* nodeName);

        /** Appends node in the end of the document */
        void append(const XMLNode& node);

    private:

        /** Name of the file with document */
        String mDocumentName;

        /** Stores actual xml document */
        rapidxml::xml_document<> mDocument;

        /** Current node for iterating */
        mutable rapidxml::xml_node<>* mCurrent = nullptr;

    };

} // namespace Berserk

#endif //BERSERK_XMLDOCUMENT_H
