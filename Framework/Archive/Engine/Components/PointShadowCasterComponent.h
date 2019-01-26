//
// Created by Egor Orachyov on 14.08.2018.
//

#ifndef BERSERKENGINE_POINTSHADOWCASTERCOMPONENT_H
#define BERSERKENGINE_POINTSHADOWCASTERCOMPONENT_H

#include "Math/Matrix4x4f.h"

namespace Berserk
{

    class PointShadowCasterComponent
    {
    public:
        Matrix4x4f mView[6];
        Matrix4x4f mProjection;
    };

} // namespace Berserk

#endif //BERSERKENGINE_POINTSHADOWCASTERCOMPONENT_H