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
    class CORE_EXPORT TVector3
    {
    public:

        /**
         * Initialize (0,0,0) vector
         */
        TVector3();

        /**
         * Initialize by a value
         * @param a
         */
        explicit TVector3(T a);

        /**
         * Initialize (x,y.z) vector
         *
         * @param x
         * @param y
         * @param z
         */
        explicit TVector3(T x, T y, T z);

        /**
         * @param v Vector x and y components
         * @param z Vector z component
         */
        explicit TVector3(const TVector2<T>& v, T z);

        /**
         * @param v Vector xyz components
         */
        explicit TVector3(const TVector4<T>& v);

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
        TVector3 getNormalized() const;

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
        TVector3 operator = (const TVector3& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector this + v
         */
        TVector3 operator + (const TVector3& v) const;

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector this - v
         */
        TVector3 operator - (const TVector3& v) const;

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector this * v
         */
        TVector3 operator * (const TVector3& v) const;

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector this / v
         */
        TVector3 operator / (const TVector3& v) const;

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector this * a
         */
        TVector3 operator * (T a) const;

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector this / v
         */
        TVector3 operator / (T a) const;

        /**
         * Get x, y, z vector components via indexing
         * @param i Index (should be in [0;2] range)
         * @return x,y or z vector value
         */
        T operator [](uint32 i) const;

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const TVector3& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const TVector3& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const TVector3& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const TVector3& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const TVector3& v) const;

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
        static T dot(TVector3 v1, TVector3 v2);

        /**
         * Get vectors' cross product
         *
         * @param v1
         * @param v2
         * @return
         */
        static TVector3 cross(TVector3 v1, TVector3 v2);

        /**
         * Get vectors' triple product
         *
         * @param v1
         * @param v2
         * @param v3
         * @return
         */
        static T triple(TVector3 v1, TVector3 v2, TVector3 v3);

        /**
         * Transform vector to 1 length
         *
         * @param v
         * @return
         */
        static TVector3 normalize(TVector3 v);

        /**
         * Linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static TVector3 lerp(TVector3 v1, TVector3 v2, T t);

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
        static TVector3 slerp(TVector3 v1, TVector3 v2, T t);

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
        static TVector3 slerp(TVector3 v1, TVector3 v2, T angle, T t);

        /**
         * Smooth interpolation of t between vectors v1 and v2
         * @see smoothstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static TVector3 smoothstep(TVector3 v1, TVector3 v2, T t);

        /**
         * Smoother interpolation of t between vectors v1 and v2
         * @see smootherstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static TVector3 smootherstep(TVector3 v1, TVector3 v2, T t);

    public:

        T x;
        T y;
        T z;

    public:

        /** Right vector (1,0,0) */
        static CORE_EXPORT const TVector3 axisX;

        /** Up vector (0,1,0) */
        static CORE_EXPORT const TVector3 axisY;

        /** Forward vector (0,0,1) */
        static CORE_EXPORT const TVector3 axisZ;

    };

    template <typename T>
    TVector3<T>::TVector3()
            : x(0.0), y(0.0), z(0.0)
    {

    }

    template <typename T>
    TVector3<T>::TVector3(T a)
            : x(a), y(a), z(a)
    {

    }

    template <typename T>
    TVector3<T>::TVector3(T x, T y, T z)
            : x(x), y(y), z(z)
    {

    }

    template <typename T>
    TVector3<T>::TVector3(const TVector2<T> &v, T z)
            : x(v.x), y(v.y), z(z)
    {

    }

    template <typename T>
    TVector3<T>::TVector3(const TVector4<T> &v)
            : x(v.x), y(v.y), z(v.z)
    {

    }

    template <typename T>
    void TVector3<T>::normalize()
    {
        T l = length();
        ASSERT(l, "Length should be more than 0 to normalize");

        x /= l;
        y /= l;
        z /= l;
    }

    template <typename T>
    TVector3<T> TVector3<T>::getNormalized() const
    {
        return *this / length();
    }

    template <typename T>
    T TVector3<T>::length() const
    {
        return Math::sqrt(x * x + y * y + z * z);
    }

    template <typename T>
    T TVector3<T>::norm() const
    {
        return (x * x + y * y + z * z);
    }

    template <typename T>
    TVector3<T> TVector3<T>::operator = (const TVector3<T>& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }

    template <typename T>
    TVector3<T> TVector3<T>::operator + (const TVector3<T>& v) const
    {
        return TVector3(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    template <typename T>
    TVector3<T> TVector3<T>::operator - (const TVector3<T>& v) const
    {
        return TVector3(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    template <typename T>
    TVector3<T> TVector3<T>::operator * (const TVector3<T>& v) const
    {
        return TVector3(this->x * v.x, this->y * v.y, this->z * v.z);
    }

    template <typename T>
    TVector3<T> TVector3<T>::operator / (const TVector3<T>& v) const
    {
        return TVector3(this->x / v.x, this->y / v.y, this->z / v.z);
    }

    template <typename T>
    TVector3<T> TVector3<T>::operator * (const T a) const
    {
        return TVector3(this->x * a, this->y * a, this->z * a);
    }

    template <typename T>
    TVector3<T> TVector3<T>::operator / (const T a) const
    {
        return TVector3(this->x / a, this->y / a, this->z / a);
    }

    template <typename T>
    T TVector3<T>::operator[](uint32 i) const
    {
        return ((T*)(this))[i];
    }

    template <typename T>
    const bool TVector3<T>::operator == (const TVector3<T>& v) const
    {
        return (x == v.x && y == v.y && z == v.z);
    }

    template <typename T>
    const bool TVector3<T>::operator >= (const TVector3<T>& v) const
    {
        return (norm() >= v.norm());
    }

    template <typename T>
    const bool TVector3<T>::operator <= (const TVector3<T>& v) const
    {
        return (norm() <= v.norm());
    }

    template <typename T>
    const bool TVector3<T>::operator > (const TVector3<T>& v) const
    {
        return (norm() > v.norm());
    }

    template <typename T>
    const bool TVector3<T>::operator < (const TVector3<T>& v) const
    {
        return (norm() > v.norm());
    }

    template <typename T>
    T TVector3<T>::getX() const
    {
        return x;
    }

    template <typename T>
    T TVector3<T>::getY() const
    {
        return y;
    }

    template <typename T>
    T TVector3<T>::getZ() const
    {
        return z;
    }

    template <typename T>
    CName TVector3<T>::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "(X=%3.3f Y=%3.3f Z=%3.3f)", x, y, z);
        return CName(buffer);
    }

    template <typename T>
    T TVector3<T>::dot(TVector3<T> v1, TVector3<T> v2)
    {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
    }

    template <typename T>
    TVector3<T> TVector3<T>::cross(TVector3<T> v1, TVector3<T> v2)
    {
        return TVector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }

    template <typename T>
    T TVector3<T>::triple(TVector3<T> v1, TVector3<T> v2, TVector3<T> v3)
    {
        return dot(cross(v1, v2), v3);
    }

    template <typename T>
    TVector3<T> TVector3<T>::normalize(TVector3<T> v)
    {
        TVector3<T> r = v;
        r.normalize();
        return r;
    }

    template <typename T>
    TVector3<T> TVector3<T>::lerp(TVector3<T> v1, TVector3<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    template <typename T>
    TVector3<T> TVector3<T>::slerp(TVector3<T> v1, TVector3<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T angle = dot(v1.getNormalized(), v2.getNormalized());
        T sin_angle = Math::sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (Math::sin(angle * (1 - t)) / sin_angle) + v2 * (Math::sin(angle * t) / sin_angle));
    }

    template <typename T>
    TVector3<T> TVector3<T>::slerp(TVector3<T> v1, TVector3<T> v2, T angle, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T sin_angle = Math::sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (Math::sin(angle * (1 - t)) / sin_angle) + v2 * (Math::sin(angle * t) / sin_angle));
    }

    template <typename T>
    TVector3<T> TVector3<T>::smoothstep(TVector3<T> v1, TVector3<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = (T)(2 * t * t * (1.5 - t));
        return lerp(v1, v2, t);
    }

    template <typename T>
    TVector3<T> TVector3<T>::smootherstep(TVector3<T> v1, TVector3<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

    template <>
    const TVector3<float32> TVector3<float32>::axisX = TVector3<float32>(1.0f,0.0f,0.0f);

    template <>
    const TVector3<float32> TVector3<float32>::axisY =  TVector3<float32>(0.0f,1.0f,0.0f);

    template <>
    const TVector3<float32> TVector3<float32>::axisZ = TVector3<float32>(0.0f,0.0f,1.0f);

    template <>
    const TVector3<float64> TVector3<float64>::axisX = TVector3<float64>(1.0f,0.0f,0.0f);

    template <>
    const TVector3<float64> TVector3<float64>::axisY =  TVector3<float64>(0.0f,1.0f,0.0f);

    template <>
    const TVector3<float64> TVector3<float64>::axisZ = TVector3<float64>(0.0f,0.0f,1.0f);

} // namespace Berserk

#endif //BERSERK_VECTOR3_H
