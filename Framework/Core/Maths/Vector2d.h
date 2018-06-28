//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTOR2D_H
#define BERSERKENGINE_VECTOR2D_H

#include "../Essential/Types.h"

namespace Berserk
{

    /**
     * 2 component vector
     */
    class Vector2d
    {
    public:

        /**
         * Initialize vector with 0 values
         */
        Vector2d();

        /**
         * Initialize by values
         *
         * @param x
         * @param y
         */
        Vector2d(float32 x, float32 y);

        /**
         * Initialize by copying of another vector
         *
         * @param another
         */
        Vector2d(Vector2d& another);

        /**
         * Copy from vector
         *
         * @param another
         */
        void Copy(Vector2d another);

        /**
         * Move length to 1
         */
        void Normalize();

        /**
         * Get -vector
         */
        void Inverse();

        /**
         * Reflects vector from the plane with normal of reflection
         *
         * @param normalOfReflection
         */
        void Reflect(Vector2d normalOfReflection);

        /**
         * Projects vector on projection axis
         *
         * @param projectionAxis
         */
        void Project(Vector2d projectionAxis);

        /**
         * Projects vector on projection axis (for axis of 1 length)
         *
         * @param projectionAxis
         */
        void ProjectByNormalized(Vector2d projectionAxis);

        /**
         * Set vector's values
         *
         * @param x
         * @param y
         */
        void Set(float32 x, float32 y);

        /**
         * Set value
         *
         * @param x
         */
        void SetX(float32 x);

        /**
         * Set value
         *
         * @param y
         */
        void SetY(float32 y);

        /**
         * Get vector's length
         *
         * @return Length
         */
        float32 GetLength();

        /**
         * Get vector's length without sqrt
         *
         * @return Length without sqrt
         */
        float32 GetQuadLength();

        /**
         * Get vector's value
         *
         * @return x
         */
        float32 GetX();

        /**
         * Get vector's value
         *
         * @return y
         */
        float32 GetY();

    public:

        float32 x;
        float32 y;

    };

} // namespace Berserk

#endif //BERSERKENGINE_VECTOR2D_H