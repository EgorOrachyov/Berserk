//
// Created by Egor Orachyov on 01.07.2018.
//

#ifndef BERSERKENGINE_QUATERNION_H
#define BERSERKENGINE_QUATERNION_H

#include "../Essential/Types.h"
#include "../Essential/UsageDescriptors.h"
#include "Vector3f.h"

namespace Berserk
{

    struct CORE_EXPORT Quatf
    {
    public:

        /**
         * Initialize quaternion with 0 values
         */
        Quatf();

        /**
         * Initialize quaternion via scalar value s and vector part v
         *
         * @param s Scalar
         * @param v Vector
         */
        Quatf(FLOAT32 s, Vector3f v);

        /**
         * Initialize quaternion by element
         *
         * @param s Scalar part
         * @param x Vector x
         * @param y Vector y
         * @param z Vector z
         */
        Quatf(FLOAT32 s, FLOAT32 x, FLOAT32 y, FLOAT32 z);

        /**
         * Convert this quaternion to 1 length and return itself
         * @return That normalized quaternion
         */
        Quatf normalize();

        /**
         * Get inverse quaternion
         *
         * @return Inverse quaternion to that
         */
        Quatf inverse() const;

        /**
         * Get conjugate quaternion
         *
         * @return Conjugate quaternion to that
         */
        Quatf conjugate() const;

        /**
         * Get quaternion length without sqrt
         *
         * @return Norm
         */
        FLOAT32 getNorm() const;

        /**
         * Get true quaternion length
         *
         * @return Length
         */
        FLOAT32 getLength() const;

        /**
         * Get s scalar value
         *
         * @return Scalar part
         */
        FLOAT32 getScalar() const;

        /**
         * Get vector value
         *
         * @return Vector part
         */
        Vector3f getVector3() const;

        /**
         * Operator assignment
         *
         * @param q
         * @return
         */
        Quatf operator = (const Quatf& q);

        /**
         * Per elements addition
         *
         * @param q
         * @return
         */
        Quatf operator + (const Quatf& q) const;

        /**
         * Per elements subtraction
         *
         * @param q
         * @return
         */
        Quatf operator - (const Quatf& q) const;

        /**
         * True quaternion multiplication
         *
         * @param q
         * @return
         */
        Quatf operator * (const Quatf& q) const;

        /**
         * Per elements multiplication by value a
         *
         * @param a
         * @return
         */
        Quatf operator * (const FLOAT32 a) const;

        /**
         * Per elements division by value a
         *
         * @param a
         * @return
         */
        Quatf operator / (const FLOAT32 a) const;

        /**
         * Per elements comparison
         *
         * @param q
         * @return
         */
        const bool operator == (const Quatf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator >= (const Quatf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator <= (const Quatf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator > (const Quatf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator < (const Quatf& q) const;

    public:

        FLOAT32 s;
        FLOAT32 x;
        FLOAT32 y;
        FLOAT32 z;

    };

} // namespace Berserk

#endif //BERSERKENGINE_QUATERNION_H