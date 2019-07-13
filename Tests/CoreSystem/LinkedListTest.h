//
// Created by Egor Orachyov on 05.06.2019.
//

#ifndef BERSERK_LINKEDLISTTEST_H
#define BERSERK_LINKEDLISTTEST_H

#include <Math/MathInclude.h>
#include <Containers/TArray.h>
#include <Containers/TLinkedList.h>
#include <Utils/Profiling.h>
#include <IO/OutputDevice.h>

using namespace Berserk;

class LinkedListTest {

public:

    static void LinkedListTest1()
    {
        OutputDevice::printf("\nTLinkedList\n");

        const uint32 size = 4;
        Name buffer[] = { Name("Hello"), Name(","), Name("world"), Name("what") };

        PoolAllocator poolAllocator(TLinkedList<Name>::getNodeSize(), 10);
        TLinkedList<Name> linkedList(poolAllocator);

        TList<Name> &list = linkedList;
        OutputDevice::printf("List size: %u memory: %u \n", list.getSize(), list.getMemoryUsage());

        list.append(buffer, size);
        OutputDevice::printf("List size: %u memory: %u \n", list.getSize(), list.getMemoryUsage());

        list.add(Name("a"));
        list.add(Name("lovely"));
        list.add(Name("day"));
        OutputDevice::printf("List size: %u memory: %u \n", list.getSize(), list.getMemoryUsage());

        TLinkedList<Name> anotherList(poolAllocator);
        anotherList.add(Name("is"));
        anotherList.add(Name("not"));
        anotherList.add(Name("it"));
        anotherList.add(Name("?!"));
        anotherList.add(Name("lalalala"));
        anotherList.remove(anotherList.getSize() - 1);

        list.append(anotherList);
        OutputDevice::printf("List size: %u memory: %u \n", list.getSize(), list.getMemoryUsage());

        for (auto string = list.begin(); string != nullptr; string = list.next())
        {
            OutputDevice::printf("%s\n", string->get());
        }
        list.clear();

        Profiling::allocatorInfo(poolAllocator);

        OutputDevice::printf("\n\n");
    }

    static void LinkedListTest2()
    {
        OutputDevice::printf("\nTLinkedList\n");

        TLinkedList<Name> list;
        list.add(Name("trdss"));
        list.add(Name("aa"));
        list.add(Name("afafa"));
        list.add(Name("bba"));
        list.add(Name("abcd"));
        list.add(Name("abcc"));
        list.add(Name("adddfd"));
        list.sort([](const Name& a, const Name& b){
            return a <= b;
        });

        for (auto string = list.begin(); string != nullptr; string = list.next())
        {
            OutputDevice::printf("%s\n", string->get());
        }

        Profiling::allocatorInfo(Allocator::get());
        OutputDevice::printf("\n\n");
    }

    static void LinkedListTest3()
    {
        OutputDevice::printf("\nTLinkedList\n");

        const uint32 size = 20;
        Name buffer[] =
        {
                Name("ssva"), Name("aav"), Name("fds"), Name(""), Name("ddTd"),
                Name("oitt"), Name("ass"), Name("asdc"), Name("asd"), Name(""),
                Name("eqq"), Name(""), Name("vvvv"), Name(""), Name("MRqqqqq"),
                Name("xxpo"), Name("asd"), Name("a"), Name("ssd"), Name("P0d")
        };

        TLinkedList<Name> list(buffer, size);
        list.sort([](const Name& a, const Name& b){
            return a <= b;
        });

        for (auto string = list.begin(); string != nullptr; string = list.next())
        {
            OutputDevice::printf("%s\n", string->get());
        }

        Profiling::allocatorInfo(Allocator::get());
        OutputDevice::printf("\n\n");
    }

    static void LinkedListTest4()
    {
        OutputDevice::printf("\nTLinkedList\n");

        const uint32 size = 1000;
        PoolAllocator pool(TLinkedList<int64>::getNodeSize(), size);
        TLinkedList<int64> list(pool);
        for (uint32 i = 0; i < size; i++)
        {
            list.add(Math::random(0.0f, 1000.0f));
        }

        list.sort([](const int64& a, const int64& b){
            return a < b;
        });

        for (uint32 i = 0; i < list.getSize(); i++)
        {
            OutputDevice::printf("a[%u]=%li\n", i, list.get(i));
        }

        Profiling::allocatorInfo(Allocator::get());
        OutputDevice::printf("\n\n");
    }

    static void LinkedListTest5()
    {
        OutputDevice::printf("\nTLinkedList\n");

        TLinkedList<String> list;
        list.emplace("List also");
        list.emplace("allows to");
        list.emplace("create new object of type T");
        list.emplace("and place it in the end of the container");

        list.sort([](const String& a, const String& b){
            return a.length() < b.length();
        });

        for (uint32 i = 0; i < list.getSize(); i++)
        {
            OutputDevice::printf("a[%u]=%s\n", i, list.get(i).get());
        }

        Profiling::allocatorInfo(Allocator::get());
        OutputDevice::printf("\n\n");
    }

    static void LinkedListTest6()
    {
        String names[] = { String("Alex"), String("Simon"), String("Ramon"), String("Igrit") };

        TLinkedList<String> list;

        list.add(names[0]);
        list.add(names[1]);
        list.add(names[2]);
        list.add(names[3]);

        list.append(names, 4);

        list.emplace("Viktor");
        list.emplace("Sandor");

        TLinkedList<String> another = list;

        for (auto s = list.begin(); s != nullptr; s = list.next())
        {
            OutputDevice::printf("%s \n", s->get());
        }

        for (auto s = another.begin(); s != nullptr; s = another.next())
        {
            OutputDevice::printf("%s \n", s->get());
        }
    }

    static void LinkedListTest7()
    {
        TLinkedList<Name> list1;

        list1.emplace("1. A");
        list1.emplace("2. B");
        list1.emplace("3. C");

        /** Copy content to list2 */
        TLinkedList<Name> list2 = list1;

        OutputDevice::printf("Size: %u\n", list1.getSize());

        for (auto str = list2.begin(); str != nullptr; str = list2.next()) {
            OutputDevice::printf("%s \n", str->get());
        }

        /** Move content to list3 */
        TLinkedList<Name> list3(std::move(list1));

        OutputDevice::printf("Size: %u\n", list1.getSize());

        for (auto str = list3.begin(); str != nullptr; str = list3.next()) {
            OutputDevice::printf("%s \n", str->get());
        }
    }

    static void run()
    {
        //LinkedListTest1();
        //LinkedListTest2();
        //LinkedListTest3();
        //LinkedListTest4();
        //LinkedListTest5();
        //LinkedListTest6();
        LinkedListTest7();
    }

};


#endif //BERSERK_LINKEDLISTTEST_H
