//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_ALLOCATOR_H
#define BERSERK_ALLOCATOR_H

#include "Misc/Types.h"
#include "Misc/Include.h"
#include "Misc/UsageDescriptors.h"
#include "Memory/IAllocator.h"

namespace Berserk
{

    /**
     * @brief Allocator
     *
     * Wrapper for platform specific OS system calls
     * for acquiring memory from OS for engine specific
     * sub-systems and allocators
     *
     * @note #include <malloc.h>
     *       struct mallinfo mallinfo(void);
     *       http://man7.org/linux/man-pages/man3/mallinfo.3.html
     */
    class MEMORY_API Allocator : public IAllocator
    {
    private:

        Allocator();
        ~Allocator() override;

    public:

        /** Wrapper for malloc */
        void* allocate(uint32 size) override;

        /** Wrapper for free */
        void  free(void *pointer) override;

        /** Only one instance for the whole engine */
        static Allocator& get();

    };

}

#endif //BERSERK_ALLOCATOR_H