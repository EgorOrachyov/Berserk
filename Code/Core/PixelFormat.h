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
        R8          = 0,
        R8G8B8A8    = 1,
        Undefined   = 0xffffffff
    };

}

#endif //BERSERK_PIXELFORMAT_H
