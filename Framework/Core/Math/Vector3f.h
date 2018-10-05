//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTOR3_H
#define BERSERKENGINE_VECTOR3_H

#include "Strings/CStaticString.h"
#include "Essential/Types.h"
#include "Essential/UsageDescriptors.h"
#include "Math/Vector4f.h"

namespace Berserk
{

    struct Vector2f;
    struct Vector3f;
    struct Vector4f;

    /**
     * 3 Component vector
     */
    struct CORE_EXPORT Vector3f
    {
    public:

        /**
         * Initialize (0,0,0) vector
         */
        Vector3f();

        /**
         * Initialize by a value
         * @param a
         */
        explicit Vector3f(FLOAT32 a);

        /**
         * Initialize (x,y.z) vector
         *
         * @param x
         * @param y
         * @param z
         */
        explicit Vector3f(FLOAT32 x, FLOAT32 y, FLOAT32 z);

        /**
         * @param v Vector x and y components
         * @param z Vector z component
         */
        explicit Vector3f(const Vector2f& v, FLOAT32 z);

        /**
         * @param v Vector xyz components
         */
        explicit Vector3f(const Vector4f& v);

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
        Vector3f getNormalized() const;

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
        Vector3f operator = (const Vector3f& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector this + v
         */
        Vector3f operator + (const Vector3f& v) const;

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector this - v
         */
        Vector3f operator - (const Vector3f& v) const;

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector this * v
         */
        Vector3f operator * (const Vector3f& v) const;

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector this / v
         */
        Vector3f operator / (const Vector3f& v) const;

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector this * a
         */
        Vector3f operator * (FLOAT32 a) const;

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector this / v
         */
        Vector3f operator / (FLOAT32 a) const;

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const Vector3f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const Vector3f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const Vector3f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const Vector3f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const Vector3f& v) const;

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
        static FLOAT32 dot(Vector3f v1, Vector3f v2);

        /**
         * Get vectors' cross product
         *
         * @param v1
         * @param v2
         * @return
         */
        static Vector3f cross(Vector3f v1, Vector3f v2);

        /**
         * Get vectors' triple product
         *
         * @param v1
         * @param v2
         * @param v3
         * @return
         */
        static FLOAT32 triple(Vector3f v1, Vector3f v2, Vector3f v3);

        /**
         * Transform vector to 1 length
         *
         * @param v
         * @return
         */
        static Vector3f normalize(Vector3f v);

        /**
         * Linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static Vector3f lerp(Vector3f v1, Vector3f v2, FLOAT32 t);

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
        static Vector3f slerp(Vector3f v1, Vector3f v2, FLOAT32 t);

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
        static Vector3f slerp(Vector3f v1, Vector3f v2, FLOAT32 angle, FLOAT32 t);

        /**
         * Smooth interpolation of t between vectors v1 and v2
         * @see smoothstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vector3f smoothstep(Vector3f v1, Vector3f v2, FLOAT32 t);

        /**
         * Smoother interpolation of t between vectors v1 and v2
         * @see smootherstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vector3f smootherstep(Vector3f v1, Vector3f v2, FLOAT32 t);

    public:

        friend class Vector2f;
        friend class Vector4f;

        FLOAT32 x;
        FLOAT32 y;
        FLOAT32 z;

        /** Right vector (1,0,0) */
        static CORE_EXPORT const Vector3f axisX;

        /** Up vector (0,1,0) */
        static CORE_EXPORT const Vector3f axisY;

        /** Forward vector (0,0,1) */
        static CORE_EXPORT const Vector3f axisZ;

    };

} // namespace Berserk

#endif //BERSERKENGINE_VECTOR3_H