//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_TRANSFORM_H
#define BERSERK_TRANSFORM_H

#include "Public/Math/Rotation.h"
#include "Public/Math/MathUtility.h"

namespace Berserk
{

    class Transform
    {
    public:

    private:

        float32  mScale;
        Vec3f    mTranslation;
        Rotation mRotation;

    };

} // namespace Berserk

#endif //BERSERK_TRANSFORM_H