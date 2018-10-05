//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTOR4_H
#define BERSERKENGINE_VECTOR4_H

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

        FLOAT32 getX() const;

        FLOAT32 getY() const;

        FLOAT32 getZ() const;

        FLOAT32 getW() const;

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