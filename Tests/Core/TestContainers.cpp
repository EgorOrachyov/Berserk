/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TArray.h>
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
}