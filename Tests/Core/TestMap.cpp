
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
#include <BerserkCore/Templates/Map.hpp>
#include <BerserkCore/Debug/Debug.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(MapFixture)

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

TEST_F(MapFixture, Setup) {
    ASSERT_EQ(ARRAY_SIZE(keys), ARRAY_SIZE(values));

    Map<String, int64> map;

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

TEST_F(MapFixture, Iterator) {
    ASSERT_EQ(ARRAY_SIZE(keys), ARRAY_SIZE(values));

    Map<String, int64> map;

    for (auto i = 0; i < ARRAY_SIZE(keys); i++) {
        map.Add(keys[i], values[i]);
    }

    EXPECT_EQ(map.GetSize(), ARRAY_SIZE(keys));

    const auto& constMap = map;

    for (auto& p: constMap) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("\"{0}\":{1}"), p.GetFirst(), p.GetSecond());
    }
}

TEST_F(MapFixture, RemoveAdd) {
    ASSERT_EQ(ARRAY_SIZE(keys), ARRAY_SIZE(values));

    Map<String, int64> map;

    for (auto i = 0; i < ARRAY_SIZE(keys); i++) {
        map.Add(keys[i], values[i]);
    }

    EXPECT_EQ(map.GetSize(), ARRAY_SIZE(keys));

    for (auto i = 0; i < ARRAY_SIZE(keysRemove); i++) {
        map.Remove(keys[i]);
    }

    EXPECT_EQ(map.GetSize(), ARRAY_SIZE(keys) - ARRAY_SIZE(keysRemove));

    for (auto i = 0; i < ARRAY_SIZE(keys); i++) {
        map.AddIfNotPresent(keys[i], values[i]);
    }

    EXPECT_EQ(map.GetSize(), ARRAY_SIZE(keys));
}

TEST_F(MapFixture, StringString) {
    Map<String, String> map;

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

TEST_F(MapFixture, Iterating) {
    Map<String, String> map;

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

BERSERK_GTEST_MAIN