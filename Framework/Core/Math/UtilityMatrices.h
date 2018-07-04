//
// Created by Egor Orachyov on 30.06.2018.
//

#ifndef BERSERKENGINE_MATRICES_H
#define BERSERKENGINE_MATRICES_H

#include "Matrix2x2f.h"
#include "Matrix3x3f.h"
#include "Matrix4x4f.h"

namespace Berserk
{

    /**
     * Initialize matrix with d element on the main diagonal
     *
     * @param d
     * @return
     */
    Matrix4x4f newMatrix(float32 d);

    /**
     * Initialize matrix via vectors as its columns
     *
     * @param c1
     * @param c2
     * @param c3
     * @param c4
     * @return
     */
    Matrix4x4f newMatrix(Vector4f c1, Vector4f c2, Vector4f c3, Vector4f c4);

    /**
     * Scale matrix for x, y, z axises
     *
     * @param sX
     * @param sY
     * @param sZ
     * @return
     */
    Matrix4x4f scale(float32 sX, float32 sY, float32 sZ);

    /**
     * Translation matrix for t vector
     *
     * @param t
     * @return
     */
    Matrix4x4f translate(Vector3f t);

    /**
     * Clockwise rotation around the x-axis
     *
     * @param angle
     * @return
     */
    Matrix4x4f rotateX(float32 angle);

    /**
     * Clockwise rotation around the y-axis
     *
     * @param angle
     * @return
     */
    Matrix4x4f rotateY(float32 angle);

    /**
     * Clockwise rotation around the z-axis
     *
     * @param angle
     * @return
     */
    Matrix4x4f rotateZ(float32 angle);

    /**
     * Clockwise rotation around the axis
     *
     * @param angle
     * @return
     */
    Matrix4x4f rotate(Vector3f axis, float32 angle);

    /**
     * Look at view matrix
     *
     * @param eye Current viewer position
     * @param target Point of viewing
     * @param up Up vector to define orientation
     * @return
     */
    Matrix4x4f lookAt(Vector3f eye, Vector3f target, Vector3f up);

    /**
     * Perspective projection for OpenGL
     *
     * @warning Fovy should be me more than 0
     * @warning Aspect should be more than 0
     *
     * @param fovy Angle between left and right sides in radians
     * @param aspect Width-to-height ratio
     * @param near Near clip plane
     * @param far Far clip plane
     * @return
     */
    Matrix4x4f perspective(float32 fovy, float32 aspect, float32 near, float32 far);

    /**
     * Orthographic projection for OpenGL
     *
     * @warning Left != rigth
     * @warning Bottom != top
     * @warning Near != far
     *
     * @param left
     * @param right
     * @param bottom
     * @param top
     * @param near
     * @param far
     * @return
     */
    Matrix4x4f orthographic(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far);

} // namespace Berserk

#endif //BERSERKENGINE_MATRICES_H
