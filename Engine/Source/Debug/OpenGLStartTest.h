//
// Created by Egor Orachyov on 18.02.2019.
//

#ifndef BERSERK_OPENGLSTARTTEST_H
#define BERSERK_OPENGLSTARTTEST_H

#include "Logging/LogMacros.h"
#include "Misc/Buffers.h"
#include "GLInclude.h"
#include "GLShader.h"
#include "GLGPUBuffer.h"
#include "Platform/VertexTypes.h"

#include "OpenGL/GLFW/glfw3.h"

void LoadShaderAsString(const char* shaderName, char* buffer)
{
    using namespace Berserk;

    if (!shaderName)
    {
        ERROR("NULL shader name string");
        exit(EXIT_FAILURE);
    }

    FILE * file = fopen(shaderName, "r");
    if (!file)
    {
        ERROR("Cannot open file %s", shaderName);
        exit(EXIT_FAILURE);
    }


    int readChars = 0;
    while (!feof(file))
    {
        char c = fgetc(file);
        buffer[readChars] = c;
        readChars += 1;
    }
    readChars -= 1;
    buffer[readChars] = '\0';
}

void BasicOpenGLTest()
{
    using namespace Berserk;

    if (!glfwInit())
    {
        ERROR("Cannot initialize GLFW library");
        return;
    }

#ifdef PLATFORM_MAC
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    auto window = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        ERROR("Cannot initialize GLEW library");
        return;
    }

    glEnable(GL_DEPTH_TEST);

    ////////////////////////////////////////////////////////////////
    ///                                                          ///
    ///                        Test case                         ///
    ///                                                          ///
    ////////////////////////////////////////////////////////////////

    const char name1[] = "../Engine/Shaders/Debug/GLSLTest.vert";
    const char name2[] = "../Engine/Shaders/Debug/GLSLTest.frag";

    char source1[Buffers::SIZE_8192];
    char source2[Buffers::SIZE_8192];

    LoadShaderAsString(name1, source1);
    LoadShaderAsString(name2, source2);

    //printf("Source code:\n%s\n", source1);
    //printf("Source code:\n%s\n", source2);

    GLShader shader;
    {
        shader.initialize("Test");
        shader.createProgram();
        shader.attachShader(IShader::VERTEX, source1, name1);
        shader.attachShader(IShader::FRAGMENT, source2, name2);
        shader.link();

        shader.addUniformVariable("ModelView");
        shader.addUniformVariable("MVP");
    }

    float32 size = 1.0f;
    float32 w = size / 2;

    Vec3f n0 = Vec3f(1,0,0), n1 = Vec3f(-1,0,0),
          n2 = Vec3f(0,1,0), n3 = Vec3f(0,-1,0),
          n4 = Vec3f(0,0,1), n5 = Vec3f(0,0,-1);

    Vec3f v0 = Vec3f(-w,w,w),  v1 = Vec3f(-w,-w,w),
          v2 = Vec3f(w,-w,w),  v3 = Vec3f(w,w,w),
          v4 = Vec3f(-w,w,-w), v5 = Vec3f(-w,-w,-w),
          v6 = Vec3f(w,-w,-w), v7 = Vec3f(w,w,-w);

    const uint32 index_count = 36;
    uint16  i[index_count] = {
            0,1,2,3,4,5,
            6,7,8,9,10,11,
            12,13,14,15,16,17,
            18,19,20,21,22,23,
            24,25,26,27,28,29,
            30,31,32,33,34,35
    };

    const uint32 data_count = 36;
    VertPNf data[data_count] = {
            {v0,n4},{v1,n4},{v2,n4},{v2,n4},{v3,n4},{v0,n4},
            {v3,n0},{v2,n0},{v6,n0},{v6,n0},{v7,n0},{v3,n0},
            {v7,n5},{v6,n5},{v5,n5},{v5,n5},{v4,n5},{v7,n5},
            {v4,n1},{v5,n1},{v1,n1},{v1,n1},{v0,n1},{v4,n1},
            {v4,n2},{v0,n2},{v3,n2},{v3,n2},{v7,n2},{v4,n2},
            {v6,n3},{v2,n3},{v1,n3},{v1,n3},{v5,n3},{v6,n3}
    };

    GLGPUBuffer box;
    {
        box.initialize("Test Box");
        box.create(data_count, IGPUBuffer::VertexPN, data, index_count, i);
    }

    float32 angle = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        angle += 0.01;

        auto Model = Mat4x4f::translate(Vec3f(0, 0, Math::sin(angle) * 3.0f)) * Mat4x4f::rotate(Vec3f(0, 1, 1), angle);
        auto View = Mat4x4f::lookAt(Vec3f(0, 0, 5), Vec3f(0,0, -1.0f), Vec3f(0,1,0));
        auto Proj = Mat4x4f::perspective(Degrees(60.0f).radians().get(), 4.0f / 3.0f, 0.1f, 20.0f);

        shader.use();
        shader.setUniform("ModelView", View * Model);
        shader.setUniform("MVP", Proj * View * Model);
        box.draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    {
        box.release();
        shader.release();
    }

    glfwDestroyWindow(window);
}

#endif //BERSERK_OPENGLSTARTTEST_H
