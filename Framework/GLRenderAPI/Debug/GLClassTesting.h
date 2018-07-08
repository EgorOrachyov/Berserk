//
// Created by Egor Orachyov on 05.07.2018.
//

#ifndef BERSERKENGINE_GLCLASSTESTING_H
#define BERSERKENGINE_GLCLASSTESTING_H

#include "Context/GLContext.h"
#include "Context/GLWindow.h"
#include "GPUProgram/GLGPUProgram.h"
#include "Buffers/GLDataBufferPacker.h"
#include "Buffers/GLElementBufferObject.h"
#include "Buffers/GLVertexArrayObject.h"

#include "Sphere.h"

#include "../../Core/Math/UtilityMatrices.h"

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

void GLVertexArrayObjectTesting()
{
    using namespace Berserk;

    GLContext context;
    GLWindow window;
    GLGPUProgram program;
    GLDataBufferPacker packer;
    GLVertexArrayObject vao;

    context.initWindowContext();
    window.create();
    window.makeCurrent();
    context.initRenderingContext();

    program.init();
    program.compileShader("../GLRenderAPI/Debug/Basic.vert", GLShaderType::GLST_VERTEX);
    program.compileShader("../GLRenderAPI/Debug/Basic.frag", GLShaderType::GLST_FRAGMENT);
    program.link();
    program.validate();

    Vector3f position[] = {Vector3f(0.0, 1.0, 0.0), Vector3f(-1.0, -0.5, 0.0), Vector3f(1.0, -0.5, 0.0)};
    Vector3f color[] = {Vector3f(0.0, 1.0, 0.0), Vector3f(1.0, 0.0, 0.0), Vector3f(0.0, 0.0, 1.0)};

    packer.init();
    packer.addVertexData(position, 3, 0, GLN_DO_NOT_USE);
    packer.addVertexData(color, 3, 1, GLN_DO_NOT_USE);
    packer.pack();

    vao.init();
    vao.attachBuffer(packer);
    vao.setDrawingMode(3, GLPrimitiveMode::GLPM_TRIANGLES);

    /// there should be main cycle handled by application context

    while(!glfwWindowShouldClose(window.getHandle()))
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();
        int32 mat_location = program.getUniformLocation("RotationMatrix");
        if (program.isLocationValid(mat_location)) {
            program.setUniform(mat_location, rotate(Vector3f(0, 0, -1), glfwGetTime()));
            vao.draw();
        }

        glfwSwapBuffers(window.getHandle());
        glfwPollEvents();
    }

    /// it wil be done by rendering context

    packer.destroy();
    program.destroy();
    vao.destroy();
    window.destroy();
    context.destroy();
}

void GLLightTesting()
{
    using namespace Berserk;

    GLContext context;
    GLWindow window;
    GLGPUProgram program;
    GLDataBufferPacker packer;
    GLVertexArrayObject vao;

    context.initWindowContext();
    window.create();
    window.makeCurrent();
    context.initRenderingContext();

    program.init();
    program.compileShader("../GLRenderAPI/Debug/PhongModel.vert", GLShaderType::GLST_VERTEX);
    program.compileShader("../GLRenderAPI/Debug/PhongModel.frag", GLShaderType::GLST_FRAGMENT);
    program.link();
    program.validate();

    Vector3f position[] = {Vector3f(0.0, 1.0, 0.0), Vector3f(-1.0, -0.5, 0.0), Vector3f(1.0, -0.5, 0.0)};
    Vector3f normal[] = {Vector3f(0.0, 0.0, 1.0), Vector3f(0.0, 0.0, 1.0), Vector3f(0.0, 0.0, 1.0)};

    packer.init();
    packer.addVertexData(position, 3, 0, GLN_DO_NOT_USE);
    packer.addVertexData(normal, 3, 1, GLN_DO_NOT_USE);
    packer.pack();

    vao.init();
    vao.attachBuffer(packer);
    vao.setDrawingMode(3, GLPrimitiveMode::GLPM_TRIANGLES);

    Vector4f Lp(0.0, 0.0, 1.0, 1.0);
    Vector3f La(0.9, 0.9, 0.9);
    Vector3f Ld(0.9, 0.9, 0.9);
    Vector3f Ls(0.9, 0.9, 0.9);

    Vector3f Ka(0.1, 0.5, 0.9);
    Vector3f Kd(0.9, 0.1, 0.5);
    Vector3f Ks(0.5, 0.9, 0.1);
    float32  sh(1);

    Matrix4x4f Model = translate(Vector3f(0,0,-1.0f));
    Matrix4x4f View = lookAt(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    Matrix4x4f Projection = perspective((float32)toRadians(45), 1.0, 0.1, 100);

    //Matrix4x4f View = newMatrix(1.0);
    //Matrix4x4f Projection = newMatrix(1.0);

    /// there should be main cycle handled by application context

    //glViewport(0,0,1000,1000);


    Vector4f v = View * Lp;
    printf("(%f,%f,%f)\n", v.x, v.y, v.z);
    v = View * Model * Vector4f(0.0, 1.0, 0.0, 1.0);
    printf("(%f,%f,%f)\n", v.x, v.y, v.z);
    v = Projection * View * Model * Vector4f(0.0, 1.0, 0.0, 1.0);
    printf("(%f,%f,%f)\n", v.x / v.w, v.y / v.w, v.z / v.w);


    while(!glfwWindowShouldClose(window.getHandle()))
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();

        // subroutine
        {
            GLuint ads = program.getSubroutineIndex("phongModel", GLST_VERTEX);
            program.setSubroutines(GLST_VERTEX, 1, &ads);
        }

        // uniform block
        {
            program.setUniform("Light.Position", View * rotate(Vector3f(0,1,0), glfwGetTime()) * Lp);
            program.setUniform("Light.La", La);
            program.setUniform("Light.Ld", Ld);
            program.setUniform("Light.Ls", Ls);

            program.setUniform("Material.Ka", Ka);
            program.setUniform("Material.Kd", Kd);
            program.setUniform("Material.Ks", Ks);
            program.setUniform("Material.Shininess", sh);

            program.setUniform("ModelView", View * Model);
            program.setUniform("MVP", Projection * View * Model);
        }

        vao.draw();

        glfwSwapBuffers(window.getHandle());
        glfwPollEvents();
    }

    /// it wil be done by rendering context

    packer.destroy();
    program.destroy();
    vao.destroy();
    window.destroy();
    context.destroy();
}

