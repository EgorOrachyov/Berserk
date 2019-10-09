//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_CONTAINERS_H
#define BERSERK_CONTAINERS_H

#include <Containers/TStaticArray.h>
#include <Containers/TArray.h>
#include <Memory/StaticAllocator.h>

using namespace Berserk;

struct Containers {

    static void testStaticArray() {
        TStaticArray<int32> array = { 1999, 20, 2, 1999, 3, 18};

        for (auto i : array) {
            printf("%i\n", i);
        }

        array.append( { 5, 4, 3, 2, 1 } );

        for (auto i : array) {
            printf("%i\n", i);
        }

        auto element = array.find([](const int32& a){
            return 2 * a == 40;
        });

        if (element.isPresent()) {
            printf("Found: %i\n", *element.get());
        } else {
            printf("Not found\n");
        }

        array.append({1});

        array.sort([](const int32& a, const int32& b){
            return a < b;
        });

        for (auto i : array) {
            printf("%i\n", i);
        }

        printf("Move\n");
        TStaticArray<int32, 16> array3 = std::move(array);
        for (auto &t : array3) {
            printf("%u\n", t);
        }

        printf("Copy\n");
        TStaticArray<int32, 20> array2 = array;
        for (auto &t : array2) {
            printf("%u\n", t);
        }
    }

    static void testArray() {
        TArray<int32> array = { 14, 14, 45, 78997, 334, 3 };

        array.append( { 2, 33, 11 } );
        array.append( {} );

        array.sort([](const int32& a, const int32& b) {
            return a > b;
        });

        printf("Array test\n");

        auto element = array.find([](const int32& a){
            return ((a - 1) & a) == 0x0;
        });

        if (element.isPresent()) {
            printf("Found: %i\n", *element.get());
        }

        for (auto &i: array) {
            printf("%i\n", i);
        }

        printf("Static Allocator\n");
        TArray<uint32> sizes = { 12, 45, 44, 11, 64, 11 };
        StaticAllocator<Memory::KiB> alloc;
        for (uint32 size: sizes) {
            void* mem = alloc.malloc(size, 16);
            printf("%p \n", mem);
        }

        TArray<uint64> staticArray(alloc);
        staticArray.append( { 112, 12, 23, 21, 89, 11111, 3 } );
        printf("%u\n", alloc.getMemomyUsage());
    }

    static void run() {
        testStaticArray();
        testArray();
    }

};

#endif //BERSERK_CONTAINERS_H
