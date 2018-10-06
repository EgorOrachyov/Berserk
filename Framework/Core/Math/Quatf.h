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
        explicit Quatf(FLOAT32 s, FLOAT32 x, FLOAT32 y, FLOAT32 z);

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
        explicit Quatf(Vector3f axis, FLOAT32 angle);

        /**
         * Build rotation quaternion from Euler angles
         *
         * @note Angles should be in radians
         * @note watch param description to exclude misunderstanding because of
         * relating (roll, yaw, pitch) and rotation axises (oX,oY,oZ)
         *
         * @param roll  oX Clockwise rotation angle
         * @param yaw   oY Clockwise rotation angle
         * @param pitch oZ Clockwise rotation angle
         * @return Rotation quaternion of 1 length
         */
        explicit Quatf(FLOAT32 roll, FLOAT32 yaw, FLOAT32 pitch);

        /**
         * Build rotation quaternion from rotation matrix
         *
         * @param M Transformation matrix
         * @return Rotation quaternion of 1 length
         */
        explicit Quatf(const Matrix4x4f& M);

    public:

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

        FLOAT32 s;
        FLOAT32 x;
        FLOAT32 y;
        FLOAT32 z;

    };

} // namespace Berserk

#endif //BERSERKENGINE_QUATERNION_H