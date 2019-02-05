//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_VECTOR4_H
#define BERSERK_VECTOR4_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Assert.h"
#include "Public/Misc/UsageDescriptors.h"
#include "Public/Math/MathUtility.h"
#include "Public/Strings/StaticString.h"

namespace Berserk
{

    /**
     * 4 component generic vector type for math operations
     * @tparam T type (considered as float32 or float64)
     */
    template <typename T>
    class CORE_EXPORT TVector4
    {
    public:

        /**
         * Initialize (0,0,0,0) vector
         */
        TVector4();

        /**
         * Initialize by a value
         * @param a
         */
        explicit TVector4(T a);

        /**
         * Initialize (x,y,z,w) vector
         *
         * @param x
         * @param y
         * @param z
         * @param w
         */
        explicit TVector4(T x, T y, T z, T w);

        /**
         * @param v First x and y components
         * @param z Z component
         * @param w W component
         */
        explicit TVector4(const TVector2<T>& v, T z, T w);

        /**
         * @param xy Vector with x and y components
         * @param zw Vector with z and w components
         */
        explicit TVector4(const TVector2<T>& xy, const TVector2<T>& zw);

        /**
         * @param v Vector with xyz components
         * @param w W component
         */
        explicit TVector4(const TVector3<T>& v, T w);

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
        TVector4 getNormalized() const;

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
        TVector4 operator = (const TVector4& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector this + v
         */
        TVector4 operator + (const TVector4& v) const;

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector this - v
         */
        TVector4 operator - (const TVector4& v) const;

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector this * v
         */
        TVector4 operator * (const TVector4& v) const;

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector this / v
         */
        TVector4 operator / (const TVector4& v) const;

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector this * a
         */
        TVector4 operator * (T a) const;

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector this / v
         */
        TVector4 operator / (T a) const;

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const TVector4& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const TVector4& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const TVector4& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const TVector4& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const TVector4& v) const;

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
         * @return w component
         */
        T getW() const;

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
        static T dot(TVector4 v1, TVector4 v2);

        /**
         * Transform vector to 1 length
         *
         * @param v
         * @return
         */
        static TVector4 normalize(TVector4 v);

