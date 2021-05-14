/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkRHI/RHIDriver.hpp>
#include <BerserkRHI/RHICmdList.hpp>
#include <BerserkRHI/RHIVertexBuffer.hpp>
#include <BerserkRHI/RHIIndexBuffer.hpp>
#include <BerserkRHI/RHIUniformBuffer.hpp>
#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Platform/WindowManager.hpp>
#include <BerserkCore/Math/TVecN.hpp>
#include <BerserkCore/Math/TMatMxN.hpp>
#include <BerserkCore/Math/Utils3d.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Memory/MemoryBufferGeneric.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(RHIFixture)

struct Vertex {
    Math::Vec3f pos;
    Math::Vec3f color;
};

struct Transform {
    Math::Mat4x4f projViewModel;
};

void GetQuadVertices(const Vertex* &vertices, size_t &count) {
    static Vertex vt[] = {
        { Math::Vec3f(-1, 1, 0),  Math::Vec3f(1, 0, 0) },
        { Math::Vec3f(-1, -1, 0), Math::Vec3f(0, 1, 0) },
        { Math::Vec3f(1, -1, 0),  Math::Vec3f(0, 0, 1) },
        { Math::Vec3f(1, 1, 0),   Math::Vec3f(1, 1, 1) }
    };

    vertices = vt;
    count = sizeof(vt) / sizeof(Vertex);
}

void GetQuadIndices(const uint32* &indices, size_t &count) {
    static uint32 id[] = {
        0, 1, 2,
        2, 3, 1
    };

    indices = id;
    count = sizeof(id) / sizeof(uint32);
}

TEST_F(RHIFixture, Test) {
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Current thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());

    volatile bool finish = false;

    auto exitCallback = [&](const Window::EventData& data) {
        if (data.eventType == Window::EventType::CloseRequested) {
            finish = true;
        }
    };

    Window::Desc windowDesc;
    windowDesc.name = BERSERK_TEXT("MAIN-WINDOW");
    windowDesc.title = BERSERK_TEXT("Berserk RHI test");
    windowDesc.size = Math::Size2i(1280, 720);
    auto window = WindowManager::CreateWindow(windowDesc);
    auto exitEvent = window->OnWindowEvent.Subscribe(exitCallback);

    // Primary window is created. Now we are able to initialize RHI device.
    // In the user applications, this function will be automatically called by GuiApplication class.
    InitializeRHI();
    // At this moment we are able to make RHI calls

    auto& device = RHI::Driver::GetDevice();

    const Vertex* vertices;
    const uint32* indices;
    size_t verticesCount;
    size_t indicesCount;

    GetQuadVertices(vertices, verticesCount);
    GetQuadIndices(indices, indicesCount);

    RHI::VertexBuffer::Desc vertexBufferDesc;
    vertexBufferDesc.size = verticesCount * sizeof(Vertex);
    vertexBufferDesc.bufferUsage = RHI::BufferUsage::Static;
    vertexBufferDesc.buffer = (Ref<MemoryBuffer>) MemoryBufferGeneric<>::Create(verticesCount * sizeof(Vertex), vertices);
    auto vertexBuffer = device.CreateVertexBuffer(vertexBufferDesc);

    RHI::IndexBuffer::Desc indexBufferDesc;
    indexBufferDesc.size = indicesCount * sizeof(uint32);
    indexBufferDesc.bufferUsage = RHI::BufferUsage::Static;
    indexBufferDesc.buffer = (Ref<MemoryBuffer>) MemoryBufferGeneric<>::Create(indicesCount * sizeof(uint32), indices);
    auto indexBuffer = device.CreateIndexBuffer(indexBufferDesc);

    RHI::UniformBuffer::Desc uniformBufferDesc;
    uniformBufferDesc.size = sizeof(Transform);
    uniformBufferDesc.bufferUsage = RHI::BufferUsage::Dynamic;
    auto uniformBuffer = device.CreateUniformBuffer(uniformBufferDesc);

    RHI::Sampler::Desc samplerDesc;
    samplerDesc.minFilter = RHI::SamplerMinFilter::LinearMipmapLinear;
    samplerDesc.magFilter = RHI::SamplerMagFilter::Linear;
    samplerDesc.u = RHI::SamplerRepeatMode::Repeat;
    samplerDesc.v = RHI::SamplerRepeatMode::Repeat;
    samplerDesc.w = RHI::SamplerRepeatMode::Repeat;
    auto sampler = device.CreateSampler(samplerDesc);

    RHI::CmdList commands;

    while (!finish) {
        FixedUpdate();

        static float angle = 0.0f;
        float speed = 0.001f;

        angle += speed;

        Math::Vec3f eye(0, 0, -4);
        Math::Vec3f dir(0, 0, 1);
        Math::Vec3f up(0, 1, 0);
        float fov = Math::Utils::DegToRad(90.0f);
        float near = 0.1f, far = 10.0f;

        Math::Mat4x4f model = Math::Utils3d::RotateY(angle);
        Math::Mat4x4f view = Math::Utils3d::LookAt(eye, dir, up);
        Math::Mat4x4f proj = Math::Utils3d::Perspective(fov, window->GetAspectRatio(),near, far);
        Math::Mat4x4f projViewModel = proj * view * model;

        {
            Transform t = { projViewModel.Transpose() };

            commands.UpdateUniformBuffer(uniformBuffer, 0, sizeof(Transform), (Ref<MemoryBuffer>) MemoryBufferGeneric<>::Create(sizeof(Transform), &t));
            commands.Commit();
        }

        ThreadManager::CurrentThreadSleep(1000 * 30);
    }
}

BERSERK_GTEST_MAIN