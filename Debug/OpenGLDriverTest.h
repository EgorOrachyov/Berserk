//
// Created by Egor Orachyov on 26.02.2019.
//

#ifndef BERSERK_OPENGLDRIVERTEST_H
#define BERSERK_OPENGLDRIVERTEST_H

#include <Platform/GLShader.h>
#include <Platform/GLGPUBuffer.h>
#include <Platform/GLFrameBuffer.h>
#include <Platform/GLDepthBuffer.h>
#include <Platform/GLUniformBuffer.h>
#include <Platform/GLRenderDriver.h>
#include <Platform/VertexTypes.h>
#include <FreeImageImporter.h>

#include <Managers/GLShaderManager.h>
#include <Managers/GLBufferManager.h>
#include <Managers/GLTextureManager.h>
#include <Managers/MaterialManager.h>

#include <Logging/LogManager.h>
#include <Profiling/ProfilingUtility.h>
#include <Memory/MemorySizer.h>
#include <Time/Timer.h>

void OpenGLManagerTest()
{
    using namespace Berserk;
    using namespace Berserk::Resources;
    using namespace Berserk::Importers;

    LogManager::getSingleton().setFileWriting(false);

    struct UniformData
    {
        Mat4x4f Proj;
        Mat4x4f View;
        Mat4x4f Model;
    };

    Timer timer;
    MemorySizer memorySizer;

    IMaterial*      material;
    IShader*        frameRender;
    IShader*        screenRender;
    IWindow*        window;
    ITexture*       texture;
    ISampler*       sampler;
    IGPUBuffer*     buffer;
    IGPUBuffer*     screen;
    IFrameBuffer*   frameBuffer;
    IDepthBuffer*   depthBuffer;
    IUniformBuffer* uniformBuffer;

    IRenderDriver::ViewPort frameBufferVP;
    IRenderDriver::ViewPort displayBufferVP;

    auto setup = IWindow::WindowSetup();
    setup.width *= 1.5;
    setup.height *= 1.5;

    GLRenderDriver driver(setup);
    {
        window = driver.getMainWindow();
        driver.polygonMode(IRenderDriver::PolygonMode::FILL);
        driver.clear(Vec4f(0,0,0,0));
    }

    FreeImageImporter importer;
    GLTextureManager  textureManager(&importer, "../Engine/Textures/");
    GLBufferManager   bufferManager;
    GLShaderManager   shaderManager("../Engine/Shaders");
    MaterialManager   materialManager(&textureManager, "../Engine/Materials");

    {
        texture = textureManager.getDefaultTexture();
        sampler = textureManager.createSampler("LinearMipMapNearestClampEdge");
        sampler->create(IRenderDriver::FILTER_LINEAR_MIPMAP_NEAREST,
                        IRenderDriver::FILTER_LINEAR_MIPMAP_NEAREST,
                        IRenderDriver::WRAP_CLAMP_TO_EDGE);
        textureManager.bindSampler(texture, sampler);
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
        uint16  i[index_count] =
        {
                0,1,2,3,4,5,
                6,7,8,9,10,11,
                12,13,14,15,16,17,
                18,19,20,21,22,23,
                24,25,26,27,28,29,
                30,31,32,33,34,35
        };

        const uint32 data_count = 36;
        VertPNTf data[data_count] =
        {
                {v0,n4,t0},{v1,n4,t1},{v2,n4,t2},{v2,n4,t2},{v3,n4,t3},{v0,n4,t0},
                {v3,n0,t0},{v2,n0,t1},{v6,n0,t2},{v6,n0,t2},{v7,n0,t3},{v3,n0,t0},
                {v7,n5,t0},{v6,n5,t1},{v5,n5,t2},{v5,n5,t2},{v4,n5,t3},{v7,n5,t0},
                {v4,n1,t0},{v5,n1,t1},{v1,n1,t2},{v1,n1,t2},{v0,n1,t3},{v4,n1,t0},
                {v4,n2,t0},{v0,n2,t1},{v3,n2,t2},{v3,n2,t2},{v7,n2,t3},{v4,n2,t0},
                {v6,n3,t0},{v2,n3,t1},{v1,n3,t2},{v1,n3,t2},{v5,n3,t3},{v6,n3,t0}
        };

        buffer = bufferManager.createGPUBuffer("TestBox");
        buffer->create(data_count, IGPUBuffer::eVT_VertexPNT, data, index_count, i);
    }

    {
        Vec3f v0(-1, -1, 0), v1(1, -1, 0),
              v2(1, 1, 0),   v3(-1, 1, 0);

        Vec2f t0 = Vec2f(0,0), t1 = Vec2f(1,0),
              t2 = Vec2f(1,1), t3 = Vec2f(0,1);

        const uint32 data_count = 4;
        VertPTf data[data_count] =
        {
                {v0,t0}, {v1,t1}, {v2,t2}, {v3,t3}
        };

        const uint32 index_count = 6;
        uint16 i[index_count]
        {
            0, 1, 2, 2, 3, 0
        };

        screen = bufferManager.createGPUBuffer("ScreenPlane");
        screen->create(data_count, IGPUBuffer::eVT_VertexPT, data, index_count, i);
    }

    {
        frameRender = shaderManager.loadShader("{SHADERS}/Debug/FrameRender/meta-info.xml");
        screenRender = shaderManager.loadShader("{SHADERS}/Debug/ScreenRender/meta-info.xml");
    }

    {
        uniformBuffer = bufferManager.createUniformBuffer("UniformBuffer");
        uniformBuffer->create(0, sizeof(UniformData), nullptr);
        uniformBuffer->bind();

        uint32 width, height;
        window->getFrameBufferSize(width, height);

        float32 scale = 1.0f / 2.0f;
        frameBufferVP = IRenderDriver::ViewPort(0, 0, width * scale, height * scale);
        displayBufferVP = IRenderDriver::ViewPort(0, 0, width, height);
        frameBuffer = bufferManager.createFrameBuffer("MainFrameBuffer");
        frameBuffer->createFrameBuffer(width * scale, height * scale, 1);
        frameBuffer->attachColorBuffer(IRenderDriver::RGBA16F, IRenderDriver::FILTER_NEAREST, IRenderDriver::WRAP_CLAMP_TO_EDGE);
        frameBuffer->attachDepthBuffer();
        frameBuffer->linkBuffers();

        depthBuffer = bufferManager.createDepthBuffer("DepthBuffer");
        depthBuffer->createDepthBuffer(width, height);
    }

    {
        IRenderDriver::ViewPort view;
        window->getFrameBufferSize(view.width, view.height);

        view.x = 0;
        view.y = 0;

        driver.viewPort(view);
    }

    {
        material = materialManager.getDefaultMaterial();
    }

    {
        textureManager.getMemoryUsage(&memorySizer);
        shaderManager.getMemoryUsage(&memorySizer);
        bufferManager.getMemoryUsage(&memorySizer);
        materialManager.getMemoryUsage(&memorySizer);

        memorySizer.print(40);
    }

    timer.start();

    while (!window->shouldClose())
    {
        {
            static uint32 counter = 0;
            counter += 1;
            timer.update();

            if (counter % 30 == 0) printf("Time=%lf FPS=%lf \n", timer.elapsed(), 1.0 / timer.elapsed());

            static float32 angle = 0;
            angle += 0.01;

            auto Model = Mat4x4f::rotate(Vec3f(0, 1, 1), angle);
            auto View =  Mat4x4f::lookAt(Vec3f(0, 0, 3), Vec3f(0,0, -1.0f), Vec3f(0,1,0));
            auto Proj =  Mat4x4f::perspective(Degrees(60.0f).radians().get(), 16.0f / 9.0f, 0.1f, 20.0f);

            UniformData data = {Proj.transpose(), View.transpose(), Model.transpose()};
            uniformBuffer->update(sizeof(UniformData), &data);

            frameRender->use();
            frameRender->setUniform("Texture0", 0u);
            frameRender->setUniform("CameraPosition", Vec3f(0, 0, 3));
            frameRender->setUniform("LightPosition", Vec3f(6 * Math::sin(angle * 0.8f), 0, 3));
            frameBuffer->bindFrameBuffer();
            // material->getMaterialLayer(IMaterial::eML_ALBEDO_MAP)->bind(0u);
            texture->bind(0u);
            uniformBuffer->bind();
            driver.clear(true, true, false);
            driver.depthTest(true);
            driver.viewPort(frameBufferVP);
            buffer->draw();

            screenRender->use();
            screenRender->setUniform("Texture0", 0);
            screenRender->setUniform("ViewPort", Vec4f(displayBufferVP.x, displayBufferVP.y, displayBufferVP.width, displayBufferVP.height));
            frameBuffer->bindColorBuffer(0, 0);
            driver.bindDefaultFrameBuffer();
            driver.clear(true, true, false);
            driver.depthTest(false);
            driver.viewPort(displayBufferVP);
            screen->draw();
        }

        driver.swapBuffers();
        driver.update();
    }

    /*

    {
        char tmp[Buffers::SIZE_512];
        char cpu[Buffers::SIZE_16];
        char gpu[Buffers::SIZE_16];

        OPEN_BLOCK("-------------- OpenGL driver primitives memory usage --------------");

        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IShader",
                ProfilingUtility::print(frameRender->getMemoryUsage(), cpu),
                ProfilingUtility::print(frameRender->getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IGPUBuffer",
                ProfilingUtility::print(buffer->getMemoryUsage(), cpu),
                ProfilingUtility::print(buffer->getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "ISampler",
                ProfilingUtility::print(sampler->getMemoryUsage(), cpu),
                ProfilingUtility::print(sampler->getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "ITexture",
                ProfilingUtility::print(texture->getMemoryUsage(), cpu),
                ProfilingUtility::print(texture->getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IRenderDriver",
                ProfilingUtility::print(driver.getMemoryUsage(), cpu),
                ProfilingUtility::print(driver.getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IFrameBuffer",
                ProfilingUtility::print(frameBuffer->getMemoryUsage(), cpu),
                ProfilingUtility::print(frameBuffer->getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IDepthBuffer",
                ProfilingUtility::print(depthBuffer->getMemoryUsage(), cpu),
                ProfilingUtility::print(depthBuffer->getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IUniformBuffer",
                ProfilingUtility::print(uniformBuffer->getMemoryUsage(), cpu),
                ProfilingUtility::print(uniformBuffer->getGPUMemoryUsage(), gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IMaterial",
                ProfilingUtility::print(material->getMemoryUsage(), cpu),
                ProfilingUtility::print(0, gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "ITextureManager",
                ProfilingUtility::print(textureManager.getMemoryUsage(), cpu),
                ProfilingUtility::print(0, gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IBufferManager",
                ProfilingUtility::print(bufferManager.getMemoryUsage(), cpu),
                ProfilingUtility::print(0, gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IShaderManager",
                ProfilingUtility::print(shaderManager.getMemoryUsage(), cpu),
                ProfilingUtility::print(0, gpu));
        PUSH_BLOCK(tmp);
        sprintf(tmp, " %20s: CPU %12s | GPU %12s", "IMaterialManager",
                ProfilingUtility::print(materialManager.getMemoryUsage(), cpu),
                ProfilingUtility::print(0, gpu));
        PUSH_BLOCK(tmp);

        CLOSE_BLOCK("-------------------------------------------------------------------");
    }

    */

    textureManager.deleteTexture(texture);
    textureManager.deleteSampler(sampler);
    bufferManager.deleteGPUBuffer(buffer);
    bufferManager.deleteGPUBuffer(screen);
    bufferManager.deleteFrameBuffer(frameBuffer);
    bufferManager.deleteDepthBuffer(depthBuffer);
    bufferManager.deleteUniformBuffer(uniformBuffer);
    shaderManager.deleteShader(frameRender);
    shaderManager.deleteShader(screenRender);
    materialManager.deleteMaterial(material);
}

#endif //BERSERK_OPENGLDRIVERTEST_H
