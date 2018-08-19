//
// Created by Egor Orachyov on 10.08.2018.
//

#include "Pipeline/GLDeferredShading.h"
#include "Render/GLRenderDriver.h"
#include "Render/GLRenderSystem.h"

namespace Berserk
{

    void GLDeferredShading::init()
    {
        mProgram.init();
        mProgram.compileShader("../GLRenderAPI/Shaders/PreProcess/Deferred/GLSLDeferred.vert", GLShaderType::GLST_VERTEX);
        mProgram.compileShader("../GLRenderAPI/Shaders/PreProcess/Deferred/GLSLDeferred.frag", GLShaderType::GLST_FRAGMENT);
        mProgram.link();
        mProgram.validate();

        mUniform.Model = mProgram.getUniformLocation("Model");
        mUniform.PVM = mProgram.getUniformLocation("PVM");
        mUniform.VertPass_PN = mProgram.getSubroutineIndex("VertPass_PN", GLShaderType::GLST_VERTEX);
        mUniform.VertPass_PNT = mProgram.getSubroutineIndex("VertPass_PNT", GLShaderType::GLST_VERTEX);
        mUniform.VertPass_PNBTT = mProgram.getSubroutineIndex("VertPass_PNBTT", GLShaderType::GLST_VERTEX);

        mUniform.DiffuseColor = mProgram.getUniformLocation("DiffuseColor");
        mUniform.SpecularColor = mProgram.getUniformLocation("SpecularColor");
        mUniform.Shininess = mProgram.getUniformLocation("Shininess");

        mUniform.DiffuseMap = mProgram.getUniformLocation("DiffuseMap");
        mUniform.SpecularMap = mProgram.getUniformLocation("SpecularMap");
        mUniform.NormalMap = mProgram.getUniformLocation("NormalMap");

        mUniform.FragPass_PN = mProgram.getSubroutineIndex("FragPass_PN", GLShaderType::GLST_FRAGMENT);
        mUniform.FragPass_PNT = mProgram.getSubroutineIndex("FragPass_PNT", GLShaderType::GLST_FRAGMENT);
        mUniform.FragPass_PNBTT = mProgram.getSubroutineIndex("FragPass_PNBTT", GLShaderType::GLST_FRAGMENT);

        mUniform.MatPass_Default = mProgram.getSubroutineIndex("MatPass_Default", GLShaderType::GLST_FRAGMENT);
        mUniform.MatPass_Basic = mProgram.getSubroutineIndex("MatPass_Basic", GLShaderType::GLST_FRAGMENT);
        mUniform.MatPass_DS_map = mProgram.getSubroutineIndex("MatPass_DS_map", GLShaderType::GLST_FRAGMENT);

        debug.init();
        debug.compileShader("../GLRenderAPI/Shaders/PreProcess/Deferred/Debug.vert", GLShaderType::GLST_VERTEX);
        debug.compileShader("../GLRenderAPI/Shaders/PreProcess/Deferred/Debug.frag", GLShaderType::GLST_FRAGMENT);
        debug.link();
        debug.validate();
    }

    void GLDeferredShading::destroy()
    {
        mProgram.destroy();
        debug.destroy();
    }

