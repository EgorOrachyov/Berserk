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

            shader = manager.load("Global", "graphics_primitive");

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
            vertices = 0;
            indices = 0;
        }

        void GraphicsPrimitivesRenderer::prepareData() {
            auto& primitives = graphics->getPrimitiveItems();

            if (primitives.isEmpty())
                return;

            auto& device = RHIDevice::getSingleton();

            vertexData.clear();
            indexData.clear();
            vertices = 0;
            indices = 0;

            for (int32 i = (int32)primitives.size() - 1; i >= 0; i--) {
                auto p = primitives[i];

                uint32 verticesAdded;
                uint32 indicesAdded;
                uint32 baseIndex = vertices;

                p->packVertexData(vertexData, indexData, baseIndex, verticesAdded, indicesAdded);

                vertices += verticesAdded;
                indices += indicesAdded;
            }

            vertexData.updateGPU();
            indexData.updateGPU();

            array = device.createArrayObject({ vertexData.getRHI() }, indexData.getRHI(), shader->getDeclarationRHI(), EPrimitivesType::Triangles );
        }

        void GraphicsPrimitivesRenderer::draw(RHIDrawList &drawList) {
            auto& primitives = graphics->getPrimitiveItems();

            if (primitives.isEmpty())
                return;

            auto graphicsSize = graphics->getSize();
            auto ortho = Mat4x4f::orthographic(0, graphicsSize[0], 0, graphicsSize[1], 0, Graphics::Z_FAR);
            transform.setMat4(ortho, pProj->getOffset(), pProj->getMatrixStride(), !pProj->getIsRowMajor());
            transform.setVec2i(graphicsSize, pAreaSize->getOffset());
            transform.updateDataGPU();

            drawList.bindPipeline(pipeline);
            drawList.bindArrayObject(array);
            drawList.bindUniformSet(uniformBinding);
            drawList.drawIndexed(EIndexType::Uint32, indices);
        }

    }
}