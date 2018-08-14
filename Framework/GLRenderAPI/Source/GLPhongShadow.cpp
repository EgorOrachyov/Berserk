//
// Created by Egor Orachyov on 12.08.2018.
//

#include "Pipeline/GLPhongShadow.h"
#include "Render/GLRenderSystem.h"
#include "Render/GLRenderDriver.h"

namespace Berserk
{

    void  GLPhongShadow::init()
    {
        mProgram.init();
        mProgram.compileShader("../GLRenderAPI/Shaders/MainProcess/GLSLPhongShadow_PN.vert", GLShaderType::GLST_VERTEX);
        mProgram.compileShader("../GLRenderAPI/Shaders/MainProcess/GLSLPhongShadow_PN.frag",
                               GLShaderType::GLST_FRAGMENT);
        mProgram.link();
        mProgram.validate();

        mUniform.Model = mProgram.getUniformLocation("Model");
        mUniform.ModelView = mProgram.getUniformLocation("ModelView");
        mUniform.MVP = mProgram.getUniformLocation("MVP");

        mUniform.Material.Ambient = mProgram.getUniformLocation("Material.Ambient");
        mUniform.Material.Diffuse = mProgram.getUniformLocation("Material.Diffuse");
        mUniform.Material.Specular = mProgram.getUniformLocation("Material.Specular");
        mUniform.Material.Shininess = mProgram.getUniformLocation("Material.Shininess");

        mUniform.ambientLight = mProgram.getUniformLocation("ambientLight");

        CHAR buffer[BUFFER_SIZE_64];

        for (UINT32 i = 0; i < LightInfo::LI_MAX_DIRECTIONAL_LIGHTS; i++)
        {
            sprintf(buffer, "directionalLights[%u].Direction", i);
            mUniform.directLights[i].Direction = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "directionalLights[%u].Intensity", i);
            mUniform.directLights[i].Intensity = mProgram.getUniformLocation(buffer);
        }

        for (UINT32 i = 0; i < LightInfo::LI_MAX_POINT_LIGHTS; i++)
        {
            sprintf(buffer, "pointLights[%u].Position", i);
            mUniform.pointLights[i].Position = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLights[%u].Intensity", i);
            mUniform.pointLights[i].Intensity = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLights[%u].constant", i);
            mUniform.pointLights[i].constant = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLights[%u].linear", i);
            mUniform.pointLights[i].linear = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLights[%u].quadratic", i);
            mUniform.pointLights[i].quadratic = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLights[%u].radius", i);
            mUniform.pointLights[i].radius = mProgram.getUniformLocation(buffer);
        }

