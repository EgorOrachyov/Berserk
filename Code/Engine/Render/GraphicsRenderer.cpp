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

            mPrimitivesRenderer.draw(drawList);
            mTextRenderer.draw(drawList);
            mTexturesRenderer.draw(drawList);
        }

        void GraphicsRenderer::initialize() {
            mPrimitivesRenderer.setGraphics(mGraphics);
            mTextRenderer.setGraphics(mGraphics);
            mTexturesRenderer.setGraphics(mGraphics);
            mPrimitivesRenderer.init();
            mTextRenderer.init();
            mTexturesRenderer.init();
        }

        void GraphicsRenderer::clearState() {
            mPrimitivesRenderer.clear();
            mTextRenderer.clear();
            mTexturesRenderer.clear();
        }

        void GraphicsRenderer::prepareData() {
            mPrimitivesRenderer.prepareData();
            mTextRenderer.prepareData();
            mTexturesRenderer.prepare();
        }

    }
}