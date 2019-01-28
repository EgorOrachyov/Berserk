//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_TEST_H
#define BERSERK_TEST_H

#include "Public/Misc/Assert.h"
#include "Public/Misc/Include.h"
#include "Public/Misc/Alignment.h"

#include "Public/Memory/Allocator.h"
#include "Public/Memory/PoolAllocator.h"
#include "Public/Memory/StackAllocator.h"
#include "Public/Memory/LinearAllocator.h"

void AlignmentTest()
{
    int sizes[] = { 8, 16, 17, 32, 341, 299, 1024, 1025 };
    int newsize;

    printf("\nAlignment test\n");

    newsize = sizes[0];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[0], newsize);

    newsize = sizes[1];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[1], newsize);

    newsize = sizes[2];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[2], newsize);

    newsize = sizes[3];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[3], newsize);

    newsize = sizes[4];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[4], newsize);

    newsize = sizes[5];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[5], newsize);

    newsize = sizes[6];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[6], newsize);

    newsize = sizes[7];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[7], newsize);

}

void AssertTest()
{
    using namespace Berserk;

    ASSERT(16 == 16, "Ups!!");
    ASSERT(12 == 12, "Ups!!");
}

void AllocatorTest()
{
    using namespace Berserk;

    struct Data {
        uint64 value[8];
    };

    void* p[64];

    printf("\nPool allocator\n");
    PoolAllocator pool(sizeof(Data), 64);
    for (int32 i = 0; i < 256; i++) {
        printf("Alloc[%i] %p | usage: %u | total: %u \n",i, pool.alloc(), pool.getUsage(), pool.getTotalSize());
    }

    printf("\nLinear allocator\n");
    LinearAllocator linear(sizeof(Data) * 64);
    for (int32 i = 0; i < 64; i++) {
        printf("Alloc[%i] %p | usage: %u | total: %u \n",i, linear.alloc(sizeof(Data)), linear.getUsage(), linear.getTotalSize());
    }

    printf("\nStack allocator\n");
    StackAllocator stack(Buffers::KiB);
    for (int32 i = 0; i < 10; i++) {
        p[i] = stack.alloc(sizeof(Data));
        printf("Alloc[%i] %p | usage: %u | total: %u \n", i, p[i], stack.getUsage(), stack.getTotalSize());
    }

    for (int32 i = 9; i >= 0; i--) {
        stack.free(p[i]);
        printf("Free[%i] | usage: %u | total: %u \n", i, stack.getUsage(), stack.getTotalSize());
    }

    printf("\nAllocator data\n");
    printf("Total mem usage: %lu | Alloc calls: %u | Free calls: %u \n",
           Allocator::getSingleton().getTotalMemoryUsage(),
           Allocator::getSingleton().getAllocCalls(),
           Allocator::getSingleton().getFreeCalls()
    );

    printf("\n");
}

#endif //BERSERK_TEST_H
