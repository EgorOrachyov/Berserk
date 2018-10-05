//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTOR3_H
#define BERSERKENGINE_VECTOR3_H

#include "Essential/Types.h"
#include "Essential/UsageDescriptors.h"
#include "Vector4f.h"

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

        FLOAT32 getX() const;

        FLOAT32 getY() const;

        FLOAT32 getZ() const;

    public:

        friend class Vector2f;
        friend class Vector4f;

        FLOAT32 x;
        FLOAT32 y;
        FLOAT32 z;

        /**
         * Standard basis for 3-dimensional space
         */
        static const Vector3f axisX;
        static const Vector3f axisY;
        static const Vector3f axisZ;

    };

} // namespace Berserk

#endif //BERSERKENGINE_VECTOR3_H