//
// Created by Egor Orachyov on 18.08.2018.
//

#ifndef BERSERKENGINE_DEFERREDSTRUCTURES_H
#define BERSERKENGINE_DEFERREDSTRUCTURES_H

#include "Misc/Types.h"

namespace Berserk
{

    class UDeferred
    {
    public:

        INT32 Model;
        INT32 View;
        INT32 PVM;

        INT32 DiffuseColor;
        INT32 SpecularColor;
        INT32 Shininess;

        INT32 DiffuseMap;
        INT32 SpecularMap;
        INT32 NormalMap;
    };

} // namespace Berserk

#endif //BERSERKENGINE_DEFERREDSTRUCTURES_H
