//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_XMLNODE_H
#define BERSERK_XMLNODE_H

#include "XMLAttribute.h"
#include "RapidXML/rapidxml_utils.hpp"

namespace Berserk
{

    /**
     * Wrapper for RapidXML node
     */
    class XMLNode
    {
    private:

        friend class XMLDocument;

        explicit XMLNode(rapidxml::xml_node<> *node);

    public:

        ~XMLNode() = default;

        /** @return */
        XMLNode getChild();

        /**
         * @param name Name of child node to be found
         * @return Child node with name 'name'
         */
        XMLNode getChild(const char* name);

        /** @return Next node (or neighbor) of that */
        XMLNode getNext();

        /** @return Node parent if that node has it */
        XMLNode getParent();

        /**
         * @param name Name of the attribute
         * @return Attribute with name 'name' (need explicit check for isEmpty)
         */
        XMLAttribute getAttribute(const char *name);

        /** @return First attribute of that node */
        XMLAttribute getFirst();

        /** @return Name of the node */
        const char* getName() const;

        /** @return Pointer to text value if that node has it */
        const char* getValue() const;

        /** @return True is node is null */
        bool isEmpty() const;

    private:

        rapidxml::xml_node<> *mNode;

    };

} // namespace Berserk

#endif //BERSERK_XMLNODE_H
