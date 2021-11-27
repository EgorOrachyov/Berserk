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
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Templates/HashSet.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <chrono>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(HashSetFixture)

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

static const char* keys[] = {
        BERSERK_TEXT("Asafkjn AKFBsa nalsnfl []turt ,;lasdfkqpowe817289"),
        BERSERK_TEXT("Asaoiqwj8923 nflkwn flwenf lc eoi1984783v"),
        BERSERK_TEXT("Asancrkj wen r2o345-40=-= -=- 783v"),
        BERSERK_TEXT("Asancrkj kqejbwjk bnjq 2n4trp438y2058 vdsfv 132"),
        BERSERK_TEXT("Asancrkj kqejlr2n o635216 01263 9"),
        BERSERK_TEXT("23 408 182-3 nln f jl 1nl635216 01263 9"),
        BERSERK_TEXT("23 4qm lkm3;krmwekmjrp9432u=6b705 m0=i 439yu=0y 35216 01263 9"),
        BERSERK_TEXT("23 4qm lkm3;krmwa. f.nrnweljrnlewj prm.....we r.w/e 263 9"),
        BERSERK_TEXT("23 4qm lkm3;krmwa. f.nr;qwkjp19283-91 4363 9"),
        BERSERK_TEXT("23 4qm lkm3;krmwa. f.nr;qwk-=p-=- =-=-==-=234= 0-=04=23jknjo nonfinroefn ")
};

static const char* keysRemove[] = {
        BERSERK_TEXT("23 408 182-3 nln f jl 1nl635216 01263 9"),
        BERSERK_TEXT("23 4qm lkm3;krmwekmjrp9432u=6b705 m0=i 439yu=0y 35216 01263 9"),
        BERSERK_TEXT("23 4qm lkm3;krmwa. f.nrnweljrnlewj prm.....we r.w/e 263 9"),
        BERSERK_TEXT("23 4qm lkm3;krmwa. f.nr;qwkjp19283-91 4363 9"),
        BERSERK_TEXT("23 4qm lkm3;krmwa. f.nr;qwk-=p-=- =-=-==-=234= 0-=04=23jknjo nonfinroefn ")
};

TEST_F(HashSetFixture, Setup) {
    HashSet<String> set;

    for (auto & key : keys) {
        set.Add(key);
    }

    EXPECT_EQ(set.GetSize(), ARRAY_SIZE(keys));

    for (auto & key : keys) {
        EXPECT_TRUE(set.Contains(key));
    }

    for (auto & key : keys) {
        set.Remove(key);
    }

    EXPECT_TRUE(set.IsEmpty());
}

TEST_F(HashSetFixture, Iterator) {
    HashSet<String> set;

    for (auto & key : keys) {
        set.Add(key);
    }

    EXPECT_EQ(set.GetSize(), ARRAY_SIZE(keys));

    const auto& constSet = set;

    for (auto& p: constSet) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("\"{0}\""), p);
    }
}

TEST_F(HashSetFixture, RemoveAdd) {
    HashSet<String> set;

    for (auto & key : keys) {
        set.Add(key);
    }

    EXPECT_EQ(set.GetSize(), ARRAY_SIZE(keys));

    for (auto & i : keysRemove) {
        set.Remove(i);
    }

    EXPECT_EQ(set.GetSize(), ARRAY_SIZE(keys) - ARRAY_SIZE(keysRemove));

    for (auto & key : keys) {
        set.Add(key);
    }

    EXPECT_EQ(set.GetSize(), ARRAY_SIZE(keys));
}

TEST_F(HashSetFixture, Strings) {
    HashSet<String> set;

    Array<String> values = {
            BERSERK_TEXT("Alice"),
            BERSERK_TEXT("Bob"),
            BERSERK_TEXT("Tom"),
            BERSERK_TEXT("Sam"),
            BERSERK_TEXT("Joel")
    };

    set.Add(values.GetData(), values.GetSize());

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("HashSet stats: size={0} range={1} loadfactor={2}"), set.GetSize(), set.GetRange(), set.GetLoadFactor());

    Array<String> extractedKeys;
    set.GetKeys(extractedKeys);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("HashSet keys: {0}"), extractedKeys);
}

TEST_F(HashSetFixture, Iterating) {
    HashSet<String> set;

    Array<String> values = {
            BERSERK_TEXT("Alice"),
            BERSERK_TEXT("Bob"),
            BERSERK_TEXT("Tom"),
            BERSERK_TEXT("Sam"),
            BERSERK_TEXT("Joel")
    };

    Array<String> toRemove = {
            BERSERK_TEXT("Bob"),
            BERSERK_TEXT("Joel"),
    };

    Array<String> toKeep = {
            BERSERK_TEXT("Alice"),
            BERSERK_TEXT("Tom"),
            BERSERK_TEXT("Sam"),
    };

    set.Add(values.GetData(), values.GetSize());

    auto iter = set.begin();
    while (iter != set.end()) {
        if (toRemove.Contains<Equals<String>>((*iter)))
            iter = set.Remove(iter);
        else
            ++iter;
    }

    for (auto& keep: toKeep) {
        EXPECT_TRUE(set.Contains(keep));
    }

    EXPECT_EQ(set.GetSize(), toKeep.GetSize());
}

TEST_F(HashSetFixture, Perfomance) {
    size_t start = 0;
    size_t count = 1000;
    size_t removeStep = 10;
    size_t iters = 1000;

    using clock = std::chrono::steady_clock;
    using nanoseconds = std::chrono::nanoseconds;

    HashSet<String> cache;

    double total;
    double average = 0.0;

    auto globalStart = clock::now();

    for (size_t iteration = 0; iteration < iters; iteration++) {
        auto localStart = clock::now();

        for (uint64 i = start; i < start + count; i++) {
            auto key = String::Fromu64(i);
            cache.Add(key);
        }

        for (uint64 i = start + (iteration % removeStep); i < start + count; i += removeStep) {
            auto key = String::Fromu64(i);
            cache.Remove(key);
        }

        average += (double) std::chrono::duration_cast<nanoseconds>(clock::now() - localStart).count() / 1e6f;
    }

    total = (double) std::chrono::duration_cast<nanoseconds>(clock::now() - globalStart).count() / 1e6f;
    average /= (double) iters;

    BERSERK_CORE_LOG_INFO("Total={0} ms Average={1} ms Cache size={2}", total, average, cache.GetSize());
}

BERSERK_GTEST_MAIN