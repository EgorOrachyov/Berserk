/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_PIXELFORMAT_H
#define BERSERK_PIXELFORMAT_H

#include <Typedefs.h>

namespace Berserk {

    enum class ETextureChannel : uint8 {
        R = 0,
        G = 1,
        B = 2,
        A = 3
    };

    /** Format of the single pixel color for image */
    enum class EPixelFormat : uint32 {
        R8            = 0,
        R8G8B8A8      = 1,
        D24S8         = 2,
        D32S8         = 3,
        R16G16B16A16f = 4,
        Unknown       = 0xffffffff
    };

    class EPixelFormatUtil {
    public:
        static const char* pixelFormatToString(EPixelFormat format);
    };

}

#endif //BERSERK_PIXELFORMAT_H
