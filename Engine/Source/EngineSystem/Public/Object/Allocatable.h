//
// Created by Egor Orachyov on 2019-07-28.
//

#ifndef BERSERK_ALLOCATABLE_H
#define BERSERK_ALLOCATABLE_H

#include "stdlib.h"

namespace Berserk
{

    /** Base class supports custom engine new/delete operations */
    class Allocatable
    {
    public:

        /** No default allocation */
        void* operator new  (size_t size) = delete;

        /** No default allocation */
        void* operator new[](size_t size) = delete;

        /** Create object in specified memory region */
        void* operator new  (size_t size, void* memory)
        {
            return memory;
        }

        /** Destruct object, memory is not freed */
        void operator delete (void* memory)
        {

        }

        /** No default delete */
        void operator delete[] (void* memory) = delete;
    };

} // namespace Berserk

#endif //BERSERK_ALLOCATABLE_H