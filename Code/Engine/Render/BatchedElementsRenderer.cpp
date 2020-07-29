/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <BatchedElementsRenderer.h>
#include <ShaderCore/ShaderManager.h>

namespace Berserk {
    namespace Render {

        BatchedElementsRenderer::BatchedElementsRenderer() {

        }

        void BatchedElementsRenderer::draw(const ViewData &viewData, const BatchedElements &batch, RHIDrawList &drawList) {

        }

        void BatchedElementsRenderer::initializePipeline() {
            auto& manager = ShaderManager::getSingleton();

            mLinesShader = manager.load("BatchedLines");

        }

        void BatchedElementsRenderer::prepareData(const BatchedElements &elements) {
            
        }

    }
}