//
// Created by Egor Orachyov on 10.08.2018.
//

#include "Pipeline/GLPhongModel.h"
#include "Render/GLRenderSystem.h"
#include "Render/GLRenderDriver.h"

namespace Berserk
{

    void GLPhongModel::init()
    {
        mProgram.init();
        mProgram.compileShader("../GLRenderAPI/Shaders/MainProcess/GLSLPhongModel_PN.vert", GLShaderType::GLST_VERTEX);
        mProgram.compileShader("../GLRenderAPI/Shaders/MainProcess/GLSLPhongModel_PN.frag", GLShaderType::GLST_FRAGMENT);
        mProgram.link();
        mProgram.validate();

        mUniform.ModelView = mProgram.getUniformLocation("ModelView");
        mUniform.MVP = mProgram.getUniformLocation("MVP");

        mUniform.Material.Ambient = mProgram.getUniformLocation("Material.Ambient");
        mUniform.Material.Diffuse = mProgram.getUniformLocation("Material.Diffuse");
        mUniform.Material.Specular = mProgram.getUniformLocation("Material.Specular");
        mUniform.Material.Shininess = mProgram.getUniformLocation("Material.Shininess");

        mUniform.ambientLight = mProgram.getUniformLocation("ambientLight");

        CHAR buffer[BUFFER_SIZE_64];

        for(UINT32 i = 0; i < LightInfo::LI_MAX_DIRECTIONAL_LIGHTS; i++)
        {
            sprintf(buffer, "directionalLights[%u].Direction", i);
            mUniform.directLights[i].Direction = mProgram.getUniformLocation(buffer);

            sprintf(buffer, "directionalLights[%u].Intensity", i);
            mUniform.directLights[i].Intensity = mProgram.getUniformLocation(buffer);
        }

        for(UINT32 i = 0; i < LightInfo::LI_MAX_POINT_LIGHTS; i++)
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

        for(UINT32 i = 0; i < LightInfo::LI_MAX_SPOT_LIGHTS; i++)
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

        mUniform.NUM_OF_DIR_LIGHTS = mProgram.getUniformLocation("NUM_OF_DIR_LIGHTS");
        mUniform.NUM_OF_SPOT_LIGHTS = mProgram.getUniformLocation("NUM_OF_SPOT_LIGHTS");
        mUniform.NUM_OF_POINT_LIGHTS = mProgram.getUniformLocation("NUM_OF_POINT_LIGHTS");
    }

    void GLPhongModel::destroy()
    {
        mProgram.destroy();
    }

    void GLPhongModel::execute()
    {
        auto render = dynamic_cast<GLRenderSystem*>(gRenderSystem);
        auto driver = dynamic_cast<GLRenderDriver*>(gRenderDriver);

        /////////////////////
        ///     Debug     ///
        /////////////////////

        Camera* camera = render->getRenderCamera();
        if (camera == nullptr) return;

        const Matrix4x4f& View = camera->getComponent()->mView;
        const Matrix4x4f& Proj = camera->getComponent()->mProjection;
        const CameraComponent::Viewport& Port = camera->getComponent()->mViewport;

        driver->setClearColor(render->getClearColor());
        driver->clearBuffer();
        driver->enableDepthTest(true);
        driver->setViewPort(Port.posX, Port.posY, Port.width, Port.height);

        mProgram.use();
        mProgram.setUniform(mUniform.ambientLight, render->getAmbientLightSource());
        mProgram.setUniform(mUniform.NUM_OF_DIR_LIGHTS, render->getDirectionalLightSources().getSize());
        mProgram.setUniform(mUniform.NUM_OF_SPOT_LIGHTS, render->getSpotLightSources().getSize());
        mProgram.setUniform(mUniform.NUM_OF_POINT_LIGHTS, render->getPointLightSources().getSize());

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

        render->mRenderNodeList.iterate(true);
        while (render->mRenderNodeList.iterate())
        {
            if (render->mRenderNodeList.getCurrent().isVisible())
            {
                Material* material = render->mRenderNodeList.getCurrent().getMaterial();
                RenderMesh* mesh = render->mRenderNodeList.getCurrent().getRenderMesh();

                if (material->getType() == MT_BASIC && mesh->getType() == MT_PN)
                {
                    mProgram.setUniform(mUniform.Material.Ambient, material->getAmbientComponent());
                    mProgram.setUniform(mUniform.Material.Diffuse, material->getDiffuseComponent());
                    mProgram.setUniform(mUniform.Material.Specular, material->getSpecularComponent());
                    mProgram.setUniform(mUniform.Material.Shininess, material->getShininess());

                    Matrix4x4f ModelView = View * (render->mRenderNodeList.getCurrent().getTransformation());
                    Matrix4x4f MVP = Proj * ModelView;

                    mProgram.setUniform(mUniform.ModelView, ModelView);
                    mProgram.setUniform(mUniform.MVP, MVP);

                    mesh->getGPUBuffer().drawIndices();
                }
            }
        }
    }

} // namespace Berserk