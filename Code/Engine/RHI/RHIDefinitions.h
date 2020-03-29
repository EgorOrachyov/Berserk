/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIDEFINITIONS_H
#define BERSERK_RHIDEFINITIONS_H

#include <Typedefs.h>

namespace Berserk {

    enum class EShaderType : uint32 {
        Vertex,
        Fragment
    };

    enum class EShaderLanguage : uint32 {
        GLSL
    };

    enum class EBufferType : uint32 {
        Static,
        Dynamic,
        Shared
    };

    enum class EBufferDataUsage : uint32 {
        PerVertex,
        PerInstance
    };

    enum class ETextureType : uint32 {
        Texture2D
    };

    enum class ESamplerFilter : uint32 {

    };

    enum class ESamplerRepeatMode : uint32 {

    };

    enum class ESamplerBorderColor : uint32 {

    };

    // todo: device enums

}

#endif //BERSERK_RHIDEFINITIONS_H
