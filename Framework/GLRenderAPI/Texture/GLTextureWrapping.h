//
// Created by Egor Orachyov on 16.07.2018.
//

#ifndef BERSERKENGINE_GLTEXTUREWRAPPING_H
#define BERSERKENGINE_GLTEXTUREWRAPPING_H

#include "Essential/GLInclude.h"

namespace Berserk
{

    /**
     * Handel situations when texture's UV leave [0;1] area
     */
    enum GLWrapping
    {
        GLW_REPEAT          = GL_REPEAT,
        GLW_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        GLW_CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
        GLW_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLTEXTUREWRAPPING_H
