//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_XMLATTRIBUTE_H
#define BERSERK_XMLATTRIBUTE_H

#include "RapidXML/rapidxml_utils.hpp"

namespace Berserk
{

    /**
     * Wrapper for RapidXML attribute
     */
    class XMLAttribute
    {
    private:

        friend class XMLNode;

        explicit XMLAttribute(rapidxml::xml_attribute<> *attribute);

    public:

        ~XMLAttribute() = default;

        /** @return Next attribute after that in attributes of node */
        XMLAttribute getNext();

        /** @return Nam of the attribute */
        const char* getName() const;

        /** @return Value of the attribute */
        const char* getValue() const;

        /** @return True is attribute is null */
        bool isEmpty() const;

    private:

        rapidxml::xml_attribute<> *mAttribute;

    };

} // namespace Berserk

#endif //BERSERK_XMLATTRIBUTE_H
