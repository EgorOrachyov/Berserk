/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICSTEXTURESRENDERER_H
#define BERSERK_GRAPHICSTEXTURESRENDERER_H

#include <Graphics.h>
#include <Shader.h>
#include <RHI/RHIDevice.h>
#include <RenderResources/DynamicVertexBuffer.h>
#include <RenderResources/UniformBufferWriter.h>
#include <RenderResources/DynamicUniformBuffer.h>

namespace Berserk {
    namespace Render {

        class GraphicsTexturesRenderer {
        public:

            void setGraphics(Graphics* g) { graphics = g; }
            void init();
            void clear();
            void prepare();
            void draw(RHIDrawList& drawList);

        private:

            Graphics* graphics = nullptr;

            uint32 vertices = 0;
            uint32 instancesWithAlpha = 0;
            uint32 instancesWithoutAlpha = 0;
            uint32 textureInfoBlockSizeAligned = 0;

            static const uint32 INDICES_COUNT = 6;
            static const uint32 VERTICES_COUNT = 4;

            TRef<const ShaderParam> pTexture;
            TRef<const ShaderUniformBlock> pTransform;
            TRef<const ShaderUniformBlock> pTextureInfo;

            /** Positions and texture coords */
            DynamicVertexBuffer vertexData;

            /** Array object with vertex and instance data */
            TPtrShared<RHIArrayObject> array;

            /** Static indices to draw simple rect */
            TPtrShared<RHIIndexBuffer> indices;

            /** Program to draw textures on the GPU */
            TPtrShared<Shader> shader;

            /** Data passed to shader as uniform block */
            UniformBuffer transform;

            /** Writer to update TextureInfo blocks */
            UniformBufferWriter uniformBufferWriter;

            /** TextureInfo uniform data (for all textures) */
            DynamicUniformBuffer uniformData;
        };

    }
}




#endif //BERSERK_GRAPHICSTEXTURESRENDERER_H
