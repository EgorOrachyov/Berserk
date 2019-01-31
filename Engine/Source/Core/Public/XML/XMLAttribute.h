//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_XMLATTRIBUTE_H
#define BERSERK_XMLATTRIBUTE_H

#include "RapidXML/rapidxml_utils.hpp"

namespace Berserk
{

    class XMLAttribute
    {
    private:

        friend class XMLNode;

        explicit XMLAttribute(rapidxml::xml_attribute<> *attribute);

    public:

        ~XMLAttribute() = default;

        XMLAttribute getNext();

        const char* getName() const;

        const char* getValue() const;

        bool isEmpty() const;

    private:

        rapidxml::xml_attribute<> *mAttribute;

    };

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

#endif //BERSERK_XMLATTRIBUTE_H
