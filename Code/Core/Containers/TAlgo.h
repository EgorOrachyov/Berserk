/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TALGO_H
#define BERSERK_TALGO_H

#include <Containers/TArray.h>
#include <Containers/TArrayStatic.h>

namespace Berserk {

    class TAlgo {
    public:

        static const int32 QSORT_RUN_N2 = 8;

        template <typename T, typename C = TCompareLess<T>>
        static void sort(TArray<T> &array) {
            if (array.size() > 1) {
                C compare;
                qsort<T,C>(array.data(), 0, (int32)array.size() - 1, compare);
            }
        }

        template <typename T, uint32 K, typename C = TCompareLess<T>>
        static void sort(TArrayStatic<T,K> &array) {
            if (array.size() > 1) {
                C compare;
                qsort<T,C>(array.data(), 0, (int32)array.size() - 1, compare);
            }
        }

        template <typename T, typename C>
        static void sort(TArray<T> &array, const C& compare) {
            if (array.size() > 1) {
                qsort<T,C>(array.data(), 0, (int32)array.size() - 1, compare);
            }
        }

        template <typename T, uint32 K, typename C>
        static void sort(TArrayStatic<T,K> &array, const C& compare) {
            if (array.size() > 1) {
                qsort<T,C>(array.data(), 0, (int32)array.size() - 1, compare);
            }
        }

    private:

        template <typename T, typename C>
        static void bsort(T* data, int32 start, int32 end, const C& compare) {
            int32 back = 0;
            for (int32 i = start; i < end; i++) {
                for (int32 j = start; j < end - back; j++) {
                    if (compare(data[j + 1], data[j])) {
                        swap(data[j], data[j + 1]);
                    }
                }
                back += 1;
            }
        }

        template <typename T, typename C>
        static void qsort(T* data, int32 start, int32 end, const C& compare) {

            if (end - start <= QSORT_RUN_N2) {
                bsort<T,C>(data, start, end, compare);
            }
            else {
                int32 pivot = end;
                int32 j = start;

                for (int32 i = start; i < pivot; i++) {
                    if (compare(data[i], data[pivot])) {
                        swap(data[j], data[i]);
                        j += 1;
                    }
                }

                swap(data[j], data[pivot]);

                if (j == start)
                    return;

                qsort<T,C>(data, start, j - 1, compare);
                qsort<T,C>(data, j, end, compare);
            }
        }

    public:

        template <typename T>
        static void swap(T& a, T& b) {
            uint8 tmp[sizeof(T)];
            Memory::copy(tmp, &a, sizeof(T));
            Memory::copy(&a, &b,  sizeof(T));
            Memory::copy(&b, tmp, sizeof(T));
        }

    };

}

#endif //BERSERK_TALGO_H