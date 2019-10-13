//
// Created by Egor Orachyov on 2019-10-13.
//

#ifndef BERSERK_LINUXPLATFORMMEMORY_H
#define BERSERK_LINUXPLATFORMMEMORY_H

#include <Platform/Linux/LinuxPlatformTypes.h>

namespace Berserk {

    /** Linux platform memory operations provider */
    class LinuxPlatformMemory {
    public:
        static const uint64 DEFAULT_ALIGNMENT = 16;
        static void* malloc(uint64 size);
        static void* malloc(uint64 size, uint64 alignment);
        static void* realloc(void* original, uint64 size);
        static void free(void* pointer);
        static void copy(void* destination, const void* source, uint64 size);
        static void zero(void* source, uint64 size);
    };

    /** Export as Platform Memory class */
    typedef LinuxPlatformMemory PlatformMemory;

}

#endif //BERSERK_LINUXPLATFORMMEMORY_H
