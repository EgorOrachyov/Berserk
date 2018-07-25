//
// Created by Egor Orachyov on 18.07.2018.
//

#ifndef BERSERKENGINE_MATERIALINFO_H
#define BERSERKENGINE_MATERIALINFO_H

#include "Math/Vector3f.h"

namespace Berserk
{

    struct MaterialInfo
    {
    public:

        Vector3f mAmbient;
        Vector3f mDiffuse;
        Vector3f mSpecular;
        FLOAT32  mShininess;

    };

}

#endif //BERSERKENGINE_MATERIALINFO_H
