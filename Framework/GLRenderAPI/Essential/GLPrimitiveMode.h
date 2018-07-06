//
// Created by Egor Orachyov on 06.07.2018.
//

#ifndef BERSERKENGINE_GLPRIMITIVEMODE_H
#define BERSERKENGINE_GLPRIMITIVEMODE_H

#include "GLInclude.h"

namespace Berserk
{

    /**
     * Drawing mode to interpret vao buffers data
     * (defines how program should send to gpu's shaders this data)
     */
    enum GLPrimitiveMode
    {
        GLPM_TRIANGLES  = GL_TRIANGLES,
        GLPM_LINES      = GL_LINES,
        GLPM_LINE_STRIP = GL_LINE_STRIP,
        GLPM_LINE_LOOP  = GL_LINE_LOOP,
        GLPM_POINTS     = GL_POINTS
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLPRIMITIVEMODE_H
