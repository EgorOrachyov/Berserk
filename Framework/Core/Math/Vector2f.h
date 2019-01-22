//
// Created by Egor Orachyov on 29.06.2018.
//

#ifndef BERSERKENGINE_VECTOR2_H
#define BERSERKENGINE_VECTOR2_H

#include "Strings/CStaticString.h"
#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"

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

        /**
         * @return x component
         */
        FLOAT32 getX() const;

        /**
         * @return y component
         */
        FLOAT32 getY() const;

        /**
         * @return String interpretation of vector
         */
        CStaticString toString() const;

    public:

        /**
         * Get vectors' dot product
         *
         * @param v1
         * @param v2
         * @return
         */
        static FLOAT32 dot(Vector2f v1, Vector2f v2);

        /**
         * Transform vector to 1 length
         *
         * @param v
         * @return
         */
        static Vector2f normalize(Vector2f v);

        /**
         * Linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static Vector2f lerp(Vector2f v1, Vector2f v2, FLOAT32 t);

        /**
         * Spherical linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static Vector2f slerp(Vector2f v1, Vector2f v2, FLOAT32 t);

        /**
         * Spherical linear interpolation from vector v1 to vector v2 by param t and angle
         * @warning t should be in [0;1]
         * @warning angle between vectors should be more than 0
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static Vector2f slerp(Vector2f v1, Vector2f v2, FLOAT32 angle, FLOAT32 t);

        /**
         * Smooth interpolation of t between vectors v1 and v2
         * @see smoothstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vector2f smoothstep(Vector2f v1, Vector2f v2, FLOAT32 t);

        /**
         * Smoother interpolation of t between vectors v1 and v2
         * @see smootherstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vector2f smootherstep(Vector2f v1, Vector2f v2, FLOAT32 t);

    public:

        friend class Vector3f;
        friend class Vector4f;

        FLOAT32 x;
        FLOAT32 y;

    };

} // namespace Berserk

#endif //BERSERKENGINE_VECTOR2_H
