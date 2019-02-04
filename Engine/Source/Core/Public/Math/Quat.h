//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_QUAT_H
#define BERSERK_QUAT_H

#include "Public/Misc/Types.h"
#include "Public/Misc/UsageDescriptors.h"
#include "Public/Math/MathUtility.h"
#include "Public/Strings/StringInclude.h"

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
    private:

        typedef Vector3<T> Vector3t;
        typedef Matrix4x4<T> Matrix4x4t;

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
        explicit Quat(T s, const Vector3t& v);

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
        Quat(const Vector3t& axis, T angle);

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
        explicit Quat(const Vector3t& v);

        /**
         * Build rotation quaternion of 1 lenght from rotation matrix
         *
         * @param M Transformation matrix
         */
        explicit Quat(const Matrix4x4t& M);

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
        Vector3t getVector3() const;

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
        Vector3t rotate(const Vector3t& V) const;

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
        Vector3t unrotate(const Vector3t& v) const;

        /**
         * Get the forward direction (X axis) after it
         * has been rotated by this Quaternion.
         */
        Vector3t getAxisX() const;

        /**
         * Get the right direction (Y axis) after it
         * has been rotated by this Quaternion.
         */
        Vector3t getAxisY() const;

        /**
         * Get the up direction (Z axis) after it
         * has been rotated by this Quaternion.
         */
        Vector3t getAxisZ() const;

        /**
	     * Get the axis and angle of rotation of this quaternion
	     *
	     * @param[out] Axis  vector of axis of the quaternion
	     * @param[out] Angle angle of the quaternion
	     * @warning Assumes normalized quaternions.
	     */
        void getAxisAngle(Vector3t& axis, T& angle) const;

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
        static T dotProduct(Quat q1, Quat q2);

        /**
         * Get angle between quaternions q1 and q2
         *
         * @param q1
         * @param q2
         * @return
         */
        static T getAngle(Quat q1, Quat q2);

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

} // namespace Berserk

#endif //BERSERK_QUAT_H
