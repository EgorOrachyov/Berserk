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

        BatchedElementsRenderer::BatchedElementsRenderer(BatchedElements &elements, bool depthTest) {
            mBatch = &elements;
            init(depthTest);
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

        void BatchedElementsRenderer::init(bool depthTest) {
            auto& device = RHIDevice::getSingleton();
            auto& shaderMan = ShaderManager::getSingleton();

            shader = shaderMan.loadGlobalShader("batched");

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
                    .depthTest(depthTest)
                    .depthWrite(depthTest)
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
                    .depthTest(depthTest)
                    .depthWrite(depthTest)
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

            auto& cylinders = mBatch->getCylinders();

            for (const auto& cylinder: cylinders) {
                if (cylinder.wire) {
                    uint32 verticesAdded;
                    uint32 indicesAdded;
                    uint32 indicesOffset = verticesCountWireframe;

                    addCylinder(cylinder, indicesOffset, verticesAdded, indicesAdded, verticesWireframe, indicesWireframe);

                    verticesCountWireframe += verticesAdded;
                    indicesCountWireframe += indicesAdded;
                }
                else {
                    uint32 verticesAdded;
                    uint32 indicesAdded;
                    uint32 indicesOffset = verticesCount;

                    addCylinder(cylinder, indicesOffset, verticesAdded, indicesAdded, vertices, indices);

                    verticesCount += verticesAdded;
                    indicesCount += indicesAdded;
                }
            }

            auto& spheres = mBatch->getSpheres();

            for (const auto& sphere: spheres) {
                if (sphere.wire) {
                    uint32 verticesAdded;
                    uint32 indicesAdded;
                    uint32 indicesOffset = verticesCountWireframe;

                    addSphere(sphere, indicesOffset, verticesAdded, indicesAdded, verticesWireframe, indicesWireframe);

                    verticesCountWireframe += verticesAdded;
                    indicesCountWireframe += indicesAdded;
                }
                else {
                    uint32 verticesAdded;
                    uint32 indicesAdded;
                    uint32 indicesOffset = verticesCount;

                    addSphere(sphere, indicesOffset, verticesAdded, indicesAdded, vertices, indices);

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
            auto c = Vec3f(box.color);

            //      v4------v5
            //     /|      /|
            //    / |     / |
            //   v0-----v1  |
            //   |  v7---|--v6
            //   | /     | /
            //   |/      |/
            //   v3-----v2

            // Unit box
            const Vec3f vs[VERTICES] = {
                    Vec3f(-0.5f,0.5f,0.5f),
                    Vec3f(0.5f,0.5f,0.5f),
                    Vec3f(0.5f,-0.5f,0.5f),
                    Vec3f(-0.5f,-0.5f,0.5f),
                    Vec3f(-0.5f,0.5f,-0.5f),
                    Vec3f(0.5f,0.5f,-0.5f),
                    Vec3f(0.5f,-0.5f,-0.5f),
                    Vec3f(-0.5f,-0.5f,-0.5f)
            };

            for (auto& v: vs) {
                verts.append(p + t.rotate(s * v));
                verts.append(c);
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

        void BatchedElementsRenderer::addCylinder(const BatchedCylinder &cylinder, uint32 indicesOffset, uint32 &verticesAdded, uint32 &indicesAdded, DynamicVertexBuffer &verts, DynamicIndexBuffer &inds) {
            const auto VERTICES = CYLINDER_SIDES * 2 + 2;
            const auto INDICES = CYLINDER_SIDES * 2 * 3 + CYLINDER_SIDES * 2 * 3;

            auto& r = cylinder.rotation;
            auto& p = cylinder.position;
            auto& s = cylinder.sizeRxRyH;
            auto c = Vec3f(cylinder.color);

            float halfH = s[2] * 0.5f;
            float rX = s[0];
            float rZ = s[1];

            float da = Math::PIf * 2.0f / (float) CYLINDER_SIDES;

            uint32 centerIndexDown = indicesOffset + CYLINDER_SIDES * 2;
            uint32 centerIndexUp = indicesOffset + CYLINDER_SIDES * 2 + 1;

            for (uint32 i = 0; i < CYLINDER_SIDES; i++) {
                float a = da * (float) i;

                float x = Math::cos(a) * rX;
                float z = Math::sin(a) * rZ;
                float yDown = -halfH;
                float yUp = halfH;

                verts.append(r.rotate(Vec3f(x,yDown,z)) + p);
                verts.append(c);

                verts.append(r.rotate(Vec3f(x,yUp,z)) + p);
                verts.append(c);
            }

            verts.append(r.rotate(Vec3f(0,-halfH,0)) + p);
            verts.append(c);

            verts.append(r.rotate(Vec3f(0,halfH,0)) + p);
            verts.append(c);

            const auto INDICES_FACTOR = CYLINDER_SIDES * 2u;

            for (uint32 i = 0; i < CYLINDER_SIDES; i++) {
                // i1U --- i0U
                // |        |
                // i1D --- i0D

                uint32 i0D = indicesOffset + (( i      * 2u     ) % INDICES_FACTOR);
                uint32 i1D = indicesOffset + (((i + 1) * 2u     ) % INDICES_FACTOR);
                uint32 i0U = indicesOffset + (( i      * 2u + 1u) % INDICES_FACTOR);
                uint32 i1U = indicesOffset + (((i + 1) * 2u + 1u) % INDICES_FACTOR);

                inds.append(i0D);
                inds.append(i1D);
                inds.append(centerIndexDown);

                inds.append(i1U);
                inds.append(i0U);
                inds.append(centerIndexUp);

                inds.append(i1U);
                inds.append(i1D);
                inds.append(i0D);

                inds.append(i1U);
                inds.append(i0D);
                inds.append(i0U);
            }

            verticesAdded = VERTICES;
            indicesAdded = INDICES;
        }
        
        void BatchedElementsRenderer::addSphere(const BatchedSphere &sphere, uint32 indicesOffset, uint32 &verticesAdded, uint32 &indicesAdded, DynamicVertexBuffer &verts, DynamicIndexBuffer &inds) {
            const auto stepsV = SPHERE_V;
            const auto stepsH = SPHERE_H;

            const auto totalV = stepsV + 1;
            const auto totalH = stepsH + 1;

            const auto VERTICES = totalV * totalH;
            const auto INDICES = stepsV * stepsH * 2 * 3;

            auto daV = Math::PIf / (float) stepsV;
            auto daH = 2.0f * Math::PIf / (float) stepsH;

            auto r = sphere.radius;
            auto p = sphere.position;
            auto c = Vec3f(sphere.color);

            for (uint32 i = 0; i < totalV; i++ ) {
                auto aV = daV * (float)i - Math::HALF_PIf;

                for (uint32 j = 0; j < totalH; j++) {
                    auto aH = daH * (float) j;

                    auto rXZ = r * Math::cos(aV);
                    auto rX = rXZ * Math::cos(aH);
                    auto rZ = rXZ * Math::sin(aH);
                    auto rY = r * Math::sin(aV);

                    Vec3f v(p[0] + rX, p[1] + rY, p[2] + rZ);

                    verts.append(v);
                    verts.append(c);
                }
            }

            for (uint32 i = 0; i < stepsV; i++) {
                for (uint32 j = 0; j < stepsH; j++) {
                    inds.append((uint32)(indicesOffset + i * totalH + j + 1));
                    inds.append((uint32)(indicesOffset + i * totalH + j + 0));
                    inds.append((uint32)(indicesOffset + i * totalH + j + totalH));

                    inds.append((uint32)(indicesOffset + i * totalH + j + totalH));
                    inds.append((uint32)(indicesOffset + i * totalH + j + totalH + 1));
                    inds.append((uint32)(indicesOffset + i * totalH + j + 1));
                }
            }

            verticesAdded = VERTICES;
            indicesAdded = INDICES;
        }

    }
}