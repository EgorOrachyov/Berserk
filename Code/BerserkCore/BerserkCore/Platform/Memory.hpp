/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MEMORY_HPP
#define BERSERK_MEMORY_HPP

#include <BerserkCore/Typedefs.hpp>

namespace Berserk {
    namespace Platform {

        class Memory {
        public:
            static const size_t ALIGNMENT = 16;
            static const size_t KiB = 1024;
            static const size_t MiB = 1024 * KiB;

            static void Copy(void* destination, const void* source, size_t sizeInBytes);
            static void Set(void* destination, uint32 value, size_t sizeInBytes);

            static void* Allocate(size_t sizeInBytes);
            static void* Reallocate(void* memory, size_t sizeInBytes);
            static void Deallocate(void* memory);

            static uint64 GetAllocateCalls();
            static uint64 GetDeallocateCalls();

            static size_t AlignSize(size_t size, size_t alignment) {
                return size + (size % alignment == 0? 0: alignment - (size % alignment));
            }

            template<typename T, typename ... TArgs>
            static T* Make(TArgs&& ... args) {
                auto mem = Allocate(sizeof(T));
                return new (mem) T(std::forward<TArgs...>(args)...);
            }

            template<typename T>
            static void Release(T* t) {
                t->~T();
                Deallocate((void*) t);
            }
        };

    }
}

#endif //BERSERK_MEMORY_HPP