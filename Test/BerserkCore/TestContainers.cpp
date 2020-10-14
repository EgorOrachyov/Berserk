/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <TestFixture.hpp>

#include <BerserkCore/Containers/TArrayFixed.hpp>
#include <BerserkCore/Containers/TArray.hpp>
#include <BerserkCore/Containers/TMap.hpp>

TEST_F(TestFixture, TArrayFixed) {
    TArrayFixed<int64, 64> list1;
    TArrayFixed<int64> list2 = {1, 2, 3, 4, 5};

    list1.Add(list2);
    list1.Add({6, 7, 8, 9, 10});
    list1.Add(11);
    list1.Add(list2.GetData(), list2.GetSize());

    printf("List 1\n");
    for (auto num: list1) {
        printf("%lli\n", (long long int)num);
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
        printf("%lli\n", (long long int)num);
    }

    TArrayFixed<int64,64> removing = {1, 2, 3, 4, 1, 5, 3, 4, 5, 1};
    removing.RemoveElement(1);
    removing.RemoveElement(5);
    removing.RemoveElement(3);
    removing.RemoveElement(4);
    removing.RemoveElement(2);

    printf("Raw remove\n");
    for (auto num: removing) {
        printf("%lli\n", (long long int)num);
    }
}

TEST_F(TestFixture, TArray) {
    TArray<int64> list1;
    TArray<int64> list2 = {1, 2, 3, 4, 5};

    list1.Add(list2);
    list1.Add({6, 7, 8, 9, 10});
    list1.Add(11);
    list1.Add(list2.GetData(), list2.GetSize());

    printf("List 1\n");
    for (auto num: list1) {
        printf("%lli\n", (long long int)num);
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
        printf("%lli\n", (long long int)num);
    }

    TArray<int64> removing = {1, 2, 3, 4, 1, 5, 3, 4, 5, 1};
    removing.RemoveElement(1);
    removing.RemoveElement(5);
    removing.RemoveElement(3);
    removing.RemoveElement(4);
    removing.RemoveElement(2);

    printf("Raw remove\n");
    for (auto num: removing) {
        printf("%lli\n", (long long int)num);
    }
}

TEST_F(TestFixture, TMap) {
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

    printf("Remove TEST_F\n");
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

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}