void GLFragmentLightTesting()
{
    using namespace Berserk;

    GLContext context;
    GLWindow window;
    GLGPUProgram program;
    GLDataBufferPacker packer;
    GLElementBufferObject ebo;
    GLVertexArrayObject vao;

    context.initWindowContext();
    window.create();
    window.makeCurrent();
    context.initRenderingContext();

    program.init();
    program.compileShader("../GLRenderAPI/Debug/FragmentLight.vert", GLShaderType::GLST_VERTEX);
    program.compileShader("../GLRenderAPI/Debug/FragmentLight.frag", GLShaderType::GLST_FRAGMENT);
    //program.compileShader("../GLRenderAPI/Debug/VertexLight.vert", GLShaderType::GLST_VERTEX);
    //program.compileShader("../GLRenderAPI/Debug/VertexLight.frag", GLShaderType::GLST_FRAGMENT);
    //program.compileShader("../GLRenderAPI/Debug/Flat.vert", GLShaderType::GLST_VERTEX);
    //program.compileShader("../GLRenderAPI/Debug/Flat.frag", GLShaderType::GLST_FRAGMENT);
    program.link();
    program.validate();

    //Vector3f position[] = {Vector3f(0.0, 1.0, 0.0), Vector3f(-1.0, -0.5, 0.0), Vector3f(1.0, -0.5, 0.0)};
    //Vector3f normal[] = {Vector3f(0.0, 0.0, 1.0), Vector3f(0.0, 0.0, 1.0), Vector3f(0.0, 0.0, 1.0)};

    packer.init();
    //packer.addVertexData(position, 3, 0, GLN_DO_NOT_USE);
    //packer.addVertexData(normal, 3, 1, GLN_DO_NOT_USE);

    Sphere sphere;
    sphere.create(1.2, 62, 62);
    sphere.fill(packer, ebo);
    //packer.pack();

    vao.init();
    vao.attachBuffer(packer);
    vao.attachBuffer(ebo);
    //vao.setDrawingMode(3, GLPrimitiveMode::GLPM_TRIANGLES);

    Vector4f Lp(4.0, 4.0, 4.0, 1.0);
    Vector3f La(0.9, 0.9, 0.9);
    Vector3f Ld(0.9, 0.9, 0.9);
    Vector3f Ls(0.9, 0.9, 0.9);

    Vector3f Ka(0.05, 0.05, 0.05);
    Vector3f Kd(0.9, 0.11, 0.12);
    Vector3f Ks(0.9, 0.41, 0.52);
    float32  sh(16);

    Matrix4x4f Model = translate(Vector3f(0,0,-1.0f));
    Matrix4x4f View = lookAt(Vector3f(0, 0, 4), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    Matrix4x4f Projection = perspective((float32)toRadians(45), 1.0, 0.1, 100);

    //Matrix4x4f View = newMatrix(1.0);
    //Matrix4x4f Projection = newMatrix(1.0);

    /// there should be main cycle handled by application context

    //glViewport(0,0,1000,1000);


    Vector4f v = View * Lp;
    printf("(%f,%f,%f)\n", v.x, v.y, v.z);
    v = View * Model * Vector4f(0.0, 1.0, 0.0, 1.0);
    printf("(%f,%f,%f)\n", v.x, v.y, v.z);
    v = Projection * View * Model * Vector4f(0.0, 1.0, 0.0, 1.0);
    printf("(%f,%f,%f)\n", v.x / v.w, v.y / v.w, v.z / v.w);

    float64 time = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window.getHandle()))
    {
        //printf("%lf\n", 1.0 / (glfwGetTime() - time));

        time = glfwGetTime();

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        program.use();

        Model = rotate(Vector3f(-0.8,0.3,0.5), glfwGetTime());

        // uniform block
        {
            program.setUniform("Light.Position", View /* rotate(Vector3f(0,1,0), glfwGetTime())*/ * Lp);
            program.setUniform("Light.La", La);
            program.setUniform("Light.Ld", Ld);
            program.setUniform("Light.Ls", Ls);

            program.setUniform("Material.Ka", Ka);
            program.setUniform("Material.Kd", Kd);
            program.setUniform("Material.Ks", Ks);
            program.setUniform("Material.Shininess", sh);

            program.setUniform("ModelView", View * Model);
            program.setUniform("MVP", Projection * View * Model);
        }

        vao.draw();

        glfwSwapBuffers(window.getHandle());
        glfwPollEvents();
    }

    /// it wil be done by rendering context

    packer.destroy();
    program.destroy();
    vao.destroy();
    window.destroy();
    context.destroy();
}

