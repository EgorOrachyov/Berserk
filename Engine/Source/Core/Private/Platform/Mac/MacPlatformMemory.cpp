//
// Created by Egor Orachyov on 2019-10-08.
//

#include <Platform/Mac/MacPlatformMemory.h>
#include <stdlib.h>
#include <memory.h>

namespace Berserk {

    void* MacPlatformMemory::malloc(Berserk::uint64 size)
    {
        return ::malloc(size);
    }

    void* MacPlatformMemory::malloc(Berserk::uint64 size, Berserk::uint64 alignment)
    {
        void* pointer = nullptr;
        posix_memalign(&pointer, alignment, size);
        return pointer;
    }

    void* MacPlatformMemory::realloc(void *original, Berserk::uint64 size)
    {
        return ::realloc(original, size);
    }

    void MacPlatformMemory::free(void *&pointer)
    {
        ::free(pointer);
        pointer = nullptr;
    }

    void MacPlatformMemory::copy(void *destination, void *source, Berserk::uint64 size)
    {
        ::memcpy(destination, source, size);
    }

    void MacPlatformMemory::zero(void *source, Berserk::uint64 size)
    {
        ::memset(source, 0x0, size);
    }

}

