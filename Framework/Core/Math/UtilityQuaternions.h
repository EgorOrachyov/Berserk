//
// Created by Egor Orachyov on 01.07.2018.
//

#ifndef BERSERKENGINE_UTILITYQUATERNIONS_H
#define BERSERKENGINE_UTILITYQUATERNIONS_H

#include "Quaternionf.h"
#include "Matrix4x4f.h"

namespace Berserk
{

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
    Quaternionf fromVector(Vector3f axis, FLOAT32 angle);

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
    Quaternionf fromEuler(FLOAT32 roll, FLOAT32 yaw, FLOAT32 pitch);

    /**
     * Build rotation quaternion from rotation matrix
     *
     * @param M
     * @return Rotation quaternion of 1 length
     */
    Quaternionf fromMatrix(Matrix4x4f M);

    /**
     * Creates rotation matrix from rotation quaternion
     *
     * @warning Quaternion should be of 1 length
     *
     * @param q Rotation quaternion
     * @return Rotation 4x4 matrix
     */
    Matrix4x4f  fromQuaternion(Quaternionf q);

    /**
     * Dot product for q1 and q2 quaternions similar to vectors dot product
     *
     * @param q1
     * @param q2
     * @return
     */
    FLOAT32 dotProduct(Quaternionf q1, Quaternionf q2);

    /**
     * Get angle between quaternions q1 and q2
     *
     * @param q1
     * @param q2
     * @return
     */
    FLOAT32 getAngle(Quaternionf q1, Quaternionf q2);

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
    Vector3f rotate(Quaternionf q, Vector3f v);

    /**
     * Rotate vector v (or point with w=1) by rotation quaternion q of 1 length
     *
     * @warning Quaternion should be of 1 length
     *
     * @param q Rotation quaternion
     * @param v Vector to be rotated
     * @return Rotated vector (or point with w=1)
     */
    Vector4f rotate(Quaternionf q, Vector4f v);

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
    Quaternionf lerp(Quaternionf q1, Quaternionf q2, FLOAT32 t);

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
    Quaternionf slerp(Quaternionf q1, Quaternionf q2, FLOAT32 t);

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
    Quaternionf slerp(Quaternionf q1, Quaternionf q2, FLOAT32 angle, FLOAT32 t);

} // namespace Berserk

#endif //BERSERKENGINE_UTILITYQUATERNIONS_H
