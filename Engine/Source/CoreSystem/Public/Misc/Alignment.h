//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_ALIGNMENT_H
#define BERSERK_ALIGNMENT_H

/** Memory settings */

#ifndef VIRTUAL_MEMORY
    #define VIRTUAL_MEMORY
#endif

/**
 * Platform (architecture) dependent memory alignment for single
 * memory allocation call. All the internal custom memory allocators
 * should follow this alignment and use macro align to get proper
 * size for allocating block
 */

#ifndef MEMORY_ALIGNMENT
    #define MEMORY_ALIGNMENT 16
#endif

/** Simple macro which creates size to be the multiple of MEMORY_ALIGNMENT */

#ifdef MEMORY_ALIGNMENT
    #define ALIGN(size) \
         size = (size) + ((size) % MEMORY_ALIGNMENT != 0) * (MEMORY_ALIGNMENT - ((size) % MEMORY_ALIGNMENT));
#else
    #define ALIGN(newsize,size) \
        newsize = size
#endif

#ifndef POWER
    #define POWER(value) \
        ((((value) - 1) & (value) == 0) && ((value) > 0))
#endif

#endif //BERSERK_ALIGNMENT_H
