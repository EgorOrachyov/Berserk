//
// Created by Egor Orachyov on 05.02.2019.
//

#ifndef BERSERK_DEGREES_H
#define BERSERK_DEGREES_H

#include "Public/Misc/Types.h"
#include "Public/Math/MathUtility.h"
#include "Public/Strings/StaticString.h"
#include "Public/Misc/UsageDescriptors.h"

namespace Berserk
{

    class Degrees final
    {
    public:

        Degrees();

        Degrees(const Degrees& angle) = default;

        explicit Degrees(float32 angle);

        explicit Degrees(const Radians& angle);

        ~Degrees() = default;

        Radians radians() const;

        float32 get() const { return mAngle; }

        CName toString();

    public:

        float32 mAngle;

    };

} // namespace Berserk

#endif //BERSERK_DEGREES_H