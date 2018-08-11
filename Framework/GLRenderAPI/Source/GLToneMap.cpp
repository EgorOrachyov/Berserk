//
// Created by Egor Orachyov on 10.08.2018.
//

#include "Pipeline/GLToneMap.h"
#include "Render/GLRenderDriver.h"
#include "Render/GLRenderSystem.h"

namespace Berserk
{

    void GLToneMap::init()
    {
        mProgram.init();
        mProgram.compileShader("../GLRenderAPI/Shaders/PostProcess/GLSLToneMap.vert", GLShaderType::GLST_VERTEX);
        mProgram.compileShader("../GLRenderAPI/Shaders/PostProcess/GLSLToneMap.frag", GLShaderType::GLST_FRAGMENT);
        mProgram.link();
        mProgram.validate();

        mUniform.HDRTex = mProgram.getUniformLocation("HDRTex");
        mUniform.Exposure = mProgram.getUniformLocation("Exposure");
        mUniform.Gamma = mProgram.getUniformLocation("Gamma");
    }

    void GLToneMap::destroy()
    {
        mProgram.destroy();
    }

    void GLToneMap::execute()
    {
        auto render = dynamic_cast<GLRenderSystem*>(gRenderSystem);
        auto driver = dynamic_cast<GLRenderDriver*>(gRenderDriver);

        mProgram.use();
        mProgram.setUniform(mUniform.HDRTex, 0);
        mProgram.setUniform(mUniform.Exposure, render->getExposure());
        mProgram.setUniform(mUniform.Gamma, render->getGammaCorrection());

        render->getStageInBuffer()->useAsUniformData();
        render->getStageOutBuffer()->useAsFBO();

        driver->setClearColor(render->getClearColor());
        driver->clearBuffer();
        driver->enableDepthTest(false);
        driver->setViewPort(0, 0, render->getPixelWindowWidth(), render->getOldPixelWindowHeight());

        render->getScreenPlane()->use();
        render->getStageOutBuffer()->disable();
    }

} // namespace Berserk