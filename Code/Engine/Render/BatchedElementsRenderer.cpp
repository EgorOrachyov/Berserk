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

            mSpheres.shader = shaderManager.load("BatchedSphereShader");
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

                for (auto& sphere: spheres) {
                    if (sphere.wire) {
                        mSpheres.instancesData.emplace(sphere.position, sphere.color, sphere.radius);
                    }
                }

                auto count = mSpheres.instancesData.size();
                if (count > mSpheres.maxInstances) {
                    // Want to scale buffer by pow of the 2 to keep allocations count at log2 bound
                    mSpheres.maxInstances = getSizePowOf2BoundFor(count);
                    auto bufferSize = sizeof(SpherePack) * mSpheres.maxInstances;
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
        
        uint32 BatchedElementsRenderer::getSizePowOf2BoundFor(uint32 count) {
            uint32 size = 2;
            while (size < count) size = size << 1u;
            return size;
        }

    }
}