//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_BUFFERS_H
#define BERSERK_BUFFERS_H

#include "Types.h"

namespace Berserk
{

    enum Buffers
    {
        /** Common buffers sizes in bytes */

        SIZE_8     = 8,
        SIZE_16    = 16,
        SIZE_32    = 32,
        SIZE_64    = 64,
        SIZE_128   = 128,
        SIZE_256   = 256,
        SIZE_512   = 512,
        SIZE_1024  = 1024,
        SIZE_2048  = 2048,
        SIZE_4096  = 4096,
        SIZE_8192  = 8192,
        SIZE_16384 = 16384,

        /** Conversion from bytes to memory units */

        KiB = 1024,
        MiB = KiB * 1024,
        GiB = MiB * 1024
    };

    /** 512 byte buffer for tmp tasks */
    extern char buffer1[Buffers::SIZE_512];

    /** 512 byte buffer for tmp tasks */
    extern char buffer2[Buffers::SIZE_512];

    /** 1 KiB buffer for tmp tasks */
    extern char buffer3[Buffers::SIZE_1024];

    /** 1 KiB buffer for tmp tasks */
    extern char buffer4[Buffers::SIZE_1024];

    /** 4 KiB buffer for tmp tasks */
    extern char buffer5[Buffers::KiB * 4];

    /** 4 KiB buffer for tmp tasks */
    extern char buffer6[Buffers::KiB * 4];

} // namespace Berserk

#endif //BERSERK_BUFFERS_H
