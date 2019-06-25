//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_VECTOR3_H
#define BERSERK_VECTOR3_H

#include "Misc/Types.h"
#include "Misc/Assert.h"
#include "Misc/UsageDescriptors.h"
#include "Math/MathUtility.h"
#include "Strings/Old/StaticString.h"

namespace Berserk
{

    /**
     * 3 component vector type for math operations
     */
    class CORE_EXPORT Vec3f
    {
    public:

        /**
         * Initialize (0,0,0) vector
         */
         Vec3f();

        /**
         * Initialize by a value
         * @param a
         */
        explicit Vec3f(float32 a);

        /**
         * Initialize (x,y.z) vector
         *
         * @param x
         * @param y
         * @param z
         */
        explicit Vec3f(float32 x, float32 y, float32 z);

        /**
         * @param v Vector x and y components
         * @param z Vector z component
         */
        explicit Vec3f(const Vec2f& v, float32 z);

        /**
         * @param v Vector xyz components
         */
        explicit Vec3f(const Vec4f& v);

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
         Vec3f getNormalized() const;

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
         Vec3f operator = (const  Vec3f& v);

        /**
         * Unary minus operator for this vector
         * @return this * (-1)
         */
         Vec3f operator - () const;

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector this + v
         */
         Vec3f operator + (const  Vec3f& v) const;

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector this - v
         */
         Vec3f operator - (const  Vec3f& v) const;

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector this * v
         */
         Vec3f operator * (const  Vec3f& v) const;

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector this / v
         */
         Vec3f operator / (const  Vec3f& v) const;

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector this * a
         */
         Vec3f operator * (float32 a) const;

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector this / v
         */
         Vec3f operator / (float32 a) const;

        /**
         * Get x, y, z vector components via indexing
         * @param i Index (should be in [0;2] range)
         * @return x,y or z vector value
         */
        float32 operator [](uint32 i) const;

        /**
         * Add to `this` vector v
         * @param v Addition param
         */
        void operator += (const Vec3f& v);

        /**
         * Add to `this` vector v
         * @param v Subtraction param
         */
        void operator -= (const Vec3f& v);

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const  Vec3f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const  Vec3f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const  Vec3f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const  Vec3f& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const  Vec3f& v) const;

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
         * @return String interpretation of vector
         */
        CName toString() const;

    public:

        /**
         * Get vectors' dot product
         *
         * @param v1
         * @param v2
         * @return
         */
        static float32 dot( Vec3f v1,  Vec3f v2);

        /**
         * Get vectors' cross product
         *
         * @param v1
         * @param v2
         * @return
         */
        static  Vec3f cross( Vec3f v1,  Vec3f v2);

        /**
         * Get vectors' triple product
         *
         * @param v1
         * @param v2
         * @param v3
         * @return
         */
        static float32 triple( Vec3f v1,  Vec3f v2,  Vec3f v3);

        /**
         * Transform vector to 1 length
         *
         * @param v
         * @return
         */
        static  Vec3f normalize( Vec3f v);

        /**
         * Linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static  Vec3f lerp( Vec3f v1,  Vec3f v2, float32 t);

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
        static  Vec3f slerp( Vec3f v1,  Vec3f v2, float32 t);

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
        static  Vec3f slerp( Vec3f v1,  Vec3f v2, float32 angle, float32 t);

        /**
         * Smooth interpolation of t between vectors v1 and v2
         * @see smoothstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static  Vec3f smoothstep( Vec3f v1,  Vec3f v2, float32 t);

        /**
         * Smoother interpolation of t between vectors v1 and v2
         * @see smootherstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static  Vec3f smootherstep( Vec3f v1,  Vec3f v2, float32 t);

    public:

        float32 x;
        float32 y;
        float32 z;

    public:

        /** Right vector (1,0,0) */
        static CORE_EXPORT const Vec3f axisX;

        /** Up vector (0,1,0) */
        static CORE_EXPORT const Vec3f axisY;

        /** Forward vector (0,0,1) */
        static CORE_EXPORT const Vec3f axisZ;

    };

} // namespace Berserk

#endif //BERSERK_VECTOR3_H
