//
// Created by Egor Orachyov on 19.08.2018.
//

#include "Pipeline/GLPhongDeferred.h"
#include "Render/GLRenderDriver.h"
#include "Render/GLRenderSystem.h"
#include "Misc/Buffers.h"

namespace Berserk
{

    void GLPhongDeferred::init()
    {
        mProgram.init();
        mProgram.compileShader("../GLRenderAPI/Shaders/MainProcess/Deferred/GLSLPhongShadow.vert", GLShaderType::GLST_VERTEX);
        mProgram.compileShader("../GLRenderAPI/Shaders/MainProcess/Deferred/GLSLPhongShadow.frag", GLShaderType::GLST_FRAGMENT);
        mProgram.link();
        mProgram.validate();

        mUniform.CameraPosition = mProgram.getUniformLocation("CameraPosition");
        mUniform.AmbientLight = mProgram.getUniformLocation("AmbientLight");

        mUniform.gPosition = mProgram.getUniformLocation("gPosition");
        mUniform.gNormal = mProgram.getUniformLocation("gNormal");
        mUniform.gDiffuse = mProgram.getUniformLocation("gDiffuse");
        mUniform.gSpecularSh = mProgram.getUniformLocation("gSpecularSh");
        mUniform.gSSAO = mProgram.getUniformLocation("gSSAO");

        CHAR buffer[Buffers::SIZE_64];

        for(UINT32 i = 0; i < LightInfo::LI_MAX_DIRECTIONAL_LIGHTS; i++)
        {
            sprintf(buffer, "dirLight[%u].Direction", i);
            mUniform.dirLight[i].Direction = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "dirLight[%u].Intensity", i);
            mUniform.dirLight[i].Intensity = mProgram.getUniformLocation(buffer);
        }

        for(UINT32 i = 0; i < LightInfo::LI_MAX_POINT_LIGHTS; i++)
        {
            sprintf(buffer, "pointLight[%u].Position", i);
            mUniform.pointLight[i].Position = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLight[%u].Intensity", i);
            mUniform.pointLight[i].Intensity = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLight[%u].constant", i);
            mUniform.pointLight[i].constant = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLight[%u].linear", i);
            mUniform.pointLight[i].linear = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLight[%u].quadratic", i);
            mUniform.pointLight[i].quadratic = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointLight[%u].radius", i);
            mUniform.pointLight[i].radius = mProgram.getUniformLocation(buffer);
        }

        for(UINT32 i = 0; i < LightInfo::LI_MAX_SPOT_LIGHTS; i++)
        {
            sprintf(buffer, "spotLight[%u].Position", i);
            mUniform.spotLight[i].Position = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLight[%u].Direction", i);
            mUniform.spotLight[i].Direction = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLight[%u].Intensity", i);
            mUniform.spotLight[i].Intensity = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLight[%u].cutoff", i);
            mUniform.spotLight[i].cutoff = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLight[%u].outerCutoff", i);
            mUniform.spotLight[i].outerCutoff = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLight[%u].epsilon", i);
            mUniform.spotLight[i].epsilon = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotLight[%u].exponent", i);
            mUniform.spotLight[i].exponent = mProgram.getUniformLocation(buffer);
        }

