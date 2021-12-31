/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <core/math/Geometry.hpp>
#include <core/math/TMatMxN.hpp>

BRK_NS_BEGIN

void Geometry::GenTangentSpace(const std::array<Vec3f, 3> &positions, const std::array<Vec3f, 3> &normals, const std::array<Vec2f, 3> &texCoords, std::array<Vec3f, 3> &tangents, std::array<Vec3f, 3> &bitangents) {
    auto &p0 = positions[0];
    auto &p1 = positions[1];
    auto &p2 = positions[2];

    auto &uv0 = texCoords[0];
    auto &uv1 = texCoords[1];
    auto &uv2 = texCoords[2];

    float du1 = uv1[0] - uv0[0];
    float du2 = uv2[0] - uv0[0];

    float dv1 = uv1[1] - uv0[1];
    float dv2 = uv2[1] - uv0[1];

    TVecN<float, 3> e1 = p1 - p0;
    TVecN<float, 3> e2 = p2 - p0;

    float det = TDetMxN<float, 2, 2>()(Mat2x2f(du1, dv1, du2, dv2));
    float invDet = (MathUtils::Abs(det) > MathUtils::SMALL_NUMBER_FLOAT32 ? 1.0f / det : -1.0f);

    Mat2x2f dUVI(invDet * dv2, -invDet * dv1,
                 -invDet * du2, invDet * du1);

    TMatMxN<float, 2, 3> E({e1, e2});
    TMatMxN<float, 2, 3> TB = dUVI * E;

    auto tg = TB.GetRow(0);
    auto btg = TB.GetRow(1);

    for (uint32 i = 0; i < 3; i++) {
        auto n = normals[i];
        auto ntg = (tg - n * Vec3f::Dot(tg, n)).Normalized();
        auto nbtg = (btg - n * Vec3f::Dot(btg, n) - tg * Vec3f::Dot(btg, tg) * tg).Normalized();

        tangents[i] = ntg;
        bitangents[i] = nbtg;
    }
}

BRK_NS_END