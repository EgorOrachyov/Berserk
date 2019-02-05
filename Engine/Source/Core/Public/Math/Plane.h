//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_PLANE_H
#define BERSERK_PLANE_H

#include "Public/Math/TVector3.h"
#include "Public/Strings/StaticString.h"
#include "Public/Misc/UsageDescriptors.h"

namespace Berserk
{

    class CORE_EXPORT Plane
    {
    public:



    private:

        Vector3f mNorm;
        Vector3f mPoint;

    };

} // namespace Berserk

#endif //BERSERK_PLANE_H