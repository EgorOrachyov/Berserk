/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Format.h>
#include <TestMacro.h>
#include <ErrorMacro.h>
#include <Platform/Memory.h>
#include <Math/Math.h>
#include <TPtrUnique.h>
#include <AllocPool.h>
#include <TimeValue.h>

#include <iostream>

using namespace Berserk;

void* operator new(size_t size) {
    printf("Allocate (by new): %lu\n", size);
    return malloc(size);
}

BERSERK_TEST_SECTION(CoreMinimals)
{
    BERSERK_TEST(Format)
    {
        const uint32 bufferSize = 30;
        char buffer[bufferSize] = {0};
        uint32 written = 0;

        written = Format::printf(buffer, bufferSize, "Message");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %f %s", 0.345f, "TString");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %s", "abcabcabcabcabcabcabcabc");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %20s", "Aligned");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %9.5f", 1241.001f);
        printf("%u %s\n", written, buffer);
    };

    BERSERK_TEST(Assert)
    {
        BERSERK_EXPECT_TRUE(1 > 0)
        BERSERK_EXPECT_TRUE(10 - 12 < -100)
        BERSERK_EXPECT_FALSE(100 * 0 == 10)
        BERSERK_EXPECT_FALSE(true)
    };

    BERSERK_TEST(Pointers)
    {
        TPtrUnique<uint64> i1 = objectNew<uint64>(10);
        TPtrUnique<uint64> i2 = objectNew<uint64>(15);

        auto r = *i1 + *i2;

        printf("%llu %llu %llu \n", r, *i1, *i2);

        auto i3 = i1;
        auto i4 = i2;

        printf("%p %p %i %i\n", i3.getPtr(), i4.getPtr(), i3 > i4, i3 < i4);
    };

    BERSERK_TEST(AllocPool)
    {
        AllocPool pool1(sizeof(uint64), 1);

        for (uint32 i = 0; i < 10; i++) {
            auto p = pool1.allocate(sizeof(uint64));
            printf("[%u]=%p\n", i, p);
        }
    };

    BERSERK_TEST(AllocStat)
    {
        auto allocs = Memory::getAllocCalls();
        auto frees = Memory::getFreeCalls();
        printf("Alloc calls: %lu Free calls: %lu \n", allocs, frees);
    };

    BERSERK_TEST(Function)
    {
        struct Data {
            uint32 buffer[6];
        };

        Data d1;
        Data d2;

        auto l1 = [&](){
            d1.buffer[0] = 1;
        };

        auto l2 = [=](){
            auto i = d1.buffer[0];
        };

        Function<void()> f1 = l1;
        Function<void()> f2 = l2;

        printf("Sizeof %lu\n", sizeof(Function<void()>));
    };

    BERSERK_TEST(TimeValue)
    {
        TimeValue start = TimeValue::now();

        //int32 i;
        //std::cin >> i;

        TimeValue end = TimeValue::now();

        printf("Sizeof(TimeValue): %u\n", (uint32) sizeof(TimeValue));
        printf("Time: %lfs\n", (end - start).getSeconds());
        printf("Time: %lfms\n", (end - start).getMilliseconds());
    };
}