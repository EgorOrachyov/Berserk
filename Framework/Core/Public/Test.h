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

void AlignmentTest()
{
    int sizes[] = { 8, 16, 17, 32, 341, 299, 1024, 1025 };
    int newsize;

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

    void* ptr = Allocator::getSingleton().memoryAllocate(Buffers::KiB);
    printf("Alloc at address: %p\n", ptr);
    Allocator::getSingleton().memoryFree(ptr);
}

void PoolAllocatorTest()
{
    using namespace Berserk;

    struct Data {
        uint64 value[8];
    };

    PoolAllocator pool(sizeof(Data), 64);

    for (uint32 i = 0; i < 256; i++) {
        pool.alloc();
    }
}

#endif //BERSERK_TEST_H
