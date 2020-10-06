/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include "TestBasicCase.hpp"

#include <BerserkCore/Platform/Thread.hpp>
#include <BerserkCore/Containers/TArray.hpp>
#include <BerserkCore/Containers/TMap.hpp>
#include <BerserkCore/String/String.hpp>
#include <BerserkCore/String/StringBuilder.h>
#include <BerserkCore/LogMacro.hpp>
#include <BerserkCore/LogHtmlDump.hpp>


TEST_F(BasicCase, TArray) {
    TArray<int64> list1;
    TArray<int64> list2 = {1, 2, 3, 4, 5};

    list1.Add(list2);
    list1.Add({6, 7, 8, 9, 10});
    list1.Add(11);
    list1.Add(list2.GetData(), list2.GetSize());

    printf("List 1\n");
    for (auto num: list1) {
        printf("%lli\n", num);
    }

    for (auto itr = list1.begin(); itr != list1.end();) {
        if (*itr % 2 == 1) {
            itr.Remove();
        } else {
            ++itr;
        }
    }

    printf("List 1 on remove\n");
    for (auto num: list1) {
        printf("%lli\n", num);
    }

    TArray<int64> removing = {1, 2, 3, 4, 1, 5, 3, 4, 5, 1};
    removing.RemoveElement(1);
    removing.RemoveElement(5);
    removing.RemoveElement(3);
    removing.RemoveElement(4);
    removing.RemoveElement(2);

    printf("Raw remove\n");
    for (auto num: removing) {
        printf("%lli\n", num);
    }
}

TEST_F(BasicCase, TMap) {
    using Pair = TPair<int32, int32>;
    TMap<int32, int32, THashRaw<int32>> map;

    map.Add(1, 2);
    map.Add({Pair(3, 4), Pair(5, 5), Pair(0, 1)});

    for (auto &p: map) {
        printf("%i %i\n", p.GetFirst(), p.GetSecond());
    }

    for (int32 i = 0; i < 1000; i++) {
        map.Add(i + 10, i * 2 + 1);
    }

    printf("Size: %u\n", map.GetSize());
    printf("Range: %u\n", map.GetRange());
    printf("Load factor: %f\n", map.GetLoadFactor());

    TMap<String, uint32> ages;

    ages.Add("Some verrrrryy loooong nammmmmeeee man!", 11000);
    ages.Add("Name1", 123);
    ages.Add("Name2", 4);
    ages.Emplace("Name3", 434);

    auto otherMap = std::move(ages);
    otherMap.Add("Entry", 11211);

    printf("Remove test\n");
    for (auto iter = otherMap.begin(); iter != otherMap.end();) {
        auto &p = *iter;
        if (p.GetSecond() == 123 || p.GetSecond() == 11211) {
            iter.Remove();
        } else {
            ++iter;
        }
    }

    for (auto &p: otherMap) {
        printf("%s %u\n", p.GetFirst().GetStr(), p.GetSecond());
    }
}

TEST_F(BasicCase, String) {
    TArray<String> names = {"apple", "banana", "orange"};
    names.Add("lemon");
    names.Emplace("lime");
    names.Add("some big very big and long fruit name goes here");

    for (auto name: names) {
        printf("buffer: %s capacity: %u length: %u\n", name.GetStr(), name.GetCapacity(), name.GetLength());
    }

    names[3] = "replaced lemon";
    names[4] = std::move(names[0]);
    names[0] = std::move(names[5]);

    for (auto &name: names) {
        printf("buffer: %s capacity: %u length: %u\n", name.GetStr(), name.GetCapacity(), name.GetLength());
    }

    String a = "Abc";
    String b = " another";
    String c;

    c += a;
    c += b;

    String d = a + " " + c + " some sequence in the end";

    printf("%s\n", c.GetStr());
    printf("%s\n", d.GetStr());

    String strFloat = String::ToString(-0.142f);
    String strInt = String::ToString(-12312);

    printf("%s %f\n", strFloat.GetStr(), strFloat.ToFloat());
    printf("%s %i\n", strInt.GetStr(), strInt.ToInt32());
}

TEST_F(BasicCase, StringBuilder) {
    StringBuilder builder;

    builder.Append('H');
    builder.Append("ello, ");
    builder.Append(String("World!"));

    builder << " This is " << "some (number: " << 0.52818f << ") fancy builder!";
    String string = builder.ToString();

    printf("%s\n", string.GetStr());
}

TEST_F(BasicCase, LogDump) {
    BERSERK_LOG_ALWAYS("Some fancy message");
    BERSERK_LOG_INFO("Some fancy message");
    BERSERK_LOG_WARNING("Some fancy message");
    BERSERK_LOG_ERROR("Some fancy message");

    if (Log* log = Application::GetSingleton().GetLog()) {
        LogHtmlDump logHtmlDump("log_dump.html", *log);
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}