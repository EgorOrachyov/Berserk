/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICSPRIMITIVESRENDERER_H
#define BERSERK_GRAPHICSPRIMITIVESRENDERER_H

#include <Graphics.h>
#include <Shader.h>
#include <RHI/RHIDevice.h>
#include <RenderResources/DynamicIndexBuffer.h>
#include <RenderResources/DynamicVertexBuffer.h>
#include <RenderResources/UniformBufferWriter.h>
#include <RenderResources/DynamicUniformBuffer.h>

namespace Berserk {
    namespace Render {

        class GraphicsPrimitivesRenderer {
        public:

            void setGraphics(Graphics* g) { graphics = g; }
            void init();
            void clear();
            void prepareData();
            void draw(RHIDrawList& drawList);

        private:

            Graphics* graphics = nullptr;

            TRef<const ShaderUniformBlock> pTransform;
            TRef<const ShaderBlockMember> pProj;
            TRef<const ShaderBlockMember> pAreaSize;

            uint32 vertices = 0;
            uint32 indices = 0;
            uint32 verticesWithAlpha = 0;
            uint32 indicesWithAlpha = 0;

            DynamicVertexBuffer vertexData;
            DynamicIndexBuffer indexData;
            DynamicVertexBuffer vertexDataWithAlpha;
            DynamicIndexBuffer indexDataWithAlpha;

            TPtrShared<Shader> shader;
            TPtrShared<RHIUniformSet> uniformBinding;
            TPtrShared<RHIArrayObject> array;
            TPtrShared<RHIArrayObject> arrayWithAlpha;

            UniformBuffer transform;
            RHIGraphicsPipelineState pipeline;
        };

    }
}

#endif //BERSERK_GRAPHICSPRIMITIVESRENDERER_H