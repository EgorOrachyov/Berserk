//
// Created by Egor Orachyov on 03.06.2019.
//

#ifndef BERSERK_ARRAYTEST_H
#define BERSERK_ARRAYTEST_H

#include <Math/MathInclude.h>
#include <Containers/TArray.h>
#include <Strings/String.h>

using namespace Berserk;

class ArrayTest
{

public:

    static void ArrayTest1()
    {
        printf("\nTArray\n");

        TArray<int64> array1(2000);
        TArray<int64> array2(1000);

        TList<int64> &list = array1;
        TList<int64> &another = array2;

        for (uint32 i = 0; i < 1400; i++)
        { list.add(Math::random(0.0f, 1000.0f)); }

        for (uint32 i = 0; i < 714; i++)
        { another.add(Math::random(0.0f, 1000.0f)); }

        list.append(another);
        list.sort([](const int64 &a, const int64 &b){
            return a < b;
        });

        int32 i = 0;
        for (int64* e = list.begin(); e != nullptr; e = list.next())
        { printf("a[%i]=%li\n", i++, *e); }

        printf("\n\n");
    }

    static void ArrayTest2()
    {
        printf("\nTArray\n");

        TArray<String> stringArray;

        new(stringArray.addUninitialized()) String("Hello");
        new(stringArray.addUninitialized()) String("World");
        new(stringArray.addUninitialized()) String("Something");
        new(stringArray.addUninitialized()) String("aaa");
        new(stringArray.addUninitialized()) String("aaba");
        new(stringArray.addUninitialized()) String("aaacc");

        stringArray.emplace("it is");
        stringArray.emplace("new");
        stringArray.emplace("feature, which allows");
        stringArray.emplace("to create object in the container");

        stringArray.sort([](const String &a, const String &b){
            return a.length() < b.length();
        });

        uint32 i = 0;
        for (auto s = stringArray.begin(); s != nullptr; s = stringArray.next())
        {
            printf("a[%i] = %s\n", i++, s->get());
        }
        printf("\n\n");
    }

    static void ArrayTest3()
    {
        printf("\nTArray\n");

        Name buffer[] = { Name("Hello"), Name("some"), Name("world"), Name("in"), Name("the") };
        uint32 size = 5;

        TArray<Name> array(buffer, size);

        array.add(Name("cool"));
        array.add(Name("fresh"));
        array.add(Name("universe"));

        array.sort([](const Name& a, const Name& b){ return a <= b; });

        for (uint32 i = 0; i < array.getSize(); i++)
        {
            printf("a[%i] = %s\n", i, array.get(i).get());
        }
        printf("\n\n");
    }

    static void ArrayTest4()
    {
        printf("\nTArray\n");

        Name buffer[] = { Name("Hello"), Name("some"), Name("world"), Name("in"), Name("the") };
        uint32 size = 5;

        char memory[1024];
        TList<Name> *list = new(memory) TArray<Name>();

        list->append(buffer, size);
        for (auto string = list->begin(); string != nullptr; string = list->next())
        {
            printf("%s\n", string->get());
        }
        delete list;

        printf("\n\n");
    }

    static void ArrayTest5()
    {
        String names[] = { String("Alex"), String("Simon"), String("Ramon"), String("Igrit") };

        TArray<String> array;

        array.add(names[0]);
        array.add(names[1]);
        array.add(names[2]);
        array.add(names[3]);

        array.append(names, 4);

        array.emplace("Viktor");
        array.emplace("Sandor");

        for (auto s = array.begin(); s != nullptr; s = array.next())
        {
            printf("%s \n", s->get());
        }
    }

    static void run()
    {
        //ArrayTest1();
        //ArrayTest2();
        //ArrayTest3();
        //ArrayTest4();
        ArrayTest5();
    }

};


#endif //BERSERK_ARRAYTEST_H
