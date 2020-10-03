/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>

#include <BerserkCore/Platform/Thread.hpp>
#include <BerserkCore/Containers/TArray.hpp>
#include <BerserkCore/Containers/TMap.hpp>
#include <BerserkCore/Application.hpp>
#include <BerserkCore/Memory/TPoolAllocator.hpp>
#include <BerserkCore/String/String.hpp>

#if defined(BERSERK_TARGET_LINUX)
#include <BerserkPlatform/Linux.hpp>
#elif defined(BERSERK_TARGET_MACOS)
#include <BerserkPlatform/MacOS.hpp>
#endif

using namespace Berserk;

class App: public Application {
public:
    App() : Application() { }

    Allocator *GetGlobalAllocator() override {
        return &alloc;
    }

private:

    struct Alloc: public Allocator {
    public:
        ~Alloc() override { printf("Alloc stat: %i\n", allocStat);  };
        void *Allocate(uint64 size) override { allocStat += 1; return ::malloc(size); }
        void Free(void *memory) override { ::free(memory); }
        int32 allocStat = 0;
    };

    Alloc alloc;
};

class BasicCase : public ::testing::Test {
protected:
    App app;
};

TEST_F(BasicCase, Array) {
    TArray<int64> list1;
    TArray<int64> list2 = { 1, 2, 3, 4, 5 };

    list1.Add(list2);
    list1.Add({6, 7, 8, 9, 10});
    list1.Add(11);
    list1.Add(list2.GetData(), list2.GetSize());

    printf("List 1\n");
    for (auto num: list1) {
        printf("%lli\n", num);
    }

    for (auto itr = list1.begin(); itr != list1.end(); ) {
        if (*itr % 2 == 1) {
            itr.Remove();
        }
        else {
            ++itr;
        }
    }

    printf("List 1 on remove\n");
    for (auto num: list1) {
        printf("%lli\n", num);
    }

    TArray<int64> removing = { 1, 2, 3, 4, 1, 5, 3, 4, 5, 1 };
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

TEST_F(BasicCase,Map) {
    using Pair = TPair<int32,int32>;
    TMap<int32,int32,THashRaw<int32>> map;

    map.Add(1,2);
    map.Add({ Pair(3,4), Pair(5, 5), Pair(0, 1) });

    for (auto& p: map) {
        printf("%i %i\n", p.GetFirst(), p.GetSecond());
    }

    for (int32 i = 0; i < 1000; i++) {
        map.Add(i + 10, i * 2 + 1);
    }

    printf("Size: %u\n", map.GetSize());
    printf("Range: %u\n", map.GetRange());
    printf("Load factor: %f\n", map.GetLoadFactor());

    TMap<String,uint32> ages;

    ages.Add("Some verrrrryy loooong nammmmmeeee man!", 11000);
    ages.Add("Name1", 123);
    ages.Add("Name2", 4);
    ages.Emplace("Name3", 434);

    auto otherMap = std::move(ages);
    otherMap.Add("Entry", 11211);

    printf("Remove test\n");
    for (auto iter = otherMap.begin(); iter != otherMap.end(); ) {
        auto& p = *iter;
        if (p.GetSecond() == 123 || p.GetSecond() == 11211) {
            iter.Remove();
        } else {
            ++iter;
        }
    }

    for (auto& p: otherMap) {
        printf("%s %u\n", p.GetFirst().GetStr(), p.GetSecond());
    }
}

TEST_F(BasicCase,String) {
    TArray<String> names = { "apple", "banana", "orange" };
    names.Add("lemon");
    names.Emplace("lime");
    names.Add("some big very big and long fruit name goes here");

    for (auto name: names) {
        printf("buffer: %s capacity: %u length: %u\n", name.GetStr(), name.GetCapacity(), name.GetLength());
    }

    names[3] = "replaced lemon";
    names[4] = std::move(names[0]);
    names[0] = std::move(names[5]);

    for (auto& name: names) {
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
}

TEST_F(BasicCase,ExePath) {
#if defined(BERSERK_TARGET_LINUX)
    Linux platform;
#elif defined(BERSERK_TARGET_MACOS)
    MacOS platform;
#endif

    printf("Executable path length: %u\n", platform.GetExecutablePath().GetLength());
    printf("Executable path: %s\n", platform.GetExecutablePath().GetStr());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}