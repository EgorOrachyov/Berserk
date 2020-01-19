/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Format.h>
#include <TestMacro.h>
#include <Errors.h>
#include <Platform/Memory.h>
#include <Math/Math.h>
#include <TPtrUnique.h>

using namespace Berserk;

BERSERK_TEST_SECTION(CoreMinimals)
{
    BERSERK_TEST(Format)
    {
        const uint32 bufferSize = 30;
        char buffer[bufferSize] = {0};
        uint32 written = 0;

        written = Format::printf(buffer, bufferSize, "Message");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %f %s", 0.345f, "CString");
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
}