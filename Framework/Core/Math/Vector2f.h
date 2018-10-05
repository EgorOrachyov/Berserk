//
// Created by Egor Orachyov on 29.06.2018.
//

#ifndef BERSERKENGINE_VECTOR2_H
#define BERSERKENGINE_VECTOR2_H

#include "Essential/Types.h"
#include "Essential/UsageDescriptors.h"

namespace Berserk
{

    struct Vector2f;
    struct Vector3f;
    struct Vector4f;

    /**
     * 2 Component vector
     */
    struct CORE_EXPORT Vector2f
    {
    public:

        /**
         * Initialize (0,0) vector
         */
        Vector2f();

        /**
         * Initialize by a value
         * @param a
         */
        explicit Vector2f(FLOAT32 a);

        /**
         * Initialize (x,y) vector
         *
         * @param x
         * @param y
         */
        explicit Vector2f(FLOAT32 x, FLOAT32 y);

        /**
         * @param v Firts x and y components
         */
        explicit Vector2f(const Vector3f& v);

        /**
         * @param v First x and y components
         */
        explicit Vector2f(const Vector4f& v);

    public:

        /**
         * Transform to vector with 1 length
         * @warning does not work for vectors with 0 length
         */
        void normalize();

        /**
         * Returns normalized vector
         * @warning does not work for vectors with 0 length
         */
        Vector2f getNormalized() const;

        /**
         * Get true vector length
         * @return Length
         */
        FLOAT32 getLength() const;

        /**
         * Get vector length without sqrt
         * @return Quad vector length
         */
        FLOAT32 getNorm() const;

        /**
         * Assignment operator
         * @param v To assign
         * @return Result vector
         */
        Vector2f operator = (const Vector2f& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector this + v
         */
        Vector2f operator + (const Vector2f& v) const;

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector this - v
         */
        Vector2f operator - (const Vector2f& v) const;

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector this * v
         */
        Vector2f operator * (const Vector2f& v) const;

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector this / v
         */
        Vector2f operator / (const Vector2f& v) const;

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector this * a
         */
        Vector2f operator * (FLOAT32 a) const;

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector this / v
         */
        Vector2f operator / (FLOAT32 a) const;

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const Vector2f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const Vector2f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const Vector2f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const Vector2f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const Vector2f& v) const;

        FLOAT32 getX() const;

        FLOAT32 getY() const;

    public:

        friend class Vector3f;
        friend class Vector4f;

        FLOAT32 x;
        FLOAT32 y;

    };

} // namespace Berserk

#endif //BERSERKENGINE_VECTOR2_H
