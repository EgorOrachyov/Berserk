//
// Created by Egor Orachyov on 16.07.2018.
//

#ifndef BERSERKENGINE_GLTEXTUREFORMAT_H
#define BERSERKENGINE_GLTEXTUREFORMAT_H

#include "Essential/GLInclude.h"

namespace Berserk
{

    /**
     * Specifies format of stored data
     */
    enum GLImageFormat
    {
        GLTF_RGB    = GL_RGB,
        GLTF_RGBA   = GL_RGBA
    };

    enum GLInternalTextureFormat
    {
        GLTF_RGB8   = GL_RGB8,
        GLTF_RGBA8  = GL_RGBA8,
        GLTF_RGB32F = GL_RGB32F,
        GLTF_RGBA32F = GL_RGBA32F,
        GLTF_DEPTH24 = GL_DEPTH_COMPONENT24,
        GLTF_DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
    };


}

#endif //BERSERKENGINE_GLTEXTUREFORMAT_H
