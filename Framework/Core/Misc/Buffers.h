//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_BUFFERS_H
#define BERSERKENGINE_BUFFERS_H

#include "Types.h"

namespace Berserk
{

    enum Buffers
    {
        /** Common buffers sizes if bytes */
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
        MiB = 1048576,
        GiB = 1073741824
    };

    /** 512 byte buffer for tmp tasks */
    extern CHAR buffer1[Buffers::SIZE_512];

    /** 512 byte buffer for tmp tasks */
    extern CHAR buffer2[Buffers::SIZE_512];

    /** 1 KiB buffer for tmp tasks */
    extern CHAR buffer3[Buffers::SIZE_1024];

    /** 1 KiB buffer for tmp tasks */
    extern CHAR buffer4[Buffers::SIZE_1024];

} // namespace Berserk

#endif //BERSERKENGINE_BUFFERS_H
