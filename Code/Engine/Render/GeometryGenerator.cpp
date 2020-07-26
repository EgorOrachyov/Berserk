/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GeometryGenerator.h>
#include <Math/Vec3f.h>
#include <Math/Vec2f.h>

namespace Berserk {
    namespace Render {

        void GeometryGenerator::generateSphere(float radius, int32 stepsH, int32 stepsV,
                                               ElementStream &position, IndexStream &indices) {
            generateSphere(radius, stepsH, stepsV, &position, nullptr, nullptr, indices);
        }

        void GeometryGenerator::generateSphere(float radius, int32 stepsH, int32 stepsV,
                                               ElementStream &position, ElementStream &normal,
                                               ElementStream &textureCoords, IndexStream &indices) {
            generateSphere(radius, stepsH, stepsV, &position, &normal, &textureCoords, indices);
        }
        
        void GeometryGenerator::generateSphere(float radius, int32 stepsH, int32 stepsV,
                                               ElementStream *position,
                                               ElementStream *normal,
                                               ElementStream *textureCoords,
                                               IndexStream &indices) {
            auto hasPos = position != nullptr;
            auto hasNorm = normal != nullptr;
            auto hasTexCoords = textureCoords != nullptr;

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

            uint32 verticesCount = totalV * totalH;
            uint32 indicesCount = stepsV * stepsH * 2 * 3;

            if (hasPos)
                position->ensureToAdd(verticesCount);
            if (hasNorm)
                normal->ensureToAdd(verticesCount);
            if (hasTexCoords)
                textureCoords->ensureToAdd(verticesCount);

            indices.ensureToAdd(indicesCount);

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
                        *position << Vec3f(x, y, z);
                    }

                    if (hasNorm) {
                        *normal << (Vec3f)(Vec3f(x, y, z).normalized());
                    }

                    if (hasTexCoords) {
                        *textureCoords << Vec2f(u, v);
                    }

                    u -= dU;
                }
            }

            for (uint32 i = 0; i < stepsV; i++) {
                for (uint32 j = 0; j < stepsH; j++) {
                    indices << (uint32)(i * totalH + j + 1);
                    indices << (uint32)(i * totalH + j + 0);
                    indices << (uint32)(i * totalH + j + totalH);

                    indices << (uint32)(i * totalH + j + totalH);
                    indices << (uint32)(i * totalH + j + totalH + 1);
                    indices << (uint32)(i * totalH + j + 1);
                }
            }
        }

    }
}