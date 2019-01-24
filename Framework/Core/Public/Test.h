//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_TEST_H
#define BERSERK_TEST_H

#include "Public/Misc/Assert.h"
#include "Public/Misc/Include.h"
#include "Public/Misc/Alignment.h"

#include "Public/Memory/Allocator.h"

void AlignmentTest()
{
    int sizes[] = { 8, 16, 17, 32, 341, 299, 1024, 1025 };
    int newsize;

    ALIGN(newsize,sizes[0]);
    printf("Size: %i | Aligned: %i \n", sizes[0], newsize);

    ALIGN(newsize,sizes[1]);
    printf("Size: %i | Aligned: %i \n", sizes[1], newsize);

    ALIGN(newsize,sizes[2]);
    printf("Size: %i | Aligned: %i \n", sizes[2], newsize);

    ALIGN(newsize,sizes[3]);
    printf("Size: %i | Aligned: %i \n", sizes[3], newsize);

    ALIGN(newsize,sizes[4]);
    printf("Size: %i | Aligned: %i \n", sizes[4], newsize);

    ALIGN(newsize,sizes[5]);
    printf("Size: %i | Aligned: %i \n", sizes[5], newsize);

    ALIGN(newsize,sizes[6]);
    printf("Size: %i | Aligned: %i \n", sizes[6], newsize);

    ALIGN(newsize,sizes[7]);
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

#endif //BERSERK_TEST_H
