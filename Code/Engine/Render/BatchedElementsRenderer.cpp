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

        BatchedElementsRenderer::BatchedElementsRenderer(BatchedElements &elements) {
            mBatch = &elements;
            initialize();
        }

        void BatchedElementsRenderer::draw(const ViewData &viewData, RHIDrawList &drawList) {
            if (mBatch->isEmpty())
                return;

            prepareData();

            transform.setMat4(viewData.projViewMatrix, pProjView->getOffset(), pProjView->getMatrixStride(), !pProjView->getIsRowMajor());
            transform.updateDataGPU();

            if (indicesCount > 0) {
                drawList.bindPipeline(pipeline);
                drawList.bindUniformSet(bindings);
                drawList.bindArrayObject(array);
                drawList.drawIndexed(EIndexType::Uint32, indicesCount);
            }

            if (indicesCountWireframe > 0) {
                drawList.bindPipeline(pipelineWireframe);
                drawList.bindUniformSet(bindings);
                drawList.bindArrayObject(arrayWireframe);
                drawList.drawIndexed(EIndexType::Uint32, indicesCountWireframe);
            }
        }

        void BatchedElementsRenderer::initialize() {
            auto& device = RHIDevice::getSingleton();
            auto& shaderMan = ShaderManager::getSingleton();

            shader = shaderMan.load("Global", "batched");

            auto& meta = shader->getShaderMetaRHI();
            pTransform = meta->getUniformBlock("Transform");
            pProjView = pTransform->getMember("projView");

            transform.resize(pTransform->getSize());

            RHIUniformBlockDesc desc;
            {
                desc.binding = pTransform->getBinding();
                desc.buffer = transform.getRHI();
                desc.offset = 0;
                desc.range = pTransform->getSize();
            }

            bindings = device.createUniformSet({},{desc});

            GraphicsPipelineBuilder builder;
            pipeline = builder
                    .setShader(shader->getProgram())
                    .setDeclaration(shader->getDeclaration())
                    .depthTest(true)
                    .depthWrite(true)
                    .depthFunction(ECompareFunction::Less)
                    .polygonCullMode(EPolygonCullMode::Back)
                    .polygonFrontFace(EPolygonFrontFace::CounterClockwise)
                    .polygonMode(EPolygonMode::Fill)
                    .blend(false)
                    .build();

            GraphicsPipelineBuilder builderWireframe;
            pipelineWireframe = builderWireframe
                    .setShader(shader->getProgram())
                    .setDeclaration(shader->getDeclaration())
                    .depthTest(true)
                    .depthWrite(true)
                    .depthFunction(ECompareFunction::LessEqual)
                    .polygonCullMode(EPolygonCullMode::Disabled)
                    .polygonFrontFace(EPolygonFrontFace::CounterClockwise)
                    .polygonMode(EPolygonMode::Line)
                    .blend(false)
                    .build();
        }

        void BatchedElementsRenderer::prepareData() {
            auto& device = RHIDevice::getSingleton();

            verticesCount = 0;
            indicesCount = 0;
            vertices.clear();
            indices.clear();

            verticesCountWireframe = 0;
            indicesCountWireframe = 0;
            verticesWireframe.clear();
            indicesWireframe.clear();

            auto& boxes = mBatch->getBoxes();

            for (const auto& box: boxes) {
                if (box.wire) {
                    uint32 verticesAdded;
                    uint32 indicesAdded;
                    uint32 indicesOffset = verticesCountWireframe;

                    addBox(box, indicesOffset, verticesAdded, indicesAdded, verticesWireframe, indicesWireframe);

                    verticesCountWireframe += verticesAdded;
                    indicesCountWireframe += indicesAdded;
                }
                else {
                    uint32 verticesAdded;
                    uint32 indicesAdded;
                    uint32 indicesOffset = verticesCount;

                    addBox(box, indicesOffset, verticesAdded, indicesAdded, vertices, indices);

                    verticesCount += verticesAdded;
                    indicesCount += indicesAdded;
                }
            }

            if (indicesCount > 0) {
                vertices.updateGPU();
                indices.updateGPU();
                array = device.createArrayObject({vertices.getRHI()}, indices.getRHI(), shader->getDeclarationRHI(), EPrimitivesType::Triangles);
            }

            if (indicesCountWireframe > 0) {
                verticesWireframe.updateGPU();
                indicesWireframe.updateGPU();
                arrayWireframe = device.createArrayObject({verticesWireframe.getRHI()}, indicesWireframe.getRHI(), shader->getDeclarationRHI(), EPrimitivesType::Triangles);
            }
        }

        void BatchedElementsRenderer::addBox(const BatchedBox &box, uint32 indicesOffset, uint32 &verticesAdded, uint32 &indicesAdded, DynamicVertexBuffer &verts, DynamicIndexBuffer &inds) {
            const uint32 VERTICES = 8;
            const uint32 INDICES = 6 * 6;

            auto& t = box.rotation;
            auto& p = box.position;
            auto& s = box.size;
            auto& c = box.color;

            //      v4------v5
            //     /|      /|
            //    / |     / |
            //   v0-----v1  |
            //   |  v7---|--v6
            //   | /     | /
            //   |/      |/
            //   v3-----v2

            const Vec3f vs[VERTICES] = {
                    Vec3f(-1,1,1),
                    Vec3f(1,1,1),
                    Vec3f(1,-1,1),
                    Vec3f(-1,-1,1),
                    Vec3f(-1,1,-1),
                    Vec3f(1,1,-1),
                    Vec3f(1,-1,-1),
                    Vec3f(-1,-1,-1)
            };

            for (auto& v: vs) {
                verts.append(p + t.multiply(s * v));
                verts.append(Vec3f(c));
            }

            inds.append(indicesOffset + 0u);
            inds.append(indicesOffset + 3u);
            inds.append(indicesOffset + 1u);

            inds.append(indicesOffset + 1u);
            inds.append(indicesOffset + 3u);
            inds.append(indicesOffset + 2u);

            inds.append(indicesOffset + 1u);
            inds.append(indicesOffset + 2u);
            inds.append(indicesOffset + 6u);

            inds.append(indicesOffset + 1u);
            inds.append(indicesOffset + 6u);
            inds.append(indicesOffset + 5u);

            inds.append(indicesOffset + 5u);
            inds.append(indicesOffset + 6u);
            inds.append(indicesOffset + 7u);

            inds.append(indicesOffset + 5u);
            inds.append(indicesOffset + 7u);
            inds.append(indicesOffset + 4u);

            inds.append(indicesOffset + 4u);
            inds.append(indicesOffset + 7u);
            inds.append(indicesOffset + 0u);

            inds.append(indicesOffset + 0u);
            inds.append(indicesOffset + 7u);
            inds.append(indicesOffset + 3u);

            inds.append(indicesOffset + 4u);
            inds.append(indicesOffset + 0u);
            inds.append(indicesOffset + 1u);

            inds.append(indicesOffset + 4u);
            inds.append(indicesOffset + 1u);
            inds.append(indicesOffset + 5u);

            inds.append(indicesOffset + 6u);
            inds.append(indicesOffset + 2u);
            inds.append(indicesOffset + 3u);

            inds.append(indicesOffset + 6u);
            inds.append(indicesOffset + 3u);
            inds.append(indicesOffset + 7u);

            verticesAdded = VERTICES;
            indicesAdded = INDICES;
        }

    }
}