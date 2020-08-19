/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GraphicsPrimitivesRenderer.h>
#include <GraphicsItems.h>
#include <ShaderManager.h>
#include <RenderResources/GraphicsPipelineBuilder.h>

namespace Berserk {
    namespace Render {

        void GraphicsPrimitivesRenderer::init() {
            auto& device = RHIDevice::getSingleton();
            auto& manager = ShaderManager::getSingleton();

            shader = manager.loadGlobalShader("graphics_primitive");

            auto& meta = shader->getShaderMetaRHI();
            pTransform = meta->getUniformBlock("Transform");
            pProj = pTransform->getMember("proj");
            pAreaSize = pTransform->getMember("areaSize");

            transform.resize(pTransform->getSize());

            RHIUniformBlockDesc transfDesc;
            {
                transfDesc.binding = pTransform->getBinding();
                transfDesc.buffer = transform.getRHI();
                transfDesc.offset = 0;
                transfDesc.range = pTransform->getSize();
            }

            uniformBinding = device.createUniformSet({}, { transfDesc });

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

        void GraphicsPrimitivesRenderer::clear() {
            vertexData.clear();
            indexData.clear();
            vertexDataWithAlpha.clear();
            indexDataWithAlpha.clear();
            vertices = 0;
            indices = 0;
            verticesWithAlpha = 0;
            indicesWithAlpha = 0;
        }

        void GraphicsPrimitivesRenderer::prepareData() {
            auto& primitives = graphics->getPrimitiveItems();
            auto& primitivesWithAlpha = graphics->getPrimitiveItemsWithAlpha();

            if (primitives.isEmpty() && primitivesWithAlpha.isEmpty())
                return;

            auto& device = RHIDevice::getSingleton();

            vertexData.clear();
            indexData.clear();
            vertexDataWithAlpha.clear();
            indexDataWithAlpha.clear();
            vertices = 0;
            indices = 0;
            verticesWithAlpha = 0;
            indicesWithAlpha = 0;

            for (int32 i = (int32)primitives.size() - 1; i >= 0; i--) {
                auto p = primitives[i];

                uint32 verticesAdded;
                uint32 indicesAdded;
                uint32 baseIndex = vertices;

                p->packVertexData(vertexData, indexData, baseIndex, verticesAdded, indicesAdded);

                vertices += verticesAdded;
                indices += indicesAdded;
            }

            for (auto p: primitivesWithAlpha) {
                uint32 verticesAdded;
                uint32 indicesAdded;
                uint32 baseIndex = verticesWithAlpha;

                p->packVertexData(vertexDataWithAlpha, indexDataWithAlpha, baseIndex, verticesAdded, indicesAdded);

                verticesWithAlpha += verticesAdded;
                indicesWithAlpha += indicesAdded;
            }

            if (!primitives.isEmpty()) {
                vertexData.updateGPU();
                indexData.updateGPU();
                array = device.createArrayObject({ vertexData.getRHI() }, indexData.getRHI(), shader->getDeclarationRHI(), EPrimitivesType::Triangles );
            }

            if (!primitivesWithAlpha.isEmpty()) {
                vertexDataWithAlpha.updateGPU();
                indexDataWithAlpha.updateGPU();
                arrayWithAlpha = device.createArrayObject({ vertexDataWithAlpha.getRHI() }, indexDataWithAlpha.getRHI(), shader->getDeclarationRHI(), EPrimitivesType::Triangles );
            }
        }

        void GraphicsPrimitivesRenderer::draw(RHIDrawList &drawList) {
            auto& primitives = graphics->getPrimitiveItems();
            auto& primitivesWithAlpha = graphics->getPrimitiveItemsWithAlpha();

            if (primitives.isEmpty() && primitivesWithAlpha.isEmpty())
                return;

            auto graphicsSize = graphics->getSize();
            auto ortho = Mat4x4f::orthographic(0, graphicsSize[0], 0, graphicsSize[1], 0, Graphics::Z_FAR);
            transform.setMat4(ortho, pProj->getOffset(), pProj->getMatrixStride(), !pProj->getIsRowMajor());
            transform.setVec2i(graphicsSize, pAreaSize->getOffset());
            transform.updateDataGPU();

            drawList.bindPipeline(pipeline);

            if (!primitives.isEmpty()) {
                drawList.bindArrayObject(array);
                drawList.bindUniformSet(uniformBinding);
                drawList.drawIndexed(EIndexType::Uint32, indices);
            }

            if (!primitivesWithAlpha.isEmpty()) {
                drawList.bindArrayObject(arrayWithAlpha);
                drawList.bindUniformSet(uniformBinding);
                drawList.drawIndexed(EIndexType::Uint32, indicesWithAlpha);
            }
        }

    }
}