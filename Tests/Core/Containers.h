//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_CONTAINERS_H
#define BERSERK_CONTAINERS_H

#include <Containers/TStaticArray.h>
#include <Containers/TArray.h>
#include <Containers/TLinkedList.h>
#include <Memory/StaticAllocator.h>
#include "TestMacro.h"

using namespace Berserk;

struct Containers {

    TEST_FUNCTION_OBJECT(TStaticArray) {
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

        uint32 sum = 0;
        array.forEach([&](const int32& i){
            sum += i;
        });

        printf("Sum = %u\n", sum);

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

        printf("Empty array\n");
        TStaticArray<int32> empty;
        for (auto e : empty) {
            printf("Something\n");
        }
    }

    TEST_FUNCTION_OBJECT(TArray) {
        TArray<int32> array = { 14, 14, 45, 78997, 334, 3 };

        array.append( { 2, 33, 11 } );
        array.append( {} );

        array.sort([](const int32& a, const int32& b) {
            return a > b;
        });

        auto element = array.find([](const int32& a){
            return ((a - 1) & a) == 0x0;
        });

        if (element.isPresent()) {
            printf("Found: %i\n", *element.get());
        }

        for (auto &i: array) {
            printf("%i\n", i);
        }

        printf("Static Alloc\n");
        TArray<uint32> sizes = { 12, 45, 44, 11, 64, 11 };
        StaticAllocator<Memory::KiB> alloc;
        for (uint32 size: sizes) {
            void* mem = alloc.malloc(size, 16);
            printf("%p \n", mem);
        }

        TArray<uint64> staticArray(alloc);
        staticArray.append( { 112, 12, 23, 21, 89, 11111, 3 } );
        printf("%u\n", alloc.getMemoryUsage());
    }

    TEST_FUNCTION_OBJECT(TLinkedList) {

        TLinkedListBase<int32> list;
        list.append({ 2, 535, 64, 656, 66, 6 });
        list.sort([](const int32& a, const int32& b) {
            return a < b;
        });
        for (auto& e: list) {
            printf("%i\n", e);
        }
        list.forEach([](const int32& i){
            printf("%i\n", i + 10);
        });

        TLinkedListBase<TArray<int32>> ofArrays;
        ofArrays.append( { TArray<int32>() } );
        ofArrays.get(0).append( { 2123, 213 } );
    }

    TEST_RUN() {
        TEST_SECTION_NAME("Containers")
        TEST_FUNCTION_CALL(TStaticArray);
        TEST_FUNCTION_CALL(TArray);
        TEST_FUNCTION_CALL(TLinkedList);
    }

};

#endif //BERSERK_CONTAINERS_H
