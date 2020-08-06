/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RHI/RHIDevice.h>
#include <GraphicsItems.h>
#include <GraphicsRenderer.h>
#include <ShaderManager.h>
#include <Containers/TAlgo.h>
#include <RenderResources/GraphicsPipelineBuilder.h>

namespace Berserk {
    namespace Render {

        GraphicsRenderer::GraphicsRenderer(Graphics &graphics) {
            mGraphics = &graphics;
            initialize();
        }

        void GraphicsRenderer::draw(RHIDrawList &drawList) {
            auto& target = mGraphics->getTarget();
            auto& region = mGraphics->getRegion();
            target->bind(drawList, { /** No clear */ }, region);

            // Update caches
            if (mGraphics->isDirty()) {
                prepareData();
            }

            mTexturesRenderer.draw(drawList);
            mPrimitivesRenderer.draw(drawList);
        }

        void GraphicsRenderer::initialize() {
            mTexturesRenderer.setGraphics(mGraphics);
            mPrimitivesRenderer.setGraphics(mGraphics);
            mTexturesRenderer.init();
            mPrimitivesRenderer.init();
        }

        void GraphicsRenderer::clearState() {
            mTexturesRenderer.clear();
            mPrimitivesRenderer.clear();
        }

        void GraphicsRenderer::prepareData() {
            mTexturesRenderer.prepare();
            mPrimitivesRenderer.prepareData();
        }

    }
}