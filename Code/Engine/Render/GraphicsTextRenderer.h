/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICSTEXTRENDERER_H
#define BERSERK_GRAPHICSTEXTRENDERER_H

#include <Graphics.h>
#include <Shader.h>
#include <RHI/RHIDevice.h>
#include <RenderResources/GpuFont.h>
#include <RenderResources/DynamicIndexBuffer.h>
#include <RenderResources/DynamicVertexBuffer.h>
#include <RenderResources/UniformBufferWriter.h>
#include <RenderResources/DynamicUniformBuffer.h>

namespace Berserk {
    namespace Render {

        class GraphicsTextRenderer {
        public:

            void setGraphics(Graphics* g) { graphics = g; }
            void init();
            void clear();
            void prepareData();
            void draw(RHIDrawList& drawList);

        private:

            Graphics* graphics = nullptr;

            TRef<const ShaderUniformBlock> pTransform;
            TRef<const ShaderMember> pProj;
            TRef<const ShaderMember> pAreaSize;
            TRef<const ShaderParam> pBitmap;

            uint32 vertices = 0;
            DynamicVertexBuffer vertexData;

            TPtrShared<Shader> shader;
            TPtrShared<RHIArrayObject> array;

            UniformBuffer transform;
            RHIGraphicsPipelineState pipeline;

            struct FontData {
                uint32 vertexOffset = 0;
                uint32 vertexCount = 0;
                TPtrShared<RHIUniformSet> uniformBinding;
            };

            /** Data needed to draw texts with the same font in the single batch */
            TMap<GpuFont*,FontData,THashRaw<GpuFont*>> fontMap;
            TArray<class GraphicsText*> sortedTexts;
        };

    }
}

#endif //BERSERK_GRAPHICSTEXTRENDERER_H