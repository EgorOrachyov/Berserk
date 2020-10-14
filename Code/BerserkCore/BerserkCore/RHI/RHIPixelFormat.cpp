/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/RHI/RHIPixelFormat.hpp>

namespace Berserk {

    namespace Details {

        static const char* FORMAT_NAMES[] = {
            "R8", "RGBA8", "RGBA16F", "RGBA32F"
        };

        const char* PixelFormatToString(EPixelFormat format) {
            const auto COUNT = (uint32) EPixelFormat::MarkerCount;
            auto index = (uint32) format;

            return index < COUNT? FORMAT_NAMES[index]: "Unknown";
        }
    }

}