//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_XMLATTRIBUTE_H
#define BERSERK_XMLATTRIBUTE_H

#include <Misc/UsageDescriptors.h>
#include <RapidXML/rapidxml_utils.hpp>

namespace Berserk
{

    /**
     * Wrapper for RapidXML attribute
     */
    class CORE_EXPORT XMLAttribute
    {
    public:

        /** Construct attribute from document attribute pinter */
        explicit XMLAttribute(rapidxml::xml_attribute<> *attribute, rapidxml::xml_document<> *document);

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

        friend class XMLNode;

        /** Rapid xml document attribute pointer */
        rapidxml::xml_attribute<> *mAttribute;

        /** Stores actual xml document owner */
        rapidxml::xml_document<> *mDocument;

    };

} // namespace Berserk

#endif //BERSERK_XMLATTRIBUTE_H
