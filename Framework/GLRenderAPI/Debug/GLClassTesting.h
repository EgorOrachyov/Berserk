//
// Created by Egor Orachyov on 05.07.2018.
//

#ifndef BERSERKENGINE_GLCLASSTESTING_H
#define BERSERKENGINE_GLCLASSTESTING_H

#include "../GLContext.h"
#include "../GLWindow.h"
#include "../GLGPUProgram.h"
#include "../GLDataBufferPacker.h"

void GLGPUProgramTesting()
{
    using namespace Berserk;

    GLWindow window;
    GLContext context;
    GLGPUProgram program;

    /// It is response of the GLRenderAPI class

    context.initWindowContext();
    window.create();
    window.makeCurrent();
    context.initRenderingContext();
    //context.printContextInfo();

    /// end of response of the GLRenderAPI class


    /// GLGPUProgram test

    program.init();
    program.compileShader("../GLRenderAPI/Debug/Basic.vert", GLShaderType::GLST_VERTEX);
    program.compileShader("../GLRenderAPI/Debug/Basic.frag", GLShaderType::GLST_FRAGMENT);
    program.link();
    program.validate();
    program.printActiveAttributes();
    program.printActiveUniforms();
    program.printActiveUniformsInBlocks();

    /// It is response of the GLRenderAPI class

    program.destroy();
    window.destroy();
    context.destroy();

    /// end of response of the GLRenderAPI class

}

void GLDataBufferPackerTesting()
{
    // todo: add integration with VertexArrayObject

    using namespace Berserk;

    GLDataBufferPacker packer;

    //float32 position[] = {1.0, 2.0, 3.0};
    //float32 color[] = {0.3, 0.6, 0.9};

    packer.init();
    //packer.addVertexData(position, sizeof(float32), 3, 0, GLParamType::GLPT_FLOAT, GLNormalization::GLN_DO_NOT_USE);
    //packer.addVertexData(color, sizeof(float32), 3, 1, GLParamType::GLPT_FLOAT, GLNormalization::GLN_DO_NOT_USE);

    Vector3f position[] = {Vector3f(0.0, 1.0, 0.0), Vector3f(-1.0, 0.0, 0.0), Vector3f(1.0, 0.0, 0.0)};
    Vector3f color[] = {Vector3f(0.0, 1.0, 0.0), Vector3f(1.0, 0.0, 0.0), Vector3f(0.0, 0.0, 1.0)};

    packer.addVertexData(position, 3, 0, GLN_DO_NOT_USE);
    packer.addVertexData(color, 3, 1, GLN_DO_NOT_USE);

    packer.pack();

    float32* buffer = (float32*)packer.getBuffer();
    for(int32 i = 0; i < 3 * 3 * 2; i++)
    {
        printf("%f\n", buffer[i]);
    }

}

#endif //BERSERKENGINE_GLCLASSTESTING_H
