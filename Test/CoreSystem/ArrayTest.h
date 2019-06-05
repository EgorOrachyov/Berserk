//
// Created by Egor Orachyov on 03.06.2019.
//

#ifndef BERSERK_ARRAYTEST_H
#define BERSERK_ARRAYTEST_H

#include <Math/MathInclude.h>
#include <Containers/TArray.h>
#include <Strings/String.h>
#include <Strings/StaticString.h>

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

        TArray<CString> stringArray;

        new(stringArray.addUninitialized()) CString("Hello");
        new(stringArray.addUninitialized()) CString("World");
        new(stringArray.addUninitialized()) CString("Something");
        new(stringArray.addUninitialized()) CString("aaa");
        new(stringArray.addUninitialized()) CString("aaba");
        new(stringArray.addUninitialized()) CString("aaacc");

        stringArray.sort([](const CString &a, const CString &b){
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

        CName buffer[] = { CName("Hello"), CName("some"), CName("world"), CName("in"), CName("the") };
        uint32 size = 5;

        TArray<CName> array(buffer, size);

        array.add(CName("cool"));
        array.add(CName("fresh"));
        array.add(CName("universe"));

        array.sort([](const CName& a, const CName& b){ return a <= b; });

        for (uint32 i = 0; i < array.getSize(); i++)
        {
            printf("a[%i] = %s\n", i, array.get(i).get());
        }
        printf("\n\n");
    }

    static void ArrayTest4()
    {
        printf("\nTArray\n");

        CName buffer[] = { CName("Hello"), CName("some"), CName("world"), CName("in"), CName("the") };
        uint32 size = 5;

        char memory[1024];
        TList<CName> *list = new(memory) TArray<CName>();

        list->append(buffer, size);
        for (auto string = list->begin(); string != nullptr; string = list->next())
        {
            printf("%s\n", string->get());
        }
        delete list;

        printf("\n\n");
    }

    static void run()
    {
        ArrayTest1();
        ArrayTest2();
        ArrayTest3();
        ArrayTest4();
    }

};


#endif //BERSERK_ARRAYTEST_H
