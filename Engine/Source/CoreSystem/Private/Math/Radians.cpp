//
// Created by Egor Orachyov on 05.02.2019.
//

#include <Math/Degrees.h>
#include <Math/Radians.h>
#include <IO/Printer.h>

namespace Berserk
{

    Radians::Radians() : mAngle(0.0f) {}

    Radians::Radians(float32 angle) : mAngle(angle) {}

    Radians::Radians(const Degrees &angle) : mAngle(Math::radians(angle.mAngle)) {}

    Degrees Radians::degrees() const { return Degrees(*this); }

    Name Radians::toString() const
    {
        Name string;
        Printer::print(string.get(), Name::TOTAL_BUFFER_SIZE, "(%3.3f rad)", mAngle);
        return string;
    }

} // namespace Berserk
