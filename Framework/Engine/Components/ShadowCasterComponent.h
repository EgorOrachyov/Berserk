//
// Created by Egor Orachyov on 12.08.2018.
//

#ifndef BERSERKENGINE_SHADOWCASTERCOMPONENT_H
#define BERSERKENGINE_SHADOWCASTERCOMPONENT_H

#include "Math/Matrix4x4f.h"

namespace Berserk
{

    class ShadowCasterComponent
    {
    public:
        Matrix4x4f mView;
        Matrix4x4f mProjection;
    };

} // namespace Berserk

#endif //BERSERKENGINE_SHADOWCASTERCOMPONENT_H