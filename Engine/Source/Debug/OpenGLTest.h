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
#include "GLRenderDriver.h"
#include "GLTexture.h"
#include "Platform/VertexTypes.h"
#include "FreeImageImporter.h"

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
        shader.attachShader(IRenderDriver::VERTEX, source1, name1);
        shader.attachShader(IRenderDriver::FRAGMENT, source2, name2);
        shader.link();

        shader.addUniformVariable("ModelView");
        shader.addUniformVariable("MVP");
        shader.addUniformVariable("CameraPosition");
        shader.addUniformVariable("LightPosition");
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

        auto Model = Mat4x4f::rotate(Vec3f(0, 1, 1), angle);
        auto View =  Mat4x4f::lookAt(Vec3f(0, 0, 3), Vec3f(0,0, -1.0f), Vec3f(0,1,0));
        auto Proj =  Mat4x4f::perspective(Degrees(60.0f).radians().get(), 4.0f / 3.0f, 0.1f, 20.0f);

        shader.use();
        shader.setUniform("ModelView", View * Model);
        shader.setUniform("MVP", Proj * View * Model);
        shader.setUniform("CameraPosition", Vec3f(0, 0, 3));
        shader.setUniform("LightPosition", Vec3f(6 * Math::sin(angle * 0.8f), 0, 3));
        box.draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    {
        box.release();
        shader.release();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void OpenGLDriverTest()
{
    using namespace Berserk;

    IWindow* window;
    GLRenderDriver driver;
    FreeImageImporter importer;
    IWindow::WindowSetup setup;

    driver.initialize(setup);
    importer.initialize();

    window = driver.getMainWindow();

    while (!window->shouldClose())
    {
        driver.swapBuffers();
    }

    importer.release();
    driver.release();
}

void TextureImporterTest()
{
    using namespace Berserk;

    uint8 saving[MiB * 2];

    char image[] = "../Engine/Textures/System/failed.png";
    char path1[] = "../Engine/Shaders/Debug/GLSLTextureImport.vert";
    char path2[] = "../Engine/Shaders/Debug/GLSLTextureImport.frag";

    char shader1[Buffers::SIZE_4096];
    char shader2[Buffers::SIZE_4096];

    LoadShaderAsString(path1, shader1);
    LoadShaderAsString(path2, shader2);

    IWindow* window;
    GLRenderDriver driver;
    FreeImageImporter importer;
    IWindow::WindowSetup setup;
    GLShader shader;
    GLTexture texture;
    GLGPUBuffer buffer;

    driver.initialize(setup);
    importer.initialize();

    window = driver.getMainWindow();

    {
        shader.initialize("Texture Render");
        shader.createProgram();
        shader.attachShader(IRenderDriver::VERTEX, shader1, path1);
        shader.attachShader(IRenderDriver::FRAGMENT, shader2, path2);
        shader.link();

        shader.addUniformVariable("screen");
    }

    {
        ImageImporter::ImageData data;
        importer.import(image, data);
        texture.initialize("Image Test");
        texture.create(data.width, data.height, IRenderDriver::RGBA8, data.buffer, data.pixelFormat, data.pixelType, true);
    }

    {
        ImageImporter::ImageSave save;

        texture.getData(0, IRenderDriver::RGBA, saving);
        save.width = texture.getWidth();
        save.height = texture.getHeight();
        save.buffer = saving;
        //importer.save("../Engine/Textures/System/test1.bmp", save);
    }

    {
        uint16 indices[] = {0, 1 , 2, 0, 2, 3};
        Vec3f p0(-1, -1, 0), p1(1, -1, 0), p2(1, 1, 0), p3(-1, 1, 0);
        Vec2f t0(0, 0), t1(1, 0), t2(1, 1), t3(0, 1);

        VertPTf vertices[] = { {p0, t0}, {p1, t1}, {p2, t2}, {p3, t3} };

        buffer.initialize("Rect");
        buffer.create(4, IGPUBuffer::VertexPT, vertices, 6, indices);
    }

    for (int i = 0; i < 2; i++)
    {
        driver.clear(true, true, false);

        {
            shader.use();
            texture.bind(0u);
            shader.setUniform("screen", 0);
            buffer.draw();
        }

        driver.swapBuffers();

        if (i == -1)
        {
            printf("Make Screen Shot\n");

            ImageImporter::ImageSave save;
            window->getFrameBufferSize(save.width, save.height);
            uint8* screenshot = new uint8[save.width * save.height];
            driver.makeScreenShot(IRenderDriver::RGBA, screenshot);
            save.buffer = screenshot;
            importer.save("../Engine/Textures/Screens/test1.bmp", save);
            delete screenshot;
        }
    }

    shader.release();
    buffer.release();
    texture.release();
    importer.release();
    driver.release();
}

#endif //BERSERK_OPENGLSTARTTEST_H
