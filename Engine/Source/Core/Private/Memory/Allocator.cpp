//
// Created by Egor Orachyov on 2019-10-09.
//

#include <Memory/Allocator.h>
#include <Misc/Error.h>

namespace Berserk {

    Allocator::~Allocator() {
        DEV_ERROR_CONDITION(mCallsAllocate == mCallsFree,
                "Memory leaks detected [allocate: %u][free: %u]", mCallsAllocate, mCallsFree);
    }

    void *Allocator::malloc(uint32 size) {
        void* memory = Memory::malloc(size);
        DEV_ERROR_CONDITION(memory != nullptr, "Cannot allocate memory [size: %u]", size);
        mCallsAllocate += 1;
        return memory;
    }

    void *Allocator::malloc(uint32 size, uint32 alignment) {
        void* memory = Memory::malloc(size, alignment);
        DEV_ERROR_CONDITION(memory != nullptr, "Cannot allocate aligned memory [size: %u][alignment: %u]", size, alignment);
        mCallsAllocate += 1;
        return memory;
    }

    void Allocator::free(void *pointer) {
        Memory::free(pointer);
        mCallsFree += 1;
    }

    Allocator& Allocator::getSingleton() {
        static Allocator alloc;
        return alloc;
    }

}