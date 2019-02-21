//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLRENDERDRIVER_H
#define BERSERK_GLRENDERDRIVER_H

#include "GLInclude.h"
#include "GLWindow.h"
#include "Platform/IRenderDriver.h"

namespace Berserk
{

    class GLRenderDriver : public IRenderDriver
    {
    public:

        /**
         * Types of shader programs which can be compiled and attached
         * to the GPU program
         */
        enum ShaderType : uint32
        {
            VERTEX                  = GL_VERTEX_SHADER,
            GEOMETRY                = GL_GEOMETRY_SHADER,
            TESSELLATION_CONTROL    = GL_TESS_CONTROL_SHADER,
            TESSELLATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
            FRAGMENT                = GL_FRAGMENT_SHADER,
            COMPUTE                 = GL_COMPUTE_SHADER,

            MAX_SHADER_COUNT = 6
        };

        enum BufferUsage : uint32
        {
            USAGE_STATIC_DRAW   = GL_STATIC_DRAW,
            USAGE_STREAM_DRAW   = GL_STREAM_DRAW,
            USAGE_DYNAMIC_DRAW  = GL_DYNAMIC_DRAW,

            USAGE_STATIC_COPY   = GL_STATIC_COPY,
            USAGE_STREAM_COPY   = GL_STREAM_COPY,
            USAGE_DYNAMIC_COPY  = GL_DYNAMIC_COPY,

            USAGE_STATIC_READ   = GL_STATIC_READ,
            USAGE_STREAM_READ   = GL_STREAM_READ,
            USAGE_DYNAMIC_READ  = GL_DYNAMIC_READ,
        };

        enum SamplerFilter : uint32
        {
            FILTER_NEAREST                  = GL_NEAREST,
            FILTER_LINEAR                   = GL_LINEAR,
            FILTER_NEAREST_MIPMAP_NEAREST   = GL_NEAREST_MIPMAP_NEAREST,
            FILTER_LINEAR_MIPMAP_NEAREST    = GL_LINEAR_MIPMAP_NEAREST,
            FILTER_NEAREST_MIPMAP_LINEAR    = GL_NEAREST_MIPMAP_LINEAR,
            FILTER_LINEAR_MIPMAP_LINEAR     = GL_LINEAR_MIPMAP_LINEAR,
        };

        enum SamplerWrapMode : uint32
        {
            WRAP_CLAMP          = GL_CLAMP_TO_EDGE,
            WRAP_REPEAT         = GL_REPEAT,
            WRAP_CLAMP_MIRROR   = GL_MIRROR_CLAMP_TO_EDGE,
            WRAP_REPEAT_MIRROR  = GL_MIRRORED_REPEAT,
        };

        /**
         * Drawing mode to interpret vao buffers data
         * (defines how program should send to GPU data)
         */
        enum PrimitiveType
        {
            TRIANGLES                   = GL_TRIANGLES,
            POINTS                      = GL_POINTS,
            LINE_STRIP                  = GL_LINE_STRIP,
            LINE_LOOP                   = GL_LINE_LOOP,
            LINES                       = GL_LINES,
            LINE_STRIP_ADJACENCY        = GL_LINE_STRIP_ADJACENCY,
            LINES_ADJACENCY             = GL_LINES_ADJACENCY,
            TRIANGLE_STRIP              = GL_TRIANGLE_STRIP,
            TRIANGLE_FAN                = GL_TRIANGLE_FAN,
            TRAINGLE_STRIP_ADJACENCY    = GL_TRIANGLE_STRIP_ADJACENCY,
            TRIANGLES_ADJACENCY         = GL_TRIANGLES_ADJACENCY,
            PATCHES                     = GL_PATCHES,
        };

        /**
         * Types of texture storage
         * Note: texture 3d - its cube texture type
         */
        enum TextureType : uint32
        {
            TEXTURE_1D = GL_TEXTURE_1D,
            TEXTURE_2D = GL_TEXTURE_2D,
            TEXTURE_3D = GL_TEXTURE_3D
        };

        /**
         * Formats of pixels stored in the texture arrays
         * (specify via color components ant its order)
         */
        enum PixelFormat : uint32
        {
            R                   = GL_R,
            RG                  = GL_RG,
            RGB                 = GL_RGB,
            BGR                 = GL_BGR,
            RGBA                = GL_RGBA,
            ABGR                = GL_ABGR_EXT,
            DEPTH               = GL_DEPTH_COMPONENT,
            DEPTH_AND_STENCIL   = GL_DEPTH_STENCIL
        };

