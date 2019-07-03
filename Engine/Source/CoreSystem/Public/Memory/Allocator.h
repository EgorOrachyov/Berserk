//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_ALLOCATOR_H
#define BERSERK_ALLOCATOR_H

#include <Misc/Types.h>
#include <Misc/Include.h>
#include <Misc/UsageDescriptors.h>
#include <Memory/IAllocator.h>
#include <Threading/Mutex.h>

#ifndef PROFILE_SYSTEM_ALLOCATOR
    #define PROFILE_SYSTEM_ALLOCATOR 1
#endif // PROFILE_SYSTEM_ALLOCATOR

namespace Berserk
{

    /**
     * @brief PlatformAllocator
     *
     * Wrapper for platform specific OS system calls
     * for acquiring memory from OS for engine specific
     * sub-systems and allocators
     *
     * @note Thread-Safe
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

        /** Wrapper for malloc */
        void* allocate(uint32 size, uint32 alignment) override;

        /** Wrapper for free */
        void  free(void *pointer) override;

        /** @return Total number of memoryFree calls in the engine [in bytes] */
        uint32 getFreeCalls() const override { return mFreeCalls; }

        /** @return Total number of memoryAllocate and memoryCAllocate in the engine [in bytes] */
        uint32 getAllocateCalls() const override { return mAllocCalls; }

        /** @return Total memory usage for the whole time of engine working [in bytes] */
        uint64 getTotalMemoryUsage() const override { return mTotalMemUsage; }

        /** Only one instance for the whole engine */
        static Allocator& get();

    protected:

        /** Global wrapper requires safe access from any thread */
        Mutex mMutex;

        /** Total number of free calls in the engine [in bytes] */
        volatile uint32 mFreeCalls = 0;

        /** Total number of allocate and memoryCAllocate in the engine [in bytes] */
        volatile uint32 mAllocCalls = 0;

        /** Total number of allocated mem (this mem actually could be freed) */
        volatile uint64 mTotalMemUsage = 0;

    };

}

#endif //BERSERK_ALLOCATOR_H