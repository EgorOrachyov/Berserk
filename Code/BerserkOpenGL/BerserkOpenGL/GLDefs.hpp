/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLDEFS_HPP
#define BERSERK_GLDEFS_HPP

#include <GL/glew.h>
#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Image/Color.hpp>

#define BERSERK_LOG_GL BERSERK_TEXT("GL")

#define BERSERK_GL_LOG_INFO(...) \
        Debug::GetDebugLog().LogMessageF(BERSERK_LOG_GL, Log::Verbosity::Info, __VA_ARGS__);

#define BERSERK_GL_LOG_ERROR(...) \
        Debug::GetDebugLog().LogMessageF(BERSERK_LOG_GL, Log::Verbosity::Error, __VA_ARGS__);

namespace Berserk {
    namespace RHI {

        class GLDefs {
        public:

            static GLenum getBufferUsage(BufferUsage bufferUsage) {
                switch (bufferUsage) {
                    case BufferUsage::Dynamic:
                        return GL_DYNAMIC_DRAW;
                    case BufferUsage::Static:
                        return GL_STATIC_DRAW;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported BufferUsage"));
                        return GL_NONE;
                }
            }

            static GLenum getShaderType(ShaderType type) {
                switch (type) {
                    case ShaderType::Vertex:
                        return GL_VERTEX_SHADER;
                    case ShaderType::Fragment:
                        return GL_FRAGMENT_SHADER;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported ShaderType"));
                }
            }

            static GLenum getIndexType(IndexType type) {
                switch (type) {
                    case IndexType::Uint32:
                        return GL_UNSIGNED_INT;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported IndexType"));
                }
            }

            static GLenum getIndexSize(IndexType type) {
                switch (type) {
                    case IndexType::Uint32:
                        return sizeof(uint32);
                    case IndexType::Uint16:
                        return sizeof(uint16);
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported IndexType"));
                }
            }

            static void getVertexElementType(VertexElementType type, GLenum &baseType, uint32 &count) {
                switch (type) {
                    case VertexElementType::Float1: {
                        baseType = GL_FLOAT;
                        count = 1;
                        return;
                    }
                    case VertexElementType::Float2: {
                        baseType = GL_FLOAT;
                        count = 2;
                        return;
                    }
                    case VertexElementType::Float3: {
                        baseType = GL_FLOAT;
                        count = 3;
                        return;
                    }
                    case VertexElementType::Float4: {
                        baseType = GL_FLOAT;
                        count = 4;
                        return;
                    }
                    case VertexElementType::Int1: {
                        baseType = GL_INT;
                        count = 1;
                        return;
                    }
                    case VertexElementType::Int2: {
                        baseType = GL_INT;
                        count = 2;
                        return;
                    }
                    case VertexElementType::Int3: {
                        baseType = GL_INT;
                        count = 3;
                        return;
                    }
                    case VertexElementType::Int4: {
                        baseType = GL_INT;
                        count = 4;
                        return;
                    }
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported VertexElementType"));
                }
            }

            static void getTexturePixelFormat(TextureFormat textureFormat, GLint &internalFormat, GLenum &format, GLenum &type) {
                switch (textureFormat) {
                    case TextureFormat::R8: {
                        internalFormat = GL_R8;
                        format = GL_RED;
                        type = GL_UNSIGNED_BYTE;
                        return;
                    }
                    case TextureFormat::R8G8B8A8: {
                        internalFormat = GL_RGBA8;
                        format = GL_RGBA;
                        type = GL_UNSIGNED_BYTE;
                        return;
                    }
//                    case TextureFormat::D24S8: {
//                        internalFormat = GL_DEPTH24_STENCIL8;
//                        format = GL_DEPTH_STENCIL;
//                        type = GL_UNSIGNED_INT_24_8;
//                        return;
//                    }
//                    case TextureFormat::D32S8: {
//                        internalFormat = GL_DEPTH32F_STENCIL8;
//                        format = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
//                        type = GL_FLOAT;
//                        return;
//                    }
//                    case TextureFormat::R16G16B16A16f: {
//                        internalFormat = GL_RGBA16F;
//                        format = GL_RGBA;
//                        type = GL_FLOAT;
//                        return;
//                    }
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PixelFormat"));
                }
            }

            static GLenum getSamplerMinFilter(SamplerMinFilter filter) {
                switch (filter) {
                    case SamplerMinFilter::NearestMipmapNearest:
                        return GL_NEAREST_MIPMAP_NEAREST;
                    case SamplerMinFilter::LinearMipmapNearest:
                        return GL_LINEAR_MIPMAP_NEAREST;
                    case SamplerMinFilter::NearestMipmapLinear:
                        return GL_NEAREST_MIPMAP_LINEAR;
                    case SamplerMinFilter::LinearMipmapLinear:
                        return GL_LINEAR_MIPMAP_LINEAR;
                    case SamplerMinFilter::Nearest:
                        return GL_NEAREST;
                    case SamplerMinFilter::Linear:
                        return GL_LINEAR;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported SamplerMinFilter"));
                }
            }

