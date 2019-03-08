//
// Created by Egor Orachyov on 26.02.2019.
//

#ifndef BERSERK_OPENGLDRIVERTEST_H
#define BERSERK_OPENGLDRIVERTEST_H

#include "Platform/GLShader.h"
#include "Platform/GLSampler.h"
#include "Platform/GLTexture.h"
#include "Platform/GLGPUBuffer.h"
#include "Platform/GLFrameBuffer.h"
#include "Platform/GLDepthBuffer.h"
#include "Platform/GLUniformBuffer.h"
#include "Platform/GLRenderDriver.h"
#include "Platform/VertexTypes.h"
#include "FreeImageImporter.h"

#include "Misc/FileUtility.h"
#include "Logging/LogManager.h"
#include "Profiling/ProfilingUtility.h"

void OpenGLDriverTest()
{
    using namespace Berserk;

    struct UnifromData
    {
        Mat4x4f Proj;
        Mat4x4f View;
        Mat4x4f Model;
    };

    IWindow* window;
    GLRenderDriver driver;
    FreeImageImporter importer;
    GLGPUBuffer buffer;
    GLTexture texture;
    GLSampler sampler;
    GLShader shader;
    GLFrameBuffer frameBuffer;
    GLDepthBuffer depthBuffer;
    GLUniformBuffer uniformBuffer;

    {
        IWindow::WindowSetup setup;
        driver.initialize(setup);
        driver.polygonMode(IRenderDriver::FILL);
        driver.depthTest(true);

        importer.initialize();
        window = driver.getMainWindow();
    }

    {
        sampler.initialize("Linear filtering");
        sampler.create(IRenderDriver::FILTER_LINEAR,
                       IRenderDriver::FILTER_LINEAR,
                       IRenderDriver::WRAP_CLAMP_TO_EDGE);
    }

    {
        char texturePath[] = "../Engine/Textures/System/pattern.png";

        IImageImporter::ImageData data;
        importer.import(texturePath, data);

        texture.initialize("Pattern texture");
        texture.create(data.width, data.height, data.storageFormat, data.pixelFormat, data.pixelType, data.buffer, true);
        texture.bind(&sampler);

        importer.unload();
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
        const char path1[] = "../Engine/Shaders/Debug/GLSLTest.vert";
        const char path2[] = "../Engine/Shaders/Debug/GLSLTest.frag";

        char shader1[Buffers::SIZE_4096];
        char shader2[Buffers::SIZE_4096];

        FileUtility::read(path1, shader1);
        FileUtility::read(path2, shader2);

        shader.initialize("Texture Render");
        shader.createProgram();
        shader.attachShader(IRenderDriver::VERTEX, shader1, path1);
        shader.attachShader(IRenderDriver::FRAGMENT, shader2, path2);
        shader.link();

        shader.addUniformVariable("Texture0");
        shader.addUniformVariable("CameraPosition");
        shader.addUniformVariable("LightPosition");
        shader.setUniformBlockBinding("Transformation", 0);
    }

    {
        uniformBuffer.initialize("Uniform Buffer");
        uniformBuffer.create(0, sizeof(UnifromData), nullptr);
        uniformBuffer.bind();
    }

    {
        uint32 width, height;
        window->getFrameBufferSize(width, height);

        frameBuffer.initialize("Main frame buffer");
        frameBuffer.createFrameBuffer(width, height, 1);
        frameBuffer.attachColorBuffer(IRenderDriver::RGB16F);
        frameBuffer.attachDepthStencilBuffer();
        frameBuffer.linkBuffers();

        depthBuffer.initialize("Depth buffer");
        depthBuffer.createDepthBuffer(width, height);
    }

    {
        IRenderDriver::ViewPort view;
        window->getFrameBufferSize(view.width, view.height);

        view.x = 0;
        view.y = 0;

        driver.viewPort(view);
    }

    while (!window->shouldClose())
    {
        {
            static float32 angle = 0;
            angle += 0.01;

            auto Model = Mat4x4f::rotate(Vec3f(0, 1, 1), angle);
            auto View =  Mat4x4f::lookAt(Vec3f(0, 0, 3), Vec3f(0,0, -1.0f), Vec3f(0,1,0));
            auto Proj =  Mat4x4f::perspective(Degrees(60.0f).radians().get(), 4.0f / 3.0f, 0.1f, 20.0f);

            UnifromData data = {Proj.transpose(), View.transpose(), Model.transpose()};
            uniformBuffer.update(sizeof(UnifromData), &data);

            shader.use();
            texture.bind(0u);
            uniformBuffer.bind();
            shader.setUniform("Texture0", 0);
            shader.setUniform("CameraPosition", Vec3f(0, 0, 3));
            shader.setUniform("LightPosition", Vec3f(6 * Math::sin(angle * 0.8f), 0, 3));
            buffer.draw();
        }

        driver.swapBuffers();
        driver.update();
        driver.clear(true, true, false);
    }

    {
        char tmp[Buffers::SIZE_512];
        char cpu[Buffers::SIZE_16];
        char gpu[Buffers::SIZE_16];

        OPEN_BLOCK("-------------- OpenGL driver primitives memory usage --------------");

        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IShader",
                ProfilingUtility::print(shader.getMemoryUsage(), cpu),
                ProfilingUtility::print(shader.getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IGPUBuffer",
                ProfilingUtility::print(buffer.getMemoryUsage(), cpu),
                ProfilingUtility::print(buffer.getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "ISampler",
                ProfilingUtility::print(sampler.getMemoryUsage(), cpu),
                ProfilingUtility::print(sampler.getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "ITexture",
                ProfilingUtility::print(texture.getMemoryUsage(), cpu),
                ProfilingUtility::print(texture.getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IRenderDriver",
                ProfilingUtility::print(driver.getMemoryUsage(), cpu),
                ProfilingUtility::print(driver.getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IFrameBuffer",
                ProfilingUtility::print(frameBuffer.getMemoryUsage(), cpu),
                ProfilingUtility::print(frameBuffer.getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IDepthBuffer",
                ProfilingUtility::print(depthBuffer.getMemoryUsage(), cpu),
                ProfilingUtility::print(depthBuffer.getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IUniformBuffer",
                ProfilingUtility::print(uniformBuffer.getMemoryUsage(), cpu),
                ProfilingUtility::print(uniformBuffer.getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);

        CLOSE_BLOCK("-------------------------------------------------------------------");
    }

    shader.release();
    buffer.release();
    sampler.release();
    texture.release();
    frameBuffer.release();
    depthBuffer.release();
    uniformBuffer.release();
    importer.release();
    driver.release();

}

#endif //BERSERK_OPENGLDRIVERTEST_H