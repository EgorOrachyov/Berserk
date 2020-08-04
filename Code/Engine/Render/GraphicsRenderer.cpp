/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RHI/RHIDevice.h>
#include <GraphicsRenderer.h>
#include <ShaderManager.h>
#include <Containers/TAlgo.h>
#include <RenderResources/GraphicsPipelineBuilder.h>

namespace Berserk {
    namespace Render {

        GraphicsRenderer::GraphicsRenderer(Graphics &graphics, const TPtrShared<RenderTarget> &target) {
            BERSERK_COND_ERROR_RET(target.isNotNull(), "Passed null target");
            mTarget = target;
            mGraphics = &graphics;
            mRegion = Region2i(0,0,mGraphics->getSize());

            initialize();
        }

        GraphicsRenderer::GraphicsRenderer(Graphics &graphics, const Region2i &region, const TPtrShared<RenderTarget> &target) {
            BERSERK_COND_ERROR_RET(target.isNotNull(), "Passed null target");
            mTarget = target;
            mGraphics = &graphics;
            mRegion = region;

            initialize();
        }

        void GraphicsRenderer::draw(RHIDrawList &drawList) {
            if (mGraphics->isDirty()) {
                clearState();
                prepareData();
                mGraphics->markClean();
            }

            auto graphicsSize = mGraphics->getSize();

            mTarget->bind(drawList, {EClearOption::Color,EClearOption::Depth}, mRegion);

            if (mTextures.instances > 0) {
                GraphicsPipelineBuilder builder;
                auto pipeline = builder
                    .setShader(mTextures.shader->getProgram())
                    .setDeclaration(mTextures.shader->getDeclaration())
                    .depthTest(true)
                    .depthWrite(true)
                    .depthFunction(ECompareFunction::Less)
                    .polygonFrontFace(EPolygonFrontFace::CounterClockwise)
                    .polygonCullMode(EPolygonCullMode::Back)
                    .polygonMode(EPolygonMode::Fill)
                    .blend(false)
                    .build();


                static auto pProj = mTextures.shader->getShaderMetaRHI()->getUniformBlock("Transform")->getMember("Proj");
                Mat4x4f proj = Mat4x4f::orthographic(0, graphicsSize[0], 0, graphicsSize[1], 0, (float) Graphics::Z_FAR);
                mTextures.transform.setMat4(proj, pProj->getOffset(), pProj->getMatrixStride(), !pProj->getIsRowMajor());
                mTextures.transform.updateDataGPU();

                drawList.bindPipeline(pipeline);
                drawList.bindArrayObject(mTextures.array);
                uint32 verticesOffset = 0;
                for (auto& set: mTextures.uniforms) {
                    drawList.bindUniformSet(set);
                    drawList.drawIndexedBaseOffset(EIndexType::Uint32, 6, verticesOffset);
                    verticesOffset += 4;
                }
            }
        }

        void GraphicsRenderer::setRegion(const Berserk::Region2i &rect) {
            mRegion = rect;
        }

        void GraphicsRenderer::initialize() {
            auto& device = RHIDevice::getSingleton();
            auto& shaderManager = ShaderManager::getSingleton();

            {
                mTextures.shader = shaderManager.load("Global", "GraphicsTextureShader");

                auto& meta = mTextures.shader->getShaderMetaRHI();
                auto texture = meta->getParam("Texture");
                auto transform = meta->getUniformBlock("Transform");

                mTextures.transform.resize(transform->getSize());
                mTextures.textureBindingLocation = texture->getLocation();
                mTextures.transformBindingLocation = transform->getBinding();

                // Indices to draw rect
                // v0 ---- v1
                // |       |
                // |       |
                // v3----- v2
                //
                uint32 indices[] = { 0, 3, 2, 2, 1, 0 };

                mTextures.indices = device.createIndexBuffer(sizeof(indices), EBufferUsage::Static, indices);
            }
        }

        void GraphicsRenderer::clearState() {
            {
                mTextures.uniforms.clear();
                //mTextures.instanceData.clear();
                mTextures.vertexData.clear();
            }
        }

        void GraphicsRenderer::prepareData() {
            prepareTexturesData();
        }

        void GraphicsRenderer::prepareTexturesData() {
            if (mGraphics->getTextureItems().isEmpty()) {
                // Nothing to draw
                return;
            }

            mTextures.texturesSorted = mGraphics->getTextureItems();
            TAlgo::sort(mTextures.texturesSorted, [](const GraphicsTexture* a, const GraphicsTexture* b){ return a->zOrder >= b->zOrder; });

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

//            struct TextureInstData {
//                float col[4];
//
//                TextureInstData(const Color4f& color) {
//                    col[0] = color[0];
//                    col[1] = color[1];
//                    col[2] = color[2];
//                    col[3] = color[3];
//                }
//            };

            auto& device = RHIDevice::getSingleton();
            auto graphicsSize = mGraphics->getSize();

            for (auto t: mTextures.texturesSorted) {

                //////////////////////////////////
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
                TextureVertData textureRect[4] = { TextureVertData(p0,t0), TextureVertData(p1,t1), TextureVertData(p2,t2), TextureVertData(p3,t3) };
                mTextures.vertexData.append((uint8*) textureRect, sizeof(textureRect));
                mTextures.vertices += 4;

                //////////////////////////////////
                // Create set for each texture drawn
                // todo: must be per texture, since we have to reallocate it each time we add something to Graphics

                RHIUniformTextureDesc textureDesc;
                {
                    textureDesc.location = mTextures.textureBindingLocation;
                    textureDesc.texture = t->texture->getTextureRHI();
                    textureDesc.sampler = t->texture->getSamplerRHI();
                }

                RHIUniformBlockDesc blockDesc;
                {
                    blockDesc.binding = mTextures.transformBindingLocation;
                    blockDesc.buffer = mTextures.transform.getRHI();
                    blockDesc.offset = 0;
                    blockDesc.range = mTextures.transform.getBufferSize();
                }

                auto set = device.createUniformSet({textureDesc}, {blockDesc});
                mTextures.uniforms.move(set);

                mTextures.instances += 1;
            }

            //////////////////////////////////
            // Update Array if required

            bool update = mTextures.vertexData.updateGPU(); //|| mTextures.instanceData.updateGPU();

            if (update) {
                mTextures.array = device.createArrayObject({ mTextures.vertexData.getRHI() }, mTextures.indices, mTextures.shader->getDeclarationRHI(), EPrimitivesType::Triangles);
            }
        }


    }
}