//
// Created by Egor Orachyov on 2019-07-18.
//

#ifndef BERSERK_GLENUMS_H
#define BERSERK_GLENUMS_H

#include <GLCommon.h>
#include <Rendering/Definitions.h>
#include <Logging/DebugLogMacros.h>

#ifndef COMPONENT_SIZE
#define COMPONENT_SIZE(enum, glenum) \
    case (enum): \
        return glenum;
#endif

namespace Berserk
{

    /** Static converter to GLenum types form RHI types */
    class GLEnums
    {
    public:

        static GLenum ShaderType(EShaderType value)
        {
            switch (value)
            {
                COMPONENT_SIZE(EShaderType::ST_Vertex, GL_VERTEX_SHADER);
                COMPONENT_SIZE(EShaderType::ST_Fragment, GL_FRAGMENT_SHADER);
                COMPONENT_SIZE(EShaderType::ST_Geometry, GL_GEOMETRY_SHADER);
                COMPONENT_SIZE(EShaderType::ST_Compute, GL_COMPUTE_SHADER);
                COMPONENT_SIZE(EShaderType::ST_TessellationControl, GL_TESS_CONTROL_SHADER);
                COMPONENT_SIZE(EShaderType::ST_TessellationEvaluation, GL_TESS_EVALUATION_SHADER);

                default:
                    DEBUG_LOG_WARNING("GLGLShaderType: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum BufferUsage(EBufferUsage value)
        {
            switch (value)
            {
                COMPONENT_SIZE(EBufferUsage::BU_StaticDraw, GL_STATIC_DRAW);
                COMPONENT_SIZE(EBufferUsage::BU_StreamDraw, GL_STREAM_DRAW);
                COMPONENT_SIZE(EBufferUsage::BU_DynamicDraw, GL_DYNAMIC_DRAW);
                COMPONENT_SIZE(EBufferUsage::BU_StaticCopy, GL_STATIC_COPY);
                COMPONENT_SIZE(EBufferUsage::BU_StreamCopy, GL_STREAM_COPY);
                COMPONENT_SIZE(EBufferUsage::BU_DynamicCopy, GL_DYNAMIC_COPY);
                COMPONENT_SIZE(EBufferUsage::BU_StaticRead, GL_STATIC_READ);
                COMPONENT_SIZE(EBufferUsage::BU_StreamRead, GL_STREAM_READ);
                COMPONENT_SIZE(EBufferUsage::BU_DynamicRead, GL_DYNAMIC_READ);

                default:
                    DEBUG_LOG_WARNING("GLGLBufferUsage: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum SamplerFilter(ESamplerFilter value)
        {
            switch (value)
            {
                COMPONENT_SIZE(ESamplerFilter::SF_Nearest, GL_NEAREST);
                COMPONENT_SIZE(ESamplerFilter::SF_Linear, GL_LINEAR);
                COMPONENT_SIZE(ESamplerFilter::SF_Nearest_MipmapNearest, GL_NEAREST_MIPMAP_NEAREST);
                COMPONENT_SIZE(ESamplerFilter::SF_Linear_MipmapNearest, GL_LINEAR_MIPMAP_NEAREST);
                COMPONENT_SIZE(ESamplerFilter::SF_Nearest_MipmapLinear, GL_NEAREST_MIPMAP_LINEAR);
                COMPONENT_SIZE(ESamplerFilter::SF_Linear_MipmapLinear, GL_LINEAR_MIPMAP_LINEAR);

                default:
                    DEBUG_LOG_WARNING("GLSamplerFilter: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum SamplerWrapMode(ESamplerWrapMode value)
        {
            switch (value)
            {
                COMPONENT_SIZE(ESamplerWrapMode::SWM_ClampToEdge, GL_CLAMP_TO_EDGE);
                COMPONENT_SIZE(ESamplerWrapMode::SWM_ClampToBorder, GL_CLAMP_TO_BORDER);
                COMPONENT_SIZE(ESamplerWrapMode::SWM_Repeat, GL_REPEAT);
                COMPONENT_SIZE(ESamplerWrapMode::SWM_ClampMirror, GL_MIRROR_CLAMP_TO_EDGE);
                COMPONENT_SIZE(ESamplerWrapMode::SWM_RepeatMirror, GL_MIRRORED_REPEAT);

                default:
                    DEBUG_LOG_WARNING("GLSamplerWrapMode: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum PrimitiveType(EPrimitiveType value)
        {
            switch (value)
            {
                COMPONENT_SIZE(EPrimitiveType::PT_Triangles, GL_TRIANGLES);
                COMPONENT_SIZE(EPrimitiveType::PT_Points, GL_POINTS);
                COMPONENT_SIZE(EPrimitiveType::PT_LineStrip, GL_LINE_STRIP);
                COMPONENT_SIZE(EPrimitiveType::PT_LineLoop, GL_LINE_LOOP);
                COMPONENT_SIZE(EPrimitiveType::PT_Lines, GL_LINES);
                COMPONENT_SIZE(EPrimitiveType::PT_LineStripAdjacency, GL_LINE_STRIP_ADJACENCY);
                COMPONENT_SIZE(EPrimitiveType::PT_LinesAdjacency, GL_LINES_ADJACENCY);
                COMPONENT_SIZE(EPrimitiveType::PT_TriangleStrip, GL_TRIANGLE_STRIP);
                COMPONENT_SIZE(EPrimitiveType::PT_TriangleFan, GL_TRIANGLE_FAN);
                COMPONENT_SIZE(EPrimitiveType::PT_TriangleStripAdjacency, GL_TRIANGLE_STRIP_ADJACENCY);
                COMPONENT_SIZE(EPrimitiveType::PT_TrianglesAdjacency, GL_TRIANGLES_ADJACENCY);
                COMPONENT_SIZE(EPrimitiveType::PT_Patches, GL_PATCHES);

                default:
                    DEBUG_LOG_WARNING("GLPrimitiveType: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum PixelFormat(EPixelFormat value)
        {
            switch (value)
            {
                COMPONENT_SIZE(EPixelFormat::PF_R, GL_RED);
                COMPONENT_SIZE(EPixelFormat::PF_RG, GL_RG);
                COMPONENT_SIZE(EPixelFormat::PF_RGB, GL_RGB);
                COMPONENT_SIZE(EPixelFormat::PF_BGR, GL_BGR);
                COMPONENT_SIZE(EPixelFormat::PF_RGBA, GL_RGBA);
                COMPONENT_SIZE(EPixelFormat::PF_ABGR, GL_ABGR_EXT);
                COMPONENT_SIZE(EPixelFormat::PF_BGRA, GL_BGRA);
                COMPONENT_SIZE(EPixelFormat::PF_DEPTH, GL_DEPTH_COMPONENT);
                COMPONENT_SIZE(EPixelFormat::PF_DEPTH_AND_STENCIL, GL_DEPTH_STENCIL);

                default:
                    DEBUG_LOG_WARNING("GLPixelFormat: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum DataType(EDataType value)
        {
            switch (value)
            {
                COMPONENT_SIZE(EDataType::DT_Int, GL_INT);
                COMPONENT_SIZE(EDataType::DT_Byte, GL_BYTE);
                COMPONENT_SIZE(EDataType::DT_Short, GL_SHORT);
                COMPONENT_SIZE(EDataType::DT_Float, GL_FLOAT);
                COMPONENT_SIZE(EDataType::DT_UnsignedInt_24_8, GL_UNSIGNED_INT_24_8);
                COMPONENT_SIZE(EDataType::DT_HalfFloat, GL_HALF_FLOAT);
                COMPONENT_SIZE(EDataType::DT_UnsignedInt, GL_UNSIGNED_INT);
                COMPONENT_SIZE(EDataType::DT_UnsignedByte, GL_UNSIGNED_BYTE);
                COMPONENT_SIZE(EDataType::DT_UnsignedShort, GL_UNSIGNED_SHORT);

                default:
                    DEBUG_LOG_WARNING("GLDataType: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum IndexType(EIndexType value)
        {
            switch (value)
            {
                COMPONENT_SIZE(EIndexType::IT_UnsignedByte, GL_UNSIGNED_BYTE);
                COMPONENT_SIZE(EIndexType::IT_UnsignedInt, GL_UNSIGNED_INT);
                COMPONENT_SIZE(EIndexType::IT_UnsignedShort, GL_UNSIGNED_SHORT);

                default:
                    DEBUG_LOG_WARNING("GLIndexType: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum StorageFormat(EStorageFormat value)
        {
            switch (value)
            {
                COMPONENT_SIZE(EStorageFormat::SF_R8, GL_RED);
                COMPONENT_SIZE(EStorageFormat::SF_RGB8, GL_RGB8);
                COMPONENT_SIZE(EStorageFormat::SF_RGBA8, GL_RGBA8);
                COMPONENT_SIZE(EStorageFormat::SF_RGB16F, GL_RGB16F);
                COMPONENT_SIZE(EStorageFormat::SF_RGBA16F, GL_RGBA16F);
                COMPONENT_SIZE(EStorageFormat::SF_RGB32F, GL_RGB32F);
                COMPONENT_SIZE(EStorageFormat::SF_RGBA32F, GL_RGBA32F);
                COMPONENT_SIZE(EStorageFormat::SF_DEPTH24, GL_DEPTH_COMPONENT24);
                COMPONENT_SIZE(EStorageFormat::SF_DEPTH24_STENCIL8, GL_DEPTH24_STENCIL8);

                default:
                    DEBUG_LOG_WARNING("GLStorageFormat: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum FaceCulling(EFaceCulling value)
        {
            switch (value)
            {
                COMPONENT_SIZE(EFaceCulling::FC_Back, GL_BACK);
                COMPONENT_SIZE(EFaceCulling::FC_Front, GL_FRONT);
                COMPONENT_SIZE(EFaceCulling::FC_FrontAndBack, GL_FRONT_AND_BACK);

                default:
                    DEBUG_LOG_WARNING("GLFaceCulling: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum CompareFunc(ECompareFunc value)
        {
            switch (value)
            {
                COMPONENT_SIZE(ECompareFunc::CF_Never, GL_NEVER);
                COMPONENT_SIZE(ECompareFunc::CF_Always, GL_ALWAYS);
                COMPONENT_SIZE(ECompareFunc::CF_Less, GL_LESS);
                COMPONENT_SIZE(ECompareFunc::CF_Greater, GL_GREATER);
                COMPONENT_SIZE(ECompareFunc::CF_LessEqual, GL_LEQUAL);
                COMPONENT_SIZE(ECompareFunc::CF_GreaterEqual, GL_GEQUAL);
                COMPONENT_SIZE(ECompareFunc::CF_Equal, GL_EQUAL);
                COMPONENT_SIZE(ECompareFunc::CF_NotEqual, GL_NOTEQUAL);

                default:
                    DEBUG_LOG_WARNING("GLCompareFunc: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum BlendFunc(EBlendFunc value)
        {
            switch (value)
            {
                COMPONENT_SIZE(EBlendFunc::BF_One, GL_ONE);
                COMPONENT_SIZE(EBlendFunc::BF_SrcAlpha, GL_SRC_ALPHA);
                COMPONENT_SIZE(EBlendFunc::BF_DstAlpha, GL_DST_ALPHA);
                COMPONENT_SIZE(EBlendFunc::BF_OneMinusSrcAlpha, GL_ONE_MINUS_SRC_ALPHA);
                COMPONENT_SIZE(EBlendFunc::BF_OneMinusDstAlpha, GL_ONE_MINUS_DST_ALPHA);

                default:
                    DEBUG_LOG_WARNING("GLBlendFunc: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum StencilOperation(EStencilOperation value)
        {
            switch (value)
            {
                COMPONENT_SIZE(EStencilOperation::SO_Keep, GL_KEEP);
                COMPONENT_SIZE(EStencilOperation::SO_Zero, GL_ZERO);
                COMPONENT_SIZE(EStencilOperation::SO_Replace, GL_REPLACE);
                COMPONENT_SIZE(EStencilOperation::SO_Incr, GL_INCR);
                COMPONENT_SIZE(EStencilOperation::SO_IncrWrap, GL_INCR_WRAP);
                COMPONENT_SIZE(EStencilOperation::SO_DecrWrap, GL_DECR_WRAP);
                COMPONENT_SIZE(EStencilOperation::SO_Decr, GL_DECR);
                COMPONENT_SIZE(EStencilOperation::SO_Invert, GL_INVERT);

                default:
                    DEBUG_LOG_WARNING("GLStencilOperation: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum RasterCullMode(ERasterCullMode value)
        {
            switch (value)
            {
                COMPONENT_SIZE(ERasterCullMode::RCM_Clockwise, GL_CW);
                COMPONENT_SIZE(ERasterCullMode::RCM_CounterClockwise, GL_CCW);

                default:
                    DEBUG_LOG_WARNING("GLRasterCullMode: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum RasterFillMode(ERasterFillMode value)
        {
            switch (value)
            {
                COMPONENT_SIZE(ERasterFillMode::RFM_Point, GL_POINT);
                COMPONENT_SIZE(ERasterFillMode::RFM_Wireframe, GL_LINE);
                COMPONENT_SIZE(ERasterFillMode::RFM_Solid, GL_FILL);

                default:
                    DEBUG_LOG_WARNING("GLRasterFillMode: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

    };

} // namespace Berserk

#undef COMPONENT_SIZE

#endif //BERSERK_GLENUMS_H
