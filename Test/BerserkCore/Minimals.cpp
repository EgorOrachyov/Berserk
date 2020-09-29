/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>

#include <BerserkCore/Memory/MemoryCow.hpp>
#include <BerserkCore/Platform/Thread.hpp>
#include <BerserkCore/Containers/TArray.hpp>
#include <BerserkCore/Containers/TMap.hpp>
#include <BerserkCore/Application.hpp>
#include <BerserkCore/Memory/TPoolAllocator.hpp>
#include <BerserkCore/String/TString.hpp>

#if defined(BERSERK_TARGET_LINUX)
#include <BerserkPlatform/Linux.hpp>
#elif defined(BERSERK_TARGET_MACOS)
#include <BerserkPlatform/MacOS.hpp>
#endif

using namespace Berserk;

class App: public Application {
public:
    App() : Application() { mGlobalAllocator = & alloc; }

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

    for (auto& p: ages) {
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

TEST_F(BasicCase,MemoryCow) {
    TArray<int64> first;
    TArray<int64> second;
    const uint32 N = 100;

    for (int32 i = 0; i < N; i++) {
        first.Add(i);
        second.Add(N - i);
    }

    MemoryCow memory(first.GetData(), first.GetSizeBytes());

    Thread thread1([&](){
        std::chrono::nanoseconds duration(10000000);
        ThisThread::sleep_for(duration);

        MemoryCow m = memory;

        auto buffer = (const int64*) m.getDataReadonly();
        for (auto i = 0; i < first.GetSize(); i++) {
            printf("t1: %lli\n", buffer[i]);
        }

        printf("t1: references count: %llu\n", memory.getReferencesCount());

        auto wbuffer = (int64*) m.getData();
        for (auto i = 0; i < first.GetSize(); i++) {
            wbuffer[i] = second[i];
        }

        printf("t1: references count: %llu\n", memory.getReferencesCount());

        auto rbuffer = (const int64*) m.getDataReadonly();
        for (auto i = 0; i < first.GetSize(); i++) {
            printf("t1: %lli\n", rbuffer[i]);
        }
    });

    Thread thread2([&](){
        std::chrono::nanoseconds duration(10000000);
        ThisThread::sleep_for(duration);

        MemoryCow m = memory;

        auto buffer = (const int64*) m.getDataReadonly();
        for (auto i = 0; i < first.GetSize(); i++) {
            printf("t2: %lli\n", buffer[i]);
        }

        printf("t2: references count: %llu\n", memory.getReferencesCount());

        auto wbuffer = (int64*) m.getData();
        for (auto i = 0; i < first.GetSize(); i++) {
            wbuffer[i] = second[i];
        }

        printf("t2: references count: %llu\n", memory.getReferencesCount());


        auto rbuffer = (const int64*) m.getDataReadonly();
        for (auto i = 0; i < first.GetSize(); i++) {
            printf("t2: %lli\n", rbuffer[i]);
        }
    });

    thread1.join();
    thread2.join();

    EXPECT_EQ(memory.getReferencesCount(), 1);
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