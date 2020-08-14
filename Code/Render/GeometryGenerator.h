/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GEOMETRYGENERATOR_H
#define BERSERK_GEOMETRYGENERATOR_H

#include <VertexStreams.h>

namespace Berserk {
    namespace Render {

        /** Geometry generator for common 3D primitives */
        class GeometryGenerator {
        public:

            /**
             * Generate 3d sphere with center in (0,0,0) of radius composed from indexed triangles
             *
             * @param radius Sphere radius
             * @param stepsH Number of fragments in the horizontal (oXZ) dimension
             * @param stepsV Number of fragments in the vertical (oY) dimension
             * @param position Stream to add vec3 world position
             * @param indices Stream to append indices in uint32 format
             */
            static void generateSphere(float radius, int32 stepsH, int32 stepsV, ElementStream& position, IndexStream& indices);

            /**
             * Generate 3d sphere with center in (0,0,0) of radius composed from indexed triangles
             *
             * @param radius Sphere radius
             * @param stepsH Number of fragments in the horizontal (oXZ) dimension
             * @param stepsV Number of fragments in the vertical (oY) dimension
             * @param position Stream to add vec3 world position
             * @param textureCoords Stream to add vec2 texture coordinates
             * @param indices Stream to append indices in uint32 format
             */
            static void generateSphere(float radius, int32 stepsH, int32 stepsV, ElementStream& position, ElementStream& textureCoords, IndexStream& indices);

            /**
             * Generate 3d sphere with center in (0,0,0) of radius composed from indexed triangles
             *
             * @param radius Sphere radius
             * @param stepsH Number of fragments in the horizontal (oXZ) dimension
             * @param stepsV Number of fragments in the vertical (oY) dimension
             * @param position Stream to add vec3 world position
             * @param normal Stream to add vec3 normal vectors
             * @param textureCoords Stream to add vec2 texture coordinates
             * @param indices Stream to append indices in uint32 format
             */
            static void generateSphere(float radius, int32 stepsH, int32 stepsV, ElementStream& position, ElementStream& normal, ElementStream& textureCoords, IndexStream& indices);

        private:

            static void generateSphere(float radius, int32 stepsH, int32 stepsV, ElementStream* position, ElementStream* normal, ElementStream* textureCoords, IndexStream& indices);

        };

    }
}

#endif //BERSERK_GEOMETRYGENERATOR_H