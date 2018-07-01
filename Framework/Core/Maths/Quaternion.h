//
// Created by Egor Orachyov on 01.07.2018.
//

#ifndef BERSERKENGINE_QUATERNION_H
#define BERSERKENGINE_QUATERNION_H

#include "../Essential/Types.h"
#include "../Essential/UsageDescriptors.h"
#include "Vector3.h"

namespace Berserk
{

    struct CORE_EXPORT Quaternion
    {
    public:

        /**
         * Initialize quaternion with 0 values
         */
        Quaternion();

        /**
         * Initialize quaternion via scalar value s and vector part v
         *
         * @param s Scalar
         * @param v Vector
         */
        Quaternion(float32 s, Vector3 v);

        /**
         * Initialize quaternion by element
         *
         * @param s Scalar part
         * @param x Vector x
         * @param y Vector y
         * @param z Vector z
         */
        Quaternion(float32 s, float32 x, float32 y, float32 z);

        /**
         * Transform quaternion to 1 length
         */
        void Normalize();

        /**
         * Get inverse quaternion
         *
         * @return Inverse quaternion to that
         */
        Quaternion Inverse() const;

        /**
         * Get conjugate quaternion
         *
         * @return Conjugate quaternion to that
         */
        Quaternion Conjugate() const;

        /**
         * Get quaternion length without sqrt
         *
         * @return Norm
         */
        float32 GetNorm() const;

        /**
         * Get true quaternion length
         *
         * @return Length
         */
        float32 GetLength() const;

        /**
         * Get s scalar value
         *
         * @return Scalar part
         */
        float32 GetScalar() const;

        /**
         * Get vector value
         *
         * @return Vector part
         */
        Vector3 GetVector3() const;

        /**
         * Operator assignment
         *
         * @param q
         * @return
         */
        Quaternion operator = (const Quaternion& q);

        /**
         * Per elements addition
         *
         * @param q
         * @return
         */
        Quaternion operator + (const Quaternion& q) const;

        /**
         * Per elements subtraction
         *
         * @param q
         * @return
         */
        Quaternion operator - (const Quaternion& q) const;

        /**
         * True quaternion multiplication
         *
         * @param q
         * @return
         */
        Quaternion operator * (const Quaternion& q) const;

        /**
         * Per elements comparison
         *
         * @param q
         * @return
         */
        const bool operator == (const Quaternion& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator >= (const Quaternion& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator <= (const Quaternion& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator > (const Quaternion& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator < (const Quaternion& q) const;

    private:

        float32 s;
        float32 x;
        float32 y;
        float32 z;

    };

} // namespace Berserk

#endif //BERSERKENGINE_QUATERNION_H