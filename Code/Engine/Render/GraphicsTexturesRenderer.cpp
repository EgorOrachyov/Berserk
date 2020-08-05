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

        uint32 getAlignedUniformBlockSize(uint32 size) {
            auto alignment = RHIDevice::getSingleton().getCapabilities().UNIFORM_BUFFER_OFFSET_ALIGNMENT;
            return size + (size % alignment? alignment - (size % alignment): 0);
        }

        void GraphicsTexturesRenderer::init() {
            auto& device = RHIDevice::getSingleton();
            auto& shaderManager = ShaderManager::getSingleton();

            shader = shaderManager.load("Global", "GraphicsTextureShader");
            auto& meta = shader->getShaderMetaRHI();

            pTexture = meta->getParam("Texture");
            pTransform = meta->getUniformBlock("Transform");
            pTextureInfo = meta->getUniformBlock("TextureInfo");

            pProj = pTransform->getMember("proj");
            pBaseColor = pTextureInfo->getMember("baseColor");
            pTransparentColor = pTextureInfo->getMember("transparentColor");
            pUseTransparentColor = pTextureInfo->getMember("useTransparentColor");
            pIsSRGB = pTextureInfo->getMember("isSRGB");

            transform.resize(pTransform->getSize());
            uniformBufferWriter.resize(pTextureInfo->getSize());

            textureInfoBlockSizeAligned = getAlignedUniformBlockSize(pTextureInfo->getSize());

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

            // Clear caches, which
            // will be filled now with data
            vertexData.clear();
            instancesWithAlpha = 0;
            instancesWithoutAlpha = 0;
            vertices = 0;

            auto& items = graphics->getTextureItems();

            struct alignas(8) TextureVertData {
                int32 pos[2];
                float textCoords[2];

                TextureVertData() = default;
                TextureVertData(const Point2i& p, const Vec2f& t) {
                    pos[0] = p[0];
                    pos[1] = p[1];
                    textCoords[0] = t[0];
                    textCoords[1] = t[1];
                }
            };

            auto& device = RHIDevice::getSingleton();
            auto graphicsSize = graphics->getSize();

            for (auto t: items) {
                // Evaluate texture vertex data (positions and uvs) and per instance data
                auto pos = t->position;
                auto area = t->areaSize;
                auto rect = t->textureRect;
                auto size = t->texture->getSize();

                Point2i p0, p1, p2, p3;
                Vec2f t0, t1, t2, t3;

                // Invert Y axis (in GPU y will be from down to up)
                pos[1] = graphicsSize[1] - pos[1];

                p0 = Point2i(pos[0]          , pos[1]          );
                p1 = Point2i(pos[0] + area[0], pos[1]          );
                p2 = Point2i(pos[0] + area[0], pos[1] - area[1]);
                p3 = Point2i(pos[0]          , pos[1] - area[1]);

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
                // (if texture already has cache uniform set: do nothing)
                if (t->uniformBinding.isNull()) {

                    uniformBufferWriter.setVec4(t->color, pBaseColor->getOffset());
                    uniformBufferWriter.setVec4(t->transparentColor, pTransparentColor->getOffset());
                    uniformBufferWriter.setBool(t->useTransparentColor, pUseTransparentColor->getOffset());
                    uniformBufferWriter.setBool(t->isSRGB, pIsSRGB->getOffset());

                    auto offset = uniformData.getSize();
                    uniformData.append(textureInfoBlockSizeAligned);
                    uniformData.write(offset, uniformBufferWriter.getData().data(), pTextureInfo->getSize());
                    uniformData.updateGPU(offset, pTextureInfo->getSize());
                    auto buffer = uniformData.getRHI();

                    RHIUniformBlockDesc textInfoDesc;
                    {
                        textInfoDesc.binding = pTextureInfo->getBinding();
                        textInfoDesc.buffer = buffer;
                        textInfoDesc.offset = offset;
                        textInfoDesc.range = pTextureInfo->getSize();
                    }

                    RHIUniformBlockDesc transformDesc;
                    {
                        transformDesc.binding = pTransform->getBinding();
                        transformDesc.buffer = transform.getRHI();
                        transformDesc.offset = 0;
                        transformDesc.range = pTransform->getSize();
                    }

                    RHIUniformTextureDesc textureDesc;
                    {
                        textureDesc.location = pTexture->getLocation();
                        textureDesc.texture = t->texture->getTextureRHI();
                        textureDesc.sampler = t->texture->getSamplerRHI();
                    }

                    t->uniformBinding = device.createUniformSet({textureDesc}, {transformDesc,textInfoDesc});
                }

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
            uniformData.clear();
            vertexData.clear();
            instancesWithAlpha = 0;
            instancesWithoutAlpha = 0;
            vertices = 0;
        }

        void GraphicsTexturesRenderer::draw(RHIDrawList& drawList) {
            if (instancesWithAlpha + instancesWithoutAlpha == 0)
                return;

            auto graphicsSize = graphics->getSize();
            auto& items = graphics->getTextureItems();

            Mat4x4f proj = Mat4x4f::orthographic(0, graphicsSize[0], 0, graphicsSize[1], 0, (float) Graphics::Z_FAR);
            transform.setMat4(proj, pProj->getOffset(), pProj->getMatrixStride(), !pProj->getIsRowMajor());
            transform.updateDataGPU();

            GraphicsPipelineBuilder noAlphaBuilder;
            auto noAlpha = noAlphaBuilder
                    .setShader(shader->getProgram())
                    .setDeclaration(shader->getDeclaration())
                    .depthTest(false)
                    .depthWrite(false)
                    .polygonFrontFace(EPolygonFrontFace::CounterClockwise)
                    .polygonCullMode(EPolygonCullMode::Back)
                    .polygonMode(EPolygonMode::Fill)
                    .blend(false)
                    .build();


            GraphicsPipelineBuilder withAlphaBuilder;
            auto withAlpha = withAlphaBuilder
                    .setShader(shader->getProgram())
                    .setDeclaration(shader->getDeclaration())
                    .depthTest(false)
                    .depthWrite(false)
                    .polygonFrontFace(EPolygonFrontFace::CounterClockwise)
                    .polygonCullMode(EPolygonCullMode::Back)
                    .polygonMode(EPolygonMode::Fill)
                    .blend(0, EBlendOperation::Add, EBlendOperation::Add, EBlendFactor::SrcAlpha, EBlendFactor::SrcAlpha, EBlendFactor::OneMinusSrcAlpha, EBlendFactor::OneMinusSrcAlpha)
                    .build();

            uint32 verticesOffset = 0;

            RHIGraphicsPipelineState* bound = nullptr;

            drawList.bindArrayObject(array);

            for (auto t: items) {
                if (t->useAlpha) {
                    if (bound != &withAlpha) {
                        bound = &withAlpha;
                        drawList.bindPipeline(withAlpha);
                    }
                }
                else {
                    if (bound != &noAlpha) {
                        bound = &noAlpha;
                        drawList.bindPipeline(noAlpha);
                    }
                }

                drawList.bindUniformSet(t->uniformBinding);
                drawList.drawIndexedBaseOffset(EIndexType::Uint32, INDICES_COUNT, verticesOffset);
                verticesOffset += VERTICES_COUNT;
            }
        }

    }
}