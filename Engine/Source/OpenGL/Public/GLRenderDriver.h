//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLRENDERDRIVER_H
#define BERSERK_GLRENDERDRIVER_H

#include "Misc/Types.h"
#include "GLInclude.h"
#include "Platform/IRenderDriver.h"

namespace Berserk
{

    class GLRenderDriver : public IRenderDriver
    {
    public:

        /**
         * Drawing mode to interpret vao buffers data
         * (defines how program should send to GPU data)
         */
        enum PrimitiveMode : uint32
        {
            TRIANGLES   = GL_TRIANGLES,
            LINES       = GL_LINES,
            LINE_STRIP  = GL_LINE_STRIP,
            LINE_LOOP   = GL_LINE_LOOP,
            POINTS      = GL_POINTS
        };

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

    };

} // namespace Berserk




#endif //BERSERK_GLRENDERDRIVER_H
