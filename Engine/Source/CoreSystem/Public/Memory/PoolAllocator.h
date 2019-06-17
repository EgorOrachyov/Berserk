//
// Created by Egor Orachyov on 17.06.2019.
//

#ifndef BERSERK_POOLALLOCATOR_H
#define BERSERK_POOLALLOCATOR_H

#include <Memory/IAllocator.h>

namespace Berserk
{

    class MEMORY_API PoolAllocator : public IAllocator
    {
    public:

        ~PoolAllocator() override;

        void *allocate(uint32 size) override;

        void free(void *pointer) override;
    };

} // namespace Berserk

#endif //BERSERK_POOLALLOCATOR_H
