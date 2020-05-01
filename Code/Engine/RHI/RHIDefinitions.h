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
        Float4,
        Unknown
    };

    enum class EShaderData : uint32 {
        Float1,
        Float2,
        Float3,
        Float4,
        Mat2,
        Mat3,
        Mat4,
        Sampler2D,
        Sampler3D,
        SamplerCube,
        Unknown
    };

    enum class EShaderType : uint32 {
        Vertex      = 0,
        Fragment    = 1,
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
        Nearest,
        Linear
    };

    enum class ESamplerRepeatMode : uint32 {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    enum class ESamplerBorderColor : uint32 {
        Black,
        White
    };

    enum class EDrawListState : uint32 {
        Write,
        Complete
    };

    enum class EPrimitivesType : uint32 {
        Triangles,
        Lines,
        Points
    };

    enum class EPolygonMode : uint32 {
        Fill,
        Line,
        Point
    };

    enum class EPolygonCullMode : uint32 {
        Disabled,
        Front,
        Back
    };

    enum class EPolygonFrontFace : uint32 {
        Clockwise,
        CounterClockwise
    };

    enum class ECompareFunction : uint32 {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        GreaterEqual,
        NotEqual,
        Always
    };

    enum class EBlendFactor : uint32 {
        Zero,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha
    };

    enum class EBlendOperation : uint32 {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max
    };

    enum class EClearOption : uint32 {
        Color   = 0,
        Depth   = 1,
        Stencil = 2
    };

    // todo: device enums

}

#endif //BERSERK_RHIDEFINITIONS_H
