//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_VECTOR2_H
#define BERSERK_VECTOR2_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Assert.h"
#include "Public/Misc/UsageDescriptors.h"
#include "Public/Math/MathUtility.h"
#include "Public/Strings/StaticString.h"

namespace Berserk
{

    /**
     * 2 component generic vector type for math operations
     * @tparam T type (considered as float32 or float64)
     */
    template <typename T>
    class CORE_EXPORT TVector2
    {
    public:

        /**
         * Initialize (0,0) vector
         */
        TVector2();

        /**
         * Initialize by a value
         * @param a
         */
        explicit TVector2(T a);

        /**
         * Initialize (x,y) vector
         *
         * @param x
         * @param y
         */
        explicit TVector2(T x, T y);

        /**
         * @param v Firts x and y components
         */
        explicit TVector2(const TVector3<T>& v);

        /**
         * @param v First x and y components
         */
        explicit TVector2(const TVector4<T>& v);
        
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
        TVector2 getNormalized() const;

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
        TVector2 operator = (const TVector2& v);

        /**
         * Get vector this + v (per value operation)
         * @param v
         * @return Vector2 this + v
         */
        TVector2 operator + (const TVector2& v) const;

        /**
         * Get vector this - v (per value operation)
         * @param v
         * @return Vector2 this - v
         */
        TVector2 operator - (const TVector2& v) const;

        /**
         * Get vector this * v (per value operation)
         * @param v
         * @return Vector2 this * v
         */
        TVector2 operator * (const TVector2& v) const;

        /**
         * Get vector this / v (per value operation)
         * @param v
         * @return Vector2 this / v
         */
        TVector2 operator / (const TVector2& v) const;

        /**
         * Get vector this * a (per value operation)
         * @param a
         * @return Vector2 this * a
         */
        TVector2 operator * (T a) const;

        /**
         * Get vector this / v (per value operation)
         * @param a
         * @return Vector2 this / v
         */
        TVector2 operator / (T a) const;

        /**
         * Check per value comparison
         * @param v
         * @return If (this == v)
         */
        const bool operator == (const TVector2& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length >= v.length)
         */
        const bool operator >= (const TVector2& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length <= v.length)
         */
        const bool operator <= (const TVector2& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length > v.length)
         */
        const bool operator > (const TVector2& v) const;

        /**
         * Check comparison via length
         * @param v
         * @return If (this.length < v.length)
         */
        const bool operator < (const TVector2& v) const;

        /**
         * @return x component
         */
        T getX() const;

        /**
         * @return y component
         */
        T getY() const;

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
        static T dot(TVector2 v1, TVector2 v2);

        /**
         * Transform vector to 1 length
         *
         * @param v
         * @return
         */
        static TVector2 normalize(TVector2 v);

        /**
         * Linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static TVector2 lerp(TVector2 v1, TVector2 v2, T t);

        /**
         * Spherical linear interpolation from vector v1 to vector v2 by param t
         * @warning t should be in [0;1]
         *
         * @param v1
         * @param v2
         * @param t
         * @return
         */
        static TVector2 slerp(TVector2 v1, TVector2 v2, T t);

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
        static TVector2 slerp(TVector2 v1, TVector2 v2, T angle, T t);

        /**
         * Smooth interpolation of t between vectors v1 and v2
         * @see smoothstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static TVector2 smoothstep(TVector2 v1, TVector2 v2, T t);

        /**
         * Smoother interpolation of t between vectors v1 and v2
         * @see smootherstep for float64
         *
         * @param v1 Lower limit
         * @param v2 Upper limit
         * @param t Value for interpolation
         * @return Interpolated vector
         */
        static TVector2 smootherstep(TVector2 v1, TVector2 v2, T t);

    public:

        T x;
        T y;

    };

    template <typename T>
    TVector2<T>::TVector2()
            : x(0), y(0)
    {

    }

    template <typename T>
    TVector2<T>::TVector2(T a)
            : x(a), y(a)
    {

    }

    template <typename T>
    TVector2<T>::TVector2(T x, T y)
            : x(x), y(y)
    {

    }

    template <typename T>
    TVector2<T>::TVector2(const TVector3<T> &v)
            : x(v.x), y(v.y)
    {
    }

