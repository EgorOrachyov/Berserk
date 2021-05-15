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
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Templates/Queue.hpp>

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

BERSERK_GTEST_MAIN