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

} // namespace Berserk

#endif //BERSERK_BUFFERS_H
