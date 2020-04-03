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

    enum class EVertexElementType : uint32 {
        Float1,
        Float2,
        Float3,
        Float4
    };

    enum class EShaderType : uint32 {
        Vertex,
        Fragment
    };

    enum class EShaderLanguage : uint32 {
        GLSL
    };

    enum class EMemoryType : uint32 {
        Static,
        Dynamic
    };

    enum class EVertexIterating : uint32 {
        PerVertex,
        PerInstance
    };

    enum class EIndexType : uint32 {
        Uint32
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

    enum class EDrawListState : uint32 {
        Write,
        Complete
    };

    // todo: device enums

}

#endif //BERSERK_RHIDEFINITIONS_H
