/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>

#include <BerserkCore/Platform/MemoryCow.hpp>
#include <BerserkCore/Platform/Thread.hpp>
#include <BerserkCore/Containers/TArray.hpp>
#include <BerserkCore/Application.hpp>

using namespace Berserk;

class App: public Application {
public:
    App() : Application() { mGlobalAllocator = & alloc; }

private:

    struct Alloc: public Allocator {
    public:
        ~Alloc() override = default;
        void *allocate(uint64 size) override { return ::malloc(size); }
        void free(void *memory) override { ::free(memory); }
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

    list1.add(list2);
    list1.add({ 6, 7, 8, 9, 10 });
    list1.add(11);
    list1.add(list2.getData(), list2.getSize());

    printf("List 1\n");
    for (auto num: list1) {
        printf("%lli\n", num);
    }

    for (auto itr = list1.begin(); itr != list1.end(); ) {
        if (*itr % 2 == 1) {
            itr.remove();
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
    removing.removeElement(1);
    removing.removeElement(5);
    removing.removeElement(3);
    removing.removeElement(4);
    removing.removeElement(2);

    printf("Raw remove\n");
    for (auto num: removing) {
        printf("%lli\n", num);
    }
}

TEST_F(BasicCase,MemoryCow) {
    TArray<int64> first;
    TArray<int64> second;
    const uint32 N = 100;

    for (int32 i = 0; i < N; i++) {
        first.add(i);
        second.add(N - i);
    }

    MemoryCow memory(first.getData(), first.getSizeBytes());

    Thread thread1([&](){
        std::chrono::nanoseconds duration(10000000);
        ThisThread::sleep_for(duration);

        MemoryCow m = memory;

        auto buffer = (const int64*) m.getDataReadonly();
        for (auto i = 0; i < first.getSize(); i++) {
            printf("t1: %lli\n", buffer[i]);
        }

        printf("t1: references count: %llu\n", memory.getReferencesCount());

        auto wbuffer = (int64*) m.getData();
        for (auto i = 0; i < first.getSize(); i++) {
            wbuffer[i] = second[i];
        }

        printf("t1: references count: %llu\n", memory.getReferencesCount());

        auto rbuffer = (const int64*) m.getDataReadonly();
        for (auto i = 0; i < first.getSize(); i++) {
            printf("t1: %lli\n", rbuffer[i]);
        }
    });

    Thread thread2([&](){
        std::chrono::nanoseconds duration(10000000);
        ThisThread::sleep_for(duration);

        MemoryCow m = memory;

        auto buffer = (const int64*) m.getDataReadonly();
        for (auto i = 0; i < first.getSize(); i++) {
            printf("t2: %lli\n", buffer[i]);
        }

        printf("t2: references count: %llu\n", memory.getReferencesCount());

        auto wbuffer = (int64*) m.getData();
        for (auto i = 0; i < first.getSize(); i++) {
            wbuffer[i] = second[i];
        }

        printf("t2: references count: %llu\n", memory.getReferencesCount());


        auto rbuffer = (const int64*) m.getDataReadonly();
        for (auto i = 0; i < first.getSize(); i++) {
            printf("t2: %lli\n", rbuffer[i]);
        }
    });

    thread1.join();
    thread2.join();

    printf("References count: %llu\n", memory.getReferencesCount());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}