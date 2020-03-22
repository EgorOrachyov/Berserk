/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MEMORY_H
#define BERSERK_MEMORY_H

#include <Typedefs.h>
#include <new>

namespace Berserk {

    /** Memory utility (wraps standard C operations) */
    class Memory {
    public:
        static void* allocate(uint64 size);
        static void* reallocate(void* source, uint64 newsize);
        static void free(void* memory);
        static void copy(void* destination, const void* source, uint64 size);
        static void set(void* source, uint32 value, uint64 size);

        static uint64 getAllocCalls();
        static uint64 getFreeCalls();
        static const Function<void(void*)> DEFAULT_DEALLOC;
    };

    template <typename T, typename ... TArgs>
    T* objectNew(TArgs&&... args) {
        void* memory = Memory::allocate(sizeof(T));
        return new(memory) T(std::forward<TArgs>(args)...);
    }

    template <typename T>
    void objectDelete(T* object) {
        object->~T();
        Memory::free(object);
    }

}

#endif //BERSERK_MEMORY_H