/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TPtrShared.h>
#include <BuildOptions.h>

namespace Berserk {
    PtrAllocator::Node::Node() : refcount(0), deallocate(nullptr) {

    }
    PtrAllocator::PtrAllocator() : mAlloc(sizeof(Node)) {

    }
    PtrAllocator::~PtrAllocator() {
#ifdef BERSERK_DEBUG
        auto count = mAlloc.getChunksAllocated();
        if (count != 0) {
            fprintf(stderr, "[Berserk Core] Lost TSharedPtr's [%u]\n", count);
        }
#endif
    }
    PtrAllocator::Node* PtrAllocator::allocate() {
        std::lock_guard<std::mutex> guard(mAccessMutex);
        void* mem = mAlloc.allocate(sizeof(Node));
        return new (mem) Node();
    }
    void PtrAllocator::free(Berserk::PtrAllocator::Node *node) {
        std::lock_guard<std::mutex> guard(mAccessMutex);
        mAlloc.free(node);
    }
    PtrAllocator& PtrAllocator::getSingleton() {
        static PtrAllocator sPtrAllocator;
        return sPtrAllocator;
    }
}