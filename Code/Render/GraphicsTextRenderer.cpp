/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GraphicsTextRenderer.h>
#include <GraphicsItems.h>
#include <ShaderManager.h>
#include <RenderResources/GraphicsPipelineBuilder.h>
#include <Containers/TAlgo.h>

namespace Berserk {
    namespace Render {

        void GraphicsTextRenderer::init() {
            auto& device = RHIDevice::getSingleton();
            auto& manager = ShaderManager::getSingleton();

            shader = manager.load("Global", "graphics_text");

            auto& meta = shader->getShaderMetaRHI();
            pTransform = meta->getUniformBlock("Transform");
            pProj = pTransform->getMember("proj");
            pAreaSize = pTransform->getMember("areaSize");
            pBitmap = meta->getParam("Bitmap");

            transform.resize(pTransform->getSize());

            GraphicsPipelineBuilder builder;
            pipeline = builder
                    .setShader(shader->getProgram())
                    .setDeclaration(shader->getDeclaration())
                    .depthTest(true)
                    .depthWrite(true)
                    .depthFunction(ECompareFunction::LessEqual)
                    .polygonFrontFace(EPolygonFrontFace::CounterClockwise)
                    .polygonCullMode(EPolygonCullMode::Disabled)
                    .polygonMode(EPolygonMode::Fill)
                    .blend(true)
                    .blend(0, EBlendOperation::Add, EBlendOperation::Add, EBlendFactor::SrcAlpha, EBlendFactor::SrcAlpha, EBlendFactor::OneMinusSrcAlpha, EBlendFactor::OneMinusSrcAlpha)
                    .build();
        }

        void GraphicsTextRenderer::clear() {
            vertices = 0;
            vertexData.clear();
            fontMap.clear();
            sortedTexts.clear();
        }

        void GraphicsTextRenderer::prepareData() {
            auto& texts = graphics->getTextItems();

            if (texts.isEmpty())
                return;

            auto& device = RHIDevice::getSingleton();

            vertices = 0;
            vertexData.clear();
            fontMap.clear();
            sortedTexts.clear();

            sortedTexts = texts;
            TAlgo::sort(sortedTexts, [](const GraphicsText* t1, const GraphicsText* t2) -> bool { return t1->font.getPtr() < t2->font.getPtr(); } );

            GpuFont* current = nullptr;
            FontData* data = nullptr;

            uint32 vertexOffset = 0;

            for (auto t: sortedTexts) {
                TPtrShared<GpuFont> &font = t->font;

                if (font.getPtr() != current) {
                    current = font.getPtr();
                    data = &fontMap[current];
                    data->vertexOffset = vertexOffset;

                    RHIUniformTextureDesc bitmapDesc;
                    {
                        bitmapDesc.location = pBitmap->getLocation();
                        bitmapDesc.texture = font->getBitmapTexture()->getTextureRHI();
                        bitmapDesc.sampler = font->getBitmapTexture()->getSamplerRHI();
                    }

                    RHIUniformBlockDesc transfDesc;
                    {
                        transfDesc.binding = pTransform->getBinding();
                        transfDesc.buffer = transform.getRHI();
                        transfDesc.offset = 0;
                        transfDesc.range = pTransform->getSize();
                    }

                    data->uniformBinding = device.createUniformSet({ bitmapDesc }, { transfDesc });
                }

                uint32 verticesAdded;
                t->packVertexData(vertexData, verticesAdded);

                data->vertexCount += verticesAdded;
                vertices += verticesAdded;
                vertexOffset += verticesAdded;
            }

            vertexData.updateGPU();

            array = device.createArrayObject({ vertexData.getRHI() }, nullptr, shader->getDeclarationRHI(), EPrimitivesType::Triangles );
        }

        void GraphicsTextRenderer::draw(RHIDrawList &drawList) {
            auto& texts = graphics->getTextItems();

            if (texts.isEmpty())
                return;

            auto graphicsSize = graphics->getSize();
            auto ortho = Mat4x4f::orthographic(0, graphicsSize[0], 0, graphicsSize[1], 0, Graphics::Z_FAR);
            transform.setMat4(ortho, pProj->getOffset(), pProj->getMatrixStride(), !pProj->getIsRowMajor());
            transform.setVec2i(graphicsSize, pAreaSize->getOffset());
            transform.updateDataGPU();

            drawList.bindPipeline(pipeline);
            drawList.bindArrayObject(array);

            for (auto& perFontRequests: fontMap) {
                auto& data = perFontRequests.second();

                drawList.bindUniformSet(data.uniformBinding);
                drawList.drawBaseOffset(data.vertexCount, data.vertexOffset);
            }
        }
        
    }
}