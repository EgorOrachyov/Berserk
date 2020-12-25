/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Containers/ArrayFixed.hpp>

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

    EXPECT_THROW(target.Add(reference, ARRAY_SIZE(reference)), AssertionException);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}