        for(UINT32 i = 0; i < ShadowInfo::SI_MAX_DIR_SHADOW_SOURCES; i++)
        {
            sprintf(buffer, "dirSLight[%u].Direction", i);
            mUniform.dirSLight[i].Direction = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "dirSLight[%u].Intensity", i);
            mUniform.dirSLight[i].Intensity = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "dirMat[%u]", i);
            mUniform.dirMat[i] = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "dirMap[%u]", i);
            mUniform.dirMap[i] = mProgram.getUniformLocation(buffer);
        }

        for(UINT32 i = 0; i < ShadowInfo::SI_MAX_POINT_SHADOW_SOURCES; i++)
        {
            sprintf(buffer, "pointSLight[%u].Position", i);
            mUniform.pointSLight[i].Position = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointSLight[%u].Intensity", i);
            mUniform.pointSLight[i].Intensity = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointSLight[%u].constant", i);
            mUniform.pointSLight[i].constant = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointSLight[%u].linear", i);
            mUniform.pointSLight[i].linear = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointSLight[%u].quadratic", i);
            mUniform.pointSLight[i].quadratic = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointSLight[%u].radius", i);
            mUniform.pointSLight[i].radius = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "pointMap[%u]", i);
            mUniform.pointMap[i] = mProgram.getUniformLocation(buffer);
        }

        for(UINT32 i = 0; i < ShadowInfo::SI_MAX_SPOT_SHADOW_SOURCES; i++)
        {
            sprintf(buffer, "spotSLight[%u].Position", i);
            mUniform.spotSLight[i].Position = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotSLight[%u].Direction", i);
            mUniform.spotSLight[i].Direction = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotSLight[%u].Intensity", i);
            mUniform.spotSLight[i].Intensity = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotSLight[%u].cutoff", i);
            mUniform.spotSLight[i].cutoff = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotSLight[%u].outerCutoff", i);
            mUniform.spotSLight[i].outerCutoff = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotSLight[%u].epsilon", i);
            mUniform.spotSLight[i].epsilon = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotSLight[%u].exponent", i);
            mUniform.spotSLight[i].exponent = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotMat[%u]", i);
            mUniform.spotMat[i] = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "spotMap[%u]", i);
            mUniform.spotMap[i] = mProgram.getUniformLocation(buffer);
        }

        mUniform.NUM_DIR_L = mProgram.getUniformLocation("NUM_DIR_L");
        mUniform.NUM_SPOT_L = mProgram.getUniformLocation("NUM_SPOT_L");
        mUniform.NUM_POINT_L = mProgram.getUniformLocation("NUM_POINT_L");

        mUniform.NUM_DIR_SL = mProgram.getUniformLocation("NUM_DIR_SL");
        mUniform.NUM_SPOT_SL = mProgram.getUniformLocation("NUM_SPOT_SL");
        mUniform.NUM_POINT_SL = mProgram.getUniformLocation("NUM_POINT_SL");

        mUniform.SSAOPass_Use = mProgram.getSubroutineIndex("SSAOPass_Use", GLShaderType::GLST_FRAGMENT);
        mUniform.SSAOPass_NoUse = mProgram.getSubroutineIndex("SSAOPass_NoUse", GLShaderType::GLST_FRAGMENT);
    }

    void GLPhongDeferred::destroy()
    {
        mProgram.destroy();
    }

    void GLPhongDeferred::execute()
    {
        auto render = dynamic_cast<GLRenderSystem*>(gRenderSystem);
        auto driver = dynamic_cast<GLRenderDriver*>(gRenderDriver);

        Camera* camera = render->getRenderCamera();
        if (camera == nullptr) return;

        const Vector3f& CameraPos = camera->getWorldPosition();
        const CameraComponent::Viewport& Port = camera->getComponent()->mViewport;

        render->getStageOutBuffer()->useAsFBO();

        driver->setClearColor(render->getClearColor());
        driver->clearBuffer();
        driver->enableDepthTest(false);
        driver->enableFaceCulling(true);
        driver->setBackCulling();
        driver->setWindingOrderCCW();
        driver->setViewPort(0, 0, render->getPixelWindowWidth(), render->getPixelWindowHeight());

        mProgram.use();
        mProgram.setUniform(mUniform.AmbientLight, render->getAmbientLightSource());
        mProgram.setUniform(mUniform.CameraPosition, CameraPos);

        mProgram.setUniform(mUniform.gPosition, 0);
        mProgram.setUniform(mUniform.gNormal, 1);
        mProgram.setUniform(mUniform.gDiffuse, 2);
        mProgram.setUniform(mUniform.gSpecularSh, 3);

        // Without this loop does not work

        for (INT32 i = 0; i < ShadowInfo::SI_MAX_POINT_SHADOW_SOURCES; i++)
        {
            mProgram.setUniform(mUniform.pointMap[i], i + ShadowInfo::SI_POINT_MAP_SLOT0);
        }

        render->getGBuffer()->useAsUniformLayout(GBufferInfo::GBI_POSITION_SLOT, 0);
        render->getGBuffer()->useAsUniformLayout(GBufferInfo::GBI_NORMAL_SLOT, 1);
        render->getGBuffer()->useAsUniformLayout(GBufferInfo::GBI_DIFFUSE_COLOR_SLOT, 2);
        render->getGBuffer()->useAsUniformLayout(GBufferInfo::GBI_SPECULAR_COLOR_SH_SLOT, 3);

        if (render->isEnabledSSAO())
        {
            mProgram.setUniform(mUniform.gSSAO, 4);
            mProgram.setSubroutines(GLShaderType::GLST_FRAGMENT, 1, &mUniform.SSAOPass_Use);
            render->getSSAOBuffer()->useAsUniform(4);
        }
        else
        {
            mProgram.setUniform(mUniform.gSSAO, 4);
            mProgram.setSubroutines(GLShaderType::GLST_FRAGMENT, 1, &mUniform.SSAOPass_NoUse);
        }

        mProgram.setUniform(mUniform.NUM_DIR_L, render->getDirectionalLightSources().getSize());
        mProgram.setUniform(mUniform.NUM_SPOT_L, render->getSpotLightSources().getSize());
        mProgram.setUniform(mUniform.NUM_POINT_L, render->getPointLightSources().getSize());

        mProgram.setUniform(mUniform.NUM_DIR_SL, render->getDirectionalShadowSources().getSize());
        mProgram.setUniform(mUniform.NUM_SPOT_SL, render->getSpotShadowSources().getSize());
        mProgram.setUniform(mUniform.NUM_POINT_SL, render->getPointShadowSources().getSize());

        // Do not cast shadows

        List<SpotLight*> &spot = render->getSpotLightSources();
        for(UINT32 i = 0; i < spot.getSize(); i++)
        {
            SpotLightComponent* current = spot.get(i)->getComponent();
            mProgram.setUniform(mUniform.spotLight[i].Position, current->mPosition);
            mProgram.setUniform(mUniform.spotLight[i].Direction, current->mDirection);
            mProgram.setUniform(mUniform.spotLight[i].Intensity, current->mLightIntensity);
            mProgram.setUniform(mUniform.spotLight[i].cutoff, current->mCutoff);
            mProgram.setUniform(mUniform.spotLight[i].outerCutoff, current->mOuterCutoff);
            mProgram.setUniform(mUniform.spotLight[i].epsilon, current->mEpsilon);
            mProgram.setUniform(mUniform.spotLight[i].exponent, current->mAttenuationExponent);
        }

        List<PointLight*> &point = render->getPointLightSources();
        for(UINT32 i = 0; i < point.getSize(); i++)
        {
            PointLightComponent* current = point.get(i)->getComponent();
            mProgram.setUniform(mUniform.pointLight[i].Position, current->mPosition);
            mProgram.setUniform(mUniform.pointLight[i].Intensity, current->mLightIntensity);
            mProgram.setUniform(mUniform.pointLight[i].constant, current->mConstantAttenuation);
            mProgram.setUniform(mUniform.pointLight[i].linear, current->mLinearAttenuation);
            mProgram.setUniform(mUniform.pointLight[i].quadratic, current->mQuadraticAttenuation);
            mProgram.setUniform(mUniform.pointLight[i].radius, current->mRadius);
        }

        List<DirectionalLight*> &dir = render->getDirectionalLightSources();
        for(UINT32 i = 0; i < dir.getSize(); i++)
        {
            DirectionalLightComponent* current = dir.get(i)->getComponent();
            mProgram.setUniform(mUniform.dirLight[i].Direction, current->mDirection);
            mProgram.setUniform(mUniform.dirLight[i].Intensity, current->mLightIntensity);
        }

        // Cast shadows

        List<SpotLight*> &spots = render->getSpotShadowSources();
        for(UINT32 i = 0; i < spots.getSize(); i++)
        {
            SpotLightComponent* current = spots.get(i)->getComponent();
            mProgram.setUniform(mUniform.spotSLight[i].Position, current->mPosition);
            mProgram.setUniform(mUniform.spotSLight[i].Direction, current->mDirection);
            mProgram.setUniform(mUniform.spotSLight[i].Intensity, current->mLightIntensity);
            mProgram.setUniform(mUniform.spotSLight[i].cutoff, current->mCutoff);
            mProgram.setUniform(mUniform.spotSLight[i].outerCutoff, current->mOuterCutoff);
            mProgram.setUniform(mUniform.spotSLight[i].epsilon, current->mEpsilon);
            mProgram.setUniform(mUniform.spotSLight[i].exponent, current->mAttenuationExponent);

            ShadowCasterComponent* caster = spots.get(i)->getShadowCaster();
            mProgram.setUniform(mUniform.spotMat[i], caster->mProjection * caster->mView);
            mProgram.setUniform(mUniform.spotMap[i], ShadowInfo::SI_SPOT_MAP_SLOT0 + (INT32)i);

            render->getSpotDepthMaps()[i].useAsUniform();
        }

        List<PointLight*> &points = render->getPointShadowSources();
        for(UINT32 i = 0; i < points.getSize(); i++)
        {
            PointLightComponent* current = points.get(i)->getComponent();
            mProgram.setUniform(mUniform.pointSLight[i].Position, current->mPosition);
            mProgram.setUniform(mUniform.pointSLight[i].Intensity, current->mLightIntensity);
            mProgram.setUniform(mUniform.pointSLight[i].constant, current->mConstantAttenuation);
            mProgram.setUniform(mUniform.pointSLight[i].linear, current->mLinearAttenuation);
            mProgram.setUniform(mUniform.pointSLight[i].quadratic, current->mQuadraticAttenuation);
            mProgram.setUniform(mUniform.pointSLight[i].radius, current->mRadius);

            mProgram.setUniform(mUniform.pointMap[i], ShadowInfo::SI_POINT_MAP_SLOT0 + (INT32)i);

            render->getPointDepthMaps()[i].useAsUniform();
        }

        List<DirectionalLight*> &dirs = render->getDirectionalShadowSources();
        for(UINT32 i = 0; i < dirs.getSize(); i++)
        {
            DirectionalLightComponent* current = dirs.get(i)->getComponent();
            mProgram.setUniform(mUniform.dirSLight[i].Direction, current->mDirection);
            mProgram.setUniform(mUniform.dirSLight[i].Intensity, current->mLightIntensity);

            ShadowCasterComponent* caster = dirs.get(i)->getShadowCaster();
            mProgram.setUniform(mUniform.dirMat[i], caster->mProjection * caster->mView);
            mProgram.setUniform(mUniform.dirMap[i], ShadowInfo::SI_DIR_MAP_SLOT0 + (INT32)i);

            render->getDirDepthMaps()[i].useAsUniform();
        }

        render->getScreenPlane()->use();
        render->getStageOutBuffer()->disable();
    }

} // namespace Berserk