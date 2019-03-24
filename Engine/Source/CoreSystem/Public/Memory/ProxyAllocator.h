//
// Created by Egor Orachyov on 24.03.2019.
//

#ifndef BERSERK_PROXYALLOCATOR_H
#define BERSERK_PROXYALLOCATOR_H

#include "Misc/Types.h"
#include "Misc/Buffers.h"
#include "Misc/Compilation.h"
#include "Misc/UsageDescriptors.h"
#include "Memory/IAllocator.h"

namespace Berserk
{

    /**
     * Proxy allocator creates on to of any base allocator class to
     * collect memory resources acquirement and stat. Does not have any
     * memory logic - only redirects allocation calls to profiled allocator object.
     */
    class MEMORY_API ProxyAllocator : public IAllocator
    {
    public:

        /** @param allocator Pointer to allocator object to profile it */
        explicit ProxyAllocator(IAllocator* allocator);

        ~ProxyAllocator() final = default;

        /** @copydoc IAllocator::allocate() */
        void* allocate(uint32 size) final;

        /** @copydoc IAllocator::free() */
        void free(void *pointer) final;

    private:

        /** Profiled allocator */
        IAllocator* mAllocator;

    };

} // namespace Berserk

#endif //BERSERK_PROXYALLOCATOR_H