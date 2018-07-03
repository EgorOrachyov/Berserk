//
// Created by Egor Orachyov on 29.06.2018.
//

#ifndef BERSERKENGINE_VECTOR2_H
#define BERSERKENGINE_VECTOR2_H

#include "../Essential/Types.h"
#include "../Essential/UsageDescriptors.h"

namespace Berserk
{

    /**
     * 2 Component vector
     */
    struct CORE_EXPORT Vector2
    {
    public:

        /**
         * Initialize (0,0) vector
         */
        Vector2();

        /**
         * Initialize (x,y) vector
         *
         * @param x
         * @param y
         */
        Vector2(float32 x, float32 y);

        /**
         * Transform to vector with 1 length
         * @warning does not work for vectors with 0 length
         */
        void normalize();

        /**
         * Get true vector length
         * @return Length
         */
        float32 getLength() const;

        /**
         * Get vector length without sqrt
         * @return Quad vector length
         */
        float32 getNorm() const;

        /**
         * Assignment operator
         * @param v To assign
         * @return Result vector
         */
        Vector2 operator = (const Vector2& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector this + v
         */
        Vector2 operator + (const Vector2& v);

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector this - v
         */
        Vector2 operator - (const Vector2& v);

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector this * v
         */
        Vector2 operator * (const Vector2& v);

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector this / v
         */
        Vector2 operator / (const Vector2& v);

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector this * a
         */
        Vector2 operator * (const float32 a);

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector this / v
         */
        Vector2 operator / (const float32 a);

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const Vector2& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const Vector2& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const Vector2& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const Vector2& v);

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const Vector2& v);

    public:

        float32 x;
        float32 y;

    };

} // namespace Berserk

#endif //BERSERKENGINE_VECTOR2_H
