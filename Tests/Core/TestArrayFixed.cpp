/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Templates/ArrayFixed.hpp>

using namespace Berserk;

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

static std::initializer_list<int32> values1 = {
        1,
        2,
        3,
        4,
        654,
        23,
        75
        - 123,
        -324,
        0,
        645758
};

static std::initializer_list<int32> values2 = {
        135,
        2643,
        -3234,
        4,
        0,
        654234,
        23243,
        2575
        - 13223,
        -32444,
        1110,
        645758
};

static std::initializer_list<int32> values3 = {
        1,
        2,
        3,
        4,
        654,
        23,
        75
        - 123,
        -324,
        0,
        645758,
        135,
        2643,
        -3234,
        4,
        0,
        654234,
        23243,
        2575
        - 13223,
        -32444,
        1110,
        645758
};

BERSERK_DEFINE_FIXTURE(ArrayFixedFixture)

TEST_F(ArrayFixedFixture, Setup) {
    auto i = 0;

    ArrayFixed<int32, 100> array = values1;

    ASSERT_EQ(array.GetSize(), values1.size());
    for (auto v: values1) {
        EXPECT_EQ(v, array[i]);
        i++;
    }

    ArrayFixed<int32, 200> array2;
    array2 = values2;

    ASSERT_EQ(array.GetSize(), values1.size());
    i = 0;
    for (auto v: values2) {
        EXPECT_EQ(v, array2[i]);
        i++;
    }
}

TEST_F(ArrayFixedFixture, Resize) {
    ArrayFixed<int32, 16> base = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    ArrayFixed<int32, 16> expectResize = { 1, 2, 3, 4, 5 };

    base.Resize(expectResize.GetSize());

    ASSERT_TRUE(expectResize.GetSize() == base.GetSize());

    for (uint64 i = 0; i < expectResize.GetSize(); i++)
        ASSERT_EQ(expectResize[i], base[i]);
}

TEST_F(ArrayFixedFixture, AddArray) {
    auto i = 0;

    ArrayFixed<int32, 50> a = values1;
    ArrayFixed<int32, 50> b = values2;

    a.Add(b);

    ASSERT_EQ(a.GetSize(), values3.size());
    for (auto v: values3) {
        EXPECT_EQ(v, a[i]);
        i++;
    }
}

TEST_F(ArrayFixedFixture, AddValue) {
    auto i = 0;

    ArrayFixed<int32, 50> array;

    for (auto v: values3) {
        array.Add(v);
    }

    ASSERT_EQ(array.GetSize(), values3.size());
    for (auto v: values3) {
        EXPECT_EQ(v, array[i]);
        i++;
    }
}

TEST_F(ArrayFixedFixture, IteratorConst) {
    auto i = 0;

    ArrayFixed<int32, 100> target = values3;
    ArrayFixed<int32, 100> reference = values3;

    for (auto v: target) {
        EXPECT_EQ(v, reference[i]);
        i++;
    }
}

TEST_F(ArrayFixedFixture, Iterator) {
    auto i = 0;

    ArrayFixed<int32, 100> target = values3;
    ArrayFixed<int32, 100> reference = values3;

    for (auto& v: target) {
        v += i;
        i++;
    }

    i = 0;
    for (auto v: target) {
        EXPECT_EQ(v, reference[i] + i);
        i++;
    }
}

TEST_F(ArrayFixedFixture, StringObject) {
    auto i = 0;

    String reference[] = {
            BERSERK_TEXT("Hello"),
            BERSERK_TEXT(" my "),
            BERSERK_TEXT("friend!........................."),
            BERSERK_TEXT("Привет"),
            BERSERK_TEXT(" мой "),
            BERSERK_TEXT("друг!...........................")
    };

    ArrayFixed<String> target;

    target.Add(reference, ARRAY_SIZE(reference));

    for (const auto& s: target) {
        EXPECT_EQ(s, reference[i]);
        i++;
    }
}

TEST_F(ArrayFixedFixture, Bounds) {
    String reference[] = {
            BERSERK_TEXT("Hello"),
            BERSERK_TEXT(" my "),
            BERSERK_TEXT(" my "),
            BERSERK_TEXT(" my "),
            BERSERK_TEXT("friend!........................."),
            BERSERK_TEXT("Привет"),
            BERSERK_TEXT("Привет"),
            BERSERK_TEXT("Привет"),
            BERSERK_TEXT(" мой "),
            BERSERK_TEXT("друг!..........................."),
            BERSERK_TEXT("друг!..........................."),
            BERSERK_TEXT("друг!...........................")
    };

    ArrayFixed<String> target;

    EXPECT_FALSE(target.IsAbleToAddElements(ARRAY_SIZE(reference)));
}

BERSERK_GTEST_MAIN