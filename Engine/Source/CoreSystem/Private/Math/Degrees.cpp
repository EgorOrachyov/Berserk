//
// Created by Egor Orachyov on 05.02.2019.
//

#include <Math/Radians.h>
#include <Math/Degrees.h>
#include <Utility/Printer.h>


namespace Berserk
{

    Degrees::Degrees() : mAngle(0.0f) {}

    Degrees::Degrees(float32 angle) : mAngle(angle) {}

    Degrees::Degrees(const Radians &angle) : mAngle(Math::degrees(angle.mAngle)) {}

    Radians Degrees::radians() const { return Radians(*this); }

    Name Degrees::toString() const
    {
        Name string;
        Printer::print(string.get(), Name::TOTAL_BUFFER_SIZE, "(%3.3f deg)", mAngle);
        return string;
    }

} // namespace Berserk