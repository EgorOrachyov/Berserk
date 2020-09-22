/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>

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

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}