    template <typename T>
    TVector2<T>::TVector2(const TVector4<T> &v)
            : x(v.x), y(v.y)
    {

    }

    template <typename T>
    void TVector2<T>::normalize()
    {
        T l = length();
        ASSERT(l, "Length should be more than 0 to normalize");

        x /= l;
        y /= l;
    }

    template <typename T>
    TVector2<T> TVector2<T>::getNormalized() const
    {
        return *this / length();
    }

    template <typename T>
    T TVector2<T>::length() const
    {
        return Math::sqrt(x * x + y * y);
    }

    template <typename T>
    T TVector2<T>::norm() const
    {
        return (x * x + y * y);
    }

    template <typename T>
    TVector2<T> TVector2<T>::operator = (const TVector2<T>& v)
    {
        this->x = v.x;
        this->y = v.y;
        return *this;
    }

    template <typename T>
    TVector2<T> TVector2<T>::operator + (const TVector2<T>& v) const
    {
        return TVector2(this->x + v.x, this->y + v.y);
    }

    template <typename T>
    TVector2<T> TVector2<T>::operator - (const TVector2<T>& v) const
    {
        return TVector2(this->x - v.x, this->y - v.y);
    }

    template <typename T>
    TVector2<T> TVector2<T>::operator * (const TVector2<T>& v) const
    {
        return TVector2(this->x * v.x, this->y * v.y);
    }

    template <typename T>
    TVector2<T> TVector2<T>::operator / (const TVector2<T>& v) const
    {
        return TVector2(this->x / v.x, this->y / v.y);
    }

    template <typename T>
    TVector2<T> TVector2<T>::operator * (const T a) const
    {
        return TVector2(this->x * a, this->y * a);
    }

    template <typename T>
    TVector2<T> TVector2<T>::operator / (const T a) const
    {
        return TVector2(this->x / a, this->y / a);
    }

    template <typename T>
    const bool TVector2<T>::operator == (const TVector2<T>& v) const
    {
        return (x == v.x && y == v.y);
    }

    template <typename T>
    const bool TVector2<T>::operator >= (const TVector2<T>& v) const
    {
        return (norm() >= v.norm());
    }

    template <typename T>
    const bool TVector2<T>::operator <= (const TVector2<T>& v) const
    {
        return (norm() <= v.norm());
    }

    template <typename T>
    const bool TVector2<T>::operator > (const TVector2<T>& v) const
    {
        return (norm() > v.norm());
    }

    template <typename T>
    const bool TVector2<T>::operator < (const TVector2<T>& v) const
    {
        return (norm() > v.norm());
    }

    template <typename T>
    T TVector2<T>::getX() const
    {
        return x;
    }

    template <typename T>
    T TVector2<T>::getY() const
    {
        return y;
    }

    template <typename T>
    CName TVector2<T>::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "(X=%3.3f Y=%3.3f)", x, y);
        return CName(buffer);
    }

    template <typename T>
    T TVector2<T>::dot(TVector2<T> v1, TVector2<T> v2)
    {
        return (v1.x * v2.x + v1.y * v2.y);
    }

    template <typename T>
    TVector2<T> TVector2<T>::normalize(TVector2<T> v)
    {
        TVector2<T> r = v;
        r.normalize();
        return r;
    }

    template <typename T>
    TVector2<T> TVector2<T>::lerp(TVector2<T> v1, TVector2<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    template <typename T>
    TVector2<T> TVector2<T>::slerp(TVector2<T> v1, TVector2<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T angle = dot(v1.getNormalized(), v2.getNormalized());
        T sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));

    }

    template <typename T>
    TVector2<T> TVector2<T>::slerp(TVector2<T> v1, TVector2<T> v2, T angle, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    template <typename T>
    TVector2<T> TVector2<T>::smoothstep(TVector2<T> v1, TVector2<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = (T)(2 * t * t * (1.5 - t));
        return lerp(v1, v2, t);
    }

    template <typename T>
    TVector2<T> TVector2<T>::smootherstep(TVector2<T> v1, TVector2<T> v2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }
    
}  // namespace Berserk

#endif //BERSERK_VECTOR2_H