        for (UINT32 i = 0; i < LightInfo::LI_MAX_SPOT_LIGHTS; i++)
        {
            sprintf(buffer, "spotLights[%u].Position", i);
            mUniform.spotLights[i].Position = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].Direction", i);
            mUniform.spotLights[i].Direction = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].Intensity", i);
            mUniform.spotLights[i].Intensity = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].cutoff", i);
            mUniform.spotLights[i].cutoff = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].outerCutoff", i);
            mUniform.spotLights[i].outerCutoff = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].epsilon", i);
            mUniform.spotLights[i].epsilon = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLights[%u].exponent", i);
            mUniform.spotLights[i].exponent = mProgram.getUniformLocation(buffer);
        }

        for (UINT32 i = 0; i < ShadowInfo::SI_MAX_DIR_SHADOW_SOURCES; i++)
        {
            sprintf(buffer, "dirShadowLights[%u].Direction", i);
            mUniform.dirShadowLights[i].Direction = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "dirShadowLights[%u].Intensity", i);
            mUniform.dirShadowLights[i].Intensity = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "dirShadowLights[%u].map", i);
            mUniform.dirShadowLights[i].map = mProgram.getUniformLocation(buffer);
        }

        for (UINT32 i = 0; i < ShadowInfo::SI_MAX_SPOT_SHADOW_SOURCES; i++)
        {
            sprintf(buffer, "spotShadowLights[%u].Position", i);
            mUniform.spotShadowLights[i].Position = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotShadowLights[%u].Direction", i);
            mUniform.spotShadowLights[i].Direction = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotShadowLights[%u].Intensity", i);
            mUniform.spotShadowLights[i].Intensity = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotShadowLights[%u].cutoff", i);
            mUniform.spotShadowLights[i].cutoff = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotShadowLights[%u].outerCutoff", i);
            mUniform.spotShadowLights[i].outerCutoff = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotShadowLights[%u].epsilon", i);
            mUniform.spotShadowLights[i].epsilon = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotShadowLights[%u].exponent", i);
            mUniform.spotShadowLights[i].exponent = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotShadowLights[%u].map", i);
            mUniform.spotShadowLights[i].map = mProgram.getUniformLocation(buffer);
        }

        for (UINT32 i = 0; i < ShadowInfo::SI_MAX_DIR_SHADOW_SOURCES; i++)
        {
            sprintf(buffer, "light_PV_dir[%u]", i);
            mUniform.light_PV_dir[i] = mProgram.getUniformLocation(buffer);
        }

        for (UINT32 i = 0; i < ShadowInfo::SI_MAX_SPOT_SHADOW_SOURCES; i++)
        {
            sprintf(buffer, "light_PV_spot[%u]", i);
            mUniform.light_PV_spot[i] = mProgram.getUniformLocation(buffer);
        }

        mUniform.NUM_OF_DIR_LIGHTS = mProgram.getUniformLocation("NUM_OF_DIR_LIGHTS");
        mUniform.NUM_OF_SPOT_LIGHTS = mProgram.getUniformLocation("NUM_OF_SPOT_LIGHTS");
        mUniform.NUM_OF_POINT_LIGHTS = mProgram.getUniformLocation("NUM_OF_POINT_LIGHTS");

        mUniform.NUM_OF_DIR_SHADOW_LIGHTS = mProgram.getUniformLocation("NUM_OF_DIR_SHADOW_LIGHTS");
        mUniform.NUM_OF_SPOT_SHADOW_LIGHTS = mProgram.getUniformLocation("NUM_OF_SPOT_SHADOW_LIGHTS");

        mUniform.NUM_DIR_SHADOWS = mProgram.getUniformLocation("NUM_DIR_SHADOWS");
        mUniform.NUM_SPOT_SHADOWS = mProgram.getUniformLocation("NUM_SPOT_SHADOWS");

        ///////

        debug.init();
        debug.compileShader("../GLRenderAPI/Shaders/MainProcess/Debug.vert", GLShaderType::GLST_VERTEX);
        debug.compileShader("../GLRenderAPI/Shaders/MainProcess/Debug.frag", GLShaderType::GLST_FRAGMENT);
        debug.link();
        debug.validate();
    }

    void  GLPhongShadow::destroy()
    {
        mProgram.destroy();
    }

    void  GLPhongShadow::execute()
    {
        auto render = dynamic_cast<GLRenderSystem*>(gRenderSystem);
        auto driver = dynamic_cast<GLRenderDriver*>(gRenderDriver);

        Camera* camera = render->getRenderCamera();
        if (camera == nullptr) return;

        const Matrix4x4f& View = camera->getComponent()->mView;
        const Matrix4x4f& Proj = camera->getComponent()->mProjection;


        render->getStageOutBuffer()->useAsFBO();

        driver->setClearColor(render->getClearColor());
        driver->clearBuffer();
        driver->enableDepthTest(true);
        driver->setViewPort(0, 0, render->getPixelWindowWidth(), render->getPixelWindowHeight());
        driver->enableFaceCulling(true);
        driver->setBackCulling();
        driver->setWindingOrderCCW();

        mProgram.use();
        mProgram.setUniform(mUniform.ambientLight, render->getAmbientLightSource());

        mProgram.setUniform(mUniform.NUM_OF_DIR_LIGHTS, render->getDirectionalLightSources().getSize());
        mProgram.setUniform(mUniform.NUM_OF_SPOT_LIGHTS, render->getSpotLightSources().getSize());
        mProgram.setUniform(mUniform.NUM_OF_POINT_LIGHTS, render->getPointLightSources().getSize());

        mProgram.setUniform(mUniform.NUM_OF_DIR_SHADOW_LIGHTS, render->getDirectionalShadowSources().getSize());
        mProgram.setUniform(mUniform.NUM_OF_SPOT_SHADOW_LIGHTS, render->getSpotShadowSources().getSize());
        //mProgram.setUniform(mUniform.NUM_OF_POINT_SHADOW_LIGHTS, render->getPointShadowSources().getSize());

        mProgram.setUniform(mUniform.NUM_DIR_SHADOWS, render->getDirectionalShadowSources().getSize());
        mProgram.setUniform(mUniform.NUM_SPOT_SHADOWS, render->getSpotShadowSources().getSize());
        //mProgram.setUniform(mUniform.NUM_POINT_SHADOWS, render->getPointShadowSources().getSize());

        for(UINT32 i = 0; i < render->getDirectionalShadowSources().getSize(); i++)
        {
            ShadowCasterComponent* caster = render->getDirectionalShadowSources().get(i)->getShadowCaster();
            mProgram.setUniform(mUniform.light_PV_dir[i], caster->mProjection * caster->mView);

            DirectionalLightComponent* current = render->getDirectionalShadowSources().get(i)->getComponent();
            mProgram.setUniform(mUniform.dirShadowLights[i].Direction, View * current->mDirection);
            mProgram.setUniform(mUniform.dirShadowLights[i].Intensity, current->mLightIntensity);
            mProgram.setUniform(mUniform.dirShadowLights[i].map, ShadowInfo::SI_DIR_MAP_SLOT0 + (INT32)i);

            render->getDirDepthMaps()[i].useAsUniform();
        }

        for(UINT32 i = 0; i < render->getSpotShadowSources().getSize(); i++)
        {
            ShadowCasterComponent* caster = render->getSpotShadowSources().get(i)->getShadowCaster();
            mProgram.setUniform(mUniform.light_PV_spot[i], caster->mProjection * caster->mView);

            SpotLightComponent* current = render->getSpotShadowSources().get(i)->getComponent();
            mProgram.setUniform(mUniform.spotShadowLights[i].Position, View * current->mPosition);
            mProgram.setUniform(mUniform.spotShadowLights[i].Direction, View * current->mDirection);
            mProgram.setUniform(mUniform.spotShadowLights[i].Intensity, current->mLightIntensity);
            mProgram.setUniform(mUniform.spotShadowLights[i].cutoff, current->mCutoff);
            mProgram.setUniform(mUniform.spotShadowLights[i].outerCutoff, current->mOuterCutoff);
            mProgram.setUniform(mUniform.spotShadowLights[i].epsilon, current->mEpsilon);
            mProgram.setUniform(mUniform.spotShadowLights[i].exponent, current->mAttenuationExponent);
            mProgram.setUniform(mUniform.spotShadowLights[i].map, ShadowInfo::SI_SPOT_MAP_SLOT0 + (INT32)i);

            render->getSpotDepthMaps()[i].useAsUniform();
        }

        List<SpotLight*> &spot = render->getSpotLightSources();
        for(UINT32 i = 0; i < spot.getSize(); i++)
        {
            SpotLightComponent* current = spot.get(i)->getComponent();
            mProgram.setUniform(mUniform.spotLights[i].Position, View * current->mPosition);
            mProgram.setUniform(mUniform.spotLights[i].Direction, View * current->mDirection);
            mProgram.setUniform(mUniform.spotLights[i].Intensity, current->mLightIntensity);
            mProgram.setUniform(mUniform.spotLights[i].cutoff, current->mCutoff);
            mProgram.setUniform(mUniform.spotLights[i].outerCutoff, current->mOuterCutoff);
            mProgram.setUniform(mUniform.spotLights[i].epsilon, current->mEpsilon);
            mProgram.setUniform(mUniform.spotLights[i].exponent, current->mAttenuationExponent);
        }

        List<PointLight*> &point = render->getPointLightSources();
        for(UINT32 i = 0; i < point.getSize(); i++)
        {
            PointLightComponent* current = point.get(i)->getComponent();
            mProgram.setUniform(mUniform.pointLights[i].Position, View * current->mPosition);
            mProgram.setUniform(mUniform.pointLights[i].Intensity, current->mLightIntensity);
            mProgram.setUniform(mUniform.pointLights[i].constant, current->mConstantAttenuation);
            mProgram.setUniform(mUniform.pointLights[i].linear, current->mLinearAttenuation);
            mProgram.setUniform(mUniform.pointLights[i].quadratic, current->mQuadraticAttenuation);
            mProgram.setUniform(mUniform.pointLights[i].radius, current->mRadius);
        }

        List<DirectionalLight*> &dir = render->getDirectionalLightSources();
        for(UINT32 i = 0; i < dir.getSize(); i++)
        {
            DirectionalLightComponent* current = dir.get(i)->getComponent();
            mProgram.setUniform(mUniform.directLights[i].Direction, View * current->mDirection);
            mProgram.setUniform(mUniform.directLights[i].Intensity, current->mLightIntensity);
        }

        List<RenderNode*> &renderNode = render->getRenderNodeSources();
        for(UINT32 i = 0; i < renderNode.getSize(); i++)
        {
            Material* material = renderNode.get(i)->getMaterial();
            RenderMesh* mesh = renderNode.get(i)->getRenderMesh();

            if (material->getType() == MT_BASIC && mesh->getType() == MT_PN)
            {
                mProgram.setUniform(mUniform.Material.Ambient, material->getAmbientComponent());
                mProgram.setUniform(mUniform.Material.Diffuse, material->getDiffuseComponent());
                mProgram.setUniform(mUniform.Material.Specular, material->getSpecularComponent());
                mProgram.setUniform(mUniform.Material.Shininess, material->getShininess());

                Matrix4x4f Model = renderNode.get(i)->getTransformation();
                Matrix4x4f ModelView = View * Model;
                Matrix4x4f MVP = Proj * ModelView;

                mProgram.setUniform(mUniform.Model, Model);
                mProgram.setUniform(mUniform.ModelView, ModelView);
                mProgram.setUniform(mUniform.MVP, MVP);

                mesh->getGPUBuffer().drawIndices();
            }
        }

        driver->setDefaultBuffer();
    }

} // namespace Berserk