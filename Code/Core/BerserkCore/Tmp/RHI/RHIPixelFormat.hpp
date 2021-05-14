/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIPIXELFORMAT_H
#define BERSERK_RHIPIXELFORMAT_H

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    enum class EPixelFormat : uint8 {
        R8,
        RGBA8,
        RGBA16F,
        RGBA32F,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    namespace Details {
        const char* PixelFormatToString(EPixelFormat format);
    }
}

#endif //BERSERK_RHIPIXELFORMAT_H
