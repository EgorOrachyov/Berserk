//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHIDEFINITIONS_H
#define BERSERK_RHIDEFINITIONS_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    enum EShaderType : uint8
    {
        ST_Vertex,
        ST_Geometry,
        ST_TessellationControl,
        ST_TessellationEvaluation,
        ST_Fragment,
        ST_Compute,
    };

    enum EBufferUsage : uint8
    {
        BU_StaticDraw,
        BU_StraemDraw,
        BU_DynamicDraw,

        BU_StaticCopy,
        BU_StreamCopy,
        BU_DynamicCopy,

        BU_StaticRead,
        BU_StreamRead,
        BU_DynamicRead,
    };

    enum ESamplerFilter : uint8
    {
        SF_Neares,
        SF_Linear,
        SF_Nearest_MipmapNearest,
        SF_Linear_MipmapNearest,
        SF_Nearest_MipmapLinear,
        SF_Linear_MipmapLinear,
    };

    enum ESamplerWrapMode : uint8
    {
        SWM_ClamptToEdge,
        SWM_ClampToBorder,
        SWM_Repeat,
        SWM_ClampMirror,
        SWM_RepeatMirror,
    };

    enum EPrimitiveType : uint8
    {
        PT_Triangles,
        PT_Points,
        PT_LineStrip,
        PT_LineLoop,
        PT_Lines,
        PT_LineStripAdjacency,
        PT_LinesAdjacency,
        PT_TriangleStrip,
        PT_TriangleFan,
        PT_TriangleStripAdjacency,
        PT_TrianglesAdjacency,
        PT_Patches,
    };

    enum EPixelFormat : uint8
    {
        PF_R,
        PF_RG,
        PF_RGB,
        PF_BGR,
        PF_RGBA,
        PF_ABGR,
        PF_BGRA,
        PF_DEPTH,
        PF_DEPTH_AND_STENCIL
    };

    enum EDataType : uint8
    {
        DT_Int,
        DT_Byte,
        DT_Short,
        DT_Float,
        DT_HalfFloat,
        DT_UnsignedInt,
        DT_UnsignedByte,
        DT_UnsignedShort
    };

    enum EIndexType : uint8
    {
        IT_Int,
        IT_Byte,
        IT_Short,
    };

    enum EStorageFormat : uint8
    {
        SF_R8,
        SF_RGB8,
        SF_RGBA8,
        SF_RGB16F,
        SF_RGBA16F,
        SF_RGB32F,
        SF_RGBA32F,
        SF_DEPTH24,
        SF_DEPTH24_STENCIL8,
    };

    enum EFaceCulling : uint8
    {
        FC_None,
        FC_Back,
        FC_Front,
        FC_FrontAndBack,
    };

    enum ECompareFunc : uint8
    {
        CF_Never,
        CF_Always,
        CF_Less,
        CF_Greater,
        CF_LessEqual,
        CF_GreaterEqual,
        CF_Equal,
        CF_NotEqual,
    };

    enum EBlendFunc : uint8
    {
        BF_None,
        BF_One,
        BF_SrcAlpha,
        BF_DstAlpha,
        BF_OneMinusSrcAlpha,
        BF_OneMinusDstAlpha,
    };

    enum EStencilOperation : uint8
    {
        SO_Keep,
        SO_Zero,
        SO_Replace,
        SO_Incr,
        SO_IncrWrap,
        SO_DecrWrap,
        SO_Decr,
        SO_Invert,
    };

    enum ERasterCullMode : uint8
    {
        RCM_None,
        RCM_Clockwise,
        RCM_CounterClockwise,
    };

    enum ERasterFillMode : uint8
    {
        RFM_Point,
        RFM_Wireframe,
        RFM_Solid,
    };

} // namespace Berserk

#endif //BERSERK_RHIDEFINITIONS_H
