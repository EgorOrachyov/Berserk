//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_CONTAINERSTEST_H
#define BERSERK_CONTAINERSTEST_H

#include <Containers/TSet.h>
#include <Containers/TMap.h>
#include <Containers/THashMap.h>
#include <Strings/String.h>

using namespace Berserk;

class ContainersTest
{
public:

    static void HashMapTest1()
    {
        THashMap<uint32, uint32> map;

        map.put(45, 12);
        map.put(12, 0);
        map.put(10, 999);

        printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());

        map.emplace(45, 50);

        printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());
    }

    static void HashMapTest2()
    {
        PoolAllocator pool(THashMap<String, String>::getNodeSize());
        THashMap<String, String> map(Allocator::get(), pool);

        map.emplace("Alex", "8964");
        map.emplace("Bob",  "123213");
        map.emplace("John", "81200001");
        map.emplace("Sam",  "11111101");
        map.emplace("Lucy", "4657452");

        printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());

        map.emplace("Alex", "4311");
        map.emplace("Bob",  "3212");
        map.emplace("Omar", "773212");

        printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());
    }

    static void run()
    {
        HashMapTest1();
        HashMapTest2();
    }

};

#endif //BERSERK_CONTAINERSTEST_H