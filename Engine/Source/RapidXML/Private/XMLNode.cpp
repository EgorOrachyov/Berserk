//
// Created by Egor Orachyov on 08.03.2019.
//

#include "XMLNode.h"
#include <Misc/Assert.h>

namespace Berserk
{

    XMLNode::XMLNode(rapidxml::xml_node<> *node, rapidxml::xml_document<> *document)
    {
        mNode = node;
        mDocument = document;
    }

    XMLNode XMLNode::getChild()
    {
        return XMLNode(mNode->first_node(), mDocument);
    }

    XMLNode XMLNode::getChild(const char *name)
    {
        return XMLNode(mNode->first_node(name), mDocument);
    }

    XMLNode XMLNode::getNext()
    {
        return XMLNode(mNode->next_sibling(), mDocument);
    }

    XMLNode XMLNode::getParent()
    {
        return XMLNode(mNode->parent(), mDocument);
    }

    XMLAttribute XMLNode::getAttribute(const char *name)
    {
        return XMLAttribute(mNode->first_attribute(name), mDocument);
    }

    XMLAttribute XMLNode::getFirst()
    {
        return XMLAttribute(mNode->first_attribute(), mDocument);
    }

    bool XMLNode::contains(const char *nodeName)
    {
        return mNode->first_node(nodeName) != nullptr;
    }

    bool XMLNode::isEmpty() const
    {
        return (mNode == nullptr);
    }

    void XMLNode::append(const Berserk::XMLNode &node)
    {
        assertion(node.mDocument == mDocument);
        mNode->append_node(node.mNode);
    }

    void XMLNode::append(const Berserk::XMLAttribute &attribute)
    {
        assertion(attribute.mDocument == mDocument);
        mNode->append_attribute(attribute.mAttribute);
    }

    const char* XMLNode::getName() const
    {
        return mNode->name();
    }

    const char* XMLNode::getValue() const
    {
        return mNode->value();
    }

} // namespace Berserk
