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
#include <BerserkRHI/RHITexture.hpp>
#include <BerserkRHI/RHIVertexDeclaration.hpp>
#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Platform/WindowManager.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Math/TVecN.hpp>
#include <BerserkCore/Math/TMatMxN.hpp>
#include <BerserkCore/Math/Utils3d.hpp>
#include <BerserkCore/Templates/Timer.hpp>
#include <BerserkCore/Image/Image.hpp>
#include <BerserkCore/Image/PixelUtil.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(RHIFixture)

struct Vertex {
    Math::Vec3f pos;
    Math::Vec3f color;
    Math::Vec2f texCoords;
};

struct Transform {
    Math::Mat4x4f projViewModel;
};

void GetQuadVertices(const Vertex* &vertices, size_t &count) {
    static Vertex vt[] = {
        { Math::Vec3f(-1, 1, 0),  Math::Vec3f(1, 0, 0), Math::Vec2f(0, 1), },
        { Math::Vec3f(-1, -1, 0), Math::Vec3f(0, 1, 0), Math::Vec2f(0, 0), },
        { Math::Vec3f(1, -1, 0),  Math::Vec3f(0, 0, 1), Math::Vec2f(1, 0), },
        { Math::Vec3f(1, 1, 0),   Math::Vec3f(1, 1, 1), Math::Vec2f(1, 1) }
    };

    vertices = vt;
    count = sizeof(vt) / sizeof(Vertex);
}

void GetQuadIndices(const uint32* &indices, size_t &count) {
    static uint32 id[] = {
        0, 1, 2,
        2, 3, 0
    };

    indices = id;
    count = sizeof(id) / sizeof(uint32);
}

void GetMainPassShaderVsGLSL410(const char* &code, uint32 &length) {
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

void GetMainPassShaderFsGLSL410(const char* &code, uint32 &length) {
    static const char sourceCode[] = R"(
        #version 410 core
        layout (location = 0) out vec4 outColor;

        uniform sampler2D texBackground;

        in vec3 fsColor;
        in vec2 fsTexCoords;

        void main() {
            vec3 color = fsColor;
            color *= texture(texBackground, fsTexCoords).rgb;
            outColor = vec4(pow(color, vec3(1.0f/2.2f)), 0.5f);
        }
    )";

    code = sourceCode;
    length = StringUtils::Length(sourceCode);
}

void GetMainPassShaderVsGLSL450VK(const char* &code, uint32 &length) {
    static const char sourceCode[] = R"(
        #version 450
        layout (location = 0) in vec3 vsPosition;
        layout (location = 1) in vec3 vsColor;
        layout (location = 2) in vec2 vsTexCoords;

        layout (binding = 0, std140) uniform Transform {
           mat4 MVP;
        };

        layout (location = 0) out vec3 fsColor;
        layout (location = 1) out vec2 fsTexCoords;

        void main() {
            fsColor = vsColor;
            fsTexCoords = vsTexCoords;
            gl_Position = MVP * vec4(vsPosition, 1.0f);
        }
    )";

    code = sourceCode;
    length = StringUtils::Length(sourceCode);
}

void GetMainPassShaderFsGLSL450VK(const char* &code, uint32 &length) {
    static const char sourceCode[] = R"(
        #version 450
        layout (location = 0) out vec4 outColor;

        layout (binding = 1) uniform sampler2D texBackground;

        layout (location = 0) in vec3 fsColor;
        layout (location = 1) in vec2 fsTexCoords;

        void main() {
            vec3 color = fsColor;
            color *= texture(texBackground, fsTexCoords).rgb;
            outColor = vec4(pow(color, vec3(1.0f)), 0.5f);
        }
    )";

    code = sourceCode;
    length = StringUtils::Length(sourceCode);
}

RcPtr<Data> AllocateCode(const char* code, uint32 length) {
    return Data::Make(code, length);
}

RcPtr<Data> AllocateStruct(size_t size) {
    return Data::Make(size);
}

