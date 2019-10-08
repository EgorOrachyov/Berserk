//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_VECTOR4_H
#define BERSERK_VECTOR4_H

#include "HAL/Types.h"
#include "Misc/Assert.h"
#include "Misc/UsageDescriptors.h"
#include "Math/MathUtility.h"
#include <Strings/String.h>

namespace Berserk
{

    /**
     * 4 component vector type for math operations
     */
    class CORE_EXPORT Vec4f
    {
    public:

        /**
         * Initialize (0,0,0,0) vector
         */
        Vec4f();

        /**
         * Initialize by a value
         * @param a
         */
        explicit Vec4f(float32 a);

        /**
         * Initialize (x,y,z,w) vector
         *
         * @param x
         * @param y
         * @param z
         * @param w
         */
        explicit Vec4f(float32 x, float32 y, float32 z, float32 w);

        /**
         * @param v First x and y components
         * @param z Z component
         * @param w W component
         */
        explicit Vec4f(const Vec2f& v, float32 z, float32 w);

        /**
         * @param xy Vector with x and y components
         * @param zw Vector with z and w components
         */
        explicit Vec4f(const Vec2f& xy, const Vec2f& zw);

        /**
         * @param v Vector with xyz components
         * @param w W component
         */
        explicit Vec4f(const Vec3f& v, float32 w);

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
        Vec4f getNormalized() const;

        /**
         * Get true vector length
         * @return Length
         */
        float32 length() const;

        /**
         * Get vector length without sqrt
         * @return Quad vector length
         */
        float32 norm() const;

        /**
         * Assignment operator
         * @param v To assign
         * @return Result vector
         */
        Vec4f operator = (const Vec4f& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector this + v
         */
        Vec4f operator + (const Vec4f& v) const;

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector this - v
         */
        Vec4f operator - (const Vec4f& v) const;

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector this * v
         */
        Vec4f operator * (const Vec4f& v) const;

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector this / v
         */
        Vec4f operator / (const Vec4f& v) const;

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector this * a
         */
        Vec4f operator * (float32 a) const;

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector this / v
         */
        Vec4f operator / (float32 a) const;

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const Vec4f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const Vec4f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const Vec4f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const Vec4f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const Vec4f& v) const;

        /**
         * @return x component
         */
        float32 getX() const;

        /**
         * @return y component
         */
        float32 getY() const;

        /**
         * @return z component
         */
        float32 getZ() const;

        /**
         * @return w component
         */
        float32 getW() const;

        /**
         * @return String interpretation of vector
         */
        Name toString() const;

        /**
         * Convert to string
         * @param value To convert
         * @return String representation of vector
         */
        friend StringDynamic toString(const Vec4f& v);

    public:

        /**
         * Get vectors' dot product
         *
         * @param v1
         * @param v2
         * @return
         */
        static float32 dot(Vec4f v1, Vec4f v2);

        /**
         * Transform vector to 1 length
         *
         * @param v
         * @return
         */
        static Vec4f normalize(Vec4f v);

        /**
         * Linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static Vec4f lerp(Vec4f v1, Vec4f v2, float32 t);

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
        static Vec4f slerp(Vec4f v1, Vec4f v2, float32 t);

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
        static Vec4f slerp(Vec4f v1, Vec4f v2, float32 angle, float32 t);

        /**
         * Smooth interpolation of t between vectors v1 and v2
         * @see smoothstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vec4f smoothstep(Vec4f v1, Vec4f v2, float32 t);

        /**
         * Smoother interpolation of t between vectors v1 and v2
         * @see smootherstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vec4f smootherstep(Vec4f v1, Vec4f v2, float32 t);

    public:

        float32 x;
        float32 y;
        float32 z;
        float32 w;

    };

} // namespace Berserk

#endif //BERSERK_VECTOR4_H
