//
// Created by Egor Orachyov on 19.03.2019.
//

#ifndef BERSERK_IALLOCATOR_H
#define BERSERK_IALLOCATOR_H

#include "Misc/Types.h"

namespace Berserk
{

    class IAllocator
    {
    public:

        virtual void* allocate(uint32 size) = 0;

        virtual void* reallocate(void* old, uint32 size) = 0;

        virtual void free(void* pointer) = 0;

    };

} // namespace Berserk

#endif //BERSERK_IALLOCATOR_H