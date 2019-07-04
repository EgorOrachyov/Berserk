//
// Created by Egor Orachyov on 26.03.2019.
//

#ifndef BERSERK_FRAMEBUFFERTEST_H
#define BERSERK_FRAMEBUFFERTEST_H

#include "Platform/GLShader.h"
#include "Platform/GLGPUBuffer.h"
#include "Platform/GLFrameBuffer.h"
#include "Platform/GLDepthBuffer.h"
#include "Platform/GLUniformBuffer.h"
#include "Platform/GLRenderDriver.h"
#include "Platform/VertexTypes.h"
#include "FreeImageImporter.h"

#include "Managers/GLShaderManager.h"
#include "Managers/GLBufferManager.h"
#include "Managers/GLTextureManager.h"

#include "Utility/FileUtility.h"
#include "Logging/LogManager.h"
#include "../../Engine/Source/CoreSystem/Utility/ProfilingUtility.h"

void FrameBufferTest()
{
    using namespace Berserk;
    using namespace Berserk::Resources;
    using namespace Berserk::Importers;

    LogManager::getSingleton().setFileWriting(false);

    IShader *frameRender;
    IShader *screenRender;
    IWindow *window;
    ITexture *texture;
    ISampler *sampler;
    IGPUBuffer *buffer;
    IGPUBuffer *screen;

    auto setup = IWindow::WindowSetup();

    GLRenderDriver driver(setup);
    {
        window = driver.getMainWindow();
        driver.polygonMode(IRenderDriver::PolygonMode::FILL);
        driver.clear(Vec4f(1, 1, 1, 1));
    }

    FreeImageImporter importer;
    GLTextureManager textureManager(&importer, "../Engine/Textures/");
    GLBufferManager bufferManager;
    GLShaderManager shaderManager("../Engine/Shaders");

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

        Vec3f n0 = Vec3f(1, 0, 0), n1 = Vec3f(-1, 0, 0),
                n2 = Vec3f(0, 1, 0), n3 = Vec3f(0, -1, 0),
                n4 = Vec3f(0, 0, 1), n5 = Vec3f(0, 0, -1);

        Vec3f v0 = Vec3f(-w, w, w), v1 = Vec3f(-w, -w, w),
                v2 = Vec3f(w, -w, w), v3 = Vec3f(w, w, w),
                v4 = Vec3f(-w, w, -w), v5 = Vec3f(-w, -w, -w),
                v6 = Vec3f(w, -w, -w), v7 = Vec3f(w, w, -w);

        Vec2f t0 = Vec2f(0, 1), t1 = Vec2f(0, 0),
                t2 = Vec2f(1, 0), t3 = Vec2f(1.0, 1.0);

        const uint32 index_count = 36;
        uint16 i[index_count] =
                {
                        0, 1, 2, 3, 4, 5,
                        6, 7, 8, 9, 10, 11,
                        12, 13, 14, 15, 16, 17,
                        18, 19, 20, 21, 22, 23,
                        24, 25, 26, 27, 28, 29,
                        30, 31, 32, 33, 34, 35
                };

        const uint32 data_count = 36;
        VertPNTf data[data_count] =
                {
                        {v0, n4, t0},
                        {v1, n4, t1},
                        {v2, n4, t2},
                        {v2, n4, t2},
                        {v3, n4, t3},
                        {v0, n4, t0},
                        {v3, n0, t0},
                        {v2, n0, t1},
                        {v6, n0, t2},
                        {v6, n0, t2},
                        {v7, n0, t3},
                        {v3, n0, t0},
                        {v7, n5, t0},
                        {v6, n5, t1},
                        {v5, n5, t2},
                        {v5, n5, t2},
                        {v4, n5, t3},
                        {v7, n5, t0},
                        {v4, n1, t0},
                        {v5, n1, t1},
                        {v1, n1, t2},
                        {v1, n1, t2},
                        {v0, n1, t3},
                        {v4, n1, t0},
                        {v4, n2, t0},
                        {v0, n2, t1},
                        {v3, n2, t2},
                        {v3, n2, t2},
                        {v7, n2, t3},
                        {v4, n2, t0},
                        {v6, n3, t0},
                        {v2, n3, t1},
                        {v1, n3, t2},
                        {v1, n3, t2},
                        {v5, n3, t3},
                        {v6, n3, t0}
                };

        buffer = bufferManager.createGPUBuffer("Tests Box");
        buffer->create(data_count, IGPUBuffer::VertexPNT, data, index_count, i);
    }

    {
        Vec3f v0(-1, -1, 0), v1(1, -1, 0),
                v2(1, 1, 0), v3(-1, 1, 0);

        Vec2f t0 = Vec2f(0, 0), t1 = Vec2f(1, 0),
                t2 = Vec2f(1, 1), t3 = Vec2f(0, 1);

        const uint32 data_count = 4;
        VertPTf data[data_count] =
                {
                        {v0, t0},
                        {v1, t1},
                        {v2, t2},
                        {v3, t3}
                };

        const uint32 index_count = 6;
        uint16 i[index_count]
                {
                        0, 1, 2, 2, 3, 0
                };

        screen = bufferManager.createGPUBuffer("Screen plane");
        screen->create(data_count, IGPUBuffer::VertexPT, data, index_count, i);
    }

    {
        frameRender = shaderManager.loadShader("{SHADERS}/Debug/FrameRender/meta-info.xml");
        screenRender = shaderManager.loadShader("{SHADERS}/Debug/ScreenRender/meta-info.xml");
    }

    {
        uint32 width, height;
        window->getFrameBufferSize(width, height);
    }

    class Buffer {
    public:

        Buffer(uint32 width, uint32 height)
        {
            glGenFramebuffers(1, &mFrameBufferID);
            glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);

            glGenTextures(1, &mColor);
            glBindTexture(GL_TEXTURE_2D, mColor);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColor, 0);

            //glGenTextures(1, &mDepthBuffer);
            //glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, mDepthBuffer);
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            //glBindTexture(GL_TEXTURE_2D, 0);

            //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer, 0);

            glGenRenderbuffers(1, &mDepthBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);

            uint32 attachments[1] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1, attachments);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                printf("GLFrameBuffer: incomplete\n");
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void bind()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
        }

        void uniform(uint32 textureSlot)
        {
            glActiveTexture(GL_TEXTURE0 + textureSlot);
            glBindTexture(GL_TEXTURE_2D, mColor);
        }

        uint32 mFrameBufferID;
        uint32 mColor;
        uint32 mDepthBuffer;

    };

    uint32 width, height;
    window->getFrameBufferSize(width, height);
    Buffer testFBO(width, height);

    while (!window->shouldClose()) {
        {
            static float32 angle = 0;
            angle += 0.01;

            auto Model = Mat4x4f::rotate(Vec3f(0, 1, 1), angle);
            auto View = Mat4x4f::lookAt(Vec3f(0, 0, 3), Vec3f(0, 0, -1.0f), Vec3f(0, 1, 0));
            auto Proj = Mat4x4f::perspective(Degrees(60.0f).radians().get(), 16.0f / 9.0f, 0.1f, 20.0f);

            //frameRender->use();
            //frameRender->setUniform("Texture0", 0u);
            //frameRender->setUniform("CameraPosition", Vec3f(0, 0, 3));
            //frameRender->setUniform("LightPosition", Vec3f(6 * Math::sin(angle * 0.8f), 0, 3));
            //testFBO.bind();
            //texture->bind(0u);
            //uniformBuffer->bind();
            //driver.viewPort(IRenderDriver::ViewPort(0, 0, 1280, 720));
            //driver.clear(true, true, false);
            //driver.depthTest(true);
            //buffer->draw();

            screenRender->use();
            screenRender->setUniform("Texture0", 0u);
            texture->bind(0u);
            testFBO.bind();
            driver.clear(true, true, false);
            driver.depthTest(false);
            driver.clear(true, true, false);
            driver.depthTest(false);
            //driver.viewPort(IRenderDriver::ViewPort(0, 0, 1280, 720));
            screen->draw();


            screenRender->use();
            screenRender->setUniform("Texture0", 0u);
            glBindSampler(0, 0);
            testFBO.uniform((uint32)0);
            driver.bindDefaultFrameBuffer();
            driver.clear(true, true, false);
            driver.depthTest(false);
            //driver.viewPort(IRenderDriver::ViewPort(0, 0, 1280, 720));
            screen->draw();
        }

        driver.swapBuffers();
        driver.update();
    }

    textureManager.deleteTexture(texture);
    textureManager.deleteSampler(sampler);
    bufferManager.deleteGPUBuffer(buffer);
    bufferManager.deleteGPUBuffer(screen);
    shaderManager.deleteShader(frameRender);
    shaderManager.deleteShader(screenRender);
}

#endif //BERSERK_FRAMEBUFFERTEST_H
