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
        StringManager::StringInfo* empty = manager.emptyNode();
        manager.deleteNode(empty);
    }

    static void StringManagerTest2()
    {
        StringManager& manager = StringManager::getCharStringManager();
        StringManager::StringInfo* node = manager.createNode(65);
        manager.deleteNode(node);
    }

    static void StaticStringTest()
    {
        Name str1 = "Hello, world";
        Name str2 = " ! My name is Vasya....";
        Name str3 = str1 + str2;
        Name str4 = "... ";
        str4 += str3;

        printf("%s \n", str1.get());
        printf("%s \n", str2.get());
        printf("%s \n", str3.get());
        printf("%s \n", str4.get());
    }

    static void DynamicStringTest()
    {
        String str1("Apple");
        String str2("Stone");
        String str3("Blade");
        String str4("... ");
        String str5;
        String str6 = str1 + " " + str2;
        str4 += str1;
        str5 = str2;

        printf("%s \n", str1.get());
        printf("%s \n", str2.get());
        printf("%s \n", str3.get());
        printf("%s \n", str4.get());
        printf("%s \n", str5.get());
        printf("%s \n", str6.get());
    }

    static void run()
    {
        // StringManagerTest1();
        // StringManagerTest2();
        StaticStringTest();
        DynamicStringTest();
    }

};


#endif //BERSERK_STRINGSTEST_H