    void GLDeferredShading::execute()
    {
        const UINT32 FRAG_SUBROUTINES_COUNT = 2;

        auto render = dynamic_cast<GLRenderSystem*>(gRenderSystem);
        auto driver = dynamic_cast<GLRenderDriver*>(gRenderDriver);

        Camera* camera = render->getRenderCamera();
        if (camera == nullptr) return;

        const Matrix4x4f& View = camera->getComponent()->mView;
        const Matrix4x4f& Proj = camera->getComponent()->mProjection;
        const Matrix4x4f  PV   = Proj * View;

        render->getGBuffer()->useAsFBO();

        driver->setClearColor(render->getClearColor());
        driver->clearBuffer();
        driver->enableDepthTest(true);
        driver->setViewPort(0, 0, render->getPixelWindowWidth(), render->getPixelWindowHeight());
        driver->enableFaceCulling(true);
        driver->setBackCulling();
        driver->setWindingOrderCCW();

        mProgram.use();
        UINT32 subroutines[FRAG_SUBROUTINES_COUNT];

        List<RenderNode*> &node = render->getRenderNodeSources();
        for (UINT32 i = 0; i < node.getSize(); ++i)
        {
            Material* material = node.get(i)->getMaterial();
            RenderMesh* mesh = node.get(i)->getRenderMesh();

            if (mesh->getType() == MeshType::MT_PN)
            {   printf("[%u] PN; ", i);
                mProgram.setSubroutines(GLShaderType::GLST_VERTEX, 1, &mUniform.VertPass_PN);
                subroutines[0] = mUniform.FragPass_PN;
            }
            else if (mesh->getType() == MeshType::MT_PNT)
            {   printf("[%u] PNT; ", i);
                mProgram.setSubroutines(GLShaderType::GLST_VERTEX, 1, &mUniform.VertPass_PNT);
                subroutines[0] = mUniform.FragPass_PNT;
            }
            else if (mesh->getType() == MeshType::MT_PNTBT)
            {   printf("[%u] PNTBT; ", i);
                mProgram.setSubroutines(GLShaderType::GLST_VERTEX, 1, &mUniform.VertPass_PNBTT);
                subroutines[0] = mUniform.FragPass_PNBTT;

                // requires normal map from the material
                mProgram.setUniform(mUniform.NormalMap, MaterialInfo::MI_NORMAL_MAP_SLOT);
                material->getNormalMap()->use(MaterialInfo::MI_NORMAL_MAP_SLOT);
            }

            if (material->getType() == MaterialType::MT_DEFAULT)
            {   printf("[%u] Default; ", i);
                subroutines[1] = mUniform.MatPass_Default;
            }
            else if (material->getType() == MaterialType::MT_BASIC)
            {   printf("[%u] Basic; ", i);
                subroutines[1] = mUniform.MatPass_Basic;

                mProgram.setUniform(mUniform.DiffuseColor, material->getDiffuseComponent());
                mProgram.setUniform(mUniform.SpecularColor, material->getSpecularComponent());
                mProgram.setUniform(mUniform.Shininess, material->getShininess());
            }
            else if (material->getType() == MaterialType::MT_DS_MAPPED)
            {   printf("[%u] DS Mapped; ", i);
                subroutines[1] = mUniform.MatPass_DS_map;

                mProgram.setUniform(mUniform.DiffuseColor, material->getDiffuseComponent());
                mProgram.setUniform(mUniform.SpecularColor, material->getSpecularComponent());
                mProgram.setUniform(mUniform.Shininess, material->getShininess());

                mProgram.setUniform(mUniform.DiffuseMap, MaterialInfo::MI_DIFFUSE_MAP_SLOT);
                mProgram.setUniform(mUniform.SpecularMap, MaterialInfo::MI_SPECULAR_MAP_SLOT);

                material->getDiffuseMap()->use(MaterialInfo::MI_DIFFUSE_MAP_SLOT);
                material->getSpecularMap()->use(MaterialInfo::MI_SPECULAR_MAP_SLOT);
            }

            Matrix4x4f Model = node.get(i)->getTransformation();
            Matrix4x4f PVM = PV * Model;

            mProgram.setUniform(mUniform.Model, Model);
            mProgram.setUniform(mUniform.PVM, PVM);
            mProgram.setSubroutines(GLShaderType::GLST_FRAGMENT, FRAG_SUBROUTINES_COUNT, (UINT32*)subroutines);

            mesh->getGPUBuffer().drawIndices();
        }

        driver->setDefaultBuffer();

        return;
        /////////////////////////////////////////////////////////////////////////////////////////////

        const CameraComponent::Viewport& Port = render->getRenderCamera()->getComponent()->mViewport;

        driver->setDefaultBuffer();
        driver->setClearColor(render->getClearColor());
        driver->clearBuffer();
        driver->enableDepthTest(false);
        driver->setViewPort(Port.posX, Port.posY, Port.width, Port.height);

        debug.use();
        debug.setUniform("Screen", 0);

        render->getGBuffer()->useAsUniformLayout(3, 0);
        render->getScreenPlane()->use();

        printf("Debug deferred ");
    }

} // namespace Berserk