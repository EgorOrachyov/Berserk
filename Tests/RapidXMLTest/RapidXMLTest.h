//
// Created by Egor Orachyov on 2019-07-05.
//

#ifndef BERSERK_RAPIDXMLTEST_H
#define BERSERK_RAPIDXMLTEST_H

#include <XMLDocument.h>

using namespace Berserk;

class RapidXMLTest
{
public:

    static void XMLTest1()
    {
        XMLDocument doc;

        {
            auto entity = doc.createNode("entity");
            auto name = doc.createAttribute("name", "MyFirstEntity");
            auto script = doc.createAttribute("script", "behaviour.lua");

            entity.append(name);
            entity.append(script);

            doc.append(entity);
        }

        doc.save("meta.xml");

    }

    static void XMLTest2()
    {
        XMLDocument doc("meta.xml");

        auto entity = doc.getFirst();
        for (auto attribute = entity.getFirst(); !attribute.isEmpty(); attribute = attribute.getNext())
        {
            OutputDevice::printf("name: %s value: %s \n", attribute.getName(), attribute.getValue());
        }
    }

    static void run()
    {
        //XMLTest1();
        //XMLTest2();
    }


};


#endif //BERSERK_RAPIDXMLTEST_H
