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

        CName buffer[] = { CName("Hello"), CName(","), CName("world"), CName("what") };
        const uint32 size = 4;

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

    static void run()
    {
        LinkedListTest1();
    }

};


#endif //BERSERK_LINKEDLISTTEST_H
