//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTOR3D_H
#define BERSERKENGINE_VECTOR3D_H

#include "../Essential/Types.h"

namespace Berserk
{

    /**
     * 3 component vector
     */
    class Vector3d
    {
    public:

        /**
         * Initialize vector with 0 values
         */
        Vector3d();

        /**
         * Initialize by values
         *
         * @param x
         * @param y
         * @param z
         */
        Vector3d(float32 x, float32 y, float32 z);

        /**
         * Initialize by copying of another vector
         *
         * @param another
         */
        Vector3d(Vector3d& another);

        /**
         * Copy from vector
         *
         * @param another
         */
        void Copy(Vector3d another);

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
        void Reflect(Vector3d normalOfReflection);

        /**
         * Projects vector on projection axis
         *
         * @param projectionAxis
         */
        void Project(Vector3d projectionAxis);

        /**
         * Projects vector on projection axis (for axis of 1 length)
         *
         * @param projectionAxis
         */
        void ProjectByNormalized(Vector3d projectionAxis);

        /**
         * Set vector's values
         *
         * @param x
         * @param y
         * @param z
         */
        void Set(float32 x, float32 y, float32 z);

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

    public:

        float32 x;
        float32 y;
        float32 z;

    };

} // namespace Berserk

#endif //BERSERKENGINE_VECTOR3D_H