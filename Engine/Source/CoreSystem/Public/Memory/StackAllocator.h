//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_STACKALLOCATOR_H
#define BERSERK_STACKALLOCATOR_H

#include "Misc/Types.h"
#include "Misc/Buffers.h"
#include "Misc/UsageDescriptors.h"
#include "Memory/IAllocator.h"
#include "Memory/Allocator.h"
#include "Profiling/ProfilingMacro.h"

namespace Berserk
{

    /**
     * @brief Stack Allocator
     *
     * Basic stack allocator which gives memory from pre-allocated fixed size
     * buffer. Does not allow random free of allocated memory, However, as stack
     * allows to free previously allocated chunk of the data
     *
     * Working scheme:
     *
     * data1 alloc()
     * data2 alloc()
     * data3 alloc()
     *
     * ... some important work
     *
     * free(data3)
     * free(data2)
     * free(data1)
     *
     * or call free() to free whole stack buffer
     */
    class MEMORY_API StackAllocator : public IAllocator
    {
    public:

        /** Because of the efficiency does not allow to create small buffers */
        static const uint32 MIN_BUFFER_SIZE = Buffers::KiB;

    private:

        struct Data
        {
            Data*  prev;        // Pointer to the previous state of the stack
            uint64 size;        // Size of that allocated block
                                // Total size 16 bytes (multiple of MEMORY_ALIGNMENT)
        };

    public:

        /**
         * Creates and initializes stack allocator with buffer of chosen size
         * @param size Fixed size of buffer for allocating memory
         * @param allocator  Allocator, which allocates memory for this one
         */
        explicit StackAllocator(uint32 size, IAllocator* allocator = nullptr);

        ~StackAllocator() override;

        /**
         * Allocates block in the buffer of the chosen size
         *
         * @param size Chunk to be allocated
         * @return Pointer to the memory
         */
        void* allocate(uint32 size) override;

        /**
         * Free previously allocated chunk of the data
         *
         * @warning The freed pointer should be the last allocated chunk of data
         *          by this allocator, if it is not, the allocator will crash the
         *          system with an fatal error. Therefore, user should explicitly
         *          follow the correct order of alloc and free calls
         *
         * @param pointer Pointer to the data to be freed
         */
        void free(void* pointer) override;

        /**
         * Resets or frees all the allocated data by allocator and
         * marks internal buffer as totally free
         *
         * @warning All the allocated data will be lost, therefore this
         *          method should be called only when all the allocated
         *          chunks are not used by systems elements (User should
         *          explicitly ensure this process)
         */
        void clear();

        /** @return Currently allocated bytes */
        uint32 getUsage() const;

    private:

        IAllocator * mAllocator;    // Allocator, which allocates memory for this one
        Data*  mBuffer;             // Pointer to the buffer in the memory
        uint32 mUsage;              // Currently allocated bytes

    };

}

#endif //BERSERK_STACKALLOCATOR_H