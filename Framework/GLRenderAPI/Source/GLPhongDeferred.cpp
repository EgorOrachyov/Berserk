//
// Created by Egor Orachyov on 19.08.2018.
//

#include "Pipeline/GLPhongDeferred.h"
#include "Render/GLRenderDriver.h"
#include "Render/GLRenderSystem.h"

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

        CHAR buffer[BUFFER_SIZE_64];

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

        mUniform.NUM_DIR_L = mProgram.getUniformLocation("NUM_DIR_L");
        mUniform.NUM_SPOT_L = mProgram.getUniformLocation("NUM_SPOT_L");
        mUniform.NUM_POINT_L = mProgram.getUniformLocation("NUM_POINT_L");

        //mUniform.NUM_DIR_SL = mProgram.getUniformLocation("NUM_DIR_SL");
        //mUniform.NUM_SPOT_SL = mProgram.getUniformLocation("NUM_SPOT_SL");
        //mUniform.NUM_POINT_SL = mProgram.getUniformLocation("NUM_POINT_SL");

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

        const Vector3f& CameraPos = camera->getPosition();
        const CameraComponent::Viewport& Port = camera->getComponent()->mViewport;

        mProgram.use();

        //render->getStageOutBuffer()->useAsFBO();

        driver->setDefaultBuffer();
        driver->setClearColor(render->getClearColor());
        driver->clearBuffer();
        driver->enableDepthTest(false);
        driver->enableFaceCulling(true);
        driver->setBackCulling();
        driver->setWindingOrderCCW();
        //driver->setViewPort(0, 0, render->getPixelWindowWidth(), render->getPixelWindowHeight());
        driver->setViewPort(Port.posX, Port.posY, Port.width, Port.height);

        mProgram.setUniform(mUniform.AmbientLight, render->getAmbientLightSource());
        mProgram.setUniform(mUniform.CameraPosition, CameraPos);

        mProgram.setUniform(mUniform.gPosition, 0);
        mProgram.setUniform(mUniform.gNormal, 1);
        mProgram.setUniform(mUniform.gDiffuse, 2);
        mProgram.setUniform(mUniform.gSpecularSh, 3);

        render->getGBuffer()->useAsUniform();

        mProgram.setUniform(mUniform.NUM_DIR_L, render->getDirectionalLightSources().getSize());
        mProgram.setUniform(mUniform.NUM_SPOT_L, render->getSpotLightSources().getSize());
        mProgram.setUniform(mUniform.NUM_POINT_L, render->getPointLightSources().getSize());

        List<SpotLight*> &spot = render->getSpotLightSources();
        for(UINT32 i = 0; i < spot.getSize(); i++)
        {   printf("s ");
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
        {   printf("p ");
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
        {   printf("d ");
            DirectionalLightComponent* current = dir.get(i)->getComponent();
            mProgram.setUniform(mUniform.dirLight[i].Direction, current->mDirection);
            mProgram.setUniform(mUniform.dirLight[i].Intensity, current->mLightIntensity);
        }

        render->getScreenPlane()->use();
        driver->setDefaultBuffer();
    }

} // namespace Berserk