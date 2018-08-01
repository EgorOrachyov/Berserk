//
// Created by Egor Orachyov on 01.08.2018.
//

#ifndef BERSERKENGINE_MATERIALSTRUCTURES_H
#define BERSERKENGINE_MATERIALSTRUCTURES_H

#include "Essential/Types.h"

namespace Berserk
{

    struct BaseMaterial
    {
    public:
        INT32 Ambient;
        INT32 Diffuse;
        INT32 Specular;
        INT32 Shininess;
    };

} // namespace Berserk

#endif //BERSERKENGINE_MATERIALSTRUCTURES_H
