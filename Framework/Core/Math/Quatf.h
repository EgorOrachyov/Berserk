//
// Created by Egor Orachyov on 01.07.2018.
//

#ifndef BERSERKENGINE_QUATERNION_H
#define BERSERKENGINE_QUATERNION_H

#include "Essential/Types.h"
#include "Essential/UsageDescriptors.h"
#include "Math/MathUtility.h"

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
    struct CORE_EXPORT Quatf
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
        explicit Quatf(FLOAT32 s, Vector3f v);

        /**
         * Initialize quaternion by element s
         *
         * @param s Scalar part
         * @param x Vector x component
         * @param y Vector y component
         * @param z Vector z component
         */
        Quatf(FLOAT32 s, FLOAT32 x, FLOAT32 y, FLOAT32 z);

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
        Quatf(Vector3f axis, FLOAT32 angle);

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
        explicit Quatf(FLOAT32 roll, FLOAT32 yaw, FLOAT32 pitch);

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
        explicit Quatf(const Vector3f& v);

        /**
         * Build rotation quaternion of 1 lenght from rotation matrix
         *
         * @param M Transformation matrix
         */
        explicit Quatf(const Matrix4x4f& M);

        /**
	     * Creates and initializes a new quaternion from the given rotator.
	     *
	     * @param R The rotator to initialize from.
	     */
        explicit Quatf(const Rotator& R);

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
        FLOAT32 getNorm() const;

        /**
         * Get true quaternion length
         *
         * @return Length
         */
        FLOAT32 getLength() const;

        /**
         * Get s scalar value
         *
         * @return Scalar part
         */
        FLOAT32 getScalar() const;

        /**
         * Get vector value
         *
         * @return Vector part
         */
        Vector3f getVector3() const;

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
        Quatf operator * (const FLOAT32 a) const;

        /**
         * Per elements division by value a
         *
         * @param a
         * @return
         */
        Quatf operator / (const FLOAT32 a) const;

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
        Vector3f rotate(const Vector3f& V) const;

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
        Vector3f unrotate(const Vector3f& v) const;

        /**
         * Get the forward direction (X axis) after it
         * has been rotated by this Quaternion.
         */
        Vector3f getAxisX() const;

        /**
         * Get the right direction (Y axis) after it
         * has been rotated by this Quaternion.
         */
        Vector3f getAxisY() const;

        /**
         * Get the up direction (Z axis) after it
         * has been rotated by this Quaternion.
         */
        Vector3f getAxisZ() const;

        /**
	     * Get the axis and angle of rotation of this quaternion
	     *
	     * @param[out] Axis  vector of axis of the quaternion
	     * @param[out] Angle angle of the quaternion
	     * @warning Assumes normalized quaternions.
	     */
        void getAxisAngle(Vector3f& axis, FLOAT32& angle) const;

        /**
	     * Get a textual representation of the quaternion.
	     *
	     * @return Text describing the quaternion.
	     */
        CStaticString toString() const;

    public:

        /**
         * Dot product for q1 and q2 quaternions similar to vectors dot product
         *
         * @param q1
         * @param q2
         * @return
         */
        static FLOAT32 dotProduct(Quatf q1, Quatf q2);

        /**
         * Get angle between quaternions q1 and q2
         *
         * @param q1
         * @param q2
         * @return
         */
        static FLOAT32 getAngle(Quatf q1, Quatf q2);

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
        static Quatf lerp(Quatf q1, Quatf q2, FLOAT32 t);

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
        static Quatf slerp(Quatf q1, Quatf q2, FLOAT32 t);

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
        static Quatf slerp(Quatf q1, Quatf q2, FLOAT32 angle, FLOAT32 t);

    public:

        FLOAT32 s;
        FLOAT32 x;
        FLOAT32 y;
        FLOAT32 z;

    };

} // namespace Berserk

#endif //BERSERKENGINE_QUATERNION_H