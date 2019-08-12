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
         * @note Use internal system allocation to allocate memory for buffer
         */
        explicit XMLDocument(const char *filename);

        /**
         * Parse file content as xml file
         * @param filename Name of file with xml content [for debug]
         * @param fileContent Buffer with file content terminated with '\0'
         * @note fileContent Buffer must exist in time of xml document usage
         */
        explicit XMLDocument(const char *filename, char* fileContent);

        /**
         * Creates empty xml document for modifying and write access.
         * Call this to create own xml files by code
         */
        XMLDocument();

        ~XMLDocument();

        /** @return Node with name 'name' (need explicit check for isEmpty) */
        XMLNode getNode(const char *name);

        /** @return First nod in doc structure tree */
        XMLNode getFirst();

        /** @return Creates new node for the document */
        XMLNode createNode(const char* name);

        /** @return Create comment node with comment text */
        XMLNode createComment(const char* comment);

        /** @return Create declaration header with specified version and encoding */
        XMLNode createDeclaration(const char* version, const char* encoding);

        /** @return Creates new attribute for the document */
        XMLAttribute createAttribute(const char* name, const char* value);

        /** @return True whether node with this name presented in this document hierarchy */
        bool contains(const char* nodeName);

        /** Appends node in the end of the document */
        void append(const XMLNode& node);

        /**
         * Sves document content (modyfied or not) to the specified file
         * @param filename File name to create (or rewrite) and save on disk
         */
        void save(const char* filename);

    private:

        /** Name of the file with document */
        String mDocumentName;

        /** Stores actual xml document */
        rapidxml::xml_document<> mDocument;

        /** XML file content buffer */
        char* mBuffer = nullptr;

    };

} // namespace Berserk

#endif //BERSERK_XMLDOCUMENT_H
