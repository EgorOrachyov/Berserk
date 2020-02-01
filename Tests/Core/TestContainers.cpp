/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TArray.h>
#include <THashMap.h>
#include <TAlgo.h>

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

    BERSERK_TEST(THashMap)
    {
        THashMap<CString,CString> data;

        data.add("uuid","1234abcf3f90f");
        data.add("type","image");

        BERSERK_EXPECT_TRUE(data.addIfNotPresent("format", "png"))
        BERSERK_EXPECT_FALSE(data.addIfNotPresent("format", "png"))

        CString& uuid = data["uuid"];
        CString& type = data["type"];

        printf("%s %s \n", uuid.data(), type.data());

        BERSERK_EXPECT_TRUE(data.remove("uuid"))
        BERSERK_EXPECT_FALSE(data.remove("uuid"))

        THashMap<uint32,uint32,Equals<uint32>,HashRaw<uint32>> expand;

        for (uint32 i = 0; i < 100; i++) {
            expand.add(i, i * i + 1);
        }

        for (uint32 i = 0; i < 100; i++) {
            BERSERK_EXPECT_TRUE(expand.contains(i))
            BERSERK_EXPECT_TRUE(expand.remove(i))
        }

        THashMap<CString,TArray<CString>> empty;
        empty.clear();
    };
}