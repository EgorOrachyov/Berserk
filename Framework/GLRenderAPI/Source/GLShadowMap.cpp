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

        driver->setViewPort(0,0,render->getShadowMapSize(),render->getShadowMapSize());
        driver->enableDepthTest(true);
        driver->enableFaceCulling(true);
        driver->setFrontCulling();
        driver->setWindingOrderCCW();

        DepthMap* dirDepthMap = render->getDirDepthMaps();
        List<DirectionalLight*> &dir = render->getDirectionalShadowSources();

        for (UINT32 i = 0; i < dir.getSize(); i++)
        {
            Matrix4x4f& view = dir.get(i)->getShadowCaster()->mView;
            Matrix4x4f& proj = dir.get(i)->getShadowCaster()->mProjection;
            Matrix4x4f  ViewProj = proj * view;

            dirDepthMap[i].useAsFBO();
            driver->clearDepthBuffer();

            List<RenderNode*> &node = render->getRenderNodeSources();
            for (UINT32 j = 0; j < node.getSize(); j++)
            {
                RenderNode* current = node.get(j);

                if (current->getRenderNodeType() == RenderNodeType::RNT_OBJECT)
                {
                    mDirectional.setUniform(mUniform.MVP, ViewProj * current->getTransformation());
                    current->getShadowMesh()->getGPUBuffer().drawIndices();
                }
            }
        }

        DepthMap* spotDepthMap = render->getSpotDepthMaps();
        List<SpotLight*> &spot = render->getSpotShadowSources();

        for (UINT32 i = 0; i < spot.getSize(); i++)
        {
            Matrix4x4f view = spot.get(i)->getShadowCaster()->mView;
            Matrix4x4f proj = spot.get(i)->getShadowCaster()->mProjection;
            Matrix4x4f  ViewProj = proj * view;

            spotDepthMap[i].useAsFBO();
            driver->clearDepthBuffer();

            List<RenderNode*> &node = render->getRenderNodeSources();
            for (UINT32 j = 0; j < node.getSize(); j++)
            {
                RenderNode* current = node.get(j);

                if (current->getRenderNodeType() == RenderNodeType::RNT_OBJECT)
                {
                    mDirectional.setUniform(mUniform.MVP, ViewProj * current->getTransformation());
                    current->getShadowMesh()->getGPUBuffer().drawIndices();
                }
            }
        }

        return;

        driver->setBackCulling();

        if (spot.getSize() == 0)
            return;

        debug.use();
        debug.setUniform("Depth", ShadowInfo::SI_SPOT_MAP_SLOT0);

        spotDepthMap[0].useAsUniform();

        driver->setDefaultBuffer();
        driver->enableDepthTest(false);
        driver->setViewPort(0,0,render->getPixelWindowWidth(),render->getPixelWindowHeight());

        render->getScreenPlane()->use();
    }

} // namespace Berserk