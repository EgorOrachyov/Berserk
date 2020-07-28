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
#include <Containers/TArrayStatic.h>

namespace Berserk {

    enum class EVertexElementType : uint32 {
        Float1 = 0,
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
        Undefined
    };

    enum class EShaderType : uint32 {
        Vertex      = 0,
        Fragment    = 1,
        Unknown
    };

    enum class EShaderLanguage : uint32 {
        GLSL = 1,
        Undefined = 0xffff
    };

    /** Hint for device to allocate internally buffer memory in optimised way */
    enum class EBufferUsage : uint32 {
        /** Update once, use many time */
        Static,
        /** Update many times, use many times */
        Dynamic
    };

    /** Iterating of the elements in the vertex buffer */
    enum class EVertexIterating : uint32 {
        /** Element index changes per vertex */
        PerVertex = 0,
        /** Element index changes per instance */
        PerInstance
    };

    enum class EIndexType : uint32 {
        Uint32,
        Unknown
    };

    enum class ETextureType : uint32 {
        Texture2D
    };

    enum class ETextureCubemapFace : uint32 {
        PositiveX = 0,
        NegativeX = 1,
        PositiveY = 2,
        NegativeY = 3,
        PositiveZ = 4,
        NegativeZ = 5,
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

    /** Types of the geometry formed by input vertex data */
    enum class EPrimitivesType : uint32 {
        Triangles,
        Lines,
        Points
    };

    /** How primitives rasterized */
    enum class EPolygonMode : uint32 {
        Fill,
        Line,
        Point
    };

    enum class EPolygonCullMode : uint32 {
        Disabled,
        Front,
        Back,
        FrontAndBack
    };

    /** How front of the primitive is defined */
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

    enum class EOperation : uint32 {
        Keep,
        Zero,
        Replace,
        Increment,
        Decrement,
        Invert
    };

    enum class EBlendFactor : uint32 {
        /** = 0 */
        Zero,
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
        OneMinusDstAlpha
    };

    enum class EBlendOperation : uint32 {
        /** fragment color added to the color buffer */
        Add,
        /** fragment color subtracted from the color buffer */
        Subtract,
        /** color buffer color is subracted from fragment color  */
        ReverseSubtract,
        /** the min between the fragment and color buffer */
        Min,
        /** the max between the fragment and color buffer */
        Max
    };

    enum class EClearOption : uint32 {
        Color   = 0,
        Depth   = 1,
        Stencil = 2
    };

    class RHIConst {
    public:
        /**  Limit somehow color attachment count */
        static const uint32 MAX_COLOR_ATTACHMENTS = 8;
    };

    class RHIDefinitionsUtil {
    public:
        static uint32 getIndexSize(EIndexType type);
        static uint32 getVertexElementSize(EVertexElementType elementType);
        static EShaderLanguage getLanguageFromString(const class CString& language);
        static EShaderType getShaderTypeFromString(const class CString& type);
        static const char* getShaderTypeStringFromEnum(EShaderType type);
        static const char* getVertexElementStringFromEnum(EVertexElementType elementType);
        static TArrayStatic<EShaderType> getShaderTypes();
        static TArrayStatic<CString> getShaderTypesAsString();

    };

}

#endif //BERSERK_RHIDEFINITIONS_H
