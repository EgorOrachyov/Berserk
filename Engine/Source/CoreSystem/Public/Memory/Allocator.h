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
        ~Allocator();

    public:

        /** Wrapper for malloc */
        void* allocate(uint32 size) override;

        /** Wrapper for realloc */
        void* reallocate(void *old, uint32 size) override;

        /** Wrapper for free */
        void  free(void *pointer) override;

        /** @return Total number of memoryFree calls in the engine [in bytes] */
        uint32 getFreeCalls() const;

        /** @return Total number of memoryAllocate and memoryCAllocate in the engine [in bytes] */
        uint32 getAllocateCalls() const;

        /** @retrun Total number of memoryReallocate in the engine [in bytes] */
        uint32 getReallocateCalls() const;

        /** @return Total memory usage for the whole time of engine working [in bytes] */
        uint64 getTotalMemoryUsage() const;

        /** Only one instance for the whole engine */
        static Allocator& getSingleton();

    private:

        uint32 mFreeCalls;      // Total number of free calls in the engine [in bytes]
        uint32 mAllocCalls;     // Total number of allocate and memoryCAllocate in the engine [in bytes]
        uint32 mReallocCalls;   // Total number of reallocate in the engine [in bytes]
        uint64 mTotalMemUsage;  // Total number of allocated mem (this mem actually could be freed)

    };

}

#endif //BERSERK_ALLOCATOR_H