//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTORS_H
#define BERSERKENGINE_VECTORS_H

#include "../Essential/Types.h"

#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"

namespace Berserk
{
    /**
     * Get new vector (x,y)
     *
     * @param x
     * @param y
     * @return Vector(x,y)
     */
    Vector2f newVector(FLOAT32 x, FLOAT32 y);

    /**
     * Get new vector (x,y,z)
     *
     * @param x
     * @param y
     * @param z
     * @return Vector(x,y,z)
     */
    Vector3f newVector(FLOAT32 x, FLOAT32 y, FLOAT32 z);

    /**
     * Get new vector (x,y,z,w)
     *
     * @param x
     * @param y
     * @param z
     * @param w
     * @return Vector(x,y,z,w)
     */
    Vector4f newVector(FLOAT32 x, FLOAT32 y, FLOAT32 z, FLOAT32 w);

    /**
     * Get vectors' dot product
     *
     * @param v1
     * @param v2
     * @return
     */
    FLOAT32 dotProduct(Vector2f v1, Vector2f v2);

    /**
     * Get vectors' dot product
     *
     * @param v1
     * @param v2
     * @return
     */
    FLOAT32 dotProduct(Vector3f v1, Vector3f v2);

    /**
     * Get vectors' dot product
     *
     * @param v1
     * @param v2
     * @return
     */
    FLOAT32 dotProduct(Vector4f v1, Vector4f v2);

    /**
     * Get real vector length
     *
     * @param v
     * @return
     */
    FLOAT32 length(Vector2f v);

    /**
     * Get real vector length
     *
     * @param v
     * @return
     */
    FLOAT32 length(Vector3f v);

    /**
     * Get real vector length
     *
     * @param v
     * @return
     */
    FLOAT32 length(Vector4f v);

    /**
     * Get vector length without sqrt
     *
     * @param v
     * @return
     */
    FLOAT32 norm(Vector2f v);

    /**
     * Get vector length without sqrt
     *
     * @param v
     * @return
     */
    FLOAT32 norm(Vector3f v);

    /**
     * Get vector length without sqrt
     *
     * @param v
     * @return
     */
    FLOAT32 norm(Vector4f v);

    /**
     * Get vectors' cross product
     *
     * @param v1
     * @param v2
     * @return
     */
    Vector3f crossProduct(Vector3f v1, Vector3f v2);

    /**
     * Get vectors' triple product
     *
     * @param v1
     * @param v2
     * @param v3
     * @return
     */
    FLOAT32 tripleProduct(Vector3f v1, Vector3f v2, Vector3f v3);

    /**
     * Per element multiplication
     *
     * @param v1
     * @param v2
     * @return
     */
    Vector2f multByElement(Vector2f v1, Vector2f v2);

    /**
     * Per element multiplication
     *
     * @param v1
     * @param v2
     * @return
     */
    Vector3f multByElement(Vector3f v1, Vector3f v2);

    /**
     * Per element multiplication
     *
     * @param v1
     * @param v2
     * @return
     */
    Vector4f multByElement(Vector4f v1, Vector4f v2);

    /**
     * Transform vector to 1 length
     *
     * @param v
     * @return
     */
    Vector2f normalize(Vector2f v);

    /**
     * Transform vector to 1 length
     *
     * @param v
     * @return
     */
    Vector3f normalize(Vector3f v);

    /**
     * Transform vector to 1 length
     *
     * @param v
     * @return
     */
    Vector4f normalize(Vector4f v);

    /**
     * Linear interpolation from vector v1 to vector v2 by param t
     * @warning t should be in [0;1]
     *
     * @param v1
     * @param v2
     * @param t
     * @return
     */
    Vector2f lerp(Vector2f v1, Vector2f v2, FLOAT32 t);

    /**
     * Linear interpolation from vector v1 to vector v2 by param t
     * @warning t should be in [0;1]
     *
     * @param v1
     * @param v2
     * @param t
     * @return
     */
    Vector3f lerp(Vector3f v1, Vector3f v2, FLOAT32 t);

    /**
     * Linear interpolation from vector v1 to vector v2 by param t
     * @warning t should be in [0;1]
     *
     * @param v1
     * @param v2
     * @param t
     * @return
     */
    Vector4f lerp(Vector4f v1, Vector4f v2, FLOAT32 t);

    /**
     * Spherical linear interpolation from vector v1 to vector v2 by param t
     * @warning t should be in [0;1]
     *
     * @param v1
     * @param v2
     * @param t
     * @return
     */
    Vector2f slerp(Vector2f v1, Vector2f v2, FLOAT32 t);

    /**
     * Spherical linear interpolation from vector v1 to vector v2 by param t
     * @warning t should be in [0;1]
     * @warning angle between vectors should be more than 0
     *
     * @param v1
     * @param v2
     * @param t
     * @return
     */
    Vector3f slerp(Vector3f v1, Vector3f v2, FLOAT32 t);

    /**
     * Spherical linear interpolation from vector v1 to vector v2 by param t
     * @warning t should be in [0;1]
     * @warning angle between vectors should be more than 0
     *
     * @param v1
     * @param v2
     * @param t
     * @return
     */
    Vector4f slerp(Vector4f v1, Vector4f v2, FLOAT32 t);

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
    Vector2f slerp(Vector2f v1, Vector2f v2, FLOAT32 angle, FLOAT32 t);

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
    Vector3f slerp(Vector3f v1, Vector3f v2, FLOAT32 angle, FLOAT32 t);

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
    Vector4f slerp(Vector4f v1, Vector4f v2, FLOAT32 angle, FLOAT32 t);

    /**
     * Smooth interpolation of t between vectors v1 and v2
     * @see smoothstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector2f smoothstep(Vector2f v1, Vector2f v2, FLOAT32 t);

    /**
     * Smooth interpolation of t between vectors v1 and v2
     * @see smoothstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector3f smoothstep(Vector3f v1, Vector3f v2, FLOAT32 t);

    /**
     * Smooth interpolation of t between vectors v1 and v2
     * @see smoothstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector4f smoothstep(Vector4f v1, Vector4f v2, FLOAT32 t);

    /**
     * Smoother interpolation of t between vectors v1 and v2
     * @see smootherstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector2f smootherstep(Vector2f v1, Vector2f v2, FLOAT32 t);

    /**
     * Smoother interpolation of t between vectors v1 and v2
     * @see smootherstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector3f smootherstep(Vector3f v1, Vector3f v2, FLOAT32 t);

    /**
     * Smoother interpolation of t between vectors v1 and v2
     * @see smootherstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector4f smootherstep(Vector4f v1, Vector4f v2, FLOAT32 t);

} // namespace Berserk

#endif //BERSERKENGINE_VECTORS_H