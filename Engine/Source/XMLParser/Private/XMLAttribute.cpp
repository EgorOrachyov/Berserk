//
// Created by Egor Orachyov on 08.03.2019.
//

#include "Base/XMLAttribute.h"

namespace Berserk
{

    XMLAttribute::XMLAttribute(rapidxml::xml_attribute<> *attribute)
    {
        mAttribute = attribute;
    }

    XMLAttribute XMLAttribute::getNext()
    {
        return XMLAttribute(mAttribute->next_attribute());
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