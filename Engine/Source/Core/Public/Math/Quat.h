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
#include "Vector3.h"

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
    class CORE_EXPORT Quat
    {
    public:

        /**
         * Initialize quaternion with 0 values
         */
        Quat();

        /**
         * Initialize quaternion via scalar value s and vector part v
         *
         * @param s Scalar
         * @param v Vector XYZ components
         */
        Quat(T s, const Vector3<T>& v);

        /**
         * Initialize quaternion by element s
         *
         * @param s Scalar part
         * @param x Vector x component
         * @param y Vector y component
         * @param z Vector z component
         */
        Quat(T s, T x, T y, T z);

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
        Quat(const Vector3<T>& axis, T angle);

        /**
         * Build rotation quaternion of 1 lenght from Euler angles
         *
         * @note Angles should be in radians
         * @note Watch param description to exclude misunderstanding because of
         *       relating (roll, yaw, pitch) and rotation axises (oX,oY,oZ)
         *
         * @param roll  oX Clockwise rotation angle
         * @param yaw   oY Clockwise rotation angle
         * @param pitch oZ Clockwise rotation angle
         */
        explicit Quat(T roll, T yaw, T pitch);

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
        explicit Quat(const Vector3<T>& v);

        /**
         * Build rotation quaternion of 1 lenght from rotation matrix
         *
         * @param M Transformation matrix
         */
        explicit Quat(const Matrix4x4<T>& M);

        /**
	     * Creates and initializes a new quaternion from the given rotator.
	     *
	     * @param R The rotator to initialize from.
	     */
        explicit Quat(const Rotator& R);

        /**
         * Default copy constructor for quaternion
         * @param q Source quaternion
         */
        Quat(const Quat& q) = default;

    public:

        /**
         * Get normalized quaternion without changing of this one
         * @return That normalized quaternion
         */
        Quat getNormalized() const;

        /**
         * Convert this quaternion to 1 length and return itself
         * @return That normalized quaternion
         */
        Quat normalize();

        /**
         * Get inverse quaternion
         *
         * @return Inverse quaternion to that
         */
        Quat inverse() const;

        /**
         * Get conjugate quaternion
         *
         * @return Conjugate quaternion to that
         */
        Quat conjugate() const;

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
        Vector3<T> getVector3() const;

        /**
         * Operator assignment
         *
         * @param q
         * @return
         */
        Quat operator = (const Quat& q);

        /**
         * Per elements addition
         *
         * @param q
         * @return
         */
        Quat operator + (const Quat& q) const;

        /**
         * Per elements subtraction
         *
         * @param q
         * @return
         */
        Quat operator - (const Quat& q) const;

        /**
         * True quaternion multiplication
         *
         * @param q
         * @return
         */
        Quat operator * (const Quat& q) const;

        /**
         * Per elements multiplication by value a
         *
         * @param a
         * @return
         */
        Quat operator * (T a) const;

        /**
         * Per elements division by value a
         *
         * @param a
         * @return
         */
        Quat operator / (T a) const;

        /**
         * Per elements comparison
         *
         * @param q
         * @return
         */
        const bool operator == (const Quat& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator >= (const Quat& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator <= (const Quat& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator > (const Quat& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator < (const Quat& q) const;

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
        Vector3<T> rotate(const Vector3<T> &v) const;

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
        Vector4<T> rotate(const Vector4<T> &v) const;

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
        Vector3<T> unrotate(const Vector3<T>& v) const;

        /**
         * Get the forward direction (X axis) after it
         * has been rotated by this Quaternion.
         */
        Vector3<T> getAxisX() const;

        /**
         * Get the right direction (Y axis) after it
         * has been rotated by this Quaternion.
         */
        Vector3<T> getAxisY() const;

        /**
         * Get the up direction (Z axis) after it
         * has been rotated by this Quaternion.
         */
        Vector3<T> getAxisZ() const;

        /**
         * Creates rotation matrix from rotation quaternion
         *
         * @warning Quaternion should be of 1 length
         * @return Rotation 4x4 matrix
         */
        Matrix4x4<T> getMatrix() const;

        /**
	     * Get the axis and angle of rotation of this quaternion
	     *
	     * @param[out] Axis  vector of axis of the quaternion
	     * @param[out] Angle angle of the quaternion
	     * @warning Assumes normalized quaternions.
	     */
        void getAxisAngle(Vector3<T>& axis, T& angle) const;

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
        static T dot(Quat q1, Quat q2);

        /**
         * Get angle between quaternions q1 and q2
         *
         * @param q1
         * @param q2
         * @return
         */
        static T angle(Quat q1, Quat q2);

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
        Vector3<T> rotate(Quat q, Vector3<T> v);

        /**
         * Rotate vector v (or point with w=1) by rotation quaternion q of 1 length
         *
         * @warning Quaternion should be of 1 length
         *
         * @param q Rotation quaternion
         * @param v Vector to be rotated
         * @return Rotated vector (or point with w=1)
         */
        Vector4<T> rotate(Quat q, Vector4<T> v);

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
        static Quat lerp(Quat q1, Quat q2, T t);

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
        static Quat slerp(Quat q1, Quat q2, T t);

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
        static Quat slerp(Quat q1, Quat q2, T angle, T t);

    public:

        T s;
        T x;
        T y;
        T z;

    };

    template <typename T>
    Quat<T>::Quat()
            : s(0), x(0), y(0), z(0)
    {

    }

    template <typename T>
    Quat<T>::Quat(T s, const Vector3<T> &v)
            : s(s), x(v.x), y(v.y), z(v.z)
    {

    }

    template <typename T>
    Quat<T>::Quat(T s, T x, T y, T z)
            : s(s), x(x), y(y), z(z)
    {

    }

    template <typename T>
    Quat<T>::Quat(const Vector3<T>& axis, T angle)
    {
        Vector3<T> v = Vector3<T>::normalize(axis) * Math::sin(angle / 2);
        s = Math::cos(angle / 2);
        x = v.x;
        y = v.y;
        z = v.z;
    }

    template <typename T>
    Quat<T>::Quat(T roll, T yaw, T pitch)
    {
        *this = Quat(Vector3<T>::axisX, roll) *
                Quat(Vector3<T>::axisY, yaw) *
                Quat(Vector3<T>::axisZ, pitch);
    }

    template <typename T>
    Quat<T>::Quat(const Vector3<T> &v)
    {
        *this = Quat(Vector3<T>::axisX, v.x) *
                Quat(Vector3<T>::axisY, v.y) *
                Quat(Vector3<T>::axisZ, v.z);
    }

    template <typename T>
    Quat<T>::Quat(const Matrix4x4<T> &M)
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
    Quat<T>::Quat(const Rotator &R)
    {

    }

    template <typename T>
    Quat<T> Quat<T>::getNormalized() const
    {
        Quat<T> res = *this;
        return res.normalize();
    }

    template <typename T>
    Quat<T> Quat<T>::normalize()
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
    Quat<T> Quat<T>::inverse() const
    {
        T length = Math::sqrt(s * s + x * x + y * y + z * z);
        ASSERT(length, "Cannot inverse 0 quaternion");

        return Quat(s / length, -x / length, -y / length, -z / length);
    }

    template <typename T>
    Quat<T> Quat<T>::conjugate() const
    {
        return Quat(s, -x, -y, -z);
    }

    template <typename T>
    T Quat<T>::getNorm() const
    {
        return (s * s + x * x + y * y + z * z);
    }

    template <typename T>
    T Quat<T>::getLength() const
    {
        return Math::Math::sqrt(s * s + x * x + y * y + z * z);
    }

    template <typename T>
    Vector3<T> Quat<T>::getVector3() const
    {
        return Vector3<T>(x, y, z);
    }

    template <typename T>
    T Quat<T>::getScalar() const
    {
        return s;
    }

    template <typename T>
    Quat<T> Quat<T>::operator = (const Quat<T>& q)
    {
        s = q.s;
        x = q.x;
        y = q.y;
        z = q.z;

        return *this;
    }

    template <typename T>
    Quat<T> Quat<T>::operator + (const Quat<T>& q) const
    {
        return Quat(s + q.s, x + q.x, y + q.y, z + q.z);
    }

    template <typename T>
    Quat<T> Quat<T>::operator - (const Quat<T>& q) const
    {
        return Quat(s - q.s, x - q.x, y - q.y, z - q.z);
    }

    template <typename T>
    Quat<T> Quat<T>::operator * (const Quat<T>& q) const
    {
        // todo: rewrite in more simple mode

        Vector3<T> v = Vector3<T>(x, y, z);
        Vector3<T> w = Vector3<T>(q.x, q.y, q.z);

        return Quat(s * q.s - Vector3<T>::dot(v, w), w * s + v * q.s + Vector3<T>::cross(v, w));
    }

    template <typename T>
    Quat<T> Quat<T>::operator * (T a) const
    {
        return Quat(s * a, x * a, y * a, z * a);
    }

    template <typename T>
    Quat<T> Quat<T>::operator / (T a) const
    {
        return Quat(s / a, x / a, y / a, z / a);
    }

    template <typename T>
    const bool Quat<T>::operator == (const Quat<T>& q) const
    {
        return (s == q.s && x == q.x && y == q.y && z == q.z);
    }

    template <typename T>
    const bool Quat<T>::operator >= (const Quat<T>& q) const
    {
        return (getNorm() >= q.getNorm());
    }

    template <typename T>
    const bool Quat<T>::operator <= (const Quat<T>& q) const
    {
        return (getNorm() <= q.getNorm());
    }

    template <typename T>
    const bool Quat<T>::operator > (const Quat<T>& q) const
    {
        return (getNorm() > q.getNorm());
    }

    template <typename T>
    const bool Quat<T>::operator < (const Quat<T>& q) const
    {
        return (getNorm() < q.getNorm());
    }

    template <typename T>
    CName Quat<T>::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "(S=%3.3f X=%3.3f Y=%3.3f Z=%3.3f)", s, x, y, z);
        return CName(buffer);
    }

    template <typename T>
    Vector3<T> Quat<T>::rotate(const Vector3<T> &v) const
    {
        Quat<T> m = Quat<T>(0, v);
        Quat<T> r = *this * m * conjugate();
        return r.getVector3();
    }

    template <typename T>
    Vector4<T> Quat<T>::rotate(const Vector4<T> &v) const
    {
        Quat<T> m = Quat<T>(0, v.x, v.y, v.z);
        Quat<T> r = *this * m * conjugate();
        return Vector4<T>(r.x, r.y, r.z, v.w);
    }

    template <typename T>
    Vector3<T> Quat<T>::unrotate(const Vector3<T> &v) const
    {
        Quat<T> m = Quat<T>(0, v);
        Quat<T> r = conjugate() * m * (*this);
        return r.getVector3();
    }

    template <typename T>
    Vector3<T> Quat<T>::getAxisX() const
    {
        return rotate(Vector3<T>::axisX);
    }

    template <typename T>
    Vector3<T> Quat<T>::getAxisY() const
    {
        return rotate(Vector3<T>::axisY);
    }

    template <typename T>
    Vector3<T> Quat<T>::getAxisZ() const
    {
        return rotate(Vector3<T>::axisZ);
    }

    template <typename T>
    void Quat<T>::getAxisAngle(Vector3<T> &axis, T &angle) const
    {
        angle = 2 * Math::arccos(s);

        const T S = Math::sqrt(Math::max((T)(1.0 - s*s), (T)0.0));

        if (S >= Math::THRESH_ZERO_NORM_SQUARED) axis = Vector3<T>(x / S, y / S, z / S);
        else axis = Vector3<T>(1.0, 0.0, 0.0);
    }

    template <typename T>
    Matrix4x4<T> Quat<T>::getMatrix() const
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

        return Matrix4x4<T>(1 - 2 * (yy + zz), 2 * (xy - zw),     2 * (xz + yw),     0,
                            2 * (xy + zw),     1 - 2 * (xx + zz), 2 * (yz - xw),     0,
                            2 * (xz - yw),     2 * (yz + xw),     1 - 2 * (xx + yy), 0,
                            0,                 0 ,                0,                 1);
    }

    template <typename T>
    T Quat<T>::dot(Quat q1, Quat q2)
    {
        return q1.s * q2.s + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
    }

    template <typename T>
    T Quat<T>::angle(Quat<T> q1, Quat<T> q2)
    {
        return Math::arccos(dot(q1, q2));
    }

    template <typename T>
    Vector3<T> Quat<T>::rotate(Quat q, Vector3<T> v)
    {
        return q.rotate(v);
    }

    template <typename T>
    Vector4<T> Quat<T>::rotate(Quat q, Vector4<T> v)
    {
        return q.rotate(v);
    }

    template <typename T>
    Quat<T> Quat<T>::lerp(Quat<T> q1, Quat<T> q2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (q1 * (1 - t) + q2 * t).getNormalized();
    }

    template <typename T>
    Quat<T> Quat<T>::slerp(Quat<T> q1, Quat<T> q2, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T angle = Math::arccos(dot(q1, q2));
        T s = Math::sin(angle);

        return (q1 * (Math::sin((1 - t) * angle) / s) + q2 * (Math::sin(t * angle) / s));
    }

    template <typename T>
    Quat<T> Quat<T>::slerp(Quat<T> q1, Quat q2, T angle, T t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        T s = Math::sin(angle);

        return (q1 * (Math::sin((1 - t) * angle) / s) + q2 * (Math::sin(t * angle) / s));
    }

    
} // namespace Berserk

#endif //BERSERK_QUAT_H
