//
// Created by Egor Orachyov on 05.02.2019.
//

#include "Math/Degrees.h"
#include "Math/Radians.h"

namespace Berserk
{

    Radians::Radians() : mAngle(0.0f) {}

    Radians::Radians(float32 angle) : mAngle(angle) {}

    Radians::Radians(const Degrees &angle) : mAngle(Math::radians(angle.mAngle)) {}

    Degrees Radians::degrees() const { return Degrees(*this); }

    CName Radians::toString() const
    {
        char buffer[Buffers::SIZE_16];
        sprintf(buffer, "(%3.3f rad)", mAngle);
        return CName(buffer);
    }

} // namespace Berserk
