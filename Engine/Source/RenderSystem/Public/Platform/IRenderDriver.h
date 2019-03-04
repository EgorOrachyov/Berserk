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

    /**
     * Abstract render driver platform layer
     */
    class GRAPHICS_API IRenderDriver
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
            WRAP_CLAMP_TO_EDGE          ,
            WRAP_CLAMP_TO_BORDER,
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
        enum DataType : uint32
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
            RGB16F              ,
            RGBA16F             ,
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

        enum CompareFunc : uint32
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

        enum PolygonMode : uint32
        {
            FILL,
            LINE,
            POINT
        };

    public:
        
        struct ViewPort
        {
            uint32 x;       //! X point coordinate to start writing in screen (fbo) buffer
            uint32 y;       //! Y point coordinate to start writing in screen (fbo) buffer
            uint32 width;   //! Width offset for screen (fbo) writing
            uint32 height;  //! Height offset for screen (fbo) writing
        };

        struct RenderState
        {
            PolygonMode     polygonMode;            //! Mode of interpretation for primitives data in rendering

            CompareFunc     depthFunc;              //! Compare fragment value with depth buffer value
            bool            writeDepth;             //! Enable writing to the depth buffer

            FaceCulling     faceCulling;            //! Which face to cull
            WindingOrder    windingOrder;           //! Winding order for FRONT faces

            BlendFunc       blendFuncSource;        //! Blend factor for fragment color
            BlendFunc       blendFuncDestination;   //! Blend factor for fbo color buffer data

            StencilOp       stencilOpFail;          //! What to do if stencil test failed
            StencilOp       stencilOpDepthFail;     //! What to do if stencil test passed, but depth test failed
            StencilOp       stencilOpPass;          //! What to do if stencil and depth test passed
            CompareFunc     stencilFunc;            //! How to compare CompareValue with value in the buffer
            uint32          stencilWritingMask;     //! Writing mask in stencil buffer
            uint32          stencilCompareMask;     //! AND mask for CompareValue and BufferValue before compare function call
            uint32          stencilCompareValue;    //! Compare reference value
            uint32          stencilClearValue;      //! Value to clear stincil buffer for clear(stencil) function

            Vec4f           clearColor;             //! Color buffer clear color
            ViewPort        viewPort;               //! View port (place to render fbo to screen)

            bool            useFaceCulling;         //! Enable face culling
            bool            useDepthTest;           //! Enable depth testing
            bool            useStencilTest;         //! Enable stencil test
            bool            useScissorTest;         //! Enable scissor test
            bool            useAlphaBlending;       //! Use alpha blending for semi-transparent objects
        };

    public:

        /** Initialize driver and create default main application window */
        virtual void initialize(const IWindow::WindowSetup &setup) = 0;

        /** De-initialize render driver */
        virtual void release() = 0;

        /** Clear chosen buffers */
        virtual void clear(bool color, bool depth, bool stencil) = 0;

        /** Set clear color for color buffer */
        virtual void clear(const Vec4f& color) = 0;

        /** Enable depth test */
        virtual void depthTest(bool enable) = 0;

        /**
         * Enable depth testing, specify writing mask and
         * compare function to pass test.
         *
         * @param enable True to enable, otherwise False
         * @param mask Writing mask (True to write, otherwise False)
         * @param compare Compare value of fragment with value in the buffer
         */
        virtual void depthTest(bool enable, bool mask, CompareFunc compare) = 0;

        /** Enable face culling */
        virtual void faceCulling(bool enable) = 0;

        /**
         * Enable face culling, specify faces to cull, and
         * winding order for front face.
         *
         * @param enable True to enable, otherwise False
         * @param face Which face cull
         * @param order Winding order for FRONT faces
         */
        virtual void faceCulling(bool enable, FaceCulling face, WindingOrder order) = 0;

        /** Enable blending */
        virtual void blending(bool enable) = 0;

        /**
         * Enable blending, specify blending equation source
         * and destination coefficients.
         *
         * @param enable True to enable, otherwise False
         * @param source Blending equation source coefficient
         * @param destination Blending equation destination (buffer value) coefficient
         *
         * @note Blending equation:
         *       result = source * s_color + destination * d_color
         */
        virtual void blending(bool enable, BlendFunc source, BlendFunc destination) = 0;

        /** Enable stencil test */
        virtual void stencilTest(bool enable) = 0;

        /**
         * Enable stencil test and setup test param, writing mask,
         * compare function, and stencil test pass / fail options.
         *
         * @param enable True to enable, otherwise False
         * @param mask Writing mast (8 bit unsigned integer)
         * @param clear Clear value for buffer (8 bit unsigned integer)
         * @param compare (Compare function for the test (Compare data from buffer and value)
         * @param value (Right compare value for compare function)
         * @param read (Mask to AND value and data stored in the buffer)
         * @param st_fail What to do when stencil test failed
         * @param dt_fail What to do when stencil test pass, but depth test failed
         * @param dt_pass What to do when stencil and depth test passed
         */
        virtual void stencilTest(bool enable, uint32 mask, uint32 clear,
                                 CompareFunc compare, uint32 value, uint32 read,
                                 StencilOp st_fail, StencilOp dt_fail, StencilOp dt_pass) = 0;

        /**
         * Change compare function for stencil testing
         *
         * @param compare (Compare function for the test (Compare data from buffer and value)
         * @param value (Right compare value for compare function)
         * @param read (Mask to AND value and data stored in the buffer)
         */
        virtual void stencilTest(CompareFunc compare, uint32 value, uint32 read) = 0;

        /** Specify screen viewport */
        virtual void viewPort(const ViewPort& view) = 0;

        /** Setup render state via state strucuture */
        virtual void setup(const RenderState& state) = 0;

        /** Double buffering swap buffers */
        virtual void swapBuffers() = 0;

        /** Set polygo mode to render scene in lines, points or filled triangles */
        virtual void polygonMode(PolygonMode mode) = 0;

        /** Set chosen window as active for rendering */
        virtual void setActive(IWindow* window) = 0;

        /** Copy main window frame buffer color values */
        virtual void makeScreenShot(PixelFormat format, uint8 *data) = 0;

        /** Essential update call for each frame */
        virtual void update() = 0;

        /** @return Memory cost of this resource (on CPU side only) */
        virtual uint32 getMemoryUsage() = 0;

        /** @return Pointer to main application window */
        virtual IWindow * getMainWindow() = 0;

        /** @return Current render state current structure */
        virtual const RenderState & getRenderState() = 0;

        /** @return Name of driver */
        virtual const char* getName() = 0;

        /** @return Rendering driver info */
        virtual const char* getInfo() = 0;

        /** @return Shader language info */
        virtual const char* getShaderInfo() = 0;

        /** @return Memory used at gpu side */
        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRENDERDRIVER_H