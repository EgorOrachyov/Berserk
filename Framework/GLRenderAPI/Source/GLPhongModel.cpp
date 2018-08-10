//
// Created by Egor Orachyov on 10.08.2018.
//

#include "Pipeline/GLPhongModel.h"

namespace Berserk
{

    void GLPhongModel::init()
    {
        mProgram.init();
        mProgram.compileShader("../GLRenderAPI/Shaders/MainProcess/GLSLPhongModel.vert", GLShaderType::GLST_VERTEX);
        mProgram.compileShader("../GLRenderAPI/Shaders/MainProcess/GLSLPhongModel.frag", GLShaderType::GLST_FRAGMENT);
        mProgram.link();
        mProgram.validate();

        mUniform.ModelView = mProgram.getUniformLocation("ModelView");
        mUniform.MVP = mProgram.getUniformLocation("MVP");

        mUniform.Material.Ambient = mProgram.getUniformLocation("Material.Ambient");
        mUniform.Material.Diffuse = mProgram.getUniformLocation("Material.Diffuse");
        mUniform.Material.Specular = mProgram.getUniformLocation("Material.Specular");
        mUniform.Material.Shininess = mProgram.getUniformLocation("Material.Shininess");
        mUniform.Material.DiffuseMap = mProgram.getUniformLocation("Material.DiffuseMap");
        mUniform.Material.SpecularMap = mProgram.getUniformLocation("Material.SpecularMap");

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

    }

} // namespace Berserk