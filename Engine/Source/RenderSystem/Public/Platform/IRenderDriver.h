//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_IRENDERDRIVER_H
#define BERSERK_IRENDERDRIVER_H

#include "Misc/Types.h"
#include "Math/MathInclude.h"
#include "Platform/IWindow.h"

namespace Berserk
{

    class IRenderDriver
    {
    public:

        /**
         * Types of shader programs which can be compiled and attached
         * to the GPU program
         */
        enum ShaderType : uint32
        {
            VERTEX                  ,
            GEOMETRY                ,
            TESSELLATION_CONTROL    ,
            TESSELLATION_EVALUATION ,
            FRAGMENT                ,
            COMPUTE                 ,

            MAX_SHADER_COUNT = 6
        };

        enum BufferUsage : uint32
        {
            USAGE_STATIC_DRAW   ,
            USAGE_STREAM_DRAW   ,
            USAGE_DYNAMIC_DRAW  ,

            USAGE_STATIC_COPY   ,
            USAGE_STREAM_COPY   ,
            USAGE_DYNAMIC_COPY  ,

            USAGE_STATIC_READ   ,
            USAGE_STREAM_READ   ,
            USAGE_DYNAMIC_READ  ,
        };

        enum SamplerFilter : uint32
        {
            FILTER_NEAREST                  ,
            FILTER_LINEAR                   ,
            FILTER_NEAREST_MIPMAP_NEAREST   ,
            FILTER_LINEAR_MIPMAP_NEAREST    ,
            FILTER_NEAREST_MIPMAP_LINEAR    ,
            FILTER_LINEAR_MIPMAP_LINEAR     ,
        };

        enum SamplerWrapMode : uint32
        {
            WRAP_CLAMP          ,
            WRAP_REPEAT         ,
            WRAP_CLAMP_MIRROR   ,
            WRAP_REPEAT_MIRROR  ,
        };

        /**
         * Drawing mode to interpret vao buffers data
         * (defines how program should send to GPU data)
         */
        enum PrimitiveType : uint32
        {
            TRIANGLES                   ,
            POINTS                      ,
            LINE_STRIP                  ,
            LINE_LOOP                   ,
            LINES                       ,
            LINE_STRIP_ADJACENCY        ,
            LINES_ADJACENCY             ,
            TRIANGLE_STRIP              ,
            TRIANGLE_FAN                ,
            TRIANGLE_STRIP_ADJACENCY    ,
            TRIANGLES_ADJACENCY         ,
            PATCHES                     ,
        };

        /**
         * Types of texture storage
         * Note: texture 3d - its cube texture type
         */
        enum TextureType : uint32
        {
            TEXTURE_1D ,
            TEXTURE_2D ,
            TEXTURE_3D 
        };

        /**
         * Formats of pixels stored in the texture arrays
         * (specify via color components ant its order)
         */
        enum PixelFormat : uint32
        {
            R                   ,
            RG                  ,
            RGB                 ,
            BGR                 ,
            RGBA                ,
            ABGR                ,
            BGRA                ,
            DEPTH               ,
            DEPTH_AND_STENCIL   
        };

        /**
         * Types of pixels or an arbitrary data
         * (indices for arrays of elements and etc.)
         */
        enum PixelType : uint32
        {
            INT             ,
            BYTE            ,
            SHORT           ,
            FLOAT           ,
            HALF_FLOAT      ,
            UNSIGNED_INT    ,
            UNSIGNED_BYTE   ,
            UNSIGNED_SHORT  
        };

        /**
         * How to store data of and image or frame buffer
         * in the texture array
         */
        enum StorageFormat : uint32
        {
            RGB8                ,
            RGBA8               ,
            RGB32F              ,
            RGBA32F             ,
            DEPTH24             ,
            DEPTH24_STENCIL8    ,
        };

        enum FaceCulling : uint32
        {
            FACE_CULLING_NONE,
            FACE_CULLING_BACK           ,
            FACE_CULLING_FRONT          ,
            FACE_CULLING_FRONT_AND_BACK ,
        };

        enum DrawFunc : uint32
        {
            DRAW_FUNC_NEVER     ,
            DRAW_FUNC_ALWAYS    ,
            DRAW_FUNC_LESS      ,
            DRAW_FUNC_GREATER   ,
            DRAW_FUNC_LEQUAL    ,
            DRAW_FUNC_GEQUAL    ,
            DRAW_FUNC_EQUAL     ,
            DRAW_FUNC_NOT_EQUAL ,
        };

        enum FrameBuffer : uint32
        {
            ATTACHMENT_COLOR    ,
            ATTACHMENT_DEPTH    ,
            ATTACHMENT_STENCIL  ,
        };

        enum BlendFunc : uint32
        {
            BLEND_FUNC_NONE                 ,
            BLEND_FUNC_ONE                  ,
            BLEND_FUNC_SRC_ALPHA            ,
            BLEND_FUNC_ONE_MINUS_SRC_ALPHA  ,
            BLEND_FUNC_ONE_MINUS_DST_ALPHA  ,
            BLEND_FUNC_DST_ALPHA            ,
        };

        enum StencilOp : uint32
        {
            STENCIL_KEEP        ,
            STENCIL_ZERO        ,
            STENCIL_REPLACE     ,
            STENICL_INCR        ,
            STENCIL_INCR_WRAP   ,
            STENCIL_DECR_WRAP   ,
            STENCIL_DECR        ,
            STENCIL_INVERT      ,
        };

        enum WindingOrder : uint32
        {
            CLOCKWISE           ,
            COUNTER_CLOCKWISE   
        };
        
    public:
        
        struct ViewPort
        {
            float32 startX, startY;
            float32 width, height;
        };

        struct RenderState
        {
            uint32 primitiveType;
            uint32 faceCulling;
            uint32 drawFunc;

            uint32 blendFuncSource;
            uint32 blendFuncDestination;

            uint32 stencilOpFail;
            uint32 stencilOpDepthFail;
            uint32 stencilOpPass;

            uint32 windingOrder;

            Vec3f clearColor;
            ViewPort viewPort;

            bool useDepthTest;
            bool useStencilTest;
            bool useScissorTest;
            bool useAlphaBlending;
        };

    public:

        virtual void initialize(const IWindow::WindowSetup &setup) = 0;

        virtual void release() = 0;

        virtual void clear(bool color, bool depth, bool stencil) = 0;

        virtual void setup(const RenderState& state) = 0;

        virtual void swapBuffers() = 0;

        virtual void setActive(IWindow* window) = 0;

        virtual void makeScreenShot(PixelFormat format, uint8 *data) = 0;

        virtual void update() = 0;

    public:

        virtual IWindow * getMainWindow() = 0;

        virtual const RenderState & getCurrentState() = 0;

        virtual const char* getName() = 0;

        virtual const char* getInfo() = 0;

        virtual const char* getShaderInfo() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRENDERDRIVER_H