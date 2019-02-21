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
        ERROR("Cannot open file");
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

    ////////////////////////////////////////////////////////////////
    ///                                                          ///
    ///                        Test case                         ///
    ///                                                          ///
    ////////////////////////////////////////////////////////////////

    const char name1[] = "Engine/Shaders/Debug/GLSLTest.vert";
    const char name2[] = "Engine/Shaders/Debug/GLSLTest.frag";

    char* source1 = buffer1;
    char* source2 = buffer2;

    LoadShaderAsString(name1, source1);
    LoadShaderAsString(name2, source2);

    GLShader shader;
    {
        shader.initialize("Test");
        shader.createProgram();
        shader.attachShader(IShader::VERTEX, source1, name1);
        shader.attachShader(IShader::VERTEX, source2, name2);
        shader.link();
        shader.validate();

        shader.addUniformVariable("ModelView");
        shader.addUniformVariable("MVP");
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    shader.release();

    glfwDestroyWindow(window);
}

#endif //BERSERK_OPENGLSTARTTEST_H
