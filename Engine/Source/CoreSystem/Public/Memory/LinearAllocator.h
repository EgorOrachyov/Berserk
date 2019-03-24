//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_LINEARALLOCATOR_H
#define BERSERK_LINEARALLOCATOR_H

#include "Misc/Types.h"
#include "Misc/Buffers.h"
#include "Misc/UsageDescriptors.h"
#include "Memory/IAllocator.h"
#include "Memory/Allocator.h"
#include "Profiling/ProfilingMacro.h"

namespace Berserk
{

    /**
     * @brief Linear Allocator
     *
     * List allocator which allocates blocks of chosen size
     * in the buffer of fixed capacity one by one
     *
     * Allows only to free all the allocated chunks at once
     */
    class MEMORY_API LinearAllocator : public IAllocator
    {
    public:

        /** Because of the efficiency does not allow to create small buffers */
        static const uint32 MIN_BUFFER_SIZE = Buffers::KiB;

    public:

        /**
         * Creates and initializes allocator with chosen
         * size of the buffer
         * @param size Total size of the buffer
         * @param allocator  Allocator, which allocates memory for this one
         */
        explicit LinearAllocator(uint32 size, IAllocator* allocator = nullptr);

        ~LinearAllocator() override;

        /**
         * Allocates block in the buffer of the chosen size
         *
         * @param size Chunk to be allocated
         * @return Pointer to the memory
         */
        void* allocate(uint32 size) override;

        void free(void* pointer) override { return; }

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
        void*  mBuffer;             // Pointer to the buffer in the memory
        uint32 mUsage;              // Currently allocated bytes

    };

}

#endif //BERSERK_LINEARALLOCATOR_H