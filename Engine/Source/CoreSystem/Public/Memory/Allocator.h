//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_ALLOCATOR_H
#define BERSERK_ALLOCATOR_H

#include "Misc/Types.h"
#include "Misc/Include.h"
#include "Misc/UsageDescriptors.h"

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
    class MEMORY_API Allocator
    {
    private:

        Allocator();
        ~Allocator();

    public:

        /** Wrapper for malloc */
        void* memoryAllocate(size_t size);

        /** Wrapper for calloc */
        void* memoryCAllocate(size_t count, size_t size);

        /** Wrapper for realloc */
        void* memoryReallocate(void *old, size_t size);

        /** Wrapper for free */
        void  memoryFree(void* pointer);

        /** @return Total number of memoryFree calls in the engine [in bytes] */
        uint32 getFreeCalls() const;

        /** @return Total number of memoryAllocate and memoryCAllocate in the engine [in bytes] */
        uint32 getAllocCalls() const;

        /** @retrun Total number of memoryReallocate in the engine [in bytes] */
        uint32 getReallocCalls() const;

        /** @return Total memory usage for the whole time of engine working [in bytes] */
        uint64 getTotalMemoryUsage() const;

        /** Only one instance for the whole engine */
        static Allocator& getSingleton();

    private:

        uint32 mFreeCalls;      // Total number of memoryFree calls in the engine [in bytes]
        uint32 mAllocCalls;     // Total number of memoryAllocate and memoryCAllocate in the engine [in bytes]
        uint32 mReallocCalls;   // Total number of memoryReallocate in the engine [in bytes]
        uint64 mTotalMemUsage;  // Total number of allocated mem (this mem actually could be freed)

    };

}

#endif //BERSERK_ALLOCATOR_H