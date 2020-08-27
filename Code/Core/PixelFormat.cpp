/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <PixelFormat.h>

namespace Berserk {
    
    const char* EPixelFormatUtil::pixelFormatToString(EPixelFormat format) {
        switch (format) {
            case EPixelFormat::R8:
                return "R8";
            case EPixelFormat::R8G8B8A8:
                return "R8G8B8A8";
            case EPixelFormat::D24S8:
                return "D24S8";
            case EPixelFormat::D32S8:
                return "D32S8";
            case EPixelFormat::R16G16B16A16f:
                return "R16G16B16A16f";
            default:
                return "Unknown";
        }
    }
    
}