/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Tmp/Resources/GraphicsPipeline.h>
#include <RenderTargets/RenderTarget.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Rendering {

        GraphicsPipeline::GraphicsPipeline(const TPtrShared<RHIShader> &shader, bool depthTest,
                                           bool depthWrite, EPrimitivesType primitivesType,
                                           EPolygonCullMode cullMode, EPolygonMode polygonMode,
                                           const RenderTarget &target) {
            auto& device = RHIDevice::getSingleton();

            RHIGraphicsPipelineDesc pipelineDesc;
            pipelineDesc.shader = shader;
            pipelineDesc.depthCompare = ECompareFunction::Less;
            pipelineDesc.depthTest = depthTest;
            pipelineDesc.depthWrite = depthWrite;
            pipelineDesc.lineWidth = 1.0f;
            pipelineDesc.primitivesType = primitivesType;
            pipelineDesc.polygonCullMode = cullMode;
            pipelineDesc.polygonMode = polygonMode;
            target.extractDeclaration(pipelineDesc);

            mPipeline = device.createGraphicsPipeline(pipelineDesc);
            if (mPipeline.isNotNull()) linkResource();
        }

        GraphicsPipeline::~GraphicsPipeline() {
            unlinkResource();
        }

        bool GraphicsPipeline::isInitialized() const {
            return mPipeline.isNotNull();
        }

        bool GraphicsPipeline::isInitializedRHI() const {
            return mPipeline.isNotNull();
        }

        CString GraphicsPipeline::getFriendlyName() const {
            return "GraphicsPipeline";
        }
    }
}