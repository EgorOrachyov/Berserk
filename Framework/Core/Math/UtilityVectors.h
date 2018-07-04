//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTORS_H
#define BERSERKENGINE_VECTORS_H

#include "../Essential/Types.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Berserk
{
    /**
     * Get new vector (x,y)
     *
     * @param x
     * @param y
     * @return Vector(x,y)
     */
    Vector2 newVector(float32 x, float32 y);

    /**
     * Get new vector (x,y,z)
     *
     * @param x
     * @param y
     * @param z
     * @return Vector(x,y,z)
     */
    Vector3 newVector(float32 x, float32 y, float32 z);

    /**
     * Get new vector (x,y,z,w)
     *
     * @param x
     * @param y
     * @param z
     * @param w
     * @return Vector(x,y,z,w)
     */
    Vector4 newVector(float32 x, float32 y, float32 z, float32 w);

    /**
     * Get vectors' dot product
     *
     * @param v1
     * @param v2
     * @return
     */
    float32 dotProduct(Vector2 v1, Vector2 v2);

    /**
     * Get vectors' dot product
     *
     * @param v1
     * @param v2
     * @return
     */
    float32 dotProduct(Vector3 v1, Vector3 v2);

    /**
     * Get vectors' dot product
     *
     * @param v1
     * @param v2
     * @return
     */
    float32 dotProduct(Vector4 v1, Vector4 v2);

    /**
     * Get real vector length
     *
     * @param v
     * @return
     */
    float32 length(Vector2 v);

    /**
     * Get real vector length
     *
     * @param v
     * @return
     */
    float32 length(Vector3 v);

    /**
     * Get real vector length
     *
     * @param v
     * @return
     */
    float32 length(Vector4 v);

    /**
     * Get vector length without sqrt
     *
     * @param v
     * @return
     */
    float32 norm(Vector2 v);

    /**
     * Get vector length without sqrt
     *
     * @param v
     * @return
     */
    float32 norm(Vector3 v);

    /**
     * Get vector length without sqrt
     *
     * @param v
     * @return
     */
    float32 norm(Vector4 v);

    /**
     * Get vectors' cross product
     *
     * @param v1
     * @param v2
     * @return
     */
    Vector3 crossProduct(Vector3 v1, Vector3 v2);

    /**
     * Get vectors' triple product
     *
     * @param v1
     * @param v2
     * @param v3
     * @return
     */
    float32 tripleProduct(Vector3 v1, Vector3 v2, Vector3 v3);

    /**
     * Per element multiplication
     *
     * @param v1
     * @param v2
     * @return
     */
    Vector2 multByElement(Vector2 v1, Vector2 v2);

    /**
     * Per element multiplication
     *
     * @param v1
     * @param v2
     * @return
     */
    Vector3 multByElement(Vector3 v1, Vector3 v2);

    /**
     * Per element multiplication
     *
     * @param v1
     * @param v2
     * @return
     */
    Vector4 multByElement(Vector4 v1, Vector4 v2);

    /**
     * Transform vector to 1 length
     *
     * @param v
     * @return
     */
    Vector2 normalize(Vector2 v);

    /**
     * Transform vector to 1 length
     *
     * @param v
     * @return
     */
    Vector3 normalize(Vector3 v);

    /**
     * Transform vector to 1 length
     *
     * @param v
     * @return
     */
    Vector4 normalize(Vector4 v);

    /**
     * Linear interpolation from vector v1 to vector v2 by param t
     * @warning t should be in [0;1]
     *
     * @param v1
     * @param v2
     * @param t
     * @return
     */
    Vector2 lerp(Vector2 v1, Vector2 v2, float32 t);

    /**
     * Linear interpolation from vector v1 to vector v2 by param t
     * @warning t should be in [0;1]
     *
     * @param v1
     * @param v2
     * @param t
     * @return
     */
    Vector3 lerp(Vector3 v1, Vector3 v2, float32 t);

    /**
     * Linear interpolation from vector v1 to vector v2 by param t
     * @warning t should be in [0;1]
     *
     * @param v1
     * @param v2
     * @param t
     * @return
     */
    Vector4 lerp(Vector4 v1, Vector4 v2, float32 t);

    /**
     * Spherical linear interpolation from vector v1 to vector v2 by param t
     * @warning t should be in [0;1]
     *
     * @param v1
     * @param v2
     * @param t
     * @return
     */
    Vector2 slerp(Vector2 v1, Vector2 v2, float32 t);

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
    Vector3 slerp(Vector3 v1, Vector3 v2, float32 t);

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
    Vector4 slerp(Vector4 v1, Vector4 v2, float32 t);

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
    Vector2 slerp(Vector2 v1, Vector2 v2, float32 angle, float32 t);

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
    Vector3 slerp(Vector3 v1, Vector3 v2, float32 angle, float32 t);

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
    Vector4 slerp(Vector4 v1, Vector4 v2, float32 angle, float32 t);

    /**
     * Smooth interpolation of t between vectors v1 and v2
     * @see smoothstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector2 smoothstep(Vector2 v1, Vector2 v2, float32 t);

    /**
     * Smooth interpolation of t between vectors v1 and v2
     * @see smoothstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector3 smoothstep(Vector3 v1, Vector3 v2, float32 t);

    /**
     * Smooth interpolation of t between vectors v1 and v2
     * @see smoothstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector4 smoothstep(Vector4 v1, Vector4 v2, float32 t);

    /**
     * Smoother interpolation of t between vectors v1 and v2
     * @see smootherstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector2 smootherstep(Vector2 v1, Vector2 v2, float32 t);

    /**
     * Smoother interpolation of t between vectors v1 and v2
     * @see smootherstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector3 smootherstep(Vector3 v1, Vector3 v2, float32 t);

    /**
     * Smoother interpolation of t between vectors v1 and v2
     * @see smootherstep for float64
     *
     * @param v1 Lower limit
     * @param v2 Upper limit
     * @param t Value for interpolation
     * @return Interpolated vector
     */
    Vector4 smootherstep(Vector4 v1, Vector4 v2, float32 t);

} // namespace Berserk

#endif //BERSERKENGINE_VECTORS_H