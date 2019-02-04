//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_VECTOR3_H
#define BERSERK_VECTOR3_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Assert.h"
#include "Public/Misc/UsageDescriptors.h"
#include "Public/Math/MathUtility.h"
#include "Public/Strings/StaticString.h"

namespace Berserk
{

    /**
     * 3 component generic vector type for math operations
     * @tparam T type (considered as float32 or float64)
     */
    template <typename T>
    class CORE_EXPORT Vector3
    {
    public:

        /**
         * Initialize (0,0,0) vector
         */
        Vector3();

        /**
         * Initialize by a value
         * @param a
         */
        explicit Vector3(T a);

        /**
         * Initialize (x,y.z) vector
         *
         * @param x
         * @param y
         * @param z
         */
        explicit Vector3(T x, T y, T z);

        /**
         * @param v Vector x and y components
         * @param z Vector z component
         */
        explicit Vector3(const Vector2<T>& v, T z);

        /**
         * @param v Vector xyz components
         */
        explicit Vector3(const Vector4<T>& v);

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
        Vector3 getNormalized() const;

        /**
         * Get true vector length
         * @return Length
         */
        T length() const;

        /**
         * Get vector length without sqrt
         * @return Quad vector length
         */
        T norm() const;

        /**
         * Assignment operator
         * @param v To assign
         * @return Result vector
         */
        Vector3 operator = (const Vector3& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector this + v
         */
        Vector3 operator + (const Vector3& v) const;

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector this - v
         */
        Vector3 operator - (const Vector3& v) const;

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector this * v
         */
        Vector3 operator * (const Vector3& v) const;

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector this / v
         */
        Vector3 operator / (const Vector3& v) const;

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector this * a
         */
        Vector3 operator * (T a) const;

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector this / v
         */
        Vector3 operator / (T a) const;

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const Vector3& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const Vector3& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const Vector3& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const Vector3& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const Vector3& v) const;

        /**
         * @return x component
         */
        T getX() const;

        /**
         * @return y component
         */
        T getY() const;

        /**
         * @return z component
         */
        T getZ() const;

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
        static T dot(Vector3 v1, Vector3 v2);

        /**
         * Get vectors' cross product
         *
         * @param v1
         * @param v2
         * @return
         */
        static Vector3 cross(Vector3 v1, Vector3 v2);

        /**
         * Get vectors' triple product
         *
         * @param v1
         * @param v2
         * @param v3
         * @return
         */
        static T triple(Vector3 v1, Vector3 v2, Vector3 v3);

        /**
         * Transform vector to 1 length
         *
         * @param v
         * @return
         */
        static Vector3 normalize(Vector3 v);

        /**
         * Linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static Vector3 lerp(Vector3 v1, Vector3 v2, T t);

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
        static Vector3 slerp(Vector3 v1, Vector3 v2, T t);

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
        static Vector3 slerp(Vector3 v1, Vector3 v2, T angle, T t);

        /**
         * Smooth interpolation of t between vectors v1 and v2
         * @see smoothstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vector3 smoothstep(Vector3 v1, Vector3 v2, T t);

        /**
         * Smoother interpolation of t between vectors v1 and v2
         * @see smootherstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static Vector3 smootherstep(Vector3 v1, Vector3 v2, T t);

    public:

        T x;
        T y;
        T z;

    public:

        /** Right vector (1,0,0) */
        static CORE_EXPORT const Vector3 axisX;

        /** Up vector (0,1,0) */
        static CORE_EXPORT const Vector3 axisY;

        /** Forward vector (0,0,1) */
        static CORE_EXPORT const Vector3 axisZ;

    };

    template <typename T>
    Vector3<T>::Vector3()
            : x(0), y(0), z(0)
    {

    }

    template <typename T>
    Vector3<T>::Vector3(T a)
            : x(a), y(a), z(a)
    {

    }

    template <typename T>
    Vector3<T>::Vector3(T x, T y, T z)
            : x(x), y(y), z(z)
    {

    }

    template <typename T>
    Vector3<T>::Vector3(const Vector2<T> &v, T z)
            : x(v.x), y(v.y), z(z)
    {

    }

    template <typename T>
    Vector3<T>::Vector3(const Vector4<T> &v)
            : x(v.x), y(v.y), z(v.z)
    {

    }

    template <typename T>
    void Vector3<T>::normalize()
    {
        T length = length();
        ASSERT(length, "Length should be more than 0 to normalize");

        x /= length;
        y /= length;
        z /= length;
    }

    template <typename T>
    Vector3<T> Vector3<T>::getNormalized() const
    {
        return *this / length();
    }

    template <typename T>
    T Vector3<T>::length() const
    {
        return Math::sqrt(x * x + y * y + z * z);
    }

    template <typename T>
    T Vector3<T>::norm() const
    {
        return (x * x + y * y + z * z);
    }

