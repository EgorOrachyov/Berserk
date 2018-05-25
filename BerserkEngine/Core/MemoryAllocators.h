//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_MEMORYALLOCATORS_H
#define BERSERKENGINE_MEMORYALLOCATORS_H

#include "Types.h"

namespace Berserk
{
    /**
     * @brief Allocates memory in heap
     *
     * @param size Num of bytes for allocation
     * @return Pointer to allocated memory
     */
    void* Alloc(uint32 size);

    /**
     * @brief Allocates memory in heap for element
     *
     * @param count Number of elements
     * @param size Size of one element
     * @return Pointer to allocated memory
     */
    void* CAlloc(uint32 count, uint32 size);

    /**
     * @brief Reallocates memory in heap
     *
     * @param oldPointer Pointer to data which should be reallocated
     * @param newSize Num of bytes for allocation
     * @return Pointer to allocated memory
     */
    void* ReAlloc(void* oldPointer, uint32 newSize);

    /**
     * @brief Allocates memory in heap
     *
     * @param size Num of bytes for allocation
     * @param alignment Should be power of 2
     * @return Pointer to allocated memory
     */
    void* Alloc_aligned(uint32 size, uint8 alignment);

    /**
     * @brief Allocates memory in heap for element
     *
     * @param count Number of elements
     * @param size Size of one element
     * @param alignment Should be power of 2
     * @return Pointer to allocated memory
     */
    void* CAlloc_aligned(uint32 count, uint32 size, uint8 alignment);

    /**
     * @brief Reallocates memory in heap
     *
     * @param oldPointer Pointer to data which should be reallocated
     * @param newSize Num of bytes for allocation
     * @param alignment Should be power of 2
     * @return Pointer to allocated memory
     */
    void* ReAlloc_aligned(void* oldPointer, uint32 newSize, uint8 alignment);

} // namespace Berserk

#endif //BERSERKENGINE_MEMORYALLOCATORS_H
