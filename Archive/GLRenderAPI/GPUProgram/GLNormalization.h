//
// Created by Egor Orachyov on 06.07.2018.
//

#ifndef BERSERKENGINE_GLNORMALIZATION_H
#define BERSERKENGINE_GLNORMALIZATION_H

#include "Essential/GLInclude.h"

namespace Berserk
{

    /**
     * Flags to use or do not use normalization of params (transform to [0;1] or to [-1;1])
     */
    enum GLNormalization
    {
        GLN_USE         = GL_TRUE,
        GLN_DO_NOT_USE  = GL_FALSE
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLNORMALIZATION_H