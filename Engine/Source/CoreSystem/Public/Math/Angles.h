//
// Created by Egor Orachyov on 2019-08-25.
//

#ifndef BERSERK_ANGLES_H
#define BERSERK_ANGLES_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Math/MathUtility.h>

namespace Berserk
{

    /** Represents float angle in degrees */
    class CORE_EXPORT Degrees
    {
    public:

        /** Construct from raw value */
        Degrees(float32 degrees) : mAngle(degrees) {}

        /** Construct from radians */
        Degrees(const Radians& radians);

        Degrees operator+(const Degrees& degrees) const { return Degrees(mAngle + degrees.mAngle); }
        Degrees operator-(const Degrees& degrees) const { return Degrees(mAngle - degrees.mAngle); }
        Degrees operator*(const Degrees& degrees) const { return Degrees(mAngle * degrees.mAngle); }
        Degrees operator/(const Degrees& degrees) const { return Degrees(mAngle / degrees.mAngle); }

        const Degrees& operator+=(const Degrees& degrees) { mAngle += degrees.mAngle; }
        const Degrees& operator-=(const Degrees& degrees) { mAngle -= degrees.mAngle; }
        const Degrees& operator*=(const Degrees& degrees) { mAngle *= degrees.mAngle; }
        const Degrees& operator/=(const Degrees& degrees) { mAngle /= degrees.mAngle; }

        Degrees operator-() const { return Degrees(-mAngle); }
        Degrees operator+() const { return Degrees(mAngle); }

        explicit operator Radians() const;

        /** @return Raw float value */
        float32 get() const { return mAngle; }

        /** @return Radian representation of the angle */
        Radians radians() const;

        /**
         * Convert to string
         * @param degrees Angle to convert
         * @return String representation of the angle
         */
        friend StringDynamic toString(const Degrees& degrees);

    private:

        float32 mAngle;

    };

    /** Represents float angle in radians */
    class CORE_EXPORT Radians
    {
    public:

        /** Construct from raw value */
        Radians(float32 radians) : mAngle(radians) {}

        /** Construct from degrees */
        Radians(const Degrees& degrees);

        Radians operator+(const Radians& radians) const { return Radians(mAngle + radians.mAngle); }
        Radians operator-(const Radians& radians) const { return Radians(mAngle - radians.mAngle); }
        Radians operator*(const Radians& radians) const { return Radians(mAngle * radians.mAngle); }
        Radians operator/(const Radians& radians) const { return Radians(mAngle / radians.mAngle); }

        const Radians& operator+=(const Radians& radians) { mAngle += radians.mAngle; }
        const Radians& operator-=(const Radians& radians) { mAngle -= radians.mAngle; }
        const Radians& operator*=(const Radians& radians) { mAngle *= radians.mAngle; }
        const Radians& operator/=(const Radians& radians) { mAngle /= radians.mAngle; }

        Radians operator-() const { return Radians(-mAngle); }
        Radians operator+() const { return Radians(mAngle); }

        explicit operator Degrees() const;

        /** @return Raw float value */
        float32 get() const { return mAngle; }

        /** @return Radian representation of the angle */
        Degrees degrees() const;

        /**
         * Convert to string
         * @param radians Angle to convert
         * @return String representation of the angle
         */
        friend StringDynamic toString(const Radians& radians);

    private:

        float32 mAngle;

    };

} // namespace Berserk

#endif //BERSERK_ANGLES_H
