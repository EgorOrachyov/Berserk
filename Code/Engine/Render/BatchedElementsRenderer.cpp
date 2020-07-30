/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <BatchedElementsRenderer.h>
#include <ShaderCore/ShaderManager.h>
#include <RenderResources/VertexArrayData.h>
#include <RenderResources/GraphicsPipelineBuilder.h>
#include <GeometryGenerator.h>

namespace Berserk {
    namespace Render {

        BatchedElementsRenderer::BatchedElementsRenderer() {

        }

        void BatchedElementsRenderer::draw(const ViewData &viewData, const BatchedElements &batch, RHIDrawList &drawList) {
            auto& proj = viewData.projectionMatrix;
            auto& view = viewData.viewMatrix;

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

                drawList.bindPipeline(pipeline);
                drawList.bindArrayObject(mSpheres.array);
                drawList.drawIndexedInstanced(EIndexType::Uint32, mSpheres.indicesCount, mSpheres.instancesCount);

                mSpheres.instancesCount = 0;
            }
        }

        void BatchedElementsRenderer::initializePipeline() {
            initializeSpheresRendering();
        }

        void BatchedElementsRenderer::initializeSpheresRendering() {
            auto& shaderManager = ShaderManager::getSingleton();
            auto& device = RHIDevice::getSingleton();

            mSpheres.shader = shaderManager.load("BatchedSphereShader");
            mSpheres.bindings = mSpheres.shader->allocateBindings();

            VertexArrayData data;
            data.setDeclaration(mSpheres.shader->getDeclaration());
            data.useIndices(EIndexType::Uint32);

            auto positions = data.getStreamFor("inPos");
            auto indices = data.getIndexStream();

            GeometryGenerator::generateSphere(1.0f, 10, 10, positions, indices);

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
                for (auto& sphere: spheres) {
                    if (sphere.wire) {
                        mSpheres.instancesData.emplace(sphere.position, sphere.color, sphere.radius);
                    }
                }

                auto count = mSpheres.instancesData.size();
                if (count > mSpheres.maxInstances) {
                    auto bufferSize = sizeof(SpherePack) * count;
                    mSpheres.maxInstances = count;
                    mSpheres.instances = device.createVertexBuffer(bufferSize, EBufferUsage::Dynamic, nullptr);
                    mSpheres.array = device.createArrayObject({mSpheres.vertices,mSpheres.instances}, mSpheres.indices, mSpheres.shader->getDeclaration()->getRHI(), EPrimitivesType::Triangles);
                }

                if (count > 0) {
                    auto bufferSize = sizeof(SpherePack) * count;
                    mSpheres.instances->update(bufferSize, 0, mSpheres.instancesData.data());
                }

                mSpheres.instancesCount = count;
                mSpheres.instancesData.clearNoDestructorCall();
            }
        }

    }
}