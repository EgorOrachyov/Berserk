/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/Aux/GeometryGenerator.h>
#include <Math/Vec3f.h>
#include <Math/Vec2f.h>

namespace Berserk {
    namespace Rendering {

        void GeometryGenerator::generateSphere(float radius, int32 stepsH, int32 stepsV, const VertexPolicy &policy,
                                               uint32 &vertsCount, TArray<uint8> &verts, TArray<uint32> &indices) {
            Vec3f pos;
            Vec3f norm;
            Vec2f texcoords;

            auto &vertexInput = policy.getInput();

            auto hasPos = vertexInput.hasPosition();
            auto hasNorm = vertexInput.hasNormal();
            auto hasTexCoords = vertexInput.hasTexCoords();

            auto posOffset = policy.getOffset(EVertexAttribute::Position);
            auto normOffset = policy.getOffset(EVertexAttribute::Normal);
            auto texcoordsOffset = policy.getOffset(EVertexAttribute::TexCoords);
            auto stride = policy.getStride(EVertexAttribute::Position);

            uint8 buffer[sizeof(float) * 100];

            radius = Math::max(radius, 0.01f);
            stepsH = Math::max(stepsH, 3);
            stepsV = Math::max(stepsV, 2);

            uint32 totalH = stepsH + 1;
            uint32 totalV = stepsV + 1;

            float dAngleH = Math::PIf * 2 / (float) stepsH;
            float dAngleV = Math::PIf / (float) stepsV;

            float dU = 1.0f / (float) stepsH;
            float dV = 1.0f / (float) stepsV;

            float HALF_PI = Math::HALF_PIf;

            vertsCount = totalV * totalH;
            verts.ensureToAdd(stride * vertsCount);
            indices.ensureToAdd(stepsV * stepsH * 2 * 3);

            for (uint32 i = 0; i < totalV; i++) {
                float alpha = dAngleV * (float) i - HALF_PI;
                float u = 1.0f;
                float v = 0.0f + dV * (float) i;

                for (uint32 j = 0; j < totalH; j++) {
                    float beta = dAngleH * (float) j;

                    float rproj = radius * Math::cos(alpha);
                    float x = rproj * Math::cos(beta);
                    float y = radius * Math::sin(alpha);
                    float z = rproj * Math::sin(beta);

                    if (hasPos) {
                        pos = Vec3f(x, y, z);
                        Memory::copy(buffer + posOffset, &pos, sizeof(pos));
                    }

                    if (hasNorm) {
                        norm = Vec3f(x, y, z).normalized();
                        Memory::copy(buffer + normOffset, &norm, sizeof(norm));
                    }

                    if (hasTexCoords) {
                        texcoords = Vec2f(u, v);
                        Memory::copy(buffer + texcoordsOffset, &texcoords, sizeof(texcoords));
                    }

                    verts.add(buffer, stride);

                    u -= dU;
                }
            }

            for (uint32 i = 0; i < stepsV; i++) {
                for (uint32 j = 0; j < stepsH; j++) {
                    indices.add(i * totalH + j + 1);
                    indices.add(i * totalH + j + 0);
                    indices.add(i * totalH + j + totalH);

                    indices.add(i * totalH + j + totalH);
                    indices.add(i * totalH + j + totalH + 1);
                    indices.add(i * totalH + j + 1);
                }
            }
        }

    }
}