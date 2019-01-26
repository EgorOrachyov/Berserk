//
// Created by Egor Orachyov on 06.07.2018.
//

#ifndef BERSERKENGINE_GLSHADERTYPE_H
#define BERSERKENGINE_GLSHADERTYPE_H

#include "Essential/GLInclude.h"

namespace Berserk
{

    /**
     * Types of OpenGL Shaders
     */
    enum GLShaderType
    {
        GLST_VERTEX             = GL_VERTEX_SHADER,
        GLST_FRAGMENT           = GL_FRAGMENT_SHADER,
        GLST_GEOMETRY           = GL_GEOMETRY_SHADER,
        GLST_TESS_CONTROL       = GL_TESS_CONTROL_SHADER,
        GLST_TESS_EVALUATION    = GL_TESS_EVALUATION_SHADER,
        GLST_COMPUTE            = GL_COMPUTE_SHADER
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLSHADERTYPE_H