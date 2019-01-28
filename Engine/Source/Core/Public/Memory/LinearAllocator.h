//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_LINEARALLOCATOR_H
#define BERSERK_LINEARALLOCATOR_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Buffers.h"
#include "Public/Misc/UsageDescriptors.h"

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
    class MEMORY_API LinearAllocator
    {
    public:

        /** Because of the efficiency does not allow to create small buffers */
        static const uint32 MIN_BUFFER_SIZE = Buffers::KiB;

    public:

        /**
         * Creates and initializes allocator with chosen
         * size of the buffer
         * @param size Total size of the buffer
         */
        explicit LinearAllocator(uint32 size);

        ~LinearAllocator();

        /**
         * Allocates block in the buffer of the chosen size
         *
         * @param size Chunk to be allocated
         * @return Pointer to the memory
         */
        void* alloc(uint32 size);

        /**
         * Resets or frees all the allocated data by allocator and
         * marks internal buffer as totally free
         *
         * @warning All the allocated data will be lost, therefore this
         *          method should be called only when all the allocated
         *          chunks are not used by systems elements (User should
         *          explicitly ensure this process)
         */
        void free();

        /** @return Currently allocated bytes */
        uint32 getUsage() const;

        /** @return Total size of the whole buffer */
        uint32 getTotalSize() const;

    private:

        void*  mBuffer;         // Pointer to the buffer in the memory
        uint32 mUsage;          // Currently allocated bytes
        uint32 mTotalSize;      // Total size of the buffer in bytes

    };

}

#endif //BERSERK_LINEARALLOCATOR_H