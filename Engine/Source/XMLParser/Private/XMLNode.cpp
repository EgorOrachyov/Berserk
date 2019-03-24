//
// Created by Egor Orachyov on 08.03.2019.
//

#include "Base/XMLNode.h"

namespace Berserk
{

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

    XMLNode XMLNode::getNext()
    {
        return XMLNode(mNode->next_sibling());
    }

    XMLNode XMLNode::getParent()
    {
        return XMLNode(mNode->parent());
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

    const char* XMLNode::getValue() const
    {
        return mNode->value();
    }

    bool XMLNode::isEmpty() const
    {
        return (mNode == nullptr);
    }

} // namespace Berserk
