//
// Created by Egor Orachyov on 2019-07-18.
//

#ifndef BERSERK_GLENUMS_H
#define BERSERK_GLENUMS_H

#include <GLCommon.h>
#include <RHIDefinitions.h>
#include <Logging/DebugLogMacros.h>

#ifndef CONVERT
#define CONVERT(enum, glenum) \
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
                CONVERT(EShaderType::ST_Vertex, GL_VERTEX_SHADER);
                CONVERT(EShaderType::ST_Fragment, GL_FRAGMENT_SHADER);
                CONVERT(EShaderType::ST_Geometry, GL_GEOMETRY_SHADER);
                CONVERT(EShaderType::ST_Compute, GL_COMPUTE_SHADER);
                CONVERT(EShaderType::ST_TessellationControl, GL_TESS_CONTROL_SHADER);
                CONVERT(EShaderType::ST_TessellationEvaluation, GL_TESS_EVALUATION_SHADER);

                default:
                    DEBUG_LOG_WARNING("GLGLShaderType: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum BufferUsage(EBufferUsage value)
        {
            switch (value)
            {
                CONVERT(EBufferUsage::BU_StaticDraw, GL_STATIC_DRAW);
                CONVERT(EBufferUsage::BU_StreamDraw, GL_STREAM_DRAW);
                CONVERT(EBufferUsage::BU_DynamicDraw, GL_DYNAMIC_DRAW);
                CONVERT(EBufferUsage::BU_StaticCopy, GL_STATIC_COPY);
                CONVERT(EBufferUsage::BU_StreamCopy, GL_STREAM_COPY);
                CONVERT(EBufferUsage::BU_DynamicCopy, GL_DYNAMIC_COPY);
                CONVERT(EBufferUsage::BU_StaticRead, GL_STATIC_READ);
                CONVERT(EBufferUsage::BU_StreamRead, GL_STREAM_READ);
                CONVERT(EBufferUsage::BU_DynamicRead, GL_DYNAMIC_READ);

                default:
                    DEBUG_LOG_WARNING("GLGLBufferUsage: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum SamplerFilter(ESamplerFilter value)
        {
            switch (value)
            {
                CONVERT(ESamplerFilter::SF_Neares, GL_NEAREST);
                CONVERT(ESamplerFilter::SF_Linear, GL_LINEAR);
                CONVERT(ESamplerFilter::SF_Nearest_MipmapNearest, GL_NEAREST_MIPMAP_NEAREST);
                CONVERT(ESamplerFilter::SF_Linear_MipmapNearest, GL_LINEAR_MIPMAP_NEAREST);
                CONVERT(ESamplerFilter::SF_Nearest_MipmapLinear, GL_NEAREST_MIPMAP_LINEAR);
                CONVERT(ESamplerFilter::SF_Linear_MipmapLinear, GL_LINEAR_MIPMAP_LINEAR);

                default:
                    DEBUG_LOG_WARNING("GLSamplerFilter: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum SamplerWrapMode(ESamplerWrapMode value)
        {
            switch (value)
            {
                CONVERT(ESamplerWrapMode::SWM_ClamptToEdge, GL_CLAMP_TO_EDGE);
                CONVERT(ESamplerWrapMode::SWM_ClampToBorder, GL_CLAMP_TO_BORDER);
                CONVERT(ESamplerWrapMode::SWM_Repeat, GL_REPEAT);
                CONVERT(ESamplerWrapMode::SWM_ClampMirror, GL_MIRROR_CLAMP_TO_EDGE);
                CONVERT(ESamplerWrapMode::SWM_RepeatMirror, GL_MIRRORED_REPEAT);

                default:
                    DEBUG_LOG_WARNING("GLSamplerWrapMode: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum PrimitiveType(EPrimitiveType value)
        {
            switch (value)
            {
                CONVERT(EPrimitiveType::PT_Triangles, GL_TRIANGLES);
                CONVERT(EPrimitiveType::PT_Points, GL_POINTS);
                CONVERT(EPrimitiveType::PT_LineStrip, GL_LINE_STRIP);
                CONVERT(EPrimitiveType::PT_LineLoop, GL_LINE_LOOP);
                CONVERT(EPrimitiveType::PT_Lines, GL_LINES);
                CONVERT(EPrimitiveType::PT_LineStripAdjacency, GL_LINE_STRIP_ADJACENCY);
                CONVERT(EPrimitiveType::PT_LinesAdjacency, GL_LINES_ADJACENCY);
                CONVERT(EPrimitiveType::PT_TriangleStrip, GL_TRIANGLE_STRIP);
                CONVERT(EPrimitiveType::PT_TriangleFan, GL_TRIANGLE_FAN);
                CONVERT(EPrimitiveType::PT_TriangleStripAdjacency, GL_TRIANGLE_STRIP_ADJACENCY);
                CONVERT(EPrimitiveType::PT_TrianglesAdjacency, GL_TRIANGLES_ADJACENCY);
                CONVERT(EPrimitiveType::PT_Patches, GL_PATCHES);

                default:
                    DEBUG_LOG_WARNING("GLPrimitiveType: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum PixelFormat(EPixelFormat value)
        {
            switch (value)
            {
                CONVERT(EPixelFormat::PF_R, GL_RED);
                CONVERT(EPixelFormat::PF_RG, GL_RG);
                CONVERT(EPixelFormat::PF_RGB, GL_RGB);
                CONVERT(EPixelFormat::PF_BGR, GL_BGR);
                CONVERT(EPixelFormat::PF_RGBA, GL_RGBA);
                CONVERT(EPixelFormat::PF_ABGR, GL_ABGR_EXT);
                CONVERT(EPixelFormat::PF_BGRA, GL_BGRA);
                CONVERT(EPixelFormat::PF_DEPTH, GL_DEPTH_COMPONENT);
                CONVERT(EPixelFormat::PF_DEPTH_AND_STENCIL, GL_DEPTH_STENCIL);

                default:
                    DEBUG_LOG_WARNING("GLPixelFormat: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum DataType(EDataType value)
        {
            switch (value)
            {
                CONVERT(EDataType::DT_Int, GL_INT);
                CONVERT(EDataType::DT_Byte, GL_BYTE);
                CONVERT(EDataType::DT_Short, GL_SHORT);
                CONVERT(EDataType::DT_Float, GL_FLOAT);
                CONVERT(EDataType::DT_HalfFloat, GL_HALF_FLOAT);
                CONVERT(EDataType::DT_UnsignedInt, GL_UNSIGNED_INT);
                CONVERT(EDataType::DT_UnsignedByte, GL_UNSIGNED_BYTE);
                CONVERT(EDataType::DT_UnsignedShort, GL_UNSIGNED_SHORT);

                default:
                    DEBUG_LOG_WARNING("GLDataType: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum IndexType(EIndexType value)
        {
            switch (value)
            {
                CONVERT(EIndexType::IT_UnsignedByte, GL_UNSIGNED_BYTE);
                CONVERT(EIndexType::IT_UnsignedInt, GL_UNSIGNED_INT);
                CONVERT(EIndexType::IT_UnsignedShort, GL_UNSIGNED_SHORT);

                default:
                    DEBUG_LOG_WARNING("GLIndexType: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum StorageFormat(EStorageFormat value)
        {
            switch (value)
            {
                CONVERT(EStorageFormat::SF_R8, GL_RED);
                CONVERT(EStorageFormat::SF_RGB8, GL_RGB8);
                CONVERT(EStorageFormat::SF_RGBA8, GL_RGBA8);
                CONVERT(EStorageFormat::SF_RGB16F, GL_RGB16F);
                CONVERT(EStorageFormat::SF_RGBA16F, GL_RGBA16F);
                CONVERT(EStorageFormat::SF_RGB32F, GL_RGB32F);
                CONVERT(EStorageFormat::SF_RGBA32F, GL_RGBA32F);
                CONVERT(EStorageFormat::SF_DEPTH24, GL_DEPTH_COMPONENT24);
                CONVERT(EStorageFormat::SF_DEPTH24_STENCIL8, GL_DEPTH24_STENCIL8);

                default:
                    DEBUG_LOG_WARNING("GLStorageFormat: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum FaceCulling(EFaceCulling value)
        {
            switch (value)
            {
                CONVERT(EFaceCulling::FC_Back, GL_BACK);
                CONVERT(EFaceCulling::FC_Front, GL_FRONT);
                CONVERT(EFaceCulling::FC_FrontAndBack, GL_FRONT_AND_BACK);

                default:
                    DEBUG_LOG_WARNING("GLFaceCulling: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum CompareFunc(ECompareFunc value)
        {
            switch (value)
            {
                CONVERT(ECompareFunc::CF_Never, GL_NEVER);
                CONVERT(ECompareFunc::CF_Always, GL_ALWAYS);
                CONVERT(ECompareFunc::CF_Less, GL_LESS);
                CONVERT(ECompareFunc::CF_Greater, GL_GREATER);
                CONVERT(ECompareFunc::CF_LessEqual, GL_LEQUAL);
                CONVERT(ECompareFunc::CF_GreaterEqual, GL_GEQUAL);
                CONVERT(ECompareFunc::CF_Equal, GL_EQUAL);
                CONVERT(ECompareFunc::CF_NotEqual, GL_NOTEQUAL);

                default:
                    DEBUG_LOG_WARNING("GLCompareFunc: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum BlendFunc(EBlendFunc value)
        {
            switch (value)
            {
                CONVERT(EBlendFunc::BF_One, GL_ONE);
                CONVERT(EBlendFunc::BF_SrcAlpha, GL_SRC_ALPHA);
                CONVERT(EBlendFunc::BF_DstAlpha, GL_DST_ALPHA);
                CONVERT(EBlendFunc::BF_OneMinusSrcAlpha, GL_ONE_MINUS_SRC_ALPHA);
                CONVERT(EBlendFunc::BF_OneMinusDstAlpha, GL_ONE_MINUS_DST_ALPHA);

                default:
                    DEBUG_LOG_WARNING("GLBlendFunc: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum StencilOperation(EStencilOperation value)
        {
            switch (value)
            {
                CONVERT(EStencilOperation::SO_Keep, GL_KEEP);
                CONVERT(EStencilOperation::SO_Zero, GL_ZERO);
                CONVERT(EStencilOperation::SO_Replace, GL_REPLACE);
                CONVERT(EStencilOperation::SO_Incr, GL_INCR);
                CONVERT(EStencilOperation::SO_IncrWrap, GL_INCR_WRAP);
                CONVERT(EStencilOperation::SO_DecrWrap, GL_DECR_WRAP);
                CONVERT(EStencilOperation::SO_Decr, GL_DECR);
                CONVERT(EStencilOperation::SO_Invert, GL_INVERT);

                default:
                    DEBUG_LOG_WARNING("GLStencilOperation: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum RasterCullMode(ERasterCullMode value)
        {
            switch (value)
            {
                CONVERT(ERasterCullMode::RCM_Clockwise, GL_CW);
                CONVERT(ERasterCullMode::RCM_CounterClockwise, GL_CCW);

                default:
                    DEBUG_LOG_WARNING("GLRasterCullMode: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

        static GLenum RasterFillMode(ERasterFillMode value)
        {
            switch (value)
            {
                CONVERT(ERasterFillMode::RFM_Point, GL_POINT);
                CONVERT(ERasterFillMode::RFM_Wireframe, GL_LINE);
                CONVERT(ERasterFillMode::RFM_Solid, GL_FILL);

                default:
                    DEBUG_LOG_WARNING("GLRasterFillMode: invalid enum");
            }

            return GL_INVALID_VALUE;
        }

    };

} // namespace Berserk

#undef CONVERT

#endif //BERSERK_GLENUMS_H
