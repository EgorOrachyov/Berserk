//
// Created by Egor Orachyov on 2019-08-25.
//

#include <Math/Angles.h>

namespace Berserk
{

    Degrees::Degrees(const Berserk::Radians &radians)
        : mAngle(Math::degrees(radians.get()))
    {

    }

    Radians Degrees::radians() const
    {
        return Radians(Math::radians(mAngle));
    }

    Degrees::operator Radians() const
    {
        return radians();
    }

    Radians::Radians(const Berserk::Degrees &degrees)
        : mAngle(Math::radians(degrees.get()))
    {

    }

    Degrees Radians::degrees() const
    {
        return Degrees(Math::degrees(mAngle));
    }

    Radians::operator Degrees() const
    {
        return degrees();
    }

}