//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_CONTAINERSTEST_H
#define BERSERK_CONTAINERSTEST_H

#include <Containers/TSet.h>
#include <Containers/TMap.h>
#include <Containers/THashMap.h>
#include <Strings/String.h>
#include <IO/OutputDevice.h>

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

        OutputDevice::printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());

        map.emplace(45, 50);

        OutputDevice::printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());
    }

    static void HashMapTest2()
    {
        PoolAllocator pool(THashMap<String, String>::getNodeSize());
        THashMap<String, String> map(Allocator::get(), pool);
        map.setHashFunction(String::hash);

        map.emplace_key("Alex", "8964");
        map.emplace_key("Bob", "123213");
        map.emplace_key("John", "81200001");
        map.emplace_key("Sam", "11111101");
        map.emplace_key("Lucy", "4657452");

        OutputDevice::printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());

        map.emplace_key("Alex", "4311");
        map.emplace_key("Bob", "3212");
        map.emplace_key("Omar", "773212");

        OutputDevice::printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());

        OutputDevice::printf("Contains key: %s  %i \n", "Bob", map.contains(String("Bob")));
        OutputDevice::printf("Contains key: %s  %i \n", "Alice", map.contains(String("Alice")));

        auto itr = map.createIterator();
        for (auto pair = itr.begin(); pair != nullptr; pair = itr.next())
        {
            OutputDevice::printf("Entry: key: %s, value: %s \n", pair->key()->get(), pair->value()->get());
        }
    }

    static void HashMapTest3()
    {
        typedef THashMap<String, uint32> UniformMap;
        PoolAllocator pool(UniformMap::getNodeSize());

        UniformMap map(Allocator::get(), pool);
        map.setHashFunction(String::hash);

        map.emplace("MVP", 0);
        map.emplace("View", 1);
        map.emplace("Model", 2);

        OutputDevice::printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());

        map.emplace("CameraPosition", 3);
        map.emplace("CameraDirection", 4);
        map.emplace("CameraOrientation", 5);

        map.emplace("Something1", 6);
        map.emplace("Something2", 7);
        map.emplace("Something3", 8);
        map.emplace("Something3", 1111);

        OutputDevice::printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());

        auto itr = map.createIterator();
        for (auto pair = itr.begin(); pair != nullptr; pair = itr.next())
        {
            OutputDevice::printf("Entry: key: %s, value: %u \n", pair->key()->get(), *pair->value());
        }
    }

    static void HashMapTest4()
    {
        typedef THashMap<String, uint32> UniformMap;
        PoolAllocator pool(UniformMap::getNodeSize());

        UniformMap map(Allocator::get(), pool);
        map.setHashFunction(String::hash);

        /** Put any kind of object */

        map.put(String("Now we can put"), 123);
        map.put(String("Very complex objects"), 123);

        /** Ore emplace - build inside the map */

        map.emplace("Now we can put", 567);
        map.emplace("Strings and create it inside", 6780);

        /** Also remove */

        map.remove("Strings and create it inside");

        auto itr = map.createIterator();
        for (auto pair = itr.begin(); pair != nullptr; pair = itr.next())
        {
            OutputDevice::printf("Entry: key: %s, value: %u \n", pair->key()->get(), *pair->value());
        }

        OutputDevice::printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());
    }

    static void HashMapTest5()
    {
        typedef THashMap<uint32, TSharedPtr<String>> Map;
        PoolAllocator pool(Map::getNodeSize());

        Map map(Allocator::get(), pool);

        map.put(1, TSharedPtr<String>(Allocator::get(), "Some string"));
        map.emplace(2, TSharedPtr<String>(Allocator::get(), "Other string"));

        OutputDevice::printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
               map.getRange(), map.getSize(), map.getLoadFactor(), map.getUsedBuckets(), map.getNodeSize(), map.getMemoryUsage());
    }

    static void HashMapTest6()
    {
        THashMap<String,String> map1;

        map1.emplace("Egor", "0x0012");
        map1.emplace("Alex", "0x0107");
        map1.emplace("Enje", "0x0f0f");

        THashMap<String,String> map2 = map1;

        OutputDevice::printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
                             map1.getRange(), map1.getSize(), map1.getLoadFactor(), map1.getUsedBuckets(), map1.getNodeSize(), map1.getMemoryUsage());

        for (auto pair = map2.begin(); pair != nullptr; pair = map2.next()) {
            OutputDevice::printf("%s %s \n", pair->key()->get(), pair->value()->get());
        }

        THashMap<String,String> map3(std::move(map1));

        OutputDevice::printf("range: %u, size: %u, load factor: %f, used buckets: %u, node size: %u, mem usage: %u\n",
                             map1.getRange(), map1.getSize(), map1.getLoadFactor(), map1.getUsedBuckets(), map1.getNodeSize(), map1.getMemoryUsage());

    }

    static void run()
    {
        //HashMapTest1();
        //HashMapTest2();
        //HashMapTest3();
        //HashMapTest4();
        //HashMapTest5();
        HashMapTest6();
    }

};

#endif //BERSERK_CONTAINERSTEST_H