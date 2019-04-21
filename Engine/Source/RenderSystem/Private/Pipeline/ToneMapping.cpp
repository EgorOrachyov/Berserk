//
// Created by Egor Orachyov on 12.04.2019.
//

#include "Pipeline/ToneMapping.h"

namespace Berserk::Render
{

    ToneMapping::ToneMapping(const char *name, IAllocator *allocator) : IPipelineStage(name, allocator)
    {
        mScreenRender = mShaderManager->loadShader("{SHADERS}/ToneMapping/meta-info.xml");
        mScreenPlane = mBufferManager->getGPUBuffer("ScreenPlane");

        PUSH("Stage: ToneMapping: initialized");
    }

    ToneMapping::~ToneMapping()
    {
        mShaderManager->deleteShader(mScreenRender);
        mBufferManager->deleteGPUBuffer(mScreenPlane);

        PUSH("Stage: ToneMapping: de-initialized");
    }

    void ToneMapping::reload()
    {
        /** Nothing */
    }

    void ToneMapping::execute(RenderPassInfo &passInfo)
    {
        mRenderDriver->bindDefaultFrameBuffer();
        mRenderDriver->depthTest(false);
        mRenderDriver->clear(true, false, false);
        mRenderDriver->viewPort(passInfo.getDisplayViewPort());

        RenderInfoToneMapping& ToneMapping = mRenderSystem->getToneMappingInfo();

        mScreenRender->use();
        mScreenRender->setUniform("Texture0", 0u);
        mScreenRender->setUniform("Exposure", ToneMapping.Exposure);
        mScreenRender->setUniform("GammaCorrection", ToneMapping.GammaCorrection);

        passInfo.getFrameBuffer()->bindColorBuffer(0u, 0u);

        mScreenPlane->draw();
    }

} // namespace Berserk::Render