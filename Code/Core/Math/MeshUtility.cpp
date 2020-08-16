/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Math/Mat2x2f.h>
#include <Math/MeshUtility.h>

namespace Berserk {

    void MeshUtility::generateTangentSpace(const TArrayStatic<Vec3f,3> &positions, const TArrayStatic<Vec3f,3> &normals, const TArrayStatic<Vec2f,3> &texCoords, TArrayStatic<Vec3f,3>& tangents, TArrayStatic<Vec3f,3>& bitangents) {
        auto& p0 = positions[0];
        auto& p1 = positions[1];
        auto& p2 = positions[2];

        auto& uv0 = texCoords[0];
        auto& uv1 = texCoords[1];
        auto& uv2 = texCoords[2];

        float du1 = uv1[0] - uv0[0];
        float du2 = uv2[0] - uv0[0];

        float dv1 = uv1[1] - uv0[1];
        float dv2 = uv2[1] - uv0[1];

        TVecN<float,3> e1 = p1 - p0;
        TVecN<float,3> e2 = p2 - p0;

        float det = Mat2x2f(du1, dv1, du2, dv2).det();
        float invDet = (det > Math::SMALL_NUMBER_FLOAT32 ? 1.0f / det: -1.0f);

        Mat2x2f dUVI(invDet * dv2, -invDet * dv1,
                     -invDet * du2, invDet * du1);

        TMatMxN<float,2,3> E = TArrayStatic<TVecN<float,3>,2>{ e1, e2 };
        TMatMxN<float,2,3> TB = dUVI * E;

        auto tg = TB.getRow(0);
        auto btg = TB.getRow(1);

        tangents.clear();
        bitangents.clear();

        for (auto& n: normals) {
            auto ntg = (tg - n * Vec3f::dot(tg, n)).normalized();
            auto nbtg = (btg - n * Vec3f::dot(btg, n) - tg * Vec3f::dot(btg,tg) * tg).normalized();

            tangents.add(ntg);
            bitangents.add(nbtg);
        }
    }

}