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
    namespace VectorUtility
    {

        /**
         * Get vectors' dot product
         *
         * @param v1
         * @param v2
         * @return
         */
        FLOAT32 dot(Vector2f v1, Vector2f v2);

        /**
         * Get vectors' dot product
         *
         * @param v1
         * @param v2
         * @return
         */
        FLOAT32 dot(Vector3f v1, Vector3f v2);

        /**
         * Get vectors' dot product
         *
         * @param v1
         * @param v2
         * @return
         */
        FLOAT32 dot(Vector4f v1, Vector4f v2);

        /**
         * Get vectors' cross product
         *
         * @param v1
         * @param v2
         * @return
         */
        Vector3f cross(Vector3f v1, Vector3f v2);

        /**
         * Get vectors' triple product
         *
         * @param v1
         * @param v2
         * @param v3
         * @return
         */
        FLOAT32 triple(Vector3f v1, Vector3f v2, Vector3f v3);

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

    } // namespace VectorUtility

} // namespace Berserk

#endif //BERSERKENGINE_VECTORS_H