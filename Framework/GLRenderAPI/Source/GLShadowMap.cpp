//
// Created by Egor Orachyov on 12.08.2018.
//

#include "Pipeline/GLShadowMap.h"
#include "Render/GLRenderSystem.h"
#include "Render/GLRenderDriver.h"
#include "Math/UtilityMatrices.h"

namespace Berserk
{

    void GLShadowMap::init()
    {
        mDirectional.init();
        mDirectional.compileShader("../GLRenderAPI/Shaders/PreProcess/GLSLShadowMapGen.vert", GLShaderType::GLST_VERTEX);
        mDirectional.compileShader("../GLRenderAPI/Shaders/PreProcess/GLSLShadowMapGen.frag", GLShaderType::GLST_FRAGMENT);
        mDirectional.link();
        mDirectional.validate();

        mUniform.MVP = mDirectional.getUniformLocation("MVP");

        debug.init();
        debug.compileShader("../GLRenderAPI/Shaders/PreProcess/GLSLShadowDebug.vert", GLShaderType::GLST_VERTEX);
        debug.compileShader("../GLRenderAPI/Shaders/PreProcess/GLSLShadowDebug.frag", GLShaderType::GLST_FRAGMENT);
        debug.link();
        debug.validate();
    }

    void GLShadowMap::destroy()
    {
        mDirectional.destroy();
    }

    void GLShadowMap::execute()
    {
        auto render = dynamic_cast<GLRenderSystem*>(gRenderSystem);
        auto driver = dynamic_cast<GLRenderDriver*>(gRenderDriver);

        mDirectional.use();

        DepthMap* depthMap = render->getDepthMaps();
        List<Light*> &dir = render->getDirectionalSources();
        for (UINT32 i = 0; i < dir.getSize(); i++)
        {
            Matrix4x4f& view = dir.get(i)->getShadowCaster()->mView;
            Matrix4x4f& proj = dir.get(i)->getShadowCaster()->mProjection;

            //////////////////////////////////////////////////

            //view = lookAt(Vector3f(0,0,10), Vector3f(0,0,0), Vector3f(0,1,0));
            proj = orthographic(-12,12,-12,12,0,30);

            //////////////////////////////////////////////////


            depthMap[i].useAsFBO();
            driver->clearDepthBuffer();
            driver->enableDepthTest(true);

            List<RenderNode*> &node = render->getRenderNodeSources();
            for (UINT32 j = 0; j < node.getSize(); j++)
            {
                RenderNode* current = node.get(j);

                if (current->getRenderNodeType() == RenderNodeType::RNT_OBJECT)
                {
                    mDirectional.setUniform(mUniform.MVP, proj * view * current->getTransformation());
                    current->getShadowMesh()->getGPUBuffer().drawIndices();
                }
            }
        }

        if (dir.getSize() == 0)
            return;

        debug.use();
        debug.setUniform("Depth", 0);

        depthMap[0].useAsUniform();

        driver->setDefaultBuffer();
        driver->enableDepthTest(false);
        driver->setViewPort(0,0,render->getPixelWindowWidth(),render->getPixelWindowHeight());

        render->getScreenPlane()->use();
    }

} // namespace Berserk