/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkRHI/RHIDriver.hpp>
#include <BerserkRHI/RHICmdList.hpp>
#include <BerserkRHI/RHIVertexBuffer.hpp>
#include <BerserkRHI/RHIIndexBuffer.hpp>
#include <BerserkRHI/RHIUniformBuffer.hpp>
#include <BerserkRHI/RHIProgram.hpp>
#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Platform/WindowManager.hpp>
#include <BerserkCore/Math/TVecN.hpp>
#include <BerserkCore/Math/TMatMxN.hpp>
#include <BerserkCore/Math/Utils3d.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Templates/MemoryBuffer.hpp>

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

void GetOpenGLVertexShaderCode(const char* &code, uint32 &length) {
    static const char sourceCode[] = R"(
        #version 410 core
        layout (location = 0) in vec3 vsPosition;
        layout (location = 1) in vec3 vsColor;
        layout (location = 2) in vec2 vsTexCoords;

        layout (std140) uniform Transform {
            mat4 MVP;
        };

        out vec3 fsColor;
        out vec2 fsTexCoords;

        void main() {
            fsColor = vsColor;
            fsTexCoords = vsTexCoords;
            gl_Position = MVP * vec4(vsPosition, 1.0f);
        }
    )";

    code = sourceCode;
    length = StringUtils::Length(sourceCode);
}

void GetOpenGLFragmentShaderCode(const char* &code, uint32 &length) {
    static const char sourceCode[] = R"(
        #version 410 core
        layout (location = 0) out vec4 outColor;

        uniform sampler2D texBackground[2];

        in vec3 fsColor;
        in vec2 fsTexCoords;

        void main() {
            vec3 color = fsColor;
            color += texture(texBackground[0], fsTexCoords).rgb;
            color += texture(texBackground[1], fsTexCoords).rgb;
            outColor = vec4(color, 1.0f);
        }
    )";

    code = sourceCode;
    length = StringUtils::Length(sourceCode);
}

RefCounted<ReadOnlyMemoryBuffer> AllocateCode(const char* code, uint32 length) {
    return (RefCounted<ReadOnlyMemoryBuffer>) SystemMemoryBuffer::Create(length, code);
}

template <typename T>
RefCounted<ReadOnlyMemoryBuffer> AllocateStruct(const T& t) {
    return (RefCounted<ReadOnlyMemoryBuffer>) SystemMemoryBuffer::Create(sizeof(T), (const void*) &t);
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
    // In the user applications, this function will be automatically called by "GuiApplication" class.
    InitializeRHI();
    // At this moment we are able to make RHI calls

    auto& device = RHI::Driver::GetDevice();

    const Vertex* vertices;
    const uint32* indices;
    size_t verticesCount;
    size_t indicesCount;

    GetQuadVertices(vertices, verticesCount);
    GetQuadIndices(indices, indicesCount);

    auto commands = device.CreateCmdList();

    RHI::VertexBuffer::Desc vertexBufferDesc{};
    vertexBufferDesc.size = verticesCount * sizeof(Vertex);
    vertexBufferDesc.bufferUsage = RHI::BufferUsage::Static;
    auto vertexBuffer = device.CreateVertexBuffer(vertexBufferDesc);
    auto vertexBufferData = (RefCounted<ReadOnlyMemoryBuffer>) MemoryBufferGeneric<>::Create(verticesCount * sizeof(Vertex), vertices);;

    RHI::IndexBuffer::Desc indexBufferDesc{};
    indexBufferDesc.size = indicesCount * sizeof(uint32);
    indexBufferDesc.bufferUsage = RHI::BufferUsage::Static;
    auto indexBuffer = device.CreateIndexBuffer(indexBufferDesc);
    auto indexBufferData = (RefCounted<ReadOnlyMemoryBuffer>) SystemMemoryBuffer::Create(indicesCount * sizeof(uint32), indices);

    RHI::UniformBuffer::Desc uniformBufferDesc{};
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

    uint32 vertexShaderLength, fragmentShaderLength;
    const char *vertexShaderCode, *fragmentShaderCode;

    GetOpenGLVertexShaderCode(vertexShaderCode, vertexShaderLength);
    GetOpenGLFragmentShaderCode(fragmentShaderCode, fragmentShaderLength);

    RHI::Program::Desc programDesc;
    programDesc.name = "Test Shader";
    programDesc.language = RHI::ShaderLanguage::GLSL;
    programDesc.stages.Resize(2);
    programDesc.stages[0].type = RHI::ShaderType::Vertex;
    programDesc.stages[0].sourceCode = AllocateCode(vertexShaderCode, vertexShaderLength);
    programDesc.stages[1].type = RHI::ShaderType::Fragment;
    programDesc.stages[1].sourceCode = AllocateCode(fragmentShaderCode, fragmentShaderLength);
    auto program = device.CreateProgram(programDesc);

    commands->UpdateVertexBuffer(vertexBuffer, 0,verticesCount * sizeof(Vertex), vertexBufferData);
    commands->UpdateIndexBuffer(indexBuffer, 0,indicesCount * sizeof(uint32), indexBufferData);

    while (!finish) {
        FixedUpdate();

        // Check errors program
        if (program->GetCompilationStatus() == RHI::Program::CompilationStatus::FailedCompile) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Failed to compile shader {0}"), program->GetShaderName());
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("{0}"), program->GetCompilerMessage());
            return;
        }

        // If Ok, continue execution
        if (program->GetCompilationStatus() == RHI::Program::CompilationStatus::Compiled) {
            auto meta = program->GetProgramMeta();

            BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Program \"{0}\" inputs:"), program->GetShaderName());
            for (auto& entry: meta->inputs) {
                auto& inputAttribute = entry.GetSecond();
                BERSERK_CORE_LOG_INFO(BERSERK_TEXT(" - name={0} location={1}"),
                                      inputAttribute.name, inputAttribute.location);
            }

            BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Program \"{0}\" params:"), program->GetShaderName());
            for (auto& entry: meta->params) {
                auto& dataParam = entry.GetSecond();
                BERSERK_CORE_LOG_INFO(BERSERK_TEXT(" - name={0} arraySize={1} arrayStride={2} elementSize={3} blockIndex={4} blockOffset={5}"),
                                      dataParam.name, dataParam.arraySize, dataParam.arrayStride,
                                      dataParam.elementSize, dataParam.blockIndex, dataParam.blockOffset);
            }

            BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Program \"{0}\" param blocks:"), program->GetShaderName());
            for (auto& entry: meta->paramBlocks) {
                auto& dataParamBlock = entry.GetSecond();
                BERSERK_CORE_LOG_INFO(BERSERK_TEXT(" - name={0} size={1} slot={2}"),
                                      dataParamBlock.name, dataParamBlock.size, dataParamBlock.slot);
            }

            BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Program \"{0}\" samplers:"), program->GetShaderName());
            for (auto& entry: meta->samplers) {
                auto& objectParam = entry.GetSecond();
                BERSERK_CORE_LOG_INFO(BERSERK_TEXT(" - name={0} location={1} arraySize={2}"),
                                      objectParam.name, objectParam.location, objectParam.arraySize);
            }

            break;
        }
    }

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

        Transform t = { projViewModel.Transpose() };

        //commands->UpdateUniformBuffer(uniformBuffer, 0, sizeof(Transform), AllocateStruct(t));
        commands->Flush();

        ThreadManager::CurrentThreadSleep(1000 * 30);
    }
}

BERSERK_GTEST_MAIN