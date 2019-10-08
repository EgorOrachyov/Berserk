//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_MACPLATFORMMEMORY_H
#define BERSERK_MACPLATFORMMEMORY_H

#include <Platform/Mac/MacPlatformTypes.h>

namespace Berserk {

    /** MacOS platform memory operations provider */
    class MacPlatformMemory {
    public:
        static const uint64 DEFAULT_ALIGNMENT = 16;
        static void* malloc(uint64 size);
        static void* malloc(uint64 size, uint64 alignment);
        static void* realloc(void* original, uint64 size);
        static void free(void* &pointer);
        static void copy(void* destination, void* source, uint64 size);
        static void zero(void* source, uint64 size);
    };

    /** Export as Platform Memory class */
    typedef MacPlatformMemory PlatformMemory;

}

#endif //BERSERK_MACPLATFORMMEMORY_H