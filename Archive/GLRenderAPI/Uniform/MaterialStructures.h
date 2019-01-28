//
// Created by Egor Orachyov on 01.08.2018.
//

#ifndef BERSERKENGINE_MATERIALSTRUCTURES_H
#define BERSERKENGINE_MATERIALSTRUCTURES_H

#include "Misc/Types.h"

namespace Berserk
{

    class BaseMaterial
    {
    public:
        INT32 Ambient;
        INT32 Diffuse;
        INT32 Specular;
        INT32 Shininess;
    };

    class UMaterialInfo
    {
    public:
        INT32 Ambient;
        INT32 Diffuse;
        INT32 Specular;
        INT32 Shininess;

        INT32 DiffuseMap;
        INT32 SpecularMap;
        INT32 NormalMap;
    };

} // namespace Berserk

#endif //BERSERKENGINE_MATERIALSTRUCTURES_H
