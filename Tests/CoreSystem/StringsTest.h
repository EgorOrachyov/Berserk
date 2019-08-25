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
        Name str5 = str1.substring(4, 900);

        printf("%s \n", str1.get());
        printf("%s \n", str2.get());
        printf("%s \n", str3.get());
        printf("%s \n", str4.get());
        printf("%s \n", str5.get());
    }

    static void DynamicStringTest()
    {
        String str1("Apple");
        String str2("Stone");
        String str3("Blade");
        String str4("... ");
        String str5;
        String str6 = str1 + " " + str2;
        String str7 = str6.substring(3, 4);
        str4 += str1;
        str5 = str2;

        String empty;
        String name = "Name";
        String city = "Some city";
        String full = name + ": Vasya. He came" + " from " + city;

        empty = name;

        printf("%s %u \n", full.get(), full.length());

        printf("%s \n", str1.get());
        printf("%s \n", str2.get());
        printf("%s \n", str3.get());
        printf("%s \n", str4.get());
        printf("%s \n", str5.get());
        printf("%s \n", str6.get());

        printf("%s < %s = %i \n", str1.get(), str2.get(), str1 < str2);
        printf("%s > %s = %i \n", str1.get(), str2.get(), str1 > str2);
        printf("%s >= %s = %i \n", str1.get(), str1.get(), str1 >= str1);
        printf("%s <= %s = %i \n", str1.get(), str2.get(), str1 <= str2);
        printf("%s == %s = %i \n", str1.get(), str1.get(), str1 == str1);
        printf("%s == %s = %i \n", str1.get(), str3.get(), str1 == str3);
        printf("%s != %s = %i \n", str1.get(), str1.get(), str1 != str1);
        printf("%s != %s = %i \n", str1.get(), str6.get(), str1 != str6);

        printf("length %s %u \n", str1.get(), str1.length());
        printf("length %s %u \n", str2.get(), str2.length());
        printf("length %s %u \n", str3.get(), str3.length());
        printf("length %s %u \n", str4.get(), str4.length());
        printf("length %s %u \n", str5.get(), str5.length());
        printf("length %s %u \n", str6.get(), str6.length());
        printf("length %s %u \n", str7.get(), str7.length());

        OutputDevice::printf("%s\n", toString(Quatf(Vec3f::axisY, 0.5f)).get());
        OutputDevice::printf("%s\n", toString(Vec2f(1,2)).get());
        OutputDevice::printf("%s\n", toString(Vec3f(1,2,3)).get());
        OutputDevice::printf("%s\n", toString(Vec4f(1,2,3,4)).get());
    }

    static void run()
    {
        // StringManagerTest1();
        // StringManagerTest2();
        //StaticStringTest();
        DynamicStringTest();
    }

};


#endif //BERSERK_STRINGSTEST_H
