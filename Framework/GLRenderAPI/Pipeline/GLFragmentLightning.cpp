//
// Created by Egor Orachyov on 29.07.2018.
//

#include "GLFragmentLightning.h"

namespace Berserk
{

    GLFragmentLightning::GLFragmentLightning()
    {

    }

    GLFragmentLightning::~GLFragmentLightning()
    {
        mProgram.destroy();
    }

    void GLFragmentLightning::init()
    {
        // Init shader mProgram to use by this pipeline step

        mProgram.init();
        mProgram.compileShader("../GLRenderAPI/Debug/FragmentLight.vert", GLShaderType::GLST_VERTEX);
        mProgram.compileShader("../GLRenderAPI/Debug/FragmentLight.frag", GLShaderType::GLST_FRAGMENT);
        mProgram.link();
        mProgram.validate();

        // Get and save locations of the uniforms

        // todo: add uniform table to bind uniforms locations
    }

    void GLFragmentLightning::destroy()
    {

    }

    GLFrameBufferObject* GLFragmentLightning::process(RenderManager *manager, GLFrameBufferObject *object)
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mProgram.use();

        // uniform block
        {
            mProgram.setUniform("Light.Position", manager->getCamera()->mView * manager->getPointLights().get(0)->mPosition);
            mProgram.setUniform("Light.La", manager->getPointLights().get(0)->mLightIntensity);
            mProgram.setUniform("Light.Ld", manager->getPointLights().get(0)->mLightIntensity);
            mProgram.setUniform("Light.Ls", manager->getPointLights().get(0)->mLightIntensity);

            mProgram.setUniform("Material.Ka", manager->getBaseRenderMaterials().get(0).mMaterialComponent->mAmbient);
            mProgram.setUniform("Material.Kd", manager->getBaseRenderMaterials().get(0).mMaterialComponent->mDiffuse);
            mProgram.setUniform("Material.Ks", manager->getBaseRenderMaterials().get(0).mMaterialComponent->mSpecular);
            mProgram.setUniform("Material.Shininess", manager->getBaseRenderMaterials().get(0).mMaterialComponent->mShininess);

            mProgram.setUniform("ModelView", manager->getCamera()->mView *
                    (*manager->getBaseRenderMaterials().get(0).mTransformationComponent));
            mProgram.setUniform("MVP", manager->getCamera()->mProjection *
                    manager->getCamera()->mView *
                    (*manager->getBaseRenderMaterials().get(0).mTransformationComponent));

            manager->getBaseRenderMaterials().get(0).mRenderBufferComponent->drawIndices();
        }

        return NULL;
    }

}