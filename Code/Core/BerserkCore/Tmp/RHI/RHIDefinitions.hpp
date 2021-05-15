/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_RHIDEFINITIONS_H
#define BERSERK_RHIDEFINITIONS_H

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Containers/TArray.hpp>

namespace Berserk {

    enum class EDrawBackend: uint8 {
        OpenGL4,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class EShaderStage: uint8 {
        Vertex = 0,
        Fragment,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class EShaderLang: uint8 {
        GLSL4 = 0,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class EUniformType : uint8 {
        Float1 = 0,
        Float2,
        Float3,
        Float4,
        Int1,
        Int2,
        Int3,
        Int4,
        Bool1,
        Bool2,
        Bool3,
        Bool4,
        Mat2,
        Mat3,
        Mat4,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class ESamplerType: uint8 {
        Sampler2d = 0,
        Sampler3d,
        SamplerCube,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class EAttribute: uint8 {
        Float1 = 0,
        Float2,
        Float3,
        Float4,
        Int1,
        Int2,
        Int3,
        Int4,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class EAttributeIterating: uint8 {
        PerVertex = 0,
        PerInstance,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class EBufferUsage: uint8 {
        Dynamic = 0,
        Static,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class EIndexType: uint8 {
        Uint32 = 0,
        Uint16,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class ESamplerFilter : uint8 {
        Nearest = 0,
        Linear,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class ESamplerMinFilter : uint8 {
        /** No filtering. Nearest neighbor is used */
        Nearest = 0,
        /** Box filtering. Weighted average of 4 neighbors is used */
        Linear,
        /** Mip-mapping is activated. But no filtering occurs */
        NearestMipmapNearest,
        /** Box filtering within a mip-map level */
        LinearMipmapNearest,
        /** Mip-map levels are interpolated, but no other filtering occurs */
        NearestMipmapLinear,
        /** Both interpolated Mip-mapping and linear filtering are used */
        LinearMipmapLinear,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class ESamplerMagFilter: uint8 {
        /** No filtering. Nearest neighbor is used */
        Nearest = 0,
        /** Box filtering. Weighted average of 4 neighbors is used */
        Linear,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class ESamplerRepeatMode : uint8 {
        Repeat = 0,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class ESamplerBorderColor : uint8 {
        Black = 0,
        White,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class ETextureUsage: uint8 {
        Sampling = 0,
        ColorAttachment,
        DepthAttachment,
        StencilAttachment,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    /** Types of the geometry formed by input vertex data */
    enum class EPrimitivesType : uint32 {
        Triangles = 0,
        Lines,
        Points,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    /** How primitives rasterized */
    enum class EPolygonMode : uint8 {
        Fill = 0,
        Line,
        Point,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class EPolygonCullMode : uint8 {
        Disabled = 0,
        Front,
        Back,
        FrontAndBack,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    /** How front of the primitive is defined */
    enum class EPolygonFrontFace : uint8 {
        Clockwise = 0,
        CounterClockwise,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class ECompareFunction : uint8 {
        Never = 0,
        Less,
        Equal,
        LessEqual,
        Greater,
        GreaterEqual,
        NotEqual,
        Always,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class EOperation : uint8 {
        Keep = 0,
        Zero,
        Replace,
        Increment,
        Decrement,
        Invert,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class EBlendFactor : uint8 {
        /** = 0 */
        Zero = 0,
        /** = 1 */
        One,
        /** = src */
        SrcColor,
        /** = 1 - src */
        OneMinusSrcColor,
        /** = dst */
        DstColor,
        /** = 1 - dst */
        OneMinusDstColor,
        /** = src.a */
        SrcAlpha,
        /** = 1 - src.a */
        OneMinusSrcAlpha,
        /** = dst.a */
        DstAlpha,
        /** = 1 - dst.a  */
        OneMinusDstAlpha,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    enum class EBlendOperation : uint8 {
        /** fragment color added to the color buffer */
        Add = 0,
        /** fragment color subtracted from the color buffer */
        Subtract,
        /** color buffer color is subtracted from fragment color  */
        ReverseSubtract,
        /** the min between the fragment and color buffer */
        Min,
        /** the max between the fragment and color buffer */
        Max,
        // Do not touch this
        MarkerCount,
        MarkerUnknown
    };

    namespace RHI {
        static const uint32 MAX_VERTEX_ATTRIBUTE_COUNT = 8;
        static const uint32 MAX_VERTEX_BUFFER_COUNT = 8;
        static const uint32 MAX_COLOR_BUFFER_COUNT = 8;
        static const uint32 MAX_UNIFORM_BIND_COUNT = 8;
        static const uint32 MAX_SAMPLER_COUNT = 8;
        static const uint32 MAX_PROGRAM_STAGES = 5;

        const char* DrawBackendToString(EDrawBackend backend);
        uint32 GetIndexSize(EIndexType indexType);
    }

    struct RHIProgramStage {
        EShaderStage shaderStage;
        TArray<char> stageSource;
    };

    struct RHIViewArea {
        uint32 left;
        uint32 bottom;
        uint32 width;
        uint32 height;
    };

    struct RHIVertexAttribute {
        uint32 offset;
        uint8 location;
        uint8 stride;
        EAttribute type;
    };

}

#endif //BERSERK_RHIDEFINITIONS_H