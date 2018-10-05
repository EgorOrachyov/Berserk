//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTOR4_H
#define BERSERKENGINE_VECTOR4_H

#include "Strings/CStaticString.h"
#include "Essential/Types.h"
#include "Essential/UsageDescriptors.h"

namespace Berserk
{
    struct Vector2f;
    struct Vector3f;
    struct Vector4f;

    /**
     * 4 Component vector
     */
    struct CORE_EXPORT Vector4f
    {
    public:

        /**
         * Initialize (0,0,0,0) vector
         */
        Vector4f();

        /**
         * Initialize by a value
         * @param a
         */
        explicit Vector4f(FLOAT32 a);

        /**
         * Initialize (x,y,z,w) vector
         *
         * @param x
         * @param y
         * @param z
         * @param w
         */
        explicit Vector4f(FLOAT32 x, FLOAT32 y, FLOAT32 z, FLOAT32 w);

        /**
         * @param v First x and y components
         * @param z Z component
         * @param w W component
         */
        explicit Vector4f(const Vector2f& v, FLOAT32 z, FLOAT32 w);

        /**
         * @param xy Vector with x and y components
         * @param zw Vector with z and w components
         */
        explicit Vector4f(const Vector2f& xy, const Vector2f& zw);

        /**
         * @param v Vector with xyz components
         * @param w W component
         */
        explicit Vector4f(const Vector3f& v, FLOAT32 w);

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
        Vector4f getNormalized() const;

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
        Vector4f operator = (const Vector4f& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector this + v
         */
        Vector4f operator + (const Vector4f& v) const;

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector this - v
         */
        Vector4f operator - (const Vector4f& v) const;

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector this * v
         */
        Vector4f operator * (const Vector4f& v) const;

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector this / v
         */
        Vector4f operator / (const Vector4f& v) const;

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector this * a
         */
        Vector4f operator * (FLOAT32 a) const;

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector this / v
         */
        Vector4f operator / (FLOAT32 a) const;

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const Vector4f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const Vector4f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const Vector4f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const Vector4f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const Vector4f& v) const;

        /**
         * @return x component
         */
        FLOAT32 getX() const;

        /**
         * @return y component
         */
        FLOAT32 getY() const;

        /**
         * @return z component
         */
        FLOAT32 getZ() const;

        /**
         * @return w component
         */
        FLOAT32 getW() const;

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
        static FLOAT32 dot(Vector4f v1, Vector4f v2);

        /**
         * Transform vector to 1 length
         *
         * @param v
         * @return
         */
        static Vector4f normalize(Vector4f v);

        /**
         * Linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static Vector4f lerp(Vector4f v1, Vector4f v2, FLOAT32 t);

        /**
         * Spherical linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         * @warning angle between vectors should be more than 0
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static Vector4f slerp(Vector4f v1, Vector4f v2, FLOAT32 t);

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
        static Vector4f slerp(Vector4f v1, Vector4f v2, FLOAT32 angle, FLOAT32 t);

        /**
         * Smooth interpolation of t between vectors v1 and v2
         * @see smoothstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vector4f smoothstep(Vector4f v1, Vector4f v2, FLOAT32 t);

        /**
         * Smoother interpolation of t between vectors v1 and v2
         * @see smootherstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vector4f smootherstep(Vector4f v1, Vector4f v2, FLOAT32 t);

    public:

        friend class Vector2f;
        friend class Vector3f;

        FLOAT32 x;
        FLOAT32 y;
        FLOAT32 z;
        FLOAT32 w;

    };

} // namespace Berserk

#endif //BERSERKENGINE_VECTOR4_H