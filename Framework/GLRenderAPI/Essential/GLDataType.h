//
// Created by Egor Orachyov on 06.07.2018.
//

#ifndef BERSERKENGINE_GLDATATYPE_H
#define BERSERKENGINE_GLDATATYPE_H

#include "GLInclude.h"

namespace Berserk
{

    /**
     * Common names of basic types in gpu program
     */
    enum GLParamType
    {
        GLPT_BOOL   = GL_BOOL,
        GLPT_SHORT  = GL_SHORT,
        GLPT_USHORT = GL_UNSIGNED_SHORT,
        GLPT_INT    = GL_INT,
        GLPT_UINT   = GL_UNSIGNED_INT,
        GLPT_FLOAT  = GL_FLOAT,
        GLPT_DOUBLE = GL_DOUBLE
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLDATATYPE_H