RcPtr<PixelData> AllocatePixelBuffer(PixelDataFormat pixelDataFormat, const Image& image) {
    auto pixelData = Memory::Make<PixelData>(pixelDataFormat, PixelDataType::UBYTE, image.GetBufferRef());
    return RcPtr<PixelData>(pixelData);
}

TEST_F(RHIFixture, SimpleQuad) {
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Current thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());

    volatile bool finish = false;
    volatile bool visible = true;

    auto exitCallback = [&](const Window::EventData& data) {
        if (data.eventType == Window::EventType::CloseRequested) {
            finish = true;
        }
    };

    auto visibilityCallback = [&](const Window::EventData& data) {
        if (data.eventType == Window::EventType::Minimized)
            visible = false;
        else if (data.eventType == Window::EventType::Restored)
            visible = true;
    };

    Window::Desc windowDesc;
    windowDesc.name = BERSERK_TEXT("MAIN-WINDOW");
    windowDesc.title = BERSERK_TEXT("Berserk RHI test");
    windowDesc.size = Math::Size2i(1280, 720);
    auto window = WindowManager::CreateWindow(windowDesc);
    auto exitEvent = window->OnWindowEvent.Subscribe(exitCallback);
    auto visibilityEvent = window->OnWindowEvent.Subscribe(visibilityCallback);

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

    uint32 vertexShaderLength, fragmentShaderLength;
    const char *vertexShaderCode, *fragmentShaderCode;

    RHI::ShaderLanguage language;
    if (device.GetSupportedShaderLanguages().Contains<Equals<RHI::ShaderLanguage>>(RHI::ShaderLanguage::GLSL410GL)) {
        GetMainPassShaderVsGLSL410(vertexShaderCode, vertexShaderLength);
        GetMainPassShaderFsGLSL410(fragmentShaderCode, fragmentShaderLength);
        language = RHI::ShaderLanguage::GLSL410GL;
    } else if (device.GetSupportedShaderLanguages().Contains<Equals<RHI::ShaderLanguage>>(RHI::ShaderLanguage::GLSL450VK)) {
        GetMainPassShaderVsGLSL450VK(vertexShaderCode, vertexShaderLength);
        GetMainPassShaderFsGLSL450VK(fragmentShaderCode, fragmentShaderLength);
        language = RHI::ShaderLanguage::GLSL450VK;
    } else {
        BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Failed to find shader sources}"));
        return;
    }

    RHI::Program::Desc programDesc;
    programDesc.name = "Test Shader";
    programDesc.language = language;
    programDesc.stages.Resize(2);
    programDesc.stages[0].type = RHI::ShaderType::Vertex;
    programDesc.stages[0].sourceCode = AllocateCode(vertexShaderCode, vertexShaderLength);
    programDesc.stages[1].type = RHI::ShaderType::Fragment;
    programDesc.stages[1].sourceCode = AllocateCode(fragmentShaderCode, fragmentShaderLength);
    auto program = device.CreateProgram(programDesc);

    while (!finish) {
        FixedUpdate();

        // Check errors program
        if (program->GetCompilationStatus() == RHI::Program::Status::FailedCompile) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Failed to compile shader: {0}"), program->GetShaderName());
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("{0}"), program->GetCompilerMessage());
            return;
        }

        // If Ok, continue execution
        if (program->GetCompilationStatus() == RHI::Program::Status::Compiled) {
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
                BERSERK_CORE_LOG_INFO(BERSERK_TEXT(" - name={0} arraySize={1} arrayStride={2} "
                                                   "elementSize={3} blockSlot={4} blockOffset={5}"),
                                      dataParam.name, dataParam.arraySize, dataParam.arrayStride,
                                      dataParam.elementSize, dataParam.blockSlot, dataParam.blockOffset);
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

    RHI::VertexDeclaration::Desc vertexDeclarationDesc{};
    vertexDeclarationDesc.Resize(3);
    vertexDeclarationDesc[0].type = RHI::VertexElementType::Float3;
    vertexDeclarationDesc[0].frequency = RHI::VertexFrequency::PerVertex;
    vertexDeclarationDesc[0].buffer = 0;
    vertexDeclarationDesc[0].offset = offsetof(Vertex, pos);
    vertexDeclarationDesc[0].stride = sizeof(Vertex);
    vertexDeclarationDesc[1].type = RHI::VertexElementType::Float3;
    vertexDeclarationDesc[1].frequency = RHI::VertexFrequency::PerVertex;
    vertexDeclarationDesc[1].buffer = 0;
    vertexDeclarationDesc[1].offset = offsetof(Vertex, color);
    vertexDeclarationDesc[1].stride = sizeof(Vertex);
    vertexDeclarationDesc[2].type = RHI::VertexElementType::Float2;
    vertexDeclarationDesc[2].frequency = RHI::VertexFrequency::PerVertex;
    vertexDeclarationDesc[2].buffer = 0;
    vertexDeclarationDesc[2].offset = offsetof(Vertex, texCoords);
    vertexDeclarationDesc[2].stride = sizeof(Vertex);
    auto vertexDeclaration = device.CreateVertexDeclaration(vertexDeclarationDesc);

    RHI::VertexBuffer::Desc vertexBufferDesc{};
    vertexBufferDesc.size = verticesCount * sizeof(Vertex);
    vertexBufferDesc.bufferUsage = RHI::BufferUsage::Static;
    auto vertexBuffer = device.CreateVertexBuffer(vertexBufferDesc);
    auto vertexBufferData = Data::Make(vertices, verticesCount * sizeof(Vertex));

    commands->UpdateVertexBuffer(vertexBuffer, 0,verticesCount * sizeof(Vertex), vertexBufferData);

    RHI::IndexBuffer::Desc indexBufferDesc{};
    indexBufferDesc.size = indicesCount * sizeof(uint32);
    indexBufferDesc.bufferUsage = RHI::BufferUsage::Static;
    auto indexBuffer = device.CreateIndexBuffer(indexBufferDesc);
    auto indexBufferData = Data::Make(indices, indicesCount * sizeof(uint32));

    commands->UpdateIndexBuffer(indexBuffer, 0,indicesCount * sizeof(uint32), indexBufferData);

    RHI::UniformBuffer::Desc uniformBufferDesc{};
    uniformBufferDesc.size = sizeof(Transform);
    uniformBufferDesc.bufferUsage = RHI::BufferUsage::Dynamic;
    auto uniformBuffer = device.CreateUniformBuffer(uniformBufferDesc);
    auto transformBuffer = AllocateStruct(sizeof(Transform));

    RHI::Sampler::Desc samplerDesc;
    samplerDesc.minFilter = RHI::SamplerMinFilter::LinearMipmapLinear;
    samplerDesc.magFilter = RHI::SamplerMagFilter::Linear;
    samplerDesc.maxAnisotropy = 16.0f;
    samplerDesc.useAnisotropy = true;
    samplerDesc.u = RHI::SamplerRepeatMode::ClampToEdge;
    samplerDesc.v = RHI::SamplerRepeatMode::ClampToEdge;
    samplerDesc.w = RHI::SamplerRepeatMode::ClampToEdge;
    auto sampler = device.CreateSampler(samplerDesc);

    auto image = Image::Load(BERSERK_TEXT("../../Engine/Resources/Textures/background-32x8.png"), Image::Channels::RGBA);
    assert(!image.IsEmpty());

    RHI::Texture::Desc textureDesc;
    textureDesc.name = "Background-32x8.png";
    textureDesc.width = image.GetWidth();
    textureDesc.height = image.GetHeight();
    textureDesc.depth = 1;
    textureDesc.arraySlices = 1;
    textureDesc.mipsCount = PixelUtil::GetMaxMipsCount(textureDesc.width, textureDesc.height, textureDesc.depth);
    textureDesc.textureType = RHI::TextureType::Texture2d;
    textureDesc.textureFormat = RHI::TextureFormat::RGBA8;
    textureDesc.textureUsage = { RHI::TextureUsage::Sampling };
    auto texture = device.CreateTexture(textureDesc);

    commands->UpdateTexture2D(texture, 0, {0, 0, image.GetWidth(), image.GetHeight()}, AllocatePixelBuffer(PixelDataFormat::RGBA, image));
    commands->GenerateMipMaps(texture);

    while (!finish) {
        FixedUpdate();

        static auto clip = device.GetClipMatrix();
        static float angle = 0.0f;
        static float gamma = device.GetDriverType() == Berserk::RHI::Type::Vulkan? 2.2f: 1.0f;
        float speed = 0.01f;

        angle += speed;

        Math::Vec3f eye(0, 0, 3);
        Math::Vec3f dir(0, 0, -1);
        Math::Vec3f up(0, 1, 0);
        float fov = Math::Utils::DegToRad(60.0f);
        float near = 0.1f, far = 10.0f;

        Math::Mat4x4f model = Math::Utils3d::RotateY(angle);
        Math::Mat4x4f view = Math::Utils3d::LookAt(eye, dir, up);
        Math::Mat4x4f proj = Math::Utils3d::Perspective(fov, window->GetAspectRatio(), near, far);
        Math::Mat4x4f projViewModel = proj * view * model;

        Transform t = { (clip * projViewModel).Transpose() };
        Memory::Copy(transformBuffer->GetDataWrite(), &t, sizeof(Transform));

        auto meta = program->GetProgramMeta();
        auto size = window->GetFramebufferSize();

        RHI::RenderPass renderPass{};
        renderPass.name = "Draw Rect Pass";
        renderPass.viewport.left = 0;
        renderPass.viewport.bottom = 0;
        renderPass.viewport.width = size.x();
        renderPass.viewport.height = size.y();
        renderPass.depthStencilAttachment.depthClear = 1.0f;
        renderPass.depthStencilAttachment.depthOption = RHI::RenderTargetOption::ClearStore;
        renderPass.depthStencilAttachment.stencilClear = 0;
        renderPass.depthStencilAttachment.stencilOption = RHI::RenderTargetOption::DiscardDiscard;
        renderPass.colorAttachments.Resize(1);
        renderPass.colorAttachments[0].clearColor = Color(0.298f, 0.0f, 0.321f).PowA(gamma);
        renderPass.colorAttachments[0].option = RHI::RenderTargetOption::ClearStore;

        RHI::PipelineState pipelineState{};
        pipelineState.primitivesType = RHI::PrimitivesType::Triangles;
        pipelineState.program = program;
        pipelineState.declaration = vertexDeclaration;
        pipelineState.depthStencilState = RHI::PipelineState::DepthStencilState::CreateDepthState(false);
        pipelineState.blendState.attachments.Resize(1);

        commands->BeginScene(window);
        commands->UpdateUniformBuffer(uniformBuffer, 0, sizeof(Transform), transformBuffer);
        commands->BeginRenderPass(renderPass);

        if (visible) {
            commands->BindPipelineState(pipelineState);
            commands->BindUniformBuffer(uniformBuffer, meta->paramBlocks["Transform"].slot, 0, sizeof(Transform));
            commands->BindTexture(texture, meta->samplers["texBackground"].location);
            commands->BindSampler(sampler, meta->samplers["texBackground"].location);
            commands->BindVertexBuffers({vertexBuffer});
            commands->BindIndexBuffer({indexBuffer}, RHI::IndexType::Uint32);
            commands->DrawIndexed(indicesCount, 0, 1);
        }

        commands->EndRenderPass();
        commands->EndScene();
        commands->Flush();
    }
}

BERSERK_GTEST_MAIN