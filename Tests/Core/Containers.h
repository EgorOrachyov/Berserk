//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_CONTAINERS_H
#define BERSERK_CONTAINERS_H

#include <Containers/TStaticArray.h>

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

        array.sort([](const int32& a, const int32& b){
            return a < b;
        });

        for (auto i : array) {
            printf("%i\n", i);
        }
    }

    static void testArray() {

    }

    static void run() {
        testStaticArray();
    }

};

#endif //BERSERK_CONTAINERS_H
