//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_XMLNODE_H
#define BERSERK_XMLNODE_H

#include <XMLAttribute.h>

namespace Berserk
{

    /**
     * Wrapper for RapidXML node
     */
    class CORE_EXPORT XMLNode
    {
    public:

        /** Construct node from document node pointer */
        explicit XMLNode(rapidxml::xml_node<> *node, rapidxml::xml_document<> *document);

        ~XMLNode() = default;

        /** @return First child node of this node */
        XMLNode getChild();

        /** @return Child node with name 'name' */
        XMLNode getChild(const char* name);

        /** @return Next node (or neighbor) of that */
        XMLNode getNext();

        /** @return Node parent if that node has it */
        XMLNode getParent();

        /** @return Attribute with name 'name' (need explicit check for isEmpty) */
        XMLAttribute getAttribute(const char *name);

        /** @return First attribute of that node */
        XMLAttribute getFirst();

        /** @return True whether node with this name presented in this node hierarchy */
        bool contains(const char* nodeName);

        /** @return True is attribute is null */
        bool isEmpty() const;

        /** Appends node to this node */
        void append(const XMLNode& node);

        /** Appends attribute to this node */
        void append(const XMLAttribute& attribute);

        /** @return Name of the node */
        const char* getName() const;

        /** @return Pointer to text value if that node has it */
        const char* getValue() const;

    private:

        friend class XMLDocument;

        /** Rapid xml document node pointer */
        rapidxml::xml_node<> *mNode;

        /** Stores actual xml document owner */
        rapidxml::xml_document<> *mDocument;



    };

} // namespace Berserk

#endif //BERSERK_XMLNODE_H
