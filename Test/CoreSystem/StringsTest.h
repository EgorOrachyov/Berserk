//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_STRINGSTEST_H
#define BERSERK_STRINGSTEST_H

#include <Strings/String.h>

using namespace Berserk;

class StringsTest
{
public:

    static void StringManagerTest1()
    {
        StringManager& manager = StringManager::getCharStringManager();
        StringManager::StringInfo& empty = manager.emptyNode();
        manager.deleteNode(empty);
    }

    static void StringManagerTest2()
    {
        StringManager& manager = StringManager::getCharStringManager();
        StringManager::StringInfo& node = manager.createNode(65);
        manager.deleteNode(node);
    }

    static void StaticStringTest()
    {
        
    }

    static void run()
    {
        // StringManagerTest1();
        // StringManagerTest2();
    }

};


#endif //BERSERK_STRINGSTEST_H
