//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_QUAT_H
#define BERSERK_QUAT_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Assert.h"
#include "Public/Misc/UsageDescriptors.h"
#include "Public/Math/MathUtility.h"
#include "Public/Strings/StaticString.h"
#include "TVector3.h"

namespace Berserk
{

    /**
     * Floating point quaternion that can represent a rotation about an axis in 3D space.
     * The s, x, y, z components corresponds as the Angle/Axis format.
     *
     * @note Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
     *       first applies B then A to any subsequent transformation (right first, then left).
     * @note Only unit length quaternions could be used for rotating of vectors in 3D space
     *
     * @example LocalToWorld = (LocalToWorld * DeltaRotation) will change rotation in local space by DeltaRotation.
     * @example LocalToWorld = (DeltaRotation * LocalToWorld) will change rotation in world space by DeltaRotation.
     *
     * @tparam Type of quaternion values
     */
    template <typename T>
    class CORE_EXPORT TQuat
    {
    public:

        /**
         * Initialize quaternion with 0 values
         */
        TQuat();

        /**
         * Initialize quaternion via scalar value s and vector part v
         *
         * @param s Scalar
         * @param v Vector XYZ components
         */
        TQuat(T s, const TVector3<T>& v);

        /**
         * Initialize quaternion by element s
         *
         * @param s Scalar part
         * @param x Vector x component
         * @param y Vector y component
         * @param z Vector z component
         */
        TQuat(T s, T x, T y, T z);

        /**
         * Build rotation quaternion of 1 length from rotation axis
         * and angle in radians
         *
         * @note Clockwise rotation
         * @warning Axis should be not 0 length
         *
         * @param axis The vector of rotation
         * @param angle The angle of rotation in radians
         * @return Rotation quaternion of 1 length
         */
        TQuat(const TVector3<T>& axis, T angle);

        /**
         * Build rotation quaternion of 1 length from Euler angles
         *
         * @note Angles should be in radians
         * @note Watch param description to exclude misunderstanding because of
         *       relating (roll, yaw, pitch) and rotation axises (oX,oY,oZ)
         *
         * @param roll  oX Clockwise rotation angle
         * @param yaw   oY Clockwise rotation angle
         * @param pitch oZ Clockwise rotation angle
         */
        explicit TQuat(T roll, T yaw, T pitch);

        /**
         * Build rotation quaternion of 1 lenght from Euler angles
         *
         * @note Angles should be in radians
         * @note Watch param description to exclude misunderstanding because of
         *       relating (roll, yaw, pitch) and rotation axises (oX,oY,oZ)
         *
         * @param v x = roll  oX Clockwise rotation angle
         *          y = yaw   oY Clockwise rotation angle
         *          z = pitch oZ Clockwise rotation angle
         */
        explicit TQuat(const TVector3<T>& v);

        /**
         * Build rotation quaternion of 1 lenght from rotation matrix
         *
         * @param M Transformation matrix
         */
        explicit TQuat(const TMatrix4x4<T>& M);

        /**
	     * Creates and initializes a new quaternion from the given rotator.
	     *
	     * @param R The rotator to initialize from.
	     */
        explicit TQuat(const Rotation& R);

        /**
         * Default copy constructor for quaternion
         * @param q Source quaternion
         */
        TQuat(const TQuat& q) = default;

    public:

        /**
         * Get normalized quaternion without changing of this one
         * @return That normalized quaternion
         */
        TQuat getNormalized() const;

        /**
         * Convert this quaternion to 1 length and return itself
         * @return That normalized quaternion
         */
        TQuat normalize();

        /**
         * Get inverse quaternion
         *
         * @return Inverse quaternion to that
         */
        TQuat inverse() const;

        /**
         * Get conjugate quaternion
         *
         * @return Conjugate quaternion to that
         */
        TQuat conjugate() const;

        /**
         * Get quaternion length without sqrt
         *
         * @return Norm
         */
        T getNorm() const;

        /**
         * Get true quaternion length
         *
         * @return Length
         */
        T getLength() const;

        /**
         * Get s scalar value
         *
         * @return Scalar part
         */
        T getScalar() const;

