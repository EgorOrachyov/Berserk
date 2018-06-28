//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTOR4D_H
#define BERSERKENGINE_VECTOR4D_H

#include "../Essential/Types.h"

namespace Berserk
{

    /**
     * 4 component vector
     */
    class Vector4d
    {
    public:

        /**
         * Initialize vector with 0 values
         */
        Vector4d();

        /**
         * Initialize by values
         *
         * @param x
         * @param y
         * @param z
         * @param w
         */
        Vector4d(float32 x, float32 y, float32 z, float32 w);

        /**
         * Initialize by copying of another vector
         *
         * @param another
         */
        Vector4d(Vector4d& another);

        /**
         * Copy from vector
         *
         * @param another
         */
        void Copy(Vector4d another);

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
        void Reflect(Vector4d normalOfReflection);

        /**
         * Projects vector on projection axis
         *
         * @param projectionAxis
         */
        void Project(Vector4d projectionAxis);

        /**
         * Projects vector on projection axis (for axis of 1 length)
         *
         * @param projectionAxis
         */
        void ProjectByNormalized(Vector4d projectionAxis);

        /**
         * Set vector's values
         *
         * @param x
         * @param y
         * @param z
         * @param z
         */
        void Set(float32 x, float32 y, float32 z, float32 z);

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
         * Set value
         *
         * @param z
         */
        void SetZ(float32 z);

        /**
         * Set value
         *
         * @param w
         */
        void SetW(float32 w);

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

        /**
         * Get vector's value
         *
         * @return z
         */
        float32 GetZ();
        /**
         * Get vector's value
         *
         * @return w
         */
        float32 GetW();

    public:

        float32 x;
        float32 y;
        float32 z;
        float32 w;

    };

} // namespace Berserk

#endif //BERSERKENGINE_VECTOR4D_H