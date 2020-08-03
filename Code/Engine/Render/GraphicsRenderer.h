/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICSRENDERER_H
#define BERSERK_GRAPHICSRENDERER_H

#include <Graphics.h>
#include <Shader.h>
#include <RenderResources/DynamicVertexBuffer.h>

namespace Berserk {
    namespace Render {

        /** Encapsulates logic, required for rendering graphics area on GPU */
        class GraphicsRenderer {
        public:

            /**
             * Construct renderer for provided graphics instance
             * @param graphics Graphics area to be drawn
             * @param target Actual GPU target (must support depth buffering for proper positioning)
             */
            GraphicsRenderer(Graphics& graphics, const TPtrShared<RenderTarget> &target);

            /**
             * Construct renderer for provided graphics instance
             * @param graphics Graphics area to be drawn
             * @param region Region of the target where to render data (by default use 0,0,g.w,g.h)
             * @param target Actual GPU target (must support depth buffering for proper positioning)
             */
            GraphicsRenderer(Graphics& graphics, const  Region2i& region, const TPtrShared<RenderTarget> &target);

            /** Draw graphics state with specified RHI draw list */
            void draw(class RHIDrawList& drawList);

        private:

            /** Init rendering pipeline for drawing */
            void initialize();

            /** Clear cache if graphics was updated */
            void clearState();

            /** Prepare data before rendering */
            void prepareData();

            /** Prepare textures only data before rendering */
            void prepareTexturesData();

            /** Graphics area to be drawn */
            Graphics* mGraphics;

            /** GPU target */
            TPtrShared<RenderTarget> mTarget;

            /** Viewport of the target where to render */
            Region2i mRegion;

            /** Scale required to fit graphics into viewport area */
            Vec2f mScale = Vec2f(1.0f,1.0f);

            struct GraphicsTexturesRendering {
                /** Positions and texture coords */
                DynamicVertexBuffer vertexData;

                /** Colors, transparency, etc. iterated over instances */
                DynamicVertexBuffer instanceData;

                /** Uniform data per texture */
                TArray<TPtrShared<RHIUniformSet>> uniforms;

                /** Textures to draw */
                TArray<GraphicsTexture*> texturesSorted;

                /** Program to draw textures on the GPU */
                TPtrShared<Shader> shader;
            } mTextures;
        };

    }
}

#endif //BERSERK_GRAPHICSRENDERER_H