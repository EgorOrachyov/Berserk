/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_PIXELFORMAT_H
#define BERSERK_PIXELFORMAT_H

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    enum class EPixelFormat : uint8 {
        R8,
        RGBA8,
        RGBA16F,
        RGBA32F,
        Unknown
    };

    namespace Details {
        const char* PixelFormatToString(EPixelFormat format) {
            switch (format) {
                case EPixelFormat::R8:
                    return "R8";
                case EPixelFormat::RGBA8:
                    return "RGBA8";
                case EPixelFormat::RGBA16F:
                    return "RGBA16F";
                case EPixelFormat::RGBA32F:
                    return "RGBA32F";
                default:
                    return "Unknown";
            }
        }
    }
}

#endif //BERSERK_PIXELFORMAT_H