void GLElementBufferTesting()
{
    using namespace Berserk;

    GLContext context;
    GLWindow window;
    GLGPUProgram program;
    GLDataBufferPacker packer;
    GLElementBufferObject ebo;
    GLVertexArrayObject vao;

    context.initWindowContext();
    window.create();
    window.makeCurrent();
    context.initRenderingContext();

    program.init();
    program.compileShader("../GLRenderAPI/Debug/FragmentLight.vert", GLShaderType::GLST_VERTEX);
    program.compileShader("../GLRenderAPI/Debug/FragmentLight.frag", GLShaderType::GLST_FRAGMENT);
    program.link();
    program.validate();

    Vector3f position[] = {Vector3f(0.0, 1.0, 0.0), Vector3f(-1.0, -0.5, 0.0), Vector3f(1.0, -0.5, 0.0)};
    Vector3f normal[] = {Vector3f(0.0, 0.0, 1.0), Vector3f(0.0, 0.0, 1.0), Vector3f(0.0, 0.0, 1.0)};
    uint16 indices[] = {0,1,2};

    packer.init();
    packer.addVertexData(position, 3, 0, GLN_DO_NOT_USE);
    packer.addVertexData(normal, 3, 1, GLN_DO_NOT_USE);
    packer.pack();

    ebo.create(3, indices, GLPM_TRIANGLES);

    vao.init();
    //vao.attachBuffer(packer);
    //vao.attachBuffer(ebo);
    vao.attachBuffer(packer, ebo);

    //vao.setDrawingMode(3, GLPrimitiveMode::GLPM_TRIANGLES);

    Vector4f Lp(0.0, 0.0, 2.0, 1.0);
    Vector3f La(0.9, 0.9, 0.9);
    Vector3f Ld(0.9, 0.9, 0.3);
    Vector3f Ls(0.9, 0.2, 0.9);

    Vector3f Ka(0.01, 0.01, 0.01);
    Vector3f Kd(0.9, 0.3, 0.6);
    Vector3f Ks(0.9, 0.3, 0.1);
    float32  sh(8);

    Matrix4x4f Model = translate(Vector3f(0,0,-1.0f));
    Matrix4x4f View = lookAt(Vector3f(0, 0, 4), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    Matrix4x4f Projection = perspective((float32)toRadians(45), 1.0, 0.1, 100);

    //Matrix4x4f View = newMatrix(1.0);
    //Matrix4x4f Projection = newMatrix(1.0);

    /// there should be main cycle handled by application context

    //glViewport(0,0,1000,1000);


    Vector4f v = View * Lp;
    printf("(%f,%f,%f)\n", v.x, v.y, v.z);
    v = View * Model * Vector4f(0.0, 1.0, 0.0, 1.0);
    printf("(%f,%f,%f)\n", v.x, v.y, v.z);
    v = Projection * View * Model * Vector4f(0.0, 1.0, 0.0, 1.0);
    printf("(%f,%f,%f)\n", v.x / v.w, v.y / v.w, v.z / v.w);

    float64 time = glfwGetTime();

    while(!glfwWindowShouldClose(window.getHandle()))
    {
        printf("%lf\n", 1.0 / (glfwGetTime() - time));

        time = glfwGetTime();

        glClearColor(0.0, 0.0, 0.0, 0.0);

        program.use();

        // uniform block
        {
            program.setUniform("Light.Position", View * rotate(Vector3f(0,1,0), glfwGetTime()) * Lp);
            program.setUniform("Light.La", La);
            program.setUniform("Light.Ld", Ld);
            program.setUniform("Light.Ls", Ls);

            program.setUniform("Material.Ka", Ka);
            program.setUniform("Material.Kd", Kd);
            program.setUniform("Material.Ks", Ks);
            program.setUniform("Material.Shininess", sh);

            program.setUniform("ModelView", View * Model);
            program.setUniform("MVP", Projection * View * Model);
        }

        vao.draw();

        glfwSwapBuffers(window.getHandle());
        glfwPollEvents();
    }

    /// it wil be done by rendering context

    ebo.destroy();
    packer.destroy();
    program.destroy();
    vao.destroy();
    window.destroy();
    context.destroy();
}

#endif //BERSERKENGINE_GLCLASSTESTING_H