        /**
         * Linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static TVector4 lerp(TVector4 v1, TVector4 v2, T t);

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
        static TVector4 slerp(TVector4 v1, TVector4 v2, T t);

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
        static TVector4 slerp(TVector4 v1, TVector4 v2, T angle, T t);

        /**
         * Smooth interpolation of t between vectors v1 and v2
         * @see smoothstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static TVector4 smoothstep(TVector4 v1, TVector4 v2, T t);

        /**
         * Smoother interpolation of t between vectors v1 and v2
         * @see smootherstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static TVector4 smootherstep(TVector4 v1, TVector4 v2, T t);

    public:

        T x;
        T y;
        T z;
        T w;

    };

    template <typename T>
    TVector4<T>::TVector4()
            : x(0), y(0), z(0), w(0)
    {

    }

    template <typename T>
    TVector4<T>::TVector4(T a)
            : x(a), y(a), z(a), w(a)
    {

    }

    template <typename T>
    TVector4<T>::TVector4(T x, T y, T z, T w)
            : x(x), y(y), z(z), w(w)
    {

    }

    template <typename T>
    TVector4<T>::TVector4(const TVector2<T> &v, T z, T w)
            : x(v.x), y(v.y), z(z), w(w)
    {

    }

    template <typename T>
    TVector4<T>::TVector4(const TVector2<T> &xy, const TVector2<T> &zw)
            : x(xy.x), y(xy.y), z(zw.x), w(zw.y)
    {

    }

    template <typename T>
    TVector4<T>::TVector4(const TVector3<T> &v, T w)
            : x(v.x), y(v.y), z(v.z), w(w)
    {

    }

    template <typename T>
    void TVector4<T>::normalize()
    {
        T l = length();
        ASSERT(l, "Length should be more than 0 to normalize");

        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }

    template <typename T>
    TVector4<T> TVector4<T>::getNormalized() const
    {
        return *this / length();
    }

    template <typename T>
    T TVector4<T>::length() const
    {
        return Math::sqrt(x * x + y * y + z * z + w * w);
    }

    template <typename T>
    T TVector4<T>::norm() const
    {
        return (x * x + y * y + z * z + w * w);
    }

    template <typename T>
    TVector4<T> TVector4<T>::operator = (const TVector4<T>& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = v.w;
        return *this;
    }

    template <typename T>
    TVector4<T> TVector4<T>::operator + (const TVector4<T>& v) const
    {
        return TVector4(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
    }

    template <typename T>
    TVector4<T> TVector4<T>::operator - (const TVector4<T>& v) const
    {
        return TVector4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
    }

    template <typename T>
    TVector4<T> TVector4<T>::operator * (const TVector4<T>& v) const
    {
        return TVector4(this->x * v.x, this->y * v.y, this->z * v.z, this->w * v.w);
    }

    template <typename T>
    TVector4<T> TVector4<T>::operator / (const TVector4<T>& v) const
    {
        return TVector4(this->x / v.x, this->y / v.y, this->z / v.z, this->w / v.w);
    }

    template <typename T>
    TVector4<T> TVector4<T>::operator * (const T a) const
    {
        return TVector4(this->x * a, this->y * a, this->z * a, this->w * a);
    }

    template <typename T>
    TVector4<T> TVector4<T>::operator / (const T a) const
    {
        return TVector4(this->x / a, this->y / a, this->z / a, this->w / a);
    }

    template <typename T>
    const bool TVector4<T>::operator == (const TVector4<T>& v) const
    {
        return (x == v.x && y == v.y && z == v.z && w == v.w);
    }

    template <typename T>
    const bool TVector4<T>::operator >= (const TVector4<T>& v) const
    {
        return (norm() >= v.norm());
    }

    template <typename T>
    const bool TVector4<T>::operator <= (const TVector4<T>& v) const
    {
        return (norm() <= v.norm());
    }

    template <typename T>
    const bool TVector4<T>::operator > (const TVector4<T>& v) const
    {
        return (norm() > v.norm());
    }

    template <typename T>
    const bool TVector4<T>::operator < (const TVector4<T>& v) const
    {
        return (norm() > v.norm());
    }

    template <typename T>
    T TVector4<T>::getX() const
    {
        return x;
    }

    template <typename T>
    T TVector4<T>::getY() const
    {
        return y;
    }

    template <typename T>
    T TVector4<T>::getZ() const
    {
        return z;
    }

    template <typename T>
    T TVector4<T>::getW() const
    {
        return w;
    }

    template <typename T>
    CName TVector4<T>::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "(X=%3.3f Y=%3.3f Z=%3.3f W=%3.3f)", x, y, z, w);
        return CName(buffer);
    }

    template <typename T>
    T TVector4<T>::dot(TVector4<T> v1, TVector4<T> v2)
    {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w + v2.w);
    }

    template <typename T>
    TVector4<T> TVector4<T>::normalize(TVector4<T> v)
    {
        TVector4<T> r = v;
        r.normalize();
        return r;
    }

    template <typename T>
    TVector4<T> TVector4<T>::lerp(TVector4<T> v1, TVector4<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    template <typename T>
    TVector4<T> TVector4<T>::slerp(TVector4<T> v1, TVector4<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T angle = dot(v1.getNormalized(), v2.getNormalized());
        T sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    template <typename T>
    TVector4<T> TVector4<T>::slerp(TVector4<T> v1, TVector4<T> v2, T angle, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    template <typename T>
    TVector4<T> TVector4<T>::smoothstep(TVector4<T> v1, TVector4<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = (T)(2 * t * t * (1.5 - t));
        return lerp(v1, v2, t);
    }

    template <typename T>
    TVector4<T> TVector4<T>::smootherstep(TVector4<T> v1, TVector4<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

} // namespace Berserk

#endif //BERSERK_VECTOR4_H
