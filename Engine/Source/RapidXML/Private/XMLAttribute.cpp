//
// Created by Egor Orachyov on 08.03.2019.
//

#include "XMLAttribute.h"
#include <Misc/Assert.h>

namespace Berserk
{

    XMLAttribute::XMLAttribute(rapidxml::xml_attribute<> *attribute, rapidxml::xml_document<> *document)
    {
        mAttribute = attribute;
        mDocument = document;
    }

    XMLAttribute XMLAttribute::getNext()
    {
        return XMLAttribute(mAttribute->next_attribute(), mDocument);
    }

    const char* XMLAttribute::getName() const
    {
        return mAttribute->name();
    }

    const char* XMLAttribute::getValue() const
    {
        return mAttribute->value();
    }

    bool XMLAttribute::isEmpty() const
    {
        return (mAttribute == nullptr);
    }

} // namespace Berserk