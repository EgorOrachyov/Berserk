/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RHI/RHIDevice.h>
#include <RenderResources/GraphicsPipeline.h>
#include <RenderResources/GraphicsPipelineBuilder.h>

namespace Berserk {
    namespace Render {

        GraphicsPipeline::GraphicsPipeline(struct GraphicsPipelineBuilder &builder) {
            auto& desc = builder.mPipelineDesc;
            auto& device = RHIDevice::getSingleton();

            mShader = builder.mShaderProgram;
            mDeclaration = builder.mVertexDeclaration;
            mPipelineName = mShader->getName();

            BERSERK_COND_ERROR_RET(mShader->isDeclarationCompatible(*mDeclaration),
                                   "Declaration (%s) and shader program (%s) are not compatible",
                                   mDeclaration->getName().data(), mShader->getName().data());

            mPipelineRHI = device.createGraphicsPipeline(desc);
            BERSERK_COND_ERROR_RET(mPipelineRHI.isNotNull(), "Failed to create graphics pipeline: %s", mPipelineName.data());
        }

        bool GraphicsPipeline::isInitialized() const {
            return mPipelineRHI.isNotNull();
        }

        bool GraphicsPipeline::isInitializedRHI() const {
            return mPipelineRHI.isNotNull();
        }

        CString GraphicsPipeline::getFriendlyName() const {
            return mPipelineName;
        }

        void GraphicsPipeline::bind(Berserk::RHIDrawList &drawList) {
            BERSERK_COND_ERROR_RET(mPipelineRHI.isNotNull(), "Null RHI resource");
            drawList.bindPipeline(mPipelineRHI);
        }

    }
}