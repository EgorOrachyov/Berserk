//
// Created by Egor Orachyov on 16.07.2018.
//

#ifndef BERSERKENGINE_GLTEXTUREFILTERING_H
#define BERSERKENGINE_GLTEXTUREFILTERING_H

#include "Essential/GLInclude.h"

namespace Berserk
{

    /**
     * Mode of getting result color from the texture
     */
    enum GLFiltering
    {
        GLF_NEAREST = GL_NEAREST,
        GLF_LINEAR  = GL_LINEAR
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLTEXTUREFILTERING_H
