//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_DEFINITIONS_H
#define BERSERK_DEFINITIONS_H

#include <Misc/Bits.h>
#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /** Target platform of the shader to be loaded */
    enum ENGINE_API EShaderPlatform
    {
        SP_OpenGL,
        SP_DirectX
    };

    /** Type of single shader in program */
    enum EShaderType : uint8
    {
        ST_Vertex,
        ST_Geometry,
        ST_TessellationControl,
        ST_TessellationEvaluation,
        ST_Fragment,
        ST_Compute,
    };

    /** Buffer GPU usage to optimise access */
    enum EBufferUsage : uint8
    {
        BU_StaticDraw,
        BU_StreamDraw,
        BU_DynamicDraw,

        BU_StaticCopy,
        BU_StreamCopy,
        BU_DynamicCopy,

        BU_StaticRead,
        BU_StreamRead,
        BU_DynamicRead,
    };

    /** Texture sampling mode */
    enum ESamplerFilter : uint8
    {
        SF_Nearest,
        SF_Linear,
        SF_Nearest_MipmapNearest,
        SF_Linear_MipmapNearest,
        SF_Nearest_MipmapLinear,
        SF_Linear_MipmapLinear,
    };

    /** Texture wrapping for out of the borders values */
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
        DT_UnsignedInt_24_8,
        DT_HalfFloat,
        DT_UnsignedInt,
        DT_UnsignedByte,
        DT_UnsignedShort
    };

    enum EIndexType : uint8
    {
        IT_UnsignedInt,
        IT_UnsignedByte,
        IT_UnsignedShort,
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
        RCM_Clockwise,
        RCM_CounterClockwise,
    };

    enum ERasterFillMode : uint8
    {
        RFM_Point,
        RFM_Wireframe,
        RFM_Solid,
    };

    enum EDataLayout : uint8
    {
        DL_Position    = SHIFT(0u),
        DL_Normal      = SHIFT(1u),
        DL_Tangent     = SHIFT(2u),
        DL_Bitangent   = SHIFT(3u),
        DL_TexCoords   = SHIFT(4u),

        DL_Vertex      = DL_Position,
        DL_VertexPN    = DL_Position  | DL_Normal,
        DL_VertexPT    = DL_Position  | DL_TexCoords,
        DL_VertexPNT   = DL_VertexPN  | DL_TexCoords,
        DL_VertexPNTBT = DL_VertexPNT | DL_Tangent | DL_Bitangent,
    };

} // namespace Berserk

#endif //BERSERK_DEFINITIONS_H
