//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_BASEMATERIALCOMPONENT_H
#define BERSERKENGINE_BASEMATERIALCOMPONENT_H

#include "Math/Vector3f.h"

namespace Berserk
{

    class BaseMaterialComponent
    {
    public:

        Vector3f mAmbient;
        Vector3f mDiffuse;
        Vector3f mSpecular;
        FLOAT32  mShininess;
    };

}

#endif //BERSERKENGINE_BASEMATERIALCOMPONENT_H