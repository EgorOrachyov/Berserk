//
// Created by Egor Orachyov on 01.07.2018.
//

#ifndef BERSERKENGINE_UTILITYQUATERNIONS_H
#define BERSERKENGINE_UTILITYQUATERNIONS_H

#include "Quaternion.h"
#include "Matrix4x4.h"

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
    Quaternion fromVector(Vector3 axis, float32 angle);

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
    Quaternion fromEuler(float32 roll, float32 yaw, float32 pitch);

    /**
     * Build rotation quaternion from rotation matrix
     *
     * @param M
     * @return Rotation quaternion of 1 length
     */
    Quaternion fromMatrix(Matrix4x4 M);

    /**
     * Creates rotation matrix from rotation quaternion
     *
     * @warning Quaternion should be of 1 length
     *
     * @param q Rotation quaternion
     * @return Rotation 4x4 matrix
     */
    Matrix4x4  fromQuaternion(Quaternion q);

    /**
     * Dot product for q1 and q2 quaternions similar to vectors dot product
     *
     * @param q1
     * @param q2
     * @return
     */
    float32 dotProduct(Quaternion q1, Quaternion q2);

    /**
     * Get angle between quaternions q1 and q2
     *
     * @param q1
     * @param q2
     * @return
     */
    float32 getAngle(Quaternion q1, Quaternion q2);

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
    Vector3 rotate(Quaternion q, Vector3 v);

    /**
     * Rotate vector v (or point with w=1) by rotation quaternion q of 1 length
     *
     * @warning Quaternion should be of 1 length
     *
     * @param q Rotation quaternion
     * @param v Vector to be rotated
     * @return Rotated vector (or point with w=1)
     */
    Vector4 rotate(Quaternion q, Vector4 v);

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
    Quaternion lerp(Quaternion q1, Quaternion q2, float32 t);

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
    Quaternion slerp(Quaternion q1, Quaternion q2, float32 t);

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
    Quaternion slerp(Quaternion q1, Quaternion q2, float32 angle, float32 t);

} // namespace Berserk

#endif //BERSERKENGINE_UTILITYQUATERNIONS_H
