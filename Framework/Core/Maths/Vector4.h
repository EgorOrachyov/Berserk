//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTOR4_H
#define BERSERKENGINE_VECTOR4_H

#include "../Essential/Types.h"

namespace Berserk
{

    /**
     * 4 Component vector
     */
    struct Vector4
    {
    public:

        /**
         * Initialize (0,0,0,0) vector
         */
        Vector4();

        /**
         * Initialize (x,y,z,w) vector
         *
         * @param x
         * @param y
         * @param z
         * @param w
         */
        Vector4(float32 x, float32 y, float32 z, float32 w);

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
        Vector4 operator = (const Vector4& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector this + v
         */
        Vector4 operator + (const Vector4& v);

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector this - v
         */
        Vector4 operator - (const Vector4& v);

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector this * v
         */
        Vector4 operator * (const Vector4& v);

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector this / v
         */
        Vector4 operator / (const Vector4& v);

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector this * a
         */
        Vector4 operator * (const float32 a);

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector this / v
         */
        Vector4 operator / (const float32 a);

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const Vector4& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const Vector4& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const Vector4& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const Vector4& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const Vector4& v);

    public:

        float32 x;
        float32 y;
        float32 z;
        float32 w;

    };

} // namespace Berserk

#endif //BERSERKENGINE_VECTOR4_H