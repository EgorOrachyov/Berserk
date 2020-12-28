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
#include <BerserkCore/Containers/Array.hpp>
#include <BerserkCore/Containers/Queue.hpp>

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

BERSERK_DEFINE_FIXTURE(QueueFixture)

TEST_F(QueueFixture, Setup) {
    Queue<int32> queue = values1;

    ASSERT_EQ(queue.GetSize(), values1.size());

    for (auto v: values1) {
        int32 value;
        EXPECT_TRUE(queue.Pop(value));
        EXPECT_EQ(v, value);
    }
}

TEST_F(QueueFixture, Dynamic) {
    Queue<int32> queue;

    for (auto v: values1) {
        queue.Push(v);
    }

    EXPECT_EQ(queue.GetSize(), values1.size());

    for (auto v: values1) {
        int32 value;
        EXPECT_TRUE(queue.Pop(value));
        EXPECT_EQ(v, value);
    }
}

TEST_F(QueueFixture, Peek) {
    Queue<int32> queue;

    for (auto v: values1) {
        queue.Push(v);
    }

    EXPECT_EQ(queue.GetSize(), values1.size());

    for (auto v: values1) {
        int32 value;
        EXPECT_TRUE(queue.Peek(value));
        EXPECT_EQ(v, value);
        EXPECT_TRUE(queue.Pop(value));
    }

    EXPECT_EQ(queue.GetSize(), 0);
}

TEST_F(QueueFixture, PeekPopValues) {
    Queue<int32> queue;

    for (auto v: values1) {
        queue.Push(v);
    }

    EXPECT_EQ(queue.GetSize(), values1.size());

    size_t i;
    Array<int32> array;

    array.Clear();
    queue.PeekValues(array);

    i = 0;
    for (auto v: values1) {
        EXPECT_EQ(array[i], v);
        i++;
    }

    EXPECT_EQ(queue.GetSize(), values1.size());

    array.Clear();
    queue.PopValues(array);

    i = 0;
    for (auto v: values1) {
        EXPECT_EQ(array[i], v);
        i++;
    }

    EXPECT_EQ(queue.GetSize(), 0);
}

TEST_F(QueueFixture, PopPush) {
    static std::initializer_list<int32> v1 = {
        1,
        2,
        3,
        4,
        654,
        23,
        75
        -123,
        423,
        12425
    };

    static std::initializer_list<int32> v2 = {
            1,
            2,
            3,
            4,
            654,
            23,
            75,
            -123,
            2,
            3,
            4,
            654,
            23,
            124,
            -342,
            124,
            124
    };

    Queue<int32> queue;

    for (auto v: v1) {
        queue.Push(v);
    }

    for (auto v: v1) {
        int32 value;
        EXPECT_TRUE(queue.Pop(value));
        EXPECT_EQ(value, v);
    }

    for (auto v: v2) {
        queue.Push(v);
    }

    for (auto v: v2) {
        int32 value;
        EXPECT_TRUE(queue.Pop(value));
        EXPECT_EQ(value, v);
    }

    EXPECT_TRUE(queue.IsEmpty());
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}