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
        map.setHashFunction(String::hash);

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

        printf("Contains key: %s  %i \n", "Bob", map.contains(String("Bob")));
        printf("Contains key: %s  %i \n", "Alice", map.contains(String("Alice")));

        auto itr = map.createIterator();
        for (auto pair = itr.begin(); pair != nullptr; pair = itr.next())
        {
            printf("Entry: key: %s, value: %s \n", pair->key()->get(), pair->value()->get());
        }
    }

    static void HashMapTest3()
    {
        typedef THashMap<String, uint32> UniformMap;
        PoolAllocator pool(UniformMap::getNodeSize());

        UniformMap map(Allocator::get(), pool);
        map.setHashFunction(String::hash);

        map.emplace("MVP",   0);
        map.emplace("View",  1);
        map.emplace("Model", 2);

        printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());

        map.emplace("CameraPosition",    3);
        map.emplace("CameraDirection",   4);
        map.emplace("CameraOrientation", 5);

        map.emplace("Something1", 6);
        map.emplace("Something2", 7);
        map.emplace("Something3", 8);
        map.emplace("Something3", 1111);

        printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());

        auto itr = map.createIterator();
        for (auto pair = itr.begin(); pair != nullptr; pair = itr.next())
        {
            printf("Entry: key: %s, value: %u \n", pair->key()->get(), *pair->value());
        }
    }

    static void run()
    {
        HashMapTest1();
        HashMapTest2();
        HashMapTest3();
    }

};

#endif //BERSERK_CONTAINERSTEST_H