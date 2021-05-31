
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
#include <BerserkCore/Templates/HashTable.hpp>
#include <BerserkCore/Debug/Debug.hpp>


using namespace Berserk;

BERSERK_DEFINE_FIXTURE(HashTableFixture)

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

static int64 values[] = {
    1,
    0,
    -3425,
    123,
    214325,
    12312413523,
    -2134
    -11111,
    325345,
    12312313,
    -23402375897490
};

TEST_F(HashTableFixture, Setup) {
    ASSERT_EQ(ARRAY_SIZE(keys), ARRAY_SIZE(values));

    HashTable<String, int64> map;

    for (auto i = 0; i < ARRAY_SIZE(keys); i++) {
        map.Add(keys[i], values[i]);
    }

    EXPECT_EQ(map.GetSize(), ARRAY_SIZE(keys));

    for (auto i = 0; i < ARRAY_SIZE(keys); i++) {
        EXPECT_TRUE(map.Contains(keys[i]));
    }

    for (auto i = 0; i < ARRAY_SIZE(keys); i++) {
        map.Remove(keys[i]);
    }

    EXPECT_TRUE(map.IsEmpty());
}

TEST_F(HashTableFixture, Iterator) {
    ASSERT_EQ(ARRAY_SIZE(keys), ARRAY_SIZE(values));

    HashTable<String, int64> map;

    for (auto i = 0; i < ARRAY_SIZE(keys); i++) {
        map.Add(keys[i], values[i]);
    }

    EXPECT_EQ(map.GetSize(), ARRAY_SIZE(keys));

    const auto& constMap = map;

    for (auto& p: constMap) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("\"{0}\":{1}"), p.GetFirst(), p.GetSecond());
    }
}

TEST_F(HashTableFixture, RemoveAdd) {
    ASSERT_EQ(ARRAY_SIZE(keys), ARRAY_SIZE(values));

    HashTable<String, int64> map;

    for (auto i = 0; i < ARRAY_SIZE(keys); i++) {
        map.Add(keys[i], values[i]);
    }

    EXPECT_EQ(map.GetSize(), ARRAY_SIZE(keys));

    for (auto & i : keysRemove) {
        map.Remove(i);
    }

    EXPECT_EQ(map.GetSize(), ARRAY_SIZE(keys) - ARRAY_SIZE(keysRemove));

    for (auto i = 0; i < ARRAY_SIZE(keys); i++) {
        map.AddIfNotPresent(keys[i], values[i]);
    }

    EXPECT_EQ(map.GetSize(), ARRAY_SIZE(keys));
}

TEST_F(HashTableFixture, StringString) {
    HashTable<String, String> map;

    Array<Pair<String,String>> pairs = {
            { BERSERK_TEXT("Alice"), BERSERK_TEXT("1124-1123-12-75") },
            { BERSERK_TEXT("Bob"),   BERSERK_TEXT("1873-4353-00-75") },
            { BERSERK_TEXT("Tom"),   BERSERK_TEXT("2357-2367-45-56") },
            { BERSERK_TEXT("Sam"),   BERSERK_TEXT("8000-4463-66-59") },
            { BERSERK_TEXT("Joel"),  BERSERK_TEXT("4367-3789-11-56") }
    };

    map.Add(pairs.GetData(), pairs.GetSize());

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Map stats: size={0} range={1} loadfactor={2}"), map.GetSize(), map.GetRange(), map.GetLoadFactor());

    Array<String> extractedKeys;
    Array<Pair<String,String>> extractedKeyValues;

    map.GetKeys(extractedKeys);
    map.GetKeyValues(extractedKeyValues);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Map keys: {0}"), extractedKeys);
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Map key-values: {0}"), extractedKeyValues);
}

TEST_F(HashTableFixture, Iterating) {
    HashTable<String, String> map;

    Array<Pair<String,String>> pairs = {
        { BERSERK_TEXT("Alice"), BERSERK_TEXT("1124-1123-12-75") },
        { BERSERK_TEXT("Bob"),   BERSERK_TEXT("1873-4353-00-75") },
        { BERSERK_TEXT("Tom"),   BERSERK_TEXT("2357-2367-45-56") },
        { BERSERK_TEXT("Sam"),   BERSERK_TEXT("8000-4463-66-59") },
        { BERSERK_TEXT("Joel"),  BERSERK_TEXT("4367-3789-11-56") }
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

    map.Add(pairs.GetData(), pairs.GetSize());

    auto iter = map.begin();
    while (iter != map.end()) {
        if (toRemove.Contains((*iter).GetFirst()))
            iter = map.Remove(iter);
        else
            ++iter;
    }

    for (auto& keep: toKeep) {
        EXPECT_TRUE(map.Contains(keep));
    }

    EXPECT_EQ(map.GetSize(), toKeep.GetSize());
}

TEST_F(HashTableFixture, Perfomance) {
    struct Data {
        uint64 v1;
        uint64 v2;
        uint64 v3;
        uint64 v4;
    };

    Data d{};

    size_t start = 0;
    size_t count = 1000;
    size_t removeStep = 10;
    size_t iters = 1000;

    using clock = std::chrono::steady_clock;
    using nanoseconds = std::chrono::nanoseconds;

    HashTable<String, Data> cache;

    double total;
    double average = 0.0;

    auto globalStart = clock::now();

    for (size_t iteration = 0; iteration < iters; iteration++) {
        auto localStart = clock::now();

        for (uint64 i = start; i < start + count; i++) {
            auto key = String::Fromu64(i);
            cache.AddIfNotPresent(key, d);
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