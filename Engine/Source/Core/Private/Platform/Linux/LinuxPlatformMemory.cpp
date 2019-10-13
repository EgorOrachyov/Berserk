//
// Created by Egor Orachyov on 2019-10-13.
//

#include <Platform/Linux/LinuxPlatformMemory.h>
#include <stdlib.h>
#include <memory.h>

namespace Berserk {

    void* LinuxPlatformMemory::malloc(Berserk::uint64 size)
    {
        return ::malloc(size);
    }

    void* LinuxPlatformMemory::malloc(Berserk::uint64 size, Berserk::uint64 alignment)
    {
        void* pointer = nullptr;
        posix_memalign(&pointer, alignment, size);
        return pointer;
    }

    void* LinuxPlatformMemory::realloc(void *original, Berserk::uint64 size)
    {
        return ::realloc(original, size);
    }

    void LinuxPlatformMemory::free(void *pointer)
    {
        ::free(pointer);
        pointer = nullptr;
    }

    void LinuxPlatformMemory::copy(void *destination, const void *source, Berserk::uint64 size)
    {
        ::memcpy(destination, source, size);
    }

    void LinuxPlatformMemory::zero(void *source, Berserk::uint64 size)
    {
        ::memset(source, 0x0, size);
    }

}