    template <typename T>
    Vector3<T> Vector3<T>::operator = (const Vector3<T>& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }

    template <typename T>
    Vector3<T> Vector3<T>::operator + (const Vector3<T>& v) const
    {
        return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    template <typename T>
    Vector3<T> Vector3<T>::operator - (const Vector3<T>& v) const
    {
        return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    template <typename T>
    Vector3<T> Vector3<T>::operator * (const Vector3<T>& v) const
    {
        return Vector3(this->x * v.x, this->y * v.y, this->z * v.z);
    }

    template <typename T>
    Vector3<T> Vector3<T>::operator / (const Vector3<T>& v) const
    {
        return Vector3(this->x / v.x, this->y / v.y, this->z / v.z);
    }

    template <typename T>
    Vector3<T> Vector3<T>::operator * (const T a) const
    {
        return Vector3(this->x * a, this->y * a, this->z * a);
    }

    template <typename T>
    Vector3<T> Vector3<T>::operator / (const T a) const
    {
        return Vector3(this->x / a, this->y / a, this->z / a);
    }

    template <typename T>
    const bool Vector3<T>::operator == (const Vector3<T>& v) const
    {
        return (x == v.x && y == v.y && z == v.z);
    }

    template <typename T>
    const bool Vector3<T>::operator >= (const Vector3<T>& v) const
    {
        return (norm() >= v.norm());
    }

    template <typename T>
    const bool Vector3<T>::operator <= (const Vector3<T>& v) const
    {
        return (norm() <= v.norm());
    }

    template <typename T>
    const bool Vector3<T>::operator > (const Vector3<T>& v) const
    {
        return (norm() > v.norm());
    }

    template <typename T>
    const bool Vector3<T>::operator < (const Vector3<T>& v) const
    {
        return (norm() > v.norm());
    }

    template <typename T>
    T Vector3<T>::getX() const
    {
        return x;
    }

    template <typename T>
    T Vector3<T>::getY() const
    {
        return y;
    }

    template <typename T>
    T Vector3<T>::getZ() const
    {
        return z;
    }

    template <typename T>
    CName Vector3<T>::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "(X=%3.3f Y=%3.3f Z=%3.3f)", x, y, z);
        return CName(buffer);
    }

    template <typename T>
    T Vector3<T>::dot(Vector3<T> v1, Vector3<T> v2)
    {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
    }

    template <typename T>
    Vector3<T> Vector3<T>::cross(Vector3<T> v1, Vector3<T> v2)
    {
        return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }

    template <typename T>
    T Vector3<T>::triple(Vector3<T> v1, Vector3<T> v2, Vector3<T> v3)
    {
        return dot(cross(v1, v2), v3);
    }

    template <typename T>
    Vector3<T> Vector3<T>::normalize(Vector3<T> v)
    {
        Vector3<T> r = v;
        r.normalize();
        return r;
    }

    template <typename T>
    Vector3<T> Vector3<T>::lerp(Vector3<T> v1, Vector3<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    template <typename T>
    Vector3<T> Vector3<T>::slerp(Vector3<T> v1, Vector3<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T angle = dot(v1.getNormalized(), v2.getNormalized());
        T sin_angle = Math::sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (Math::sin(angle * (1 - t)) / sin_angle) + v2 * (Math::sin(angle * t) / sin_angle));
    }

    template <typename T>
    Vector3<T> Vector3<T>::slerp(Vector3<T> v1, Vector3<T> v2, T angle, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T sin_angle = Math::sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (Math::sin(angle * (1 - t)) / sin_angle) + v2 * (Math::sin(angle * t) / sin_angle));
    }

    template <typename T>
    Vector3<T> Vector3<T>::smoothstep(Vector3<T> v1, Vector3<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = (T)(2 * t * t * (1.5 - t));
        return lerp(v1, v2, t);
    }

    template <typename T>
    Vector3<T> Vector3<T>::smootherstep(Vector3<T> v1, Vector3<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

    template <>
    const Vector3<float32> Vector3<float32>::axisX = Vector3<float32>(1.0f,0.0f,0.0f);

    template <>
    const Vector3<float32> Vector3<float32>::axisY =  Vector3<float32>(0.0f,1.0f,0.0f);

    template <>
    const Vector3<float32> Vector3<float32>::axisZ = Vector3<float32>(0.0f,0.0f,1.0f);

    template <>
    const Vector3<float64> Vector3<float64>::axisX = Vector3<float64>(1.0f,0.0f,0.0f);

    template <>
    const Vector3<float64> Vector3<float64>::axisY =  Vector3<float64>(0.0f,1.0f,0.0f);

    template <>
    const Vector3<float64> Vector3<float64>::axisZ = Vector3<float64>(0.0f,0.0f,1.0f);

} // namespace Berserk

#endif //BERSERK_VECTOR3_H
