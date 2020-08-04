/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GraphicsTexturesRenderer.h>
#include <GraphicsItems.h>
#include <Containers/TAlgo.h>
#include <ShaderManager.h>
#include <RenderResources/GraphicsPipelineBuilder.h>

namespace Berserk {
    namespace Render {

        void GraphicsTexturesRenderer::init() {
            auto& device = RHIDevice::getSingleton();
            auto& shaderManager = ShaderManager::getSingleton();

            shader = shaderManager.load("Global", "GraphicsTextureShader");
            auto& meta = shader->getShaderMetaRHI();

            pTexture = meta->getParam("Texture");
            pTransform = meta->getUniformBlock("Transform");
            transform.resize(pTransform->getSize());

            // Indices to draw rect
            // v0 ---- v1
            // |       |
            // |       |
            // v3----- v2
            //
            uint32 indicesData[] = { 0, 3, 2, 2, 1, 0 };
            indices = device.createIndexBuffer(sizeof(indicesData), EBufferUsage::Static, indicesData);
        }

        void GraphicsTexturesRenderer::prepare() {
            if (graphics->getTextureItems().isEmpty()) {
                // Nothing to prepare
                return;
            }

            texturesSorted = graphics->getTextureItems();
            TAlgo::sort(texturesSorted, [](const GraphicsTexture* a, const GraphicsTexture* b){ return a->zOrder >= b->zOrder; });

            struct TextureVertData {
                float pos[3];
                float textCoords[2];

                TextureVertData() = default;
                TextureVertData(const Vec3f& p, const Vec2f& t) {
                    pos[0] = p[0];
                    pos[1] = p[1];
                    pos[2] = p[2];
                    textCoords[0] = t[0];
                    textCoords[1] = t[1];
                }
            };

            auto& device = RHIDevice::getSingleton();
            auto graphicsSize = graphics->getSize();

            for (auto t: texturesSorted) {
                // Evaluate texture vertex data (positions and uvs) and per instance data
                float z = t->zOrder;
                auto pos = t->position;
                auto area = t->areaSize;
                auto rect = t->textureRect;
                auto size = t->texture->getSize();

                Vec3f p0, p1, p2, p3;
                Vec2f t0, t1, t2, t3;

                // Invert Y axis (in GPU y will be from down to up)
                pos[1] = graphicsSize[1] - pos[1];

                p0 = Vec3f(pos[0]          , pos[1]          , z);
                p1 = Vec3f(pos[0] + area[0], pos[1]          , z);
                p2 = Vec3f(pos[0] + area[0], pos[1] - area[1], z);
                p3 = Vec3f(pos[0]          , pos[1] - area[1], z);

                float u0 = (float) rect.getX() / (float) size[0];
                float u1 = (float) (rect.getX() + rect.getW()) / (float) size[0];
                float v0 = (float) rect.getY() / (float) size[1];
                float v1 = (float) (rect.getY() + rect.getH()) / (float) size[1];

                t0 = Vec2f(u0,v1);
                t1 = Vec2f(u1,v1);
                t2 = Vec2f(u1,v0);
                t3 = Vec2f(u0,v0);

                // vd0 ------- vd1
                //  |           |
                //  |           |
                // vd3 ------- vd2
                //
                TextureVertData textureRect[VERTICES_COUNT] = { TextureVertData(p0,t0), TextureVertData(p1,t1), TextureVertData(p2,t2), TextureVertData(p3,t3) };
                vertexData.append((uint8*) textureRect, sizeof(textureRect));
                vertices += VERTICES_COUNT;

                // Create set for each texture drawn
                // todo: must be per texture, since we have to reallocate it each time we add something to Graphics

                RHIUniformTextureDesc textureDesc;
                {
                    textureDesc.location = pTexture->getLocation();
                    textureDesc.texture = t->texture->getTextureRHI();
                    textureDesc.sampler = t->texture->getSamplerRHI();
                }

                RHIUniformBlockDesc blockDesc;
                {
                    blockDesc.binding = pTransform->getBinding();
                    blockDesc.buffer = transform.getRHI();
                    blockDesc.offset = 0;
                    blockDesc.range = transform.getBufferSize();
                }

                auto set = device.createUniformSet({textureDesc}, {blockDesc});
                uniformSets.move(set);

                if (t->useAlpha) {
                    instancesWithAlpha += 1;
                }
                else {
                    instancesWithoutAlpha += 1;
                }
            }

            // Update Array if required
            bool update = vertexData.updateGPU();

            if (update) {
                array = device.createArrayObject({ vertexData.getRHI() }, indices, shader->getDeclarationRHI(), EPrimitivesType::Triangles);
            }
        }

        void GraphicsTexturesRenderer::clear() {
            texturesSorted.clearNoDestructorCall();
            uniformSets.clear();
            vertexData.clear();
            instancesWithAlpha = 0;
            instancesWithoutAlpha = 0;
            vertices = 0;
        }

        void GraphicsTexturesRenderer::draw(RHIDrawList& drawList) {
            auto graphicsSize = graphics->getSize();

            static auto pProj = pTransform->getMember("Proj");

            Mat4x4f proj = Mat4x4f::orthographic(0, graphicsSize[0], 0, graphicsSize[1], 0, (float) Graphics::Z_FAR);
            transform.setMat4(proj, pProj->getOffset(), pProj->getMatrixStride(), !pProj->getIsRowMajor());
            transform.updateDataGPU();

            if (instancesWithoutAlpha > 0) {
                GraphicsPipelineBuilder builder;
                auto pipeline = builder
                        .setShader(shader->getProgram())
                        .setDeclaration(shader->getDeclaration())
                        .depthTest(true)
                        .depthWrite(true)
                        .depthFunction(ECompareFunction::Less)
                        .polygonFrontFace(EPolygonFrontFace::CounterClockwise)
                        .polygonCullMode(EPolygonCullMode::Back)
                        .polygonMode(EPolygonMode::Fill)
                        .blend(false)
                        .build();

                drawList.bindPipeline(pipeline);
                drawList.bindArrayObject(array);
                uint32 verticesOffset = 0;
                for (uint32 i = 0; i < instancesWithoutAlpha; i++) {
                    drawList.bindUniformSet(uniformSets[i]);
                    drawList.drawIndexedBaseOffset(EIndexType::Uint32, INDICES_COUNT, verticesOffset);
                    verticesOffset += VERTICES_COUNT;
                }
            }
        }

    }
}