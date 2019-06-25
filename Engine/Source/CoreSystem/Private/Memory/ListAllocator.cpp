//
// Created by Egor Orachyov on 17.06.2019.
//

#include "Memory/ListAllocator.h"

namespace Berserk
{

    ListAllocator::ListAllocator(uint32 bufferSize, IAllocator &allocator)
    {

    }

    ListAllocator::~ListAllocator() {

    }

    void *ListAllocator::allocate(uint32 size) {
        return nullptr;
    }

    void ListAllocator::free(void *pointer) {

    }
} // namespace Berserk

namespace Berserk
{

} // namespace Berserk