            static GLenum getSamplerMagFilter(SamplerMagFilter filter) {
                switch (filter) {
                    case SamplerMagFilter::Nearest:
                        return GL_NEAREST;
                    case SamplerMagFilter::Linear:
                        return GL_LINEAR;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported SamplerMagFilter"));
                }
            }

            static GLenum getSamplerRepeatMode(SamplerRepeatMode mode) {
                switch (mode) {
                    case SamplerRepeatMode::Repeat:
                        return GL_REPEAT;
                    case SamplerRepeatMode::ClampToBorder:
                        return GL_CLAMP_TO_BORDER;
                    case SamplerRepeatMode::ClampToEdge:
                        return GL_CLAMP_TO_EDGE;
                    case SamplerRepeatMode::MirroredRepeat:
                        return GL_MIRRORED_REPEAT;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported SamplerRepeatMode"));
                }
            }

            static Color getBorderColor(SamplerBorderColor color) {
                switch (color) {
                    case SamplerBorderColor::Black:
                        return Color(0.0f, 0.0f, 0.0f, 0.0f);
                    case SamplerBorderColor::White:
                        return Color(1.0f, 1.0f, 1.0f, 1.0f);
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT( "Unsupported SamplerBorderColor"));
                }
            }

            static GLenum getPrimitivesType(PrimitivesType type) {
                switch (type) {
                    case PrimitivesType::Triangles:
                        return GL_TRIANGLES;
                    case PrimitivesType::Lines:
                        return GL_LINES;
                    case PrimitivesType::Points:
                        return GL_POINTS;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PrimitivesType"));
                }
            }

            static GLenum getPolygonMode(PolygonMode mode) {
                switch (mode) {
                    case PolygonMode::Fill:
                        return GL_FILL;
                    case PolygonMode::Line:
                        return GL_LINE;
                    case PolygonMode::Point:
                        return GL_POINT;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PolygonMode"));
                }
            }

            static GLenum getPolygonCullMode(PolygonCullMode mode) {
                switch (mode) {
                    case PolygonCullMode::Front:
                        return GL_FRONT;
                    case PolygonCullMode::Back:
                        return GL_BACK;
                    case PolygonCullMode::Disabled:
                        return GL_NONE;
                    case PolygonCullMode::FrontAndBack:
                        return GL_FRONT_AND_BACK;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PolygonCullMode"));
                }
            }

            static GLenum getPolygonFrontFace(PolygonFrontFace frontFace) {
                switch (frontFace) {
                    case PolygonFrontFace::Clockwise:
                        return GL_CW;
                    case PolygonFrontFace::CounterClockwise:
                        return GL_CCW;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported PolygonFrontFace"));
                }
            }

            static GLenum getCompareFunc(CompareFunction function) {
                switch (function) {
                    case CompareFunction::Never:
                        return GL_NEVER;
                    case CompareFunction::Less:
                        return GL_LESS;
                    case CompareFunction::Equal:
                        return GL_EQUAL;
                    case CompareFunction::LessEqual:
                        return GL_LEQUAL;
                    case CompareFunction::Greater:
                        return GL_GREATER;
                    case CompareFunction::GreaterEqual:
                        return GL_GEQUAL;
                    case CompareFunction::NotEqual:
                        return GL_NOTEQUAL;
                    case CompareFunction::Always:
                        return GL_ALWAYS;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported CompareFunction"));
                }
            }

            static GLenum getBlendFactor(BlendFactor factor) {
                switch (factor) {
                    case BlendFactor::Zero:
                        return GL_ZERO;
                    case BlendFactor::One:
                        return GL_ONE;
                    case BlendFactor::SrcColor:
                        return GL_SRC_COLOR;
                    case BlendFactor::OneMinusSrcColor:
                        return GL_ONE_MINUS_SRC_COLOR;
                    case BlendFactor::DstColor:
                        return GL_DST_COLOR;
                    case BlendFactor::OneMinusDstColor:
                        return GL_ONE_MINUS_DST_COLOR;
                    case BlendFactor::SrcAlpha:
                        return GL_SRC_ALPHA;
                    case BlendFactor::OneMinusSrcAlpha:
                        return GL_ONE_MINUS_SRC_ALPHA;
                    case BlendFactor::DstAlpha:
                        return GL_DST_ALPHA;
                    case BlendFactor::OneMinusDstAlpha:
                        return GL_ONE_MINUS_DST_ALPHA;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported BlendFactor"));
                }
            }

