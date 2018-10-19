//
// Created by Egor Orachyov on 22.05.2018.
//

#ifndef BERSERKENGINE_TYPES_H
#define BERSERKENGINE_TYPES_H

#include "Platform.h"

namespace Berserk
{

    #ifdef TARGET_x86_64

        typedef float   FLOAT32;
        typedef double  FLOAT64;

        typedef char    CHAR;
        typedef wchar_t WCHAR;
    
        typedef char    INT8;
        typedef short   INT16;
        typedef int     INT32;
        typedef long    INT64;

        typedef unsigned char   UINT8;
        typedef unsigned short  UINT16;
        typedef unsigned int    UINT32;
        typedef unsigned long   UINT64;

    #endif

} // namespace Berserk

#endif //BERSERKENGINE_TYPES_H
