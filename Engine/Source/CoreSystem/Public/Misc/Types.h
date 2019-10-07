//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_TYPES_H
#define BERSERK_TYPES_H

#include <Misc/Platform.h>
#include <inttypes.h>

namespace Berserk {

    #ifdef TARGET_x86_64
        typedef float    float32;
        typedef double   float64;

        typedef wchar_t  wchar;

        typedef int8_t   int8;
        typedef int16_t  int16;
        typedef int32_t  int32;
        typedef int64_t  int64;

        typedef uint8_t  uint8;
        typedef uint16_t uint16;
        typedef uint32_t uint32;
        typedef uint64_t uint64;
    #endif

} // namespace Berserk

#endif //BERSERK_TYPES_H
