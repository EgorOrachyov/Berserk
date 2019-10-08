//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_VECTOR2_H
#define BERSERK_VECTOR2_H

#include "HAL/Types.h"
#include "Misc/Assert.h"
#include "Misc/UsageDescriptors.h"
#include "Math/MathUtility.h"
#include <Strings/String.h>

namespace Berserk
{

    /**
     * 2 component vector type for math operations
     */
    class CORE_EXPORT Vec2f
    {
    public:

        /**
         * Initialize (0,0) vector
         */
        Vec2f();

        /**
         * Initialize by a value
         * @param a
         */
        explicit Vec2f(float32 a);

        /**
         * Initialize (x,y) vector
         *
         * @param x
         * @param y
         */
        explicit Vec2f(float32 x, float32 y);

        /**
         * @param v Firts x and y components
         */
        explicit Vec2f(const Vec3f& v);

        /**
         * @param v First x and y components
         */
        explicit Vec2f(const Vec4f& v);
        
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
        Vec2f getNormalized() const;

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
        Vec2f operator = (const Vec2f& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector2 this + v
         */
        Vec2f operator + (const Vec2f& v) const;

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector2 this - v
         */
        Vec2f operator - (const Vec2f& v) const;

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector2 this * v
         */
        Vec2f operator * (const Vec2f& v) const;

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector2 this / v
         */
        Vec2f operator / (const Vec2f& v) const;

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector2 this * a
         */
        Vec2f operator * (float32 a) const;

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector2 this / v
         */
        Vec2f operator / (float32 a) const;

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const Vec2f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const Vec2f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const Vec2f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const Vec2f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const Vec2f& v) const;

        /**
         * @return x component
         */
        float32 getX() const;

        /**
         * @return y component
         */
        float32 getY() const;

        /**
         * @return String interpretation of vector
         */
        Name toString() const;

        /**
         * Convert to string
         * @param value To convert
         * @return String representation of vector
         */
        friend StringDynamic toString(const Vec2f& v);

    public:

        /**
         * Get vectors' dot product
         *
         * @param v1
         * @param v2
         * @return
         */
        static float32 dot(Vec2f v1, Vec2f v2);

        /**
         * Transform vector to 1 length
         *
         * @param v
         * @return
         */
        static Vec2f normalize(Vec2f v);

        /**
         * Linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static Vec2f lerp(Vec2f v1, Vec2f v2, float32 t);

        /**
         * Spherical linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static Vec2f slerp(Vec2f v1, Vec2f v2, float32 t);

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
        static Vec2f slerp(Vec2f v1, Vec2f v2, float32 angle, float32 t);

        /**
         * Smooth interpolation of t between vectors v1 and v2
         * @see smoothstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vec2f smoothstep(Vec2f v1, Vec2f v2, float32 t);

        /**
         * Smoother interpolation of t between vectors v1 and v2
         * @see smootherstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vec2f smootherstep(Vec2f v1, Vec2f v2, float32 t);

    public:

        float32 x;
        float32 y;

    };
    
}  // namespace Berserk

#endif //BERSERK_VECTOR2_H
