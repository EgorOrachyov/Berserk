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

    /** Format of the single pixel color for image */
    enum class EPixelFormat : uint32 {
        R8            = 0,
        R8G8B8A8      = 1,
        D24_S8        = 2,
        D32_S8        = 3,
        R16G16B16A16f = 4,
        Unknown       = 0xffffffff
    };

}

#endif //BERSERK_PIXELFORMAT_H