            static GLenum getBlendOperation(BlendOperation operation) {
                switch (operation) {
                    case BlendOperation::Add:
                        return GL_FUNC_ADD;
                    case BlendOperation::Subtract:
                        return GL_FUNC_SUBTRACT;
                    case BlendOperation::ReverseSubtract:
                        return GL_FUNC_REVERSE_SUBTRACT;
                    case BlendOperation::Min:
                        return GL_MIN;
                    case BlendOperation::Max:
                        return GL_MAX;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported BlendOperation"));
                }
            }

            static ShaderDataParam getShaderDataParam(GLenum type) {
                switch (type) {
                    case GL_FLOAT:
                        return ShaderDataParam::Float1;
                    case GL_FLOAT_VEC2:
                        return ShaderDataParam::Float2;
                    case GL_FLOAT_VEC3:
                        return ShaderDataParam::Float3;
                    case GL_FLOAT_VEC4:
                        return ShaderDataParam::Float4;
                    case GL_INT:
                        return ShaderDataParam::Int1;
                    case GL_INT_VEC2:
                        return ShaderDataParam::Int2;
                    case GL_INT_VEC3:
                        return ShaderDataParam::Int3;
                    case GL_INT_VEC4:
                        return ShaderDataParam::Int4;
                    case GL_BOOL:
                        return ShaderDataParam::Bool1;
                    case GL_BOOL_VEC2:
                        return ShaderDataParam::Bool2;
                    case GL_BOOL_VEC3:
                        return ShaderDataParam::Bool3;
                    case GL_BOOL_VEC4:
                        return ShaderDataParam::Bool4;
                    case GL_FLOAT_MAT2:
                        return ShaderDataParam::Mat2;
                    case GL_FLOAT_MAT3:
                        return ShaderDataParam::Mat3;
                    case GL_FLOAT_MAT4:
                        return ShaderDataParam::Mat4;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported ShaderData"));
                }
            }

            static ShaderParam getShaderParam(GLenum type) {
                switch (type) {
                    case GL_SAMPLER_2D:
                        return ShaderParam::Sampler2d;
                    case GL_SAMPLER_3D:
                        return ShaderParam::Sampler3d;
                    case GL_SAMPLER_CUBE:
                        return ShaderParam::SamplerCube;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported ShaderData"));
                }
            }

            static VertexElementType getElementType(GLenum type) {
                switch (type) {
                    case GL_FLOAT:
                        return VertexElementType::Float1;
                    case GL_FLOAT_VEC2:
                        return VertexElementType::Float2;
                    case GL_FLOAT_VEC3:
                        return VertexElementType::Float3;
                    case GL_FLOAT_VEC4:
                        return VertexElementType::Float4;
                    case GL_INT:
                        return VertexElementType::Int1;
                    case GL_INT_VEC2:
                        return VertexElementType::Int2;
                    case GL_INT_VEC3:
                        return VertexElementType::Int3;
                    case GL_INT_VEC4:
                        return VertexElementType::Int4;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported VertexElementType"));
                }
            }

            static int32 getShaderDataSize(GLenum type) {
                switch (type) {
                    case GL_FLOAT:
                        return sizeof(float) * 1;
                    case GL_FLOAT_VEC2:
                        return sizeof(float) * 2;
                    case GL_FLOAT_VEC3:
                        return sizeof(float) * 3;
                    case GL_FLOAT_VEC4:
                        return sizeof(float) * 4;
                    case GL_INT:
                        return sizeof(int32) * 1;
                    case GL_INT_VEC2:
                        return sizeof(int32) * 2;
                    case GL_INT_VEC3:
                        return sizeof(int32) * 3;
                    case GL_INT_VEC4:
                        return sizeof(int32) * 4;
                    case GL_BOOL:
                        return sizeof(uint32) * 1;
                    case GL_BOOL_VEC2:
                        return sizeof(uint32) * 2;
                    case GL_BOOL_VEC3:
                        return sizeof(uint32) * 3;
                    case GL_BOOL_VEC4:
                        return sizeof(uint32) * 4;
                    case GL_FLOAT_MAT2:
                        return sizeof(float) * 2 * 2;
                    case GL_FLOAT_MAT3:
                        return sizeof(float) * 3 * 3;
                    case GL_FLOAT_MAT4:
                        return sizeof(float) * 4 * 4;
                    case GL_SAMPLER_2D:
                    case GL_SAMPLER_3D:
                    case GL_SAMPLER_CUBE:
                    default:
                        return 0;
                }
            }

            static GLenum getStencilOp(Operation operation) {
                switch (operation) {
                    case Operation::Decrement:
                        return GL_DECR;
                    case Operation::Increment:
                        return GL_INCR;
                    case Operation::Invert:
                        return GL_INVERT;
                    case Operation::Keep:
                        return GL_KEEP;
                    case Operation::Replace:
                        return GL_REPLACE;
                    case Operation::Zero:
                        return GL_ZERO;
                    default:
                        BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Unsupported Operation"));
                }
            }

        };

    }
}

#endif //BERSERK_GLDEFS_HPP
