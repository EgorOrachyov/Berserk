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

    struct CORE_EXPORT Quaternionf
    {
    public:

        /**
         * Initialize quaternion with 0 values
         */
        Quaternionf();

        /**
         * Initialize quaternion via scalar value s and vector part v
         *
         * @param s Scalar
         * @param v Vector
         */
        Quaternionf(float32 s, Vector3f v);

        /**
         * Initialize quaternion by element
         *
         * @param s Scalar part
         * @param x Vector x
         * @param y Vector y
         * @param z Vector z
         */
        Quaternionf(float32 s, float32 x, float32 y, float32 z);

        /**
         * Convert this quaternion to 1 length and return itself
         * @return That normalized quaternion
         */
        Quaternionf normalize();

        /**
         * Get inverse quaternion
         *
         * @return Inverse quaternion to that
         */
        Quaternionf inverse() const;

        /**
         * Get conjugate quaternion
         *
         * @return Conjugate quaternion to that
         */
        Quaternionf conjugate() const;

        /**
         * Get quaternion length without sqrt
         *
         * @return Norm
         */
        float32 getNorm() const;

        /**
         * Get true quaternion length
         *
         * @return Length
         */
        float32 getLength() const;

        /**
         * Get s scalar value
         *
         * @return Scalar part
         */
        float32 getScalar() const;

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
        Quaternionf operator = (const Quaternionf& q);

        /**
         * Per elements addition
         *
         * @param q
         * @return
         */
        Quaternionf operator + (const Quaternionf& q) const;

        /**
         * Per elements subtraction
         *
         * @param q
         * @return
         */
        Quaternionf operator - (const Quaternionf& q) const;

        /**
         * True quaternion multiplication
         *
         * @param q
         * @return
         */
        Quaternionf operator * (const Quaternionf& q) const;

        /**
         * Per elements multiplication by value a
         *
         * @param a
         * @return
         */
        Quaternionf operator * (const float32 a) const;

        /**
         * Per elements division by value a
         *
         * @param a
         * @return
         */
        Quaternionf operator / (const float32 a) const;

        /**
         * Per elements comparison
         *
         * @param q
         * @return
         */
        const bool operator == (const Quaternionf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator >= (const Quaternionf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator <= (const Quaternionf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator > (const Quaternionf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator < (const Quaternionf& q) const;

    public:

        float32 s;
        float32 x;
        float32 y;
        float32 z;

    };

} // namespace Berserk

#endif //BERSERKENGINE_QUATERNION_H