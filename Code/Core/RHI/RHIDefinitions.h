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

    enum class EVertexElementType : uint8 {
        Float1 = 0,
        Float2,
        Float3,
        Float4,
        Unknown
    };

    enum class EShaderData : uint8 {
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

    enum class EShaderType : uint8 {
        Vertex      = 0,
        Fragment    = 1,
        Unknown
    };

    enum class EShaderLanguage : uint8 {
        GLSL = 1,
        Undefined = 0xff
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

    enum class ETextureCubemapFace : uint8 {
        PositiveX = 0,
        NegativeX = 1,
        PositiveY = 2,
        NegativeY = 3,
        PositiveZ = 4,
        NegativeZ = 5,
    };

    enum class ESamplerFilter : uint8 {
        Nearest,
        Linear
    };

    enum class ESamplerRepeatMode : uint8 {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    enum class ESamplerBorderColor : uint8 {
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
    enum class EPolygonMode : uint8 {
        Fill,
        Line,
        Point
    };

    enum class EPolygonCullMode : uint8 {
        Disabled,
        Front,
        Back,
        FrontAndBack
    };

    /** How front of the primitive is defined */
    enum class EPolygonFrontFace : uint8 {
        Clockwise,
        CounterClockwise
    };

    enum class ECompareFunction : uint8 {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        GreaterEqual,
        NotEqual,
        Always
    };

    enum class EOperation : uint8 {
        Keep,
        Zero,
        Replace,
        Increment,
        Decrement,
        Invert
    };

    enum class EBlendFactor : uint8 {
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

    enum class EBlendOperation : uint8 {
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
        /** Vertex shader max input elements */
        static const uint32 MAX_VERTEX_ATTRIBUTES = 16;
        /** Vertex shader max input vertex buffers (same as elements) */
        static const uint32 MAX_VERTEX_BUFFERS = 16;
        /** Max texture units bound to the shader at the same time */
        static const uint32 MAX_UNIFORM_TEXTURES = 8;
        /** Max uniform blocks bound to the shader at the same time */
        static const uint32 MAX_UNIFORM_BLOCKS = 8;
    };

    class RHIDefinitionsUtil {
    public:
        static EVertexElementType getElementTypeFromString(const CString& type);
        static EVertexIterating getIteratingFromString(const CString& iterating);
        static uint32 getIndexSize(EIndexType type);
        static uint32 getVertexElementSize(EVertexElementType elementType);
        static EShaderLanguage getLanguageFromString(const class CString& language);
        static EShaderType getShaderTypeFromString(const class CString& type);
        static const char* getShaderTypeStringFromEnum(EShaderType type);
        static const char* getVertexElementStringFromEnum(EVertexElementType elementType);
        static const char* getVertexIteratingStringFromEnum(EVertexIterating iterating);
        static TArrayStatic<EShaderType> getShaderTypes();
        static TArrayStatic<CString> getShaderTypesAsString();

    };

}

#endif //BERSERK_RHIDEFINITIONS_H
