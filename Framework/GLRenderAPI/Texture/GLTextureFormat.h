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
        GLTF_RGBA8  = GL_RGBA8
    };


}

#endif //BERSERKENGINE_GLTEXTUREFORMAT_H
