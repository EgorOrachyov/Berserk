//
// Created by Egor Orachyov on 24.06.2019.
//

#ifndef BERSERK_MEMORYTEST_H
#define BERSERK_MEMORYTEST_H

#include <Memory/ListAllocator.h>
#include <Memory/PoolAllocator.h>
#include <Containers/TLinkedList.h>

using namespace Berserk;

class MemoryTest
{
public:

    static void PoolAllocatorTest1()
    {
        typedef TLinkedList<int64> IntsList;
        PoolAllocator poolAllocator(IntsList::getNodeSize());
        IntsList list(poolAllocator);

        list.add(1);
        list.add(2);
        list.add(8);
        list.add(6);

        int64 array[] = { 4, 44, 9000, -10 };
        list.append(array, 4);

        list.sort([](const int64& a, const int64& b){
            return a < b;
        });

        auto iter = list.createIterator();
        for (auto i = iter.begin(); i != nullptr; i = iter.next())
        {
            printf("%li\n", *i);
        }
    }

    static void PoolAllocatorTest2()
    {
        PoolAllocator pool(16, 64);
        const uint32 size = 1000;
        void* buffer[size];

        printf("%u %u %u %lu \n", pool.getMemoryUsage(), pool.getAllocateCalls(), pool.getFreeCalls(), pool.getTotalMemoryUsage());

        for (uint32 i = 0; i < size; i++)
        {
            buffer[i] = pool.allocate(0);
            printf("[%i]=%p\n", i, buffer[i]);
        }

        printf("%u %u %u %lu \n", pool.getMemoryUsage(), pool.getAllocateCalls(), pool.getFreeCalls(), pool.getTotalMemoryUsage());

        for (uint32 i = 0; i < size; i++)
        {
            pool.free(buffer[i]);
        }

        printf("%u %u %u %lu \n", pool.getMemoryUsage(), pool.getAllocateCalls(), pool.getFreeCalls(), pool.getTotalMemoryUsage());
    }

    static void PoolAllocatorTest3()
    {
        PoolAllocator pool(16);
        pool.allocate(0);
        pool.allocate(0);
        pool.allocate(0);
    }

    static void ListAllocatorTest1()
    {
        ListAllocator allocator;

        const uint32 count = 10;
        void* pointers[count];

        for (uint32 i = 0; i < count; i++)
        {
            pointers[i] = allocator.allocate((uint32) Math::random(1.0f, 1000.0f));
            printf("[%i]=%p \n", i, pointers[i]);
        }

        printf("%u %u %lu \n", allocator.getAllocateCalls(), allocator.getFreeCalls(), allocator.getTotalMemoryUsage());

        for (uint32 i = 0; i < count; i++)
        {
            allocator.free(pointers[i]);
        }

        printf("%u %u %lu \n", allocator.getAllocateCalls(), allocator.getFreeCalls(), allocator.getTotalMemoryUsage());
    }

    static void run()
    {
        // PoolAllocatorTest1();
        // PoolAllocatorTest2();
        // PoolAllocatorTest3();
        ListAllocatorTest1();
    }

};


#endif //BERSERK_MEMORYTEST_H
