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
#include <Shader.h>
#include <Scene/ViewData.h>
#include <RenderResources/UniformBuffer.h>
#include <RenderResources/DynamicVertexBuffer.h>
#include <RenderResources/DynamicIndexBuffer.h>

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
            BatchedElementsRenderer(BatchedElements& elements, bool depthTest = true);

            /**
             * Draws batched elements set to the specified draw list
             * @param viewData View settings for rendering
             * @param drawList Draw list to draw
             */
            void draw(const ViewData& viewData, RHIDrawList& drawList);

        private:

            static const uint32 SPHERE_H = 8;
            static const uint32 SPHERE_V = 8;

            void initialize(bool depthTest);
            void prepareData();
            static void addBox(const BatchedBox &box, uint32 indicesOffset, uint32 &verticesAdded, uint32 &indicesAdded, DynamicVertexBuffer &verts, DynamicIndexBuffer &inds);
            static void addSphere(const BatchedSphere &sphere, uint32 indicesOffset, uint32 &verticesAdded, uint32 &indicesAdded, DynamicVertexBuffer &verts, DynamicIndexBuffer &inds);

            BatchedElements* mBatch;

            TRef<const ShaderUniformBlock> pTransform;
            TRef<const ShaderBlockMember> pProjView;

            uint32 verticesCount = 0;
            uint32 indicesCount = 0;

            uint32 verticesCountWireframe = 0;
            uint32 indicesCountWireframe = 0;

            TPtrShared<Shader> shader;
            TPtrShared<RHIUniformSet> bindings;
            TPtrShared<RHIArrayObject> array;
            TPtrShared<RHIArrayObject> arrayWireframe;

            DynamicVertexBuffer vertices;
            DynamicIndexBuffer indices;

            DynamicVertexBuffer verticesWireframe;
            DynamicIndexBuffer indicesWireframe;

            UniformBuffer transform;
            RHIGraphicsPipelineState pipeline;
            RHIGraphicsPipelineState pipelineWireframe;

        };

    }
}

#endif //BERSERK_BATCHEDELEMENTSRENDERER_H