//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_QUAT_H
#define BERSERK_QUAT_H

#include "Math/Vec3f.h"
#include "HAL/Types.h"
#include "Misc/Assert.h"
#include "Math/MathUtility.h"
#include "Misc/UsageDescriptors.h"

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
     */
    class CORE_EXPORT Quatf
    {
    public:

        /**
         * Initialize quaternion with 0 values
         */
        Quatf();

        /**
         * Initialize quaternion via scalar value s and vector part v
         *
         * @param s Scalar
         * @param v Vector XYZ components
         */
        Quatf(float32 s, const Vec3f& v);

        /**
         * Initialize quaternion by element s
         *
         * @param s Scalar part
         * @param x Vector x component
         * @param y Vector y component
         * @param z Vector z component
         */
        Quatf(float32 s, float32 x, float32 y, float32 z);

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
        Quatf(const Vec3f& axis, float32 angle);

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
        explicit Quatf(float32 roll, float32 yaw, float32 pitch);

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
        explicit Quatf(const Vec3f& v);

        /**
         * Build rotation quaternion of 1 lenght from rotation matrix
         *
         * @param M Transformation matrix
         */
        explicit Quatf(const Mat4x4f& M);

        /**
	     * Creates and initializes a new quaternion from the given rotator.
	     *
	     * @param R The rotator to initialize from.
	     */
        explicit Quatf(const Rotation& R);

        /**
         * Default copy constructor for quaternion
         * @param q Source quaternion
         */
        Quatf(const Quatf& q) = default;

    public:

        /**
         * Get normalized quaternion without changing of this one
         * @return That normalized quaternion
         */
        Quatf getNormalized() const;

        /**
         * Convert this quaternion to 1 length and return itself
         * @return That normalized quaternion
         */
        Quatf normalize();

        /**
         * Get inverse quaternion
         *
         * @return Inverse quaternion to that
         */
        Quatf inverse() const;

        /**
         * Get conjugate quaternion
         *
         * @return Conjugate quaternion to that
         */
        Quatf conjugate() const;

        /**
         * Get quaternion length without sqrt
         *
         * @return Norm
         */
        float32 getNorm() const;

        /**
         * Get true quaternion length
         *
         * @return Length
         */
        float32 getLength() const;

        /**
         * Get s scalar value
         *
         * @return Scalar part
         */
        float32 getScalar() const;

        /**
         * Get vector value
         *
         * @return Vector part
         */
        Vec3f getVector3() const;

        /**
         * Operator assignment
         *
         * @param q
         * @return
         */
        Quatf operator = (const Quatf& q);

        /**
         * Per elements addition
         *
         * @param q
         * @return
         */
        Quatf operator + (const Quatf& q) const;

        /**
         * Per elements subtraction
         *
         * @param q
         * @return
         */
        Quatf operator - (const Quatf& q) const;

        /**
         * True quaternion multiplication
         *
         * @param q
         * @return
         */
        Quatf operator * (const Quatf& q) const;

        /**
         * Per elements multiplication by value a
         *
         * @param a
         * @return
         */
        Quatf operator * (float32 a) const;

        /**
         * Per elements division by value a
         *
         * @param a
         * @return
         */
        Quatf operator / (float32 a) const;

        /**
         * Per elements comparison
         *
         * @param q
         * @return
         */
        const bool operator == (const Quatf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator >= (const Quatf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator <= (const Quatf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator > (const Quatf& q) const;

        /**
         * Comparison via quaternions' norm
         *
         * @param q
         * @return
         */
        const bool operator < (const Quatf& q) const;

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
        Vec3f rotate(const Vec3f &v) const;

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
        Vec4f rotate(const Vec4f &v) const;

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
        Vec3f unrotate(const Vec3f& v) const;

        /**
         * Get the forward direction (X axis) after it
         * has been rotated by this Quaternion.
         */
        Vec3f getAxisX() const;

        /**
         * Get the right direction (Y axis) after it
         * has been rotated by this Quaternion.
         */
        Vec3f getAxisY() const;

        /**
         * Get the up direction (Z axis) after it
         * has been rotated by this Quaternion.
         */
        Vec3f getAxisZ() const;

        /**
         * Creates rotation matrix from rotation quaternion
         *
         * @warning Quaternion should be of 1 length
         * @return Rotation 4x4 matrix
         */
        Mat4x4f getMatrix() const;

        /**
	     * Get the axis and angle of rotation of this quaternion
	     *
	     * @param[out] Axis  vector of axis of the quaternion
	     * @param[out] Angle angle of the quaternion
	     * @warning Assumes normalized quaternions.
	     */
        void getAxisAngle(Vec3f& axis, float32& angle) const;

        /**
	     * Get a textual representation of the quaternion.
	     *
	     * @return Text describing the quaternion.
	     */
        Name toString() const;

        /**
         * Convert to string
         * @param value To convert
         * @return String representation of vector
         */
        friend StringDynamic toString(const Quatf& q);

    public:

        /**
         * Dot product for q1 and q2 quaternions similar to vectors dot product
         *
         * @param q1
         * @param q2
         * @return
         */
        static float32 dot(Quatf q1, Quatf q2);

        /**
         * Get angle between quaternions q1 and q2
         *
         * @param q1
         * @param q2
         * @return
         */
        static float32 angle(Quatf q1, Quatf q2);

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
        Vec3f rotate(Quatf q, Vec3f v);

        /**
         * Rotate vector v (or point with w=1) by rotation quaternion q of 1 length
         *
         * @warning Quaternion should be of 1 length
         *
         * @param q Rotation quaternion
         * @param v Vector to be rotated
         * @return Rotated vector (or point with w=1)
         */
        Vec4f rotate(Quatf q, Vec4f v);

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
        static Quatf lerp(Quatf q1, Quatf q2, float32 t);

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
        static Quatf slerp(Quatf q1, Quatf q2, float32 t);

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
        static Quatf slerp(Quatf q1, Quatf q2, float32 angle, float32 t);

    public:

        float32 s;
        float32 x;
        float32 y;
        float32 z;

    };
    
} // namespace Berserk

#endif //BERSERK_QUAT_H
