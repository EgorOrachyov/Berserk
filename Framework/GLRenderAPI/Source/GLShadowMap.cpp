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
        mDirectional.compileShader("../GLRenderAPI/Shaders/PreProcess/Shadow/GLSLShadowMap.vert", GLShaderType::GLST_VERTEX);
        mDirectional.compileShader("../GLRenderAPI/Shaders/PreProcess/Shadow/GLSLShadowMap.frag", GLShaderType::GLST_FRAGMENT);
        mDirectional.link();
        mDirectional.validate();

        mDirUniform.MVP = mDirectional.getUniformLocation("MVP");

        mOmnidirectional.init();
        mOmnidirectional.compileShader("../GLRenderAPI/Shaders/PreProcess/Shadow/GLSLShadowCubeMap.vert", GLShaderType::GLST_VERTEX);
        mOmnidirectional.compileShader("../GLRenderAPI/Shaders/PreProcess/Shadow/GLSLShadowCubeMap.geom", GLShaderType::GLST_GEOMETRY);
        mOmnidirectional.compileShader("../GLRenderAPI/Shaders/PreProcess/Shadow/GLSLShadowCubeMap.frag", GLShaderType::GLST_FRAGMENT);
        mOmnidirectional.link();
        mOmnidirectional.validate();

        mOmnUniform.Model = mOmnidirectional.getUniformLocation("Model");
        mOmnUniform.LightPos = mOmnidirectional.getUniformLocation("LightPos");
        mOmnUniform.FarPlane = mOmnidirectional.getUniformLocation("FarPlane");

        for(UINT32 i = 0; i < 6; i++)
        {
            CHAR  buffer[26];
            sprintf(buffer, "ShadowView[%u]", i);
            mOmnUniform.ShadowView[i] = mOmnidirectional.getUniformLocation(buffer);
        }

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

        driver->setViewPort(0,0,render->getShadowMapSize(),render->getShadowMapSize());
        driver->enableDepthTest(true);
        driver->enableFaceCulling(true);
        driver->setFrontCulling();
        driver->setWindingOrderCCW();

        mDirectional.use();

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

                if (current->getRenderNodeType() & RenderNodeType::RNT_CAST_SHADOWS)
                {
                    mDirectional.setUniform(mDirUniform.MVP, ViewProj * current->getTransformation());
                    current->getShadowMesh()->getGPUBuffer().drawIndices();
                }
            }
        }

        DepthMap* spotDepthMap = render->getSpotDepthMaps();
        List<SpotLight*> &spot = render->getSpotShadowSources();

        for (UINT32 i = 0; i < spot.getSize(); i++)
        {
            Matrix4x4f& view = spot.get(i)->getShadowCaster()->mView;
            Matrix4x4f& proj = spot.get(i)->getShadowCaster()->mProjection;
            Matrix4x4f  ViewProj = proj * view;

            spotDepthMap[i].useAsFBO();
            driver->clearDepthBuffer();

            List<RenderNode*> &node = render->getRenderNodeSources();
            for (UINT32 j = 0; j < node.getSize(); j++)
            {
                RenderNode* current = node.get(j);

                if (current->getRenderNodeType() & RenderNodeType::RNT_CAST_SHADOWS)
                {
                    mDirectional.setUniform(mDirUniform.MVP, ViewProj * current->getTransformation());
                    current->getShadowMesh()->getGPUBuffer().drawIndices();
                }
            }
        }

        mOmnidirectional.use();

        CubeDepthMap* pointDepthMap = render->getPointDepthMaps();
        List<PointLight*> &point = render->getPointShadowSources();

        for (UINT32 i = 0; i < point.getSize(); i++)
        {
            pointDepthMap[i].useAsFBO();
            driver->clearDepthBuffer();

            mOmnidirectional.setUniform(mOmnUniform.LightPos, point.get(i)->getComponent()->mPosition);
            mOmnidirectional.setUniform(mOmnUniform.FarPlane, point.get(i)->getComponent()->mRadius);

            for(UINT32 j = 0; j < 6; j++)
                mOmnidirectional.setUniform(mOmnUniform.ShadowView[j],
                                            point.get(i)->getShadowCaster()->mProjection *
                                            point.get(i)->getShadowCaster()->mView[j]);

            List<RenderNode*> &node = render->getRenderNodeSources();
            for (UINT32 j = 0; j < node.getSize(); j++)
            {
                RenderNode* current = node.get(j);

                if (current->getRenderNodeType() & RenderNodeType::RNT_CAST_SHADOWS)
                {
                    mOmnidirectional.setUniform(mOmnUniform.Model, current->getTransformation());
                    current->getShadowMesh()->getGPUBuffer().drawIndices();
                }
            }
        }
    }

} // namespace Berserk