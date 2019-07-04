//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_THIRDPARTYTEST_H
#define BERSERK_THIRDPARTYTEST_H

#include "RapidXML/rapidxml_utils.hpp"
#include <iostream>

void RapidXMLTest()
{
    using namespace std;
    using namespace rapidxml;

    rapidxml::file<> xmlFile("../Engine/Config/Tests/Hero.xml");
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    cout << "Name of my first node is: " << doc.first_node()->name() << "\n";
    xml_node<> *node = doc.first_node("hero");
    cout << "Node hero has value " << node->value() << "\n";
    for (xml_attribute<> *attr = node->first_attribute();
         attr; attr = attr->next_attribute())
    {
        cout << "Node hero has attribute " << attr->name() << " ";
        cout << "with value " << attr->value() << "\n";
    }
}

#endif //BERSERK_THIRDPARTYTEST_H
