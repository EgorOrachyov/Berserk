//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_MEMORYALLOCATORS_H
#define BERSERKENGINE_MEMORYALLOCATORS_H

#include "../Essential/Types.h"
#include "../Essential/UsageDescriptors.h"

namespace Berserk
{
    /**
     * @brief Allocates memory in heap
     *
     * @param size Num of bytes for allocation
     * @return Pointer to allocated memory
     */
    MEM_API void* mem_alloc(UINT32 size);

    /**
     * @brief Allocates memory in heap for element
     *
     * @param count Number of elements
     * @param size Size of one element
     * @return Pointer to allocated memory
     */
    MEM_API void* mem_calloc(UINT32 count, UINT32 size);

    /**
     * @brief Reallocates memory in heap
     *
     * @param oldPointer Pointer to data which should be reallocated
     * @param newSize Num of bytes for allocation
     * @return Pointer to allocated memory
     */
    MEM_API void* mem_realloc(void *oldPointer, UINT32 newSize);

    /**
     * @brief Allocates memory in heap
     *
     * @param size Num of bytes for allocation
     * @param alignment Should be power of 2
     * @return Pointer to allocated memory
     */
    MEM_API void* mem_alloc(UINT32 size, UINT8 alignment);

    /**
     * @brief Allocates memory in heap for element
     *
     * @param count Number of elements
     * @param size Size of one element
     * @param alignment Should be power of 2
     * @return Pointer to allocated memory
     */
    MEM_API void* mem_calloc(UINT32 count, UINT32 size, UINT8 alignment);

    /**
     * @brief Reallocates memory in heap
     *
     * @param oldPointer Pointer to data which should be reallocated
     * @param newSize Num of bytes for allocation
     * @param alignment Should be power of 2
     * @return Pointer to allocated memory
     */
    MEM_API void* mem_realloc(void *oldPointer, UINT32 newSize, UINT8 alignment);

    /**
     * @brief Free used memory
     *
     * @param pointer Pointer to allocated memory
     */
    MEM_API void mem_free(void *pointer);

} // namespace Berserk

#endif //BERSERKENGINE_MEMORYALLOCATORS_H
