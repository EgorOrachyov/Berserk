//
// Created by Egor Orachyov on 05.02.2019.
//

#include "Public/Math/Radians.h"
#include "Public/Math/Degrees.h"

namespace Berserk
{

    Degrees::Degrees() : mAngle(0.0f) {}

    Degrees::Degrees(float32 angle) : mAngle(angle) {}

    Degrees::Degrees(const Radians &angle) : mAngle(Math::radians(angle.mAngle)) {}

    Radians Degrees::radians() const { return Radians(*this); }

    CName Degrees::toString()
    {
        char buffer[Buffers::SIZE_16];
        sprintf(buffer, "%3.3f deg", mAngle);
        return CName(buffer);
    }

} // namespace Berserk