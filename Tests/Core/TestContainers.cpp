/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Containers/TArrayStatic.h>
#include <Containers/TArray.h>
#include <Containers/TMap.h>
#include <Containers/TSet.h>
#include <Containers/TAlgo.h>
#include <Math/Random.h>
#include <String/CString.h>
#include <String/CStringStatic.h>

#include <TestMacro.h>

using namespace Berserk;

BERSERK_TEST_SECTION(Containers)
{
    BERSERK_TEST(TArray)
    {
        TArray<uint64> a = { 1, 2, 3, 4 };
        TArray<uint64> b = { 1, 2, 3, 4, 99, 100 };
        uint32 i = 0;

        a += { 22, 1, 0, 0 };
        b += a;

        const auto c = a + b;

        auto f = [&](const uint64& e) {
            printf("[%u]: %llu\n", i, e);
            i += 1;
        };

        i = 0;
        c.forEach(f);

        auto d = c;

        for (auto& e: d) {
            e += 1;
        }

        i = 0;
        d.forEach(f);

        TArray<uint32> e;
        e *= 10;
        printf("Multiply e * 10\n");
        e.forEach([](const uint32& value){static uint32 i = 0; printf("[%i]: %i\n", i++, value);});

        e += 13;
        e += -11;
        e += { 0, 99, 11 };
        e.emplace(1111);
        e *= 4;
        printf("Multiply e * 4\n");
        e.forEach([](const uint32& value){static uint32 i = 0; printf("[%i]: %i\n", i++, value);});

        e *= 0;
        printf("Multiply e * 0\n");
        e.forEach([](const uint32& value){static uint32 i = 0; printf("[%i]: %i\n", i++, value);});

        TArray<CStringStatic> strings = { "Apple", "Orange", "Lemon", "Something" };
        strings += "Another";
        strings += { "List", "Of", "Banana" };
        strings.emplace("Capsule");

        printf("Sine: %u Capacity %u\n", strings.size(), strings.capacity());

        TArray<uint32> removing = { 1, 2, 3, 4, 1, 5, 3, 4, 5, 1 };
        removing.removeElement(1);
        removing.removeElement(5);
        removing.removeElement(3);
        removing.removeElement(4);
        removing.removeElement(2);

        BERSERK_EXPECT_TRUE(removing.size() == 5)

        removing.removeElement(0);
        removing.removeElement(10);

        BERSERK_EXPECT_TRUE(removing.size() == 5)
    };

    BERSERK_TEST(TAlgo)
    {
        TArray<uint32> a = { 1, 2, 9, 0, 1, 77, 13, 33, 44, 0, 0, 1 };
        TAlgo::sort(a);
        a.forEach([](const uint32& value){static uint32 i = 0; printf("[%i]: %i\n", i++, value);});

        TArray<CStringStatic> b = { "abc", "ab", "abcc", "bcd", "b", "bbd22", "90ad", "ABC", "A", "B", "921a", "bcd", "bcde1" };
        TAlgo::sort(b);
        b.forEach([](const CStringStatic& value){static uint32 i = 0; printf("[%i]: %s\n", i++, value.data());});
    };

    BERSERK_TEST(TMap)
    {
        TMap<CString,CString> data;

        data.add("uuid","1234abcf3f90f");
        data.add("type","image");

        BERSERK_EXPECT_TRUE(data.addIfNotPresent("format", "png"))
        BERSERK_EXPECT_FALSE(data.addIfNotPresent("format", "png"))

        CString& uuid = data["uuid"];
        CString& type = data["type"];

        printf("%s %s \n", uuid.data(), type.data());

        BERSERK_EXPECT_TRUE(data.remove("uuid"))
        BERSERK_EXPECT_FALSE(data.remove("uuid"))

        TMap<uint32,uint32,THashRaw<uint32>> expand;

        for (uint32 i = 0; i < 100; i++) {
            expand.add(i, i * i + 1);
        }

        for (uint32 i = 0; i < 100; i++) {
            BERSERK_EXPECT_TRUE(expand.contains(i))
            BERSERK_EXPECT_TRUE(expand.remove(i))
        }

        TMap<CString,TArray<CString>> empty;
        empty.clear();

        TMap<uint64,void*,THashRaw<uint64>> objects;
        Random random;
        const uint32 N = 100;

        for (uint32 i = 0; i < N; i++) {
            objects.add((uint64)random.randi32(), &i + i);
        }

        for (auto& p: objects) {
            p.second() = nullptr;
        }

        for (const auto& p: objects) {
            printf("ID: %llu Data: %p\n", p.first(), p.second());
        }

        printf("Map [THashMap<uint64,void*> objects]\n");
        printf("Size of node: %u bytes\n", objects.sizeOfNode());
        printf("Range: %u \n", objects.range());
        printf("Dyn alloc: %llu bytes\n", objects.getAllocatedMemory());

        TMap<CString,int64> cache = { { "/path/to",8 }, { "/put/some/thing",11 } };
        cache.add({ {"/some/value",1}, {"/other/value",2} });

        for (auto& p: cache) {
            printf("%s %lli\n", p.first().data(), p.second());
        }


        TMap<uint32,CString,THashRaw<uint32>> fruitMap = { { 1, "apple" }, { 2, "mango" }, { 4, "banana" } };

        for (auto& elem: fruitMap) {
            printf("{%i:%s},", elem.first(), elem.second().data());
        }
        printf("\n");

        fruitMap.emplace(0, "orange");
        fruitMap.emplace(4, "lemon");
        fruitMap.remove(2);

        for (auto& elem: fruitMap) {
            printf("{%i:%s},", elem.first(), elem.second().data());
        }
        printf("\n");
    };

    BERSERK_TEST(TSet)
    {
        auto fruits = { CString{"Grapefruit"}, CString{"Apple"}, CString{"Pineapple"}, CString{"etc."} };

        TSet<CString> fruitSet = { "Orange" };
        fruitSet.add("Apple");
        fruitSet.add("Banana");

        for (auto& s: fruitSet) {
            printf("%s\n", s.data());
        }

        fruitSet.add({ "Apple", "Pineapple" });
        fruitSet.add(fruits);
        fruitSet.remove("Apple");

        for (const auto& s: fruits) {
            printf("Contains [%s]?%i\n", s.data(), fruitSet.contains(s));
        }

        for (auto& s: fruitSet) {
            printf("%s\n", s.data());
        }

        fruitSet.clear();
        fruitSet.clear();
        fruitSet.clear();

        for (auto& s: fruitSet) {
            printf("%s\n", s.data());
        }
    };

    BERSERK_TEST(TArrayStatic)
    {
        TArrayStatic<CString> fruits;
        fruits.add("Apple");
        fruits.add("Banana");
        printf("Able to add? %i \n", fruits.ableToAddElement());
        printf("Able to add? %i \n", fruits.ableToAddElements(6));
        printf("Able to add? %i \n", fruits.ableToAddElements(7));

        TArrayStatic<CString, 2> vegetables = { "Potato", "Tomato" };
        printf("Able to add? %i \n", vegetables.ableToAddElement());

        for (auto& s: fruits) {
            printf("%s\n", s.data());
        }
        for (auto& s: vegetables) {
            printf("%s\n", s.data());
        }

        fruits *= 2;
        fruits += vegetables;

        TAlgo::sort(fruits);

        for (auto& s: fruits) {
            printf("%s\n", s.data());
        }

        fruits.removeElement("Apple");
        auto removed = fruits.removeMatchAll([](const CString& item){ return item == "Banana"; });
        printf("Removed: %u\n", removed);

        auto list = fruits + vegetables + fruits;
        for (auto i: list) {
            printf("%s\n", i.data());
        }
    };

    BERSERK_TEST(TArraySort) {
        TArray<CString> items = { "1", "2", "ab", "abc", "d", "e", "ffff", "abc1", "d0x" };
        TAlgo::sort(items, [](const CString& a, const CString& b){
            return a <= b;
        });
        for (auto& i: items) {
            printf("%s\n", i.data());
        }
    };
}