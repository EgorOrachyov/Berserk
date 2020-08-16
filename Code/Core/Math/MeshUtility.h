/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MESHUTILITY_H
#define BERSERK_MESHUTILITY_H

#include <Math/Vec2f.h>
#include <Math/Vec3f.h>
#include <Containers/TArrayStatic.h>

namespace Berserk {

    /** Mesh processing functions */
    class MeshUtility {
    public:

        /**
         * Generates tangent vectors from provided vertex data
         *
         * @note cross(tangent,bitangent) = normal
         * @note cross(normal,tangent) = bitangent
         *
         * @param positions Counterclockwise vertices positions
         * @param normals Counterclockwise vertices positions
         * @param texCoords Counterclockwise vertices texture coords
         * @param tangents Output tangents vector
         * @param bitangents Output bitangents vector
         */
        static void generateTangentSpace(const TArrayStatic<Vec3f,3> &positions, const TArrayStatic<Vec3f,3> &normals, const TArrayStatic<Vec2f,3> &texCoords, TArrayStatic<Vec3f,3>& tangents, TArrayStatic<Vec3f,3>& bitangents);

    };

}

#endif //BERSERK_MESHUTILITY_H