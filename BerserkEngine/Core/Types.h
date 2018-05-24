//
// Created by Egor Orachyov on 22.05.2018.
//

#ifndef BERSERKENGINE_TYPES_H
#define BERSERKENGINE_TYPES_H

#include "Common.h"

#ifdef TARGET_x86_64

typedef char    int8;
typedef short   int16;
typedef int     int32;
typedef long    int62;

typedef float   float32;
typedef double  float64;

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
typedef unsigned long   uint62;

#endif

#endif //BERSERKENGINE_TYPES_H
