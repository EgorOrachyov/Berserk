//
// Created by Egor Orachyov on 05.06.2019.
//

#ifndef BERSERK_LINKEDLISTTEST_H
#define BERSERK_LINKEDLISTTEST_H

#include <Math/MathInclude.h>
#include <Containers/TArray.h>
#include <Containers/TLinkedList.h>
#include <Strings/String.h>
#include <Strings/StaticString.h>
#include <Utils/Profiling.h>

using namespace Berserk;

class LinkedListTest {

public:

    static void LinkedListTest1()
    {
        printf("\nTLinkedList\n");

        const uint32 size = 4;
        CName buffer[] = { CName("Hello"), CName(","), CName("world"), CName("what") };

        PoolAllocator poolAllocator(TLinkedList<CName>::getNodeSize(), 10);
        TLinkedList<CName> linkedList(poolAllocator);

        TList<CName> &list = linkedList;
        printf("List size: %u memory: %u \n", list.getSize(), list.getMemoryUsage());

        list.append(buffer, size);
        printf("List size: %u memory: %u \n", list.getSize(), list.getMemoryUsage());

        list.add(CName("a"));
        list.add(CName("lovely"));
        list.add(CName("day"));
        printf("List size: %u memory: %u \n", list.getSize(), list.getMemoryUsage());

        TLinkedList<CName> anotherList(poolAllocator);
        anotherList.add(CName("is"));
        anotherList.add(CName("not"));
        anotherList.add(CName("it"));
        anotherList.add(CName("?!"));
        anotherList.add(CName("lalalala"));
        anotherList.remove(anotherList.getSize() - 1);

        list.append(anotherList);
        printf("List size: %u memory: %u \n", list.getSize(), list.getMemoryUsage());

        for (auto string = list.begin(); string != nullptr; string = list.next())
        {
            printf("%s\n", string->get());
        }
        list.clear();

        Profiling::allocatorInfo(poolAllocator);

        printf("\n\n");
    }

    static void LinkedListTest2()
    {
        printf("\nTLinkedList\n");

        TLinkedList<CName> list;
        list.add(CName("trdss"));
        list.add(CName("aa"));
        list.add(CName("afafa"));
        list.add(CName("bba"));
        list.add(CName("abcd"));
        list.add(CName("abcc"));
        list.add(CName("adddfd"));
        list.sort([](const CName& a, const CName& b){
            return a <= b;
        });

        for (auto string = list.begin(); string != nullptr; string = list.next())
        {
            printf("%s\n", string->get());
        }

        Profiling::allocatorInfo(Allocator::get());
        printf("\n\n");
    }

    static void LinkedListTest3()
    {
        printf("\nTLinkedList\n");

        const uint32 size = 20;
        CName buffer[] =
        {
                CName("ssva"), CName("aav"), CName("fds"), CName(""), CName("ddTd"),
                CName("oitt"), CName("ass"), CName("asdc"), CName("asd"), CName(""),
                CName("eqq"), CName(""), CName("vvvv"), CName(""), CName("MRqqqqq"),
                CName("xxpo"), CName("asd"), CName("a"), CName("ssd"), CName("P0d")
        };

        TLinkedList<CName> list(buffer, size);
        list.sort([](const CName& a, const CName& b){
            return a <= b;
        });

        for (auto string = list.begin(); string != nullptr; string = list.next())
        {
            printf("%s\n", string->get());
        }

        Profiling::allocatorInfo(Allocator::get());
        printf("\n\n");
    }

    static void LinkedListTest4()
    {
        printf("\nTLinkedList\n");

        const uint32 size = 1000;
        TLinkedList<int64> list;
        for (uint32 i = 0; i < size; i++)
        {
            list.add(Math::random(0.0f, 1000.0f));
        }

        list.sort([](const int64& a, const int64& b){
            return a < b;
        });

        for (uint32 i = 0; i < list.getSize(); i++)
        {
            printf("a[%u]=%li\n", i, list.get(i));
        }

        Profiling::allocatorInfo(Allocator::get());
        printf("\n\n");
    }

    static void LinkedListTest5()
    {
        printf("\nTLinkedList\n");

        const uint32 size = 1000;
        TLinkedList<CString> list;

        list.emplace("List also");
        list.emplace("allows to");
        list.emplace("create new object of type T");
        list.emplace("and place it in the end of the container");

        list.sort([](const CString& a, const CString& b){
            return a.length() < b.length();
        });

        for (uint32 i = 0; i < list.getSize(); i++)
        {
            printf("a[%u]=%s\n", i, list.get(i).get());
        }

        Profiling::allocatorInfo(Allocator::get());
        printf("\n\n");
    }

    static void run()
    {
        LinkedListTest1();
        LinkedListTest2();
        LinkedListTest3();
        LinkedListTest4();
        LinkedListTest5();
    }

};


#endif //BERSERK_LINKEDLISTTEST_H
