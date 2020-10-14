/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_PLATFORMALLOCATOR_HPP
#define BERSERK_PLATFORMALLOCATOR_HPP

#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Platform/Atomic.hpp>

namespace Berserk {

    class PlatformAllocator: public Allocator {
    public:
        PlatformAllocator(): mCallsAllocate(0), mCallsFree(0) {}
        ~PlatformAllocator() override = default;

        void *Allocate(uint64 size) override {
            mCallsAllocate.fetch_add(1);
            return ::malloc(size);
        }

        void Free(void *memory) override {
            mCallsFree.fetch_add(1);
            ::free(memory);
        }

    private:
        AtomicInt64 mCallsAllocate;
        AtomicInt64 mCallsFree;
    };

}

#endif //BERSERK_PLATFORMALLOCATOR_HPP