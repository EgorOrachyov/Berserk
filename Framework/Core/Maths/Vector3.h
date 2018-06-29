//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTOR3_H
#define BERSERKENGINE_VECTOR3_H

#include "../Essential/Types.h"

namespace Berserk
{

    /**
     * 3 Component vector
     */
    struct Vector3
    {
    public:

        /**
         * Initialize (0,0,0) vector
         */
        Vector3();

        /**
         * Initialize (x,y.z) vector
         *
         * @param x
         * @param y
         * @param z
         */
        Vector3(float32 x, float32 y, float32 z);

        /**
         * Transform to vector with 1 length
         * @warning does not work for vectors with 0 length
         */
        void Normalize();

        /**
         * Get true vector length
         * @return Length
         */
        float32 GetLength() const;

        /**
         * Get vector length without sqrt
         * @return Quad vector length
         */
        float32 GetQuadLength() const;

        /**
         * Assignment operator
         * @param v To assign
         * @return Result vector
         */
        Vector3 operator = (const Vector3& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector this + v
         */
        Vector3 operator + (const Vector3& v);

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector this - v
         */
        Vector3 operator - (const Vector3& v);

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector this * v
         */
        Vector3 operator * (const Vector3& v);

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector this / v
         */
        Vector3 operator / (const Vector3& v);

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector this * a
         */
        Vector3 operator * (const float32 a);

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector this / v
         */
        Vector3 operator / (const float32 a);

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const Vector3& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const Vector3& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const Vector3& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const Vector3& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const Vector3& v);

    public:

        float32 x;
        float32 y;
        float32 z;

    };

} // namespace Berserk

#endif //BERSERKENGINE_VECTOR3_H