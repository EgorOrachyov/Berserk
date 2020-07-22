/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GEOMETRYGENERATOR_H
#define BERSERK_GEOMETRYGENERATOR_H

#include <VertexPolicy.h>

namespace Berserk {
    namespace Rendering {

        /** Geometry generator for common 3D primitives */
        class GeometryGenerator {
        public:

            //void generateScreenPlane();

            //void generateBox();

            /**
             * Generate 3d sphere with center in (0,0,0) of radius composed from indexed triangles
             *
             * @note Supported primitives:
             *  - Position
             *  - Normal
             *  - Texture coords
             *
             * @param radius Sphere radius
             * @param stepsH Number of fragments in the horizontal (oXZ) dimension
             * @param stepsV Number of fragments in the vertical (oY) dimension
             * @param policy Vertices policy to pack data
             * @param[out] vertsCount Number vertices
             * @param[out] verts Vertices raw packed by stride data
             * @param[out] indices Indices
             */
            static void generateSphere(float radius, int32 stepsH, int32 stepsV, const VertexPolicy &policy, uint32 &vertsCount, TArray <uint8> &verts, TArray <uint32> &indices);

        };

    }
}

#endif //BERSERK_GEOMETRYGENERATOR_H