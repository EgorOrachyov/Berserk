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
    enum GLDataType
    {
        GLDT_BOOL   = GL_BOOL,
        GLDT_SHORT  = GL_SHORT,
        GLDT_USHORT = GL_UNSIGNED_SHORT,
        GLDT_INT    = GL_INT,
        GLDT_UINT   = GL_UNSIGNED_INT,
        GLDT_UBYTE  = GL_UNSIGNED_BYTE,
        GLDT_FLOAT  = GL_FLOAT,
        GLDT_DOUBLE = GL_DOUBLE
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLDATATYPE_H