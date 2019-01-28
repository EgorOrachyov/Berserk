//
// Created by Egor Orachyov on 16.07.2018.
//

#ifndef BERSERKENGINE_GLTEXTUREMIPMAPS_H
#define BERSERKENGINE_GLTEXTUREMIPMAPS_H

#include "Essential/GLInclude.h"

namespace Berserk
{

    /**
     * Gen or not different quality levels of the texture
     */
    enum GLMipmaps
    {
        GLM_USE         = GL_TRUE,
        GLM_DO_NOT_USE  = GL_FALSE
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLTEXTUREMIPMAPS_H
