/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Containers/Array.hpp>

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

BERSERK_DEFINE_FIXTURE(ArrayFixture)

TEST_F(ArrayFixture, Setup) {
    auto i = 0;

    Array<int32> array = values1;

    ASSERT_EQ(array.GetSize(), values1.size());
    for (auto v: values1) {
        EXPECT_EQ(v, array[i]);
        i++;
    }

    Array<int32> array2;
    array2 = values2;

    ASSERT_EQ(array.GetSize(), values1.size());
    i = 0;
    for (auto v: values2) {
        EXPECT_EQ(v, array2[i]);
        i++;
    }
}

TEST_F(ArrayFixture, AddArray) {
    auto i = 0;

    Array<int32> a = values1;
    Array<int32> b = values2;

    a.Add(b);

    ASSERT_EQ(a.GetSize(), values3.size());
    for (auto v: values3) {
        EXPECT_EQ(v, a[i]);
        i++;
    }
}

TEST_F(ArrayFixture, AddValue) {
    auto i = 0;

    Array<int32> array;

    for (auto v: values3) {
        array.Add(v);
    }

    ASSERT_EQ(array.GetSize(), values3.size());
    for (auto v: values3) {
        EXPECT_EQ(v, array[i]);
        i++;
    }
}

TEST_F(ArrayFixture, IteratorConst) {
    auto i = 0;

    Array<int32> target = values3;
    Array<int32> reference = values3;

    for (auto v: target) {
        EXPECT_EQ(v, reference[i]);
        i++;
    }
}

TEST_F(ArrayFixture, Iterator) {
    auto i = 0;

    Array<int32> target = values3;
    Array<int32> reference = values3;

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

TEST_F(ArrayFixture, StringObject) {
    auto i = 0;

    String reference[] = {
            BERSERK_TEXT("Hello"),
            BERSERK_TEXT(" my "),
            BERSERK_TEXT("friend!........................."),
            BERSERK_TEXT("Привет"),
            BERSERK_TEXT(" мой "),
            BERSERK_TEXT("друг!...........................")
    };

    Array<String> target;

    target.Add(reference, ARRAY_SIZE(reference));

    for (const auto& s: target) {
        EXPECT_EQ(s, reference[i]);
        i++;
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}