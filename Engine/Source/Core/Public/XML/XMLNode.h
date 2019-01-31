//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_XMLNODE_H
#define BERSERK_XMLNODE_H

#include "Public/XML/XMLAttribute.h"
#include "RapidXML/rapidxml_utils.hpp"

namespace Berserk
{

    class XMLNode
    {
    private:

        friend class XMLDocument;

        explicit XMLNode(rapidxml::xml_node<> *node);

    public:

        ~XMLNode() = default;

        XMLNode getChild();

        XMLNode getChild(const char* name);

        XMLNode getSibling(const char *name);

        XMLNode getNext();

        XMLAttribute getAttribute(const char *name);

        XMLAttribute getFirst();

        const char* getName() const;

        bool isEmpty() const;

    private:

        rapidxml::xml_node<> *mNode;

    };

    XMLNode::XMLNode(rapidxml::xml_node<> *node)
    {
        mNode = node;
    }

    XMLNode XMLNode::getChild()
    {
        return XMLNode(mNode->first_node());
    }

    XMLNode XMLNode::getChild(const char *name)
    {
        return XMLNode(mNode->first_node(name));
    }

    XMLNode XMLNode::getSibling(const char *name)
    {
        return XMLNode(mNode->next_sibling(name));
    }

    XMLNode XMLNode::getNext()
    {
        return XMLNode(mNode->next_sibling());
    }

    XMLAttribute XMLNode::getAttribute(const char *name)
    {
        return XMLAttribute(mNode->first_attribute(name));
    }

    XMLAttribute XMLNode::getFirst()
    {
        return XMLAttribute(mNode->first_attribute());
    }

    const char* XMLNode::getName() const
    {
        return mNode->name();
    }

    bool XMLNode::isEmpty() const
    {
        return (mNode == nullptr);
    }


} // namespace Berserk

#endif //BERSERK_XMLNODE_H
