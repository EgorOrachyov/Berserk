//
// Created by Egor Orachyov on 26.02.2019.
//

#ifndef BERSERK_OPENGLDRIVERTEST_H
#define BERSERK_OPENGLDRIVERTEST_H

#include "Platform/GLShader.h"
#include "Platform/GLSampler.h"
#include "Platform/GLTexture.h"
#include "Platform/GLGPUBuffer.h"
#include "Platform/GLRenderDriver.h"
#include "Platform/VertexTypes.h"
#include "FreeImageImporter.h"
#include "Misc/FileUtility.h"

void OpenGLDriverTest()
{
    using namespace Berserk;

    IWindow* window;
    GLRenderDriver driver;
    FreeImageImporter importer;
    GLGPUBuffer buffer;
    GLTexture texture;
    GLSampler sampler;
    GLShader shader;

    {
        IWindow::WindowSetup setup;
        driver.initialize(setup);
        driver.setPolygonMode(IRenderDriver::FILL);
        driver.depthTest(true);

        importer.initialize();
        window = driver.getMainWindow();
    }

    {
        char texturePath[] = "../Engine/Textures/System/pattern.png";

        ImageImporter::ImageData data;
        importer.import(texturePath, data);

        texture.initialize("Pattern texture");
        texture.create(data.width,
                       data.height,
                       data.storageFormat,
                       data.buffer,
                       data.pixelFormat,
                       data.pixelType,
                       true);

        importer.unload();
    }

    {
        sampler.initialize("Linear filtering");
        sampler.create(IRenderDriver::FILTER_LINEAR,
                       IRenderDriver::FILTER_LINEAR,
                       IRenderDriver::WRAP_CLAMP_TO_EDGE);
    }

    {
        float32 size = 1.0f;
        float32 w = size / 2;

        Vec3f n0 = Vec3f(1,0,0), n1 = Vec3f(-1,0,0),
                n2 = Vec3f(0,1,0), n3 = Vec3f(0,-1,0),
                n4 = Vec3f(0,0,1), n5 = Vec3f(0,0,-1);

        Vec3f v0 = Vec3f(-w,w,w),  v1 = Vec3f(-w,-w,w),
                v2 = Vec3f(w,-w,w),  v3 = Vec3f(w,w,w),
                v4 = Vec3f(-w,w,-w), v5 = Vec3f(-w,-w,-w),
                v6 = Vec3f(w,-w,-w), v7 = Vec3f(w,w,-w);

        Vec2f t0 = Vec2f(0,1), t1 = Vec2f(0,0),
                t2 = Vec2f(1,0), t3 = Vec2f(1.0,1.0);

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
        VertPNTf data[data_count] = {
                {v0,n4,t0},{v1,n4,t1},{v2,n4,t2},{v2,n4,t2},{v3,n4,t3},{v0,n4,t0},
                {v3,n0,t0},{v2,n0,t1},{v6,n0,t2},{v6,n0,t2},{v7,n0,t3},{v3,n0,t0},
                {v7,n5,t0},{v6,n5,t1},{v5,n5,t2},{v5,n5,t2},{v4,n5,t3},{v7,n5,t0},
                {v4,n1,t0},{v5,n1,t1},{v1,n1,t2},{v1,n1,t2},{v0,n1,t3},{v4,n1,t0},
                {v4,n2,t0},{v0,n2,t1},{v3,n2,t2},{v3,n2,t2},{v7,n2,t3},{v4,n2,t0},
                {v6,n3,t0},{v2,n3,t1},{v1,n3,t2},{v1,n3,t2},{v5,n3,t3},{v6,n3,t0}
        };

        buffer.initialize("Test Box");
        buffer.create(data_count, IGPUBuffer::VertexPNT, data, index_count, i);
    }

    {
        char path1[] = "../Engine/Shaders/Debug/GLSLTest.vert";
        char path2[] = "../Engine/Shaders/Debug/GLSLTest.frag";

        char shader1[Buffers::SIZE_4096];
        char shader2[Buffers::SIZE_4096];

        FileUtility::readFile(path1, shader1);
        FileUtility::readFile(path2, shader2);

        shader.initialize("Texture Render");
        shader.createProgram();
        shader.attachShader(IRenderDriver::VERTEX, shader1, path1);
        shader.attachShader(IRenderDriver::FRAGMENT, shader2, path2);
        shader.link();

        shader.addUniformVariable("Texture0");
        shader.addUniformVariable("MVP");
        shader.addUniformVariable("ModelView");
        shader.addUniformVariable("CameraPosition");
        shader.addUniformVariable("LightPosition");
    }

    while (!window->shouldClose())
    {
        {
            static float32 angle = 0;
            angle += 0.01;

            auto Model = Mat4x4f::rotate(Vec3f(0, 1, 1), angle);
            auto View =  Mat4x4f::lookAt(Vec3f(0, 0, 3), Vec3f(0,0, -1.0f), Vec3f(0,1,0));
            auto Proj =  Mat4x4f::perspective(Degrees(60.0f).radians().get(), 4.0f / 3.0f, 0.1f, 20.0f);

            shader.use();
            sampler.bind(0u);
            texture.bind(0u);
            shader.setUniform("Texture0", 0);
            shader.setUniform("ModelView", View * Model);
            shader.setUniform("MVP", Proj * View * Model);
            shader.setUniform("CameraPosition", Vec3f(0, 0, 3));
            shader.setUniform("LightPosition", Vec3f(6 * Math::sin(angle * 0.8f), 0, 3));
            buffer.draw();
        }

        driver.swapBuffers();
        driver.update();
        driver.clear(true, true, false);
    }

    shader.release();
    buffer.release();
    sampler.release();
    texture.release();
    importer.release();
    driver.release();
}

#endif //BERSERK_OPENGLDRIVERTEST_H
