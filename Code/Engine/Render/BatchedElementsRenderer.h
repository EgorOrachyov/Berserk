/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_BATCHEDELEMENTSRENDERER_H
#define BERSERK_BATCHEDELEMENTSRENDERER_H

#include <BatchedElements.h>
#include <ShaderCore/Shader.h>
#include <RenderResources/VertexArray.h>
#include <ViewData.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Bathed elements renderer
         *
         * Accepts batched elements container, view settings, allocates required GPU
         * resources, such as shaders and buffers, and performs rendering of the elements.
         */
        class BatchedElementsRenderer {
        public:

            /** Initializes render internal pipeline and buffers */
            BatchedElementsRenderer();

            void draw(const ViewData& viewData, const BatchedElements& batch, RHIDrawList& drawList);

        private:

            /** Initializes render internal pipeline and buffers */
            void initializePipeline();
            void initializeSpheresRendering();
            /** Packs data */
            void prepareData(const BatchedElements& elements);

            struct SpherePack {
                float worldPos[3];
                float color[3];
                float radius;

                SpherePack() = default;

                SpherePack(const Vec3f& p, const LinearColor& c, float r) {
                    worldPos[0] = p[0];
                    worldPos[1] = p[1];
                    worldPos[2] = p[2];
                    color[0] = c.getR();
                    color[1] = c.getG();
                    color[2] = c.getB();
                    radius = r;
                }
            };

            struct Sphere {
                uint32 verticesCount = 0;
                uint32 indicesCount = 0;
                uint32 instancesCount = 0;
                uint32 maxInstances = 0;
                TPtrShared<RHIVertexBuffer> instances;
                TPtrShared<RHIVertexBuffer> vertices;
                TPtrShared<RHIIndexBuffer> indices;
                TPtrShared<RHIArrayObject> array;
                TPtrShared<Shader> shader;
                TPtrShared<ShaderBindings> bindings;
                TArray<SpherePack> instancesData;
            } mSpheres;

        };

    }
}

#endif //BERSERK_BATCHEDELEMENTSRENDERER_H