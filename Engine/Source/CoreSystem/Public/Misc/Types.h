//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_TYPES_H
#define BERSERK_TYPES_H

#include "Platform.h"

namespace Berserk
{

    #ifdef TARGET_x86_64

        typedef float   float32;
        typedef double  float64;

        typedef wchar_t wchar;

        typedef char    int8;
        typedef short   int16;
        typedef int     int32;
        typedef long    int64;

        typedef unsigned char   uint8;
        typedef unsigned short  uint16;
        typedef unsigned int    uint32;
        typedef unsigned long   uint64;

    #endif

} // namespace Berserk

#endif //BERSERK_TYPES_H
