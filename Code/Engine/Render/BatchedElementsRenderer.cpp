/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <BatchedElementsRenderer.h>
#include <ShaderManager.h>
#include <RenderResources/VertexArrayData.h>
#include <RenderResources/GraphicsPipelineBuilder.h>
#include <GeometryGenerator.h>

namespace Berserk {
    namespace Render {

        BatchedElementsRenderer::BatchedElementsRenderer() {
            initializeSpheresRendering();
        }

        void BatchedElementsRenderer::draw(const ViewData &viewData, const BatchedElements &batch, RHIDrawList &drawList) {
            prepareData(batch);

            if (mSpheres.instancesCount > 0) {
                GraphicsPipelineBuilder builder;
                auto pipeline = builder
                    .setShader(mSpheres.shader->getProgram())
                    .setDeclaration(mSpheres.shader->getDeclaration())
                    .depthTest(true)
                    .depthWrite(true)
                    .polygonCullMode(EPolygonCullMode::Disabled)
                    .polygonMode(EPolygonMode::Line)
                    .build();

                // Cache param reference, it won't be changed
                static auto projViewParam = mSpheres.bindings->findParam("Camera", "ProjView");
                mSpheres.bindings->setMat4(projViewParam, viewData.projectionViewMatrix);
                mSpheres.bindings->updateGPU();

                drawList.bindPipeline(pipeline);
                mSpheres.bindings->bind(drawList);
                drawList.bindArrayObject(mSpheres.array);
                drawList.drawIndexedInstanced(EIndexType::Uint32, mSpheres.indicesCount, mSpheres.instancesCount);

                // Instances draw, set 0
                mSpheres.instancesCount = 0;
            }
        }

        void BatchedElementsRenderer::initializeSpheresRendering() {
            auto& shaderManager = ShaderManager::getSingleton();
            auto& device = RHIDevice::getSingleton();

            mSpheres.shader = shaderManager.load("Global", "BatchedSphereShader");
            mSpheres.bindings = mSpheres.shader->allocateBindings();
            mSpheres.bindings->associateUniformBuffers();

            VertexArrayData data;
            data.setDeclaration(mSpheres.shader->getDeclaration());
            data.useIndices(EIndexType::Uint32);

            auto positions = data.getStreamFor("inPos");
            auto indices = data.getIndexStream();

            GeometryGenerator::generateSphere(1.0f, 8, 8, positions, indices);

            data.evaluate();

            mSpheres.verticesCount = data.getVerticesCount();
            mSpheres.indicesCount = data.getIndicesCount();

            mSpheres.vertices = device.createVertexBuffer(positions.getData().size(), EBufferUsage::Static, positions.getData().data());
            mSpheres.indices = device.createIndexBuffer(indices.getData().size(), EBufferUsage::Static, indices.getData().data());
        }

        void BatchedElementsRenderer::prepareData(const BatchedElements &elements) {
            auto& device = RHIDevice::getSingleton();

            auto& spheres = elements.getSpheres();
            if (spheres.size() > 0) {
                // Pack spheres in the data array
                // Allocate new vertex buffer instances data if needed and rebuild array object

                DynamicVertexBuffer& data = mSpheres.instancesData;
                data.clear();
                uint32 count = 0;

                for (auto& sphere: spheres) {
                    if (sphere.wire) {
                        SpherePack pack(sphere.position, sphere.color, sphere.radius);
                        data.append(pack);
                        count += 1;
                    }
                }

                if (count > 0) {
                    // If RHI vertex buffer was reallocated, then we will have to update array object
                    bool allocatedNewRHI = data.updateGPU();

                    if (allocatedNewRHI) {
                        mSpheres.array = device.createArrayObject({mSpheres.vertices,data.getRHI()}, mSpheres.indices, mSpheres.shader->getDeclaration()->getRHI(), EPrimitivesType::Triangles);
                    }
                }

                mSpheres.instancesCount = count;
            }
        }
        
    }
}