        /**
         * Types of pixels or an arbitrary data
         * (indices for arrays of elements and etc.)
         */
        enum PixelType : uint32
        {
            INT             = GL_INT,
            BYTE            = GL_BYTE,
            SHORT           = GL_SHORT,
            FLOAT           = GL_FLOAT,
            HALF_FLOAT      = GL_HALF_FLOAT,
            UNSIGNED_INT    = GL_UNSIGNED_INT,
            UNSIGNED_BYTE   = GL_UNSIGNED_BYTE,
            UNSIGNED_SHORT  = GL_UNSIGNED_SHORT
        };

        /**
         * How to store data of and image or frame buffer
         * in the texture array
         */
        enum StorageFormat : uint32
        {
            RGB8                = GL_RGB8,
            RGBA8               = GL_RGBA8,
            RGB32F              = GL_RGB32F,
            RGBA32F             = GL_RGBA32F,
            DEPTH24             = GL_DEPTH,
            DEPTH24_STENCIL8    = GL_DEPTH24_STENCIL8,
        };

        enum FaceCulling : uint32
        {
            FACE_CULLING_NONE,
            FACE_CULLING_BACK           = GL_BACK,
            FACE_CULLING_FRONT          = GL_FRONT,
            FACE_CULLING_FRONT_AND_BACK = GL_FRONT_AND_BACK,
        };

        enum DrawFunc : uint32
        {
            DRAW_FUNC_NEVER     = GL_NEVER,
            DRAW_FUNC_ALWAYS    = GL_ALWAYS,
            DRAW_FUNC_LESS      = GL_LESS,
            DRAW_FUNC_GREATER   = GL_GREATER,
            DRAW_FUNC_LEQUAL    = GL_LEQUAL,
            DRAW_FUNC_GEQUAL    = GL_GEQUAL,
            DRAW_FUNC_EQUAL     = GL_EQUAL,
            DRAW_FUNC_NOT_EQUAL = GL_NOTEQUAL,
        };

        enum FrameBuffer : uint32
        {
            ATTACHMENT_COLOR    = GL_COLOR_ATTACHMENT0,
            ATTACHMENT_DEPTH    = GL_DEPTH_ATTACHMENT,
            ATTACHMENT_STENCIL  = GL_STENCIL_ATTACHMENT,
        };

        enum BlendFunc : uint32
        {
            BLEND_FUNC_NONE,
            BLEND_FUNC_ONE                  = GL_ONE,
            BLEND_FUNC_SRC_ALPHA            = GL_SRC_ALPHA,
            BLEND_FUNC_ONE_MINUS_SRC_ALPHA  = GL_ONE_MINUS_SRC_ALPHA,
            BLEND_FUNC_ONE_MINUS_DST_ALPHA  = GL_ONE_MINUS_DST_ALPHA,
            BLEND_FUNC_DST_ALPHA            = GL_DST_ALPHA,
        };

        enum StencilOp : uint32
        {
            STENCIL_KEEP        = GL_KEEP,
            STENCIL_ZERO        = GL_ZERO,
            STENCIL_REPLACE     = GL_REPLACE,
            STENICL_INCR        = GL_INCR,
            STENCIL_INCR_WRAP   = GL_INCR_WRAP,
            STENCIL_DECR_WRAP   = GL_DECR_WRAP,
            STENCIL_DECR        = GL_DECR,
            STENCIL_INVERT      = GL_INVERT,
        };

        enum WindingOrder : uint32
        {
            CLOCKWISE           = GL_CW,
            COUNTER_CLOCKWISE   = GL_CCW
        };

    public:

        void initialize(uint32 width, uint32 height, const char* caption) override;

        void release() override;

        void clear(bool color, bool depth, bool stencil) override;

        void setup(const RenderState& state) override;

        void swapBuffers() override;

        virtual void setActive(IWindow* window) override;

        const IWindow* getMainWindow() override;

        const RenderState* getCurrentState() override;

        const char* getName() override;

        const char* getInfo() override;

        const char* getShaderInfo() override;

    public:

        GLWindow mMainWindow;
        RenderState mRenderState;

    };

} // namespace Berserk




#endif //BERSERK_GLRENDERDRIVER_H