        /**
         * Get vector value
         *
         * @return Vector part
         */
        TVector3<T> getVector3() const;

        /**
         * Operator assignment
         *
         * @param q
         * @return
         */
        TQuat operator = (const TQuat& q);

        /**
         * Per elements addition
         *
         * @param q
         * @return
         */
        TQuat operator + (const TQuat& q) const;

        /**
         * Per elements subtraction
         *
         * @param q
         * @return
         */
        TQuat operator - (const TQuat& q) const;

        /**
         * True quaternion multiplication
         *
         * @param q
         * @return
         */
        TQuat operator * (const TQuat& q) const;

        /**
         * Per elements multiplication by value a
         *
         * @param a
         * @return
         */
        TQuat operator * (T a) const;

        /**
         * Per elements division by value a
         *
         * @param a
         * @return
         */
        TQuat operator / (T a) const;

        /**
         * Per elements comparison
         *
         * @param q
         * @return
         */
        const bool operator == (const TQuat& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator >= (const TQuat& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator <= (const TQuat& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator > (const TQuat& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator < (const TQuat& q) const;

    public:

        /**
         * Rotate vector v by rotation quaternion q of 1 length
         *
         * @note Clockwise rotation
         * @warning Quaternion should be of 1 length
         *
         * @param q Rotation quaternion
         * @param v Vector to be rotated
         * @return Rotated vector
         */
        TVector3<T> rotate(const TVector3<T> &v) const;

        /**
         * Rotate vector v by rotation quaternion q of 1 length
         *
         * @note Clockwise rotation
         * @warning Quaternion should be of 1 length
         *
         * @param q Rotation quaternion
         * @param v Vector to be rotated
         * @return Rotated vector
         */
        TVector4<T> rotate(const TVector4<T> &v) const;

        /**
         * Rotate vector v by the inverse of this quaternion
         *
         * @note Clockwise rotation
         * @warning Quaternion should be of 1 length
         *
         * @param q Rotation quaternion
         * @param v Vector to be rotated
         * @return Rotated vector
         */
        TVector3<T> unrotate(const TVector3<T>& v) const;

        /**
         * Get the forward direction (X axis) after it
         * has been rotated by this Quaternion.
         */
        TVector3<T> getAxisX() const;

        /**
         * Get the right direction (Y axis) after it
         * has been rotated by this Quaternion.
         */
        TVector3<T> getAxisY() const;

        /**
         * Get the up direction (Z axis) after it
         * has been rotated by this Quaternion.
         */
        TVector3<T> getAxisZ() const;

        /**
         * Creates rotation matrix from rotation quaternion
         *
         * @warning Quaternion should be of 1 length
         * @return Rotation 4x4 matrix
         */
        TMatrix4x4<T> getMatrix() const;

        /**
	     * Get the axis and angle of rotation of this quaternion
	     *
	     * @param[out] Axis  vector of axis of the quaternion
	     * @param[out] Angle angle of the quaternion
	     * @warning Assumes normalized quaternions.
	     */
        void getAxisAngle(TVector3<T>& axis, T& angle) const;

        /**
	     * Get a textual representation of the quaternion.
	     *
	     * @return Text describing the quaternion.
	     */
        CName toString() const;

    public:

        /**
         * Dot product for q1 and q2 quaternions similar to vectors dot product
         *
         * @param q1
         * @param q2
         * @return
         */
        static T dot(TQuat q1, TQuat q2);

        /**
         * Get angle between quaternions q1 and q2
         *
         * @param q1
         * @param q2
         * @return
         */
        static T angle(TQuat q1, TQuat q2);

        /**
         * Rotate vector v by rotation quaternion q of 1 length
         *
         * @note Clockwise rotation
         * @warning Quaternion should be of 1 length
         *
         * @param q Rotation quaternion
         * @param v Vector to be rotated
         * @return Rotated vector
         */
        TVector3<T> rotate(TQuat q, TVector3<T> v);

        /**
         * Rotate vector v (or point with w=1) by rotation quaternion q of 1 length
         *
         * @warning Quaternion should be of 1 length
         *
         * @param q Rotation quaternion
         * @param v Vector to be rotated
         * @return Rotated vector (or point with w=1)
         */
        TVector4<T> rotate(TQuat q, TVector4<T> v);

        /**
         * Linear interpolation from q1 to q2 via param t
         *
         * @warning Param t should be in [0;1]
         *
         * @param q1 Begin quaternion
         * @param q2 End quaternion
         * @param t Interpolation param in [0;1]
         * @return Interpolated quaternion of 1 length
         */
        static TQuat lerp(TQuat q1, TQuat q2, T t);

        /**
         * Spherical linear interpolation from q1 to q2 via param t
         *
         * @warning Param t should be in [0;1]
         * @warning Quaternions should be of 1 length
         *
         * @param q1 Begin quaternion
         * @param q2 End quaternion
         * @param t Interpolation param in [0;1]
         * @return Interpolated quaternion of 1 length
         */
        static TQuat slerp(TQuat q1, TQuat q2, T t);

        /**
         * Spherical linear interpolation from q1 to q2 via param t
         *
         * @warning Param t should be in [0;1]
         * @warning Quaternions should be of 1 length
         * @warning Get angle by getAngle(q1, q2)
         *
         * @param q1 Begin quaternion
         * @param q2 End quaternion
         * @param angle Angle between quaternions
         * @param t Interpolation param in [0;1]
         * @return Interpolated quaternion of 1 length
         */
        static TQuat slerp(TQuat q1, TQuat q2, T angle, T t);

    public:

        T s;
        T x;
        T y;
        T z;

    };

    template <typename T>
    TQuat<T>::TQuat()
            : s(0), x(0), y(0), z(0)
    {

    }

    template <typename T>
    TQuat<T>::TQuat(T s, const TVector3<T> &v)
            : s(s), x(v.x), y(v.y), z(v.z)
    {

    }

    template <typename T>
    TQuat<T>::TQuat(T s, T x, T y, T z)
            : s(s), x(x), y(y), z(z)
    {

    }

    template <typename T>
    TQuat<T>::TQuat(const TVector3<T>& axis, T angle)
    {
        TVector3<T> v = TVector3<T>::normalize(axis) * Math::sin(angle / 2);
        s = Math::cos(angle / 2);
        x = v.x;
        y = v.y;
        z = v.z;
    }

    template <typename T>
    TQuat<T>::TQuat(T roll, T yaw, T pitch)
    {
        *this = TQuat(TVector3<T>::axisX, roll) *
                TQuat(TVector3<T>::axisY, yaw) *
                TQuat(TVector3<T>::axisZ, pitch);
    }

    template <typename T>
    TQuat<T>::TQuat(const TVector3<T> &v)
    {
        *this = TQuat(TVector3<T>::axisX, v.x) *
                TQuat(TVector3<T>::axisY, v.y) *
                TQuat(TVector3<T>::axisZ, v.z);
    }

    template <typename T>
    TQuat<T>::TQuat(const TMatrix4x4<T> &M)
    {
        T q[4]; // notation: x[0] y[1] z[2] w[3]

        T trace = M.m[0] + M.m[5] + M.m[10];
        //M = M.GetTranspose();

        // Matrix 4x4 indexes
        // 0  1  2  3
        // 4  5  6  7
        // 8  9  10 11
        // 12 13 14 15

        // Check the diagonal
        if (trace > 0.0)
        {
            // positive diagonal

            T s = Math::sqrt(trace + 1.0);
            q[3] = s * 0.5f;

            T t = 0.5f / s;
            q[0] = (M.m[9] - M.m[6]) * t;
            q[1] = (M.m[2] - M.m[8]) * t;
            q[2] = (M.m[4] - M.m[1]) * t;
        }
        else
        {
            // negative diagonal

            int32 i = 0;
            if (M.m[5] > M.m[0]) i = 1;
            if (M.m[10] > M.m[4 * i + i]) i = 2;

            static const int32 NEXT[3] = {1, 2, 0};
            int32 j = NEXT[i];
            int32 k = NEXT[j];

            T s = Math::sqrt(M.m[i * 4 + i] - (M.m[j * 4 + j] + M.m[k * 4 + k]) + 1.0);

            T t;
            if (s == 0.0) t = s;
            else t = 0.5f / s;

            q[i] = s * 0.5f;
            q[3] = (M.m[k * 4 + j] - M.m[j * 4 + k]) * t;
            q[j] = (M.m[j * 4 + i] - M.m[i * 4 + j]) * t;
            q[k] = (M.m[k * 4 + i] - M.m[i * 4 + k]) * t;
        }

        s = q[3];
        x = q[0];
        y = q[1];
        z = q[2];
    }

    template <typename T>
    TQuat<T>::TQuat(const Rotation &R)
    {

    }

    template <typename T>
    TQuat<T> TQuat<T>::getNormalized() const
    {
        TQuat<T> res = *this;
        return res.normalize();
    }

    template <typename T>
    TQuat<T> TQuat<T>::normalize()
    {
        T length = Math::sqrt(s * s + x * x + y * y + z * z);
        ASSERT(length, "Quaternionf length should be more than 0");

        s /= length;
        x /= length;
        y /= length;
        z /= length;

        return (*this);
    }

    template <typename T>
    TQuat<T> TQuat<T>::inverse() const
    {
        T length = Math::sqrt(s * s + x * x + y * y + z * z);
        ASSERT(length, "Cannot inverse 0 quaternion");

        return TQuat(s / length, -x / length, -y / length, -z / length);
    }

    template <typename T>
    TQuat<T> TQuat<T>::conjugate() const
    {
        return TQuat(s, -x, -y, -z);
    }

    template <typename T>
    T TQuat<T>::getNorm() const
    {
        return (s * s + x * x + y * y + z * z);
    }

    template <typename T>
    T TQuat<T>::getLength() const
    {
        return Math::Math::sqrt(s * s + x * x + y * y + z * z);
    }

    template <typename T>
    TVector3<T> TQuat<T>::getVector3() const
    {
        return TVector3<T>(x, y, z);
    }

    template <typename T>
    T TQuat<T>::getScalar() const
    {
        return s;
    }

    template <typename T>
    TQuat<T> TQuat<T>::operator = (const TQuat<T>& q)
    {
        s = q.s;
        x = q.x;
        y = q.y;
        z = q.z;

        return *this;
    }

    template <typename T>
    TQuat<T> TQuat<T>::operator + (const TQuat<T>& q) const
    {
        return TQuat(s + q.s, x + q.x, y + q.y, z + q.z);
    }

    template <typename T>
    TQuat<T> TQuat<T>::operator - (const TQuat<T>& q) const
    {
        return TQuat(s - q.s, x - q.x, y - q.y, z - q.z);
    }

    template <typename T>
    TQuat<T> TQuat<T>::operator * (const TQuat<T>& q) const
    {
        // todo: rewrite in more simple mode

        TVector3<T> v = TVector3<T>(x, y, z);
        TVector3<T> w = TVector3<T>(q.x, q.y, q.z);

        return TQuat(s * q.s - TVector3<T>::dot(v, w), w * s + v * q.s + TVector3<T>::cross(v, w));
    }

    template <typename T>
    TQuat<T> TQuat<T>::operator * (T a) const
    {
        return TQuat(s * a, x * a, y * a, z * a);
    }

    template <typename T>
    TQuat<T> TQuat<T>::operator / (T a) const
    {
        return TQuat(s / a, x / a, y / a, z / a);
    }

    template <typename T>
    const bool TQuat<T>::operator == (const TQuat<T>& q) const
    {
        return (s == q.s && x == q.x && y == q.y && z == q.z);
    }

    template <typename T>
    const bool TQuat<T>::operator >= (const TQuat<T>& q) const
    {
        return (getNorm() >= q.getNorm());
    }

    template <typename T>
    const bool TQuat<T>::operator <= (const TQuat<T>& q) const
    {
        return (getNorm() <= q.getNorm());
    }

    template <typename T>
    const bool TQuat<T>::operator > (const TQuat<T>& q) const
    {
        return (getNorm() > q.getNorm());
    }

    template <typename T>
    const bool TQuat<T>::operator < (const TQuat<T>& q) const
    {
        return (getNorm() < q.getNorm());
    }

    template <typename T>
    CName TQuat<T>::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "(S=%3.3f X=%3.3f Y=%3.3f Z=%3.3f)", s, x, y, z);
        return CName(buffer);
    }

    template <typename T>
    TVector3<T> TQuat<T>::rotate(const TVector3<T> &v) const
    {
        TQuat<T> m = TQuat<T>(0, v);
        TQuat<T> r = *this * m * conjugate();
        return r.getVector3();
    }

    template <typename T>
    TVector4<T> TQuat<T>::rotate(const TVector4<T> &v) const
    {
        TQuat<T> m = TQuat<T>(0, v.x, v.y, v.z);
        TQuat<T> r = *this * m * conjugate();
        return TVector4<T>(r.x, r.y, r.z, v.w);
    }

    template <typename T>
    TVector3<T> TQuat<T>::unrotate(const TVector3<T> &v) const
    {
        TQuat<T> m = TQuat<T>(0, v);
        TQuat<T> r = conjugate() * m * (*this);
        return r.getVector3();
    }

    template <typename T>
    TVector3<T> TQuat<T>::getAxisX() const
    {
        return rotate(TVector3<T>::axisX);
    }

    template <typename T>
    TVector3<T> TQuat<T>::getAxisY() const
    {
        return rotate(TVector3<T>::axisY);
    }

    template <typename T>
    TVector3<T> TQuat<T>::getAxisZ() const
    {
        return rotate(TVector3<T>::axisZ);
    }

    template <typename T>
    void TQuat<T>::getAxisAngle(TVector3<T> &axis, T &angle) const
    {
        angle = 2 * Math::arccos(s);

        const T S = Math::sqrt(Math::max((T)(1.0 - s*s), (T)0.0));

        if (S >= Math::THRESH_ZERO_NORM_SQUARED) axis = TVector3<T>(x / S, y / S, z / S);
        else axis = TVector3<T>(1.0, 0.0, 0.0);
    }

    template <typename T>
    TMatrix4x4<T> TQuat<T>::getMatrix() const
    {
        T xx = x * x;
        T xy = x * y;
        T xz = x * z;
        T xw = x * s;

        T yy = y * y;
        T yz = y * z;
        T yw = y * s;

        T zz = z * z;
        T zw = z * s;

        return TMatrix4x4<T>(1 - 2 * (yy + zz), 2 * (xy - zw),     2 * (xz + yw),     0,
                            2 * (xy + zw),     1 - 2 * (xx + zz), 2 * (yz - xw),     0,
                            2 * (xz - yw),     2 * (yz + xw),     1 - 2 * (xx + yy), 0,
                            0,                 0 ,                0,                 1);
    }

    template <typename T>
    T TQuat<T>::dot(TQuat q1, TQuat q2)
    {
        return q1.s * q2.s + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
    }

    template <typename T>
    T TQuat<T>::angle(TQuat<T> q1, TQuat<T> q2)
    {
        return Math::arccos(dot(q1, q2));
    }

    template <typename T>
    TVector3<T> TQuat<T>::rotate(TQuat q, TVector3<T> v)
    {
        return q.rotate(v);
    }

    template <typename T>
    TVector4<T> TQuat<T>::rotate(TQuat q, TVector4<T> v)
    {
        return q.rotate(v);
    }

    template <typename T>
    TQuat<T> TQuat<T>::lerp(TQuat<T> q1, TQuat<T> q2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (q1 * (1 - t) + q2 * t).getNormalized();
    }

    template <typename T>
    TQuat<T> TQuat<T>::slerp(TQuat<T> q1, TQuat<T> q2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T angle = Math::arccos(dot(q1, q2));
        T s = Math::sin(angle);

        return (q1 * (Math::sin((1 - t) * angle) / s) + q2 * (Math::sin(t * angle) / s));
    }

    template <typename T>
    TQuat<T> TQuat<T>::slerp(TQuat<T> q1, TQuat q2, T angle, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T s = Math::sin(angle);

        return (q1 * (Math::sin((1 - t) * angle) / s) + q2 * (Math::sin(t * angle) / s));
    }

    
} // namespace Berserk

#endif //BERSERK_QUAT_H
