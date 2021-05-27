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
#include <BerserkCore/Templates/MemoryBuffer.hpp>
#include <BerserkCore/Image/Image.hpp>
#include <BerserkCore/Image/PixelUtil.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(RHIFixture)

struct Vertex {
    Math::Vec3f pos;
};

struct Transform {
    Math::Mat4x4f projViewModel;
};

void GetCubeVertices(const Vertex* &vertices, size_t &count) {
    static Vertex vt[] = {
            { Math::Vec3f(-1, 1, 1) },
            { Math::Vec3f(-1, -1, 1) },
            { Math::Vec3f(1, -1, 1) },
            { Math::Vec3f(1, 1, 1) },
            { Math::Vec3f(-1, 1, -1) },
            { Math::Vec3f(-1, -1, -1) },
            { Math::Vec3f(1, -1, -1) },
            { Math::Vec3f(1, 1, -1) }
    };

    vertices = vt;
    count = sizeof(vt) / sizeof(Vertex);
}

void GetCubeIndices(const uint32* &indices, size_t &count) {
    static uint32 id[] = {
            0, 1, 2,
            2, 3, 0,
            3, 2, 6,
            6, 7, 3,
            7, 6, 5,
            5, 4, 7,
            4, 5, 1,
            1, 0, 4,
            4, 0, 3,
            3, 7, 4,
            6, 2, 1,
            1, 5, 6
    };

    indices = id;
    count = sizeof(id) / sizeof(uint32);
}

void GetMainPassShaderVsGLSL410(const char* &code, uint32 &length) {
    static const char sourceCode[] = R"(
        #version 410 core
        layout (location = 0) in vec3 vsPosition;

        layout (std140) uniform Transform {
            mat4 MVP;
        };

        out vec3 fsTexCoords;

        void main() {
            fsTexCoords = vsPosition;
            vec4 position = MVP * vec4(vsPosition, 1.0f);
            gl_Position = position.xyww;
        }
    )";

    code = sourceCode;
    length = StringUtils::Length(sourceCode);
}

void GetMainPassShaderFsGLSL410(const char* &code, uint32 &length) {
    static const char sourceCode[] = R"(
        #version 410 core
        layout (location = 0) out vec4 outColor;

        uniform samplerCube texBackground;

        const float gamma = 2.2f;

        in vec3 fsTexCoords;

        void main() {
            vec3 color = texture(texBackground, fsTexCoords).rgb;
            outColor = vec4(pow(color, vec3(1.0f/gamma)), 1.0f);
        }
    )";

    code = sourceCode;
    length = StringUtils::Length(sourceCode);
}

void LoadSkyBox(Array<Image> &images) {
    String prefix = BERSERK_TEXT("../../Engine/Resources/Textures/skybox-");
    Array<String> names = {
        BERSERK_TEXT("right.jpg"),
        BERSERK_TEXT("left.jpg"),
        BERSERK_TEXT("top.jpg"),
        BERSERK_TEXT("bottom.jpg"),
        BERSERK_TEXT("back.jpg"),
        BERSERK_TEXT("front.jpg")
    };

    for (auto& name: names) {
        Image image = Image::Load(prefix + name, Image::Channels::RGB);
        assert(!image.IsEmpty());

        images.Emplace(std::move(image));
    }
}

RefCounted<ReadOnlyMemoryBuffer> AllocateCode(const char* code, uint32 length) {
    return (RefCounted<ReadOnlyMemoryBuffer>) SystemMemoryBuffer::Create(length, code);
}

RefCounted<MemoryBuffer> AllocateStruct(size_t size) {
    return (RefCounted<MemoryBuffer>) SystemMemoryBuffer::Create(size, nullptr);
}

RefCounted<PixelData> AllocatePixelBuffer(PixelDataFormat pixelDataFormat, const Image& image) {
    auto pixelData = Memory::Make<PixelData>(pixelDataFormat, PixelDataType::UBYTE, image.GetBufferRef());
    return RefCounted<PixelData>(pixelData);
}

TEST_F(RHIFixture, TestTextures) {
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
    Array<Image> skyBoxFaces;

    GetCubeVertices(vertices, verticesCount);
    GetCubeIndices(indices, indicesCount);

    auto commands = device.CreateCmdList();

    RHI::VertexDeclaration::Desc vertexDeclarationDesc{};
    vertexDeclarationDesc.Resize(1);
    vertexDeclarationDesc[0].type = RHI::VertexElementType::Float3;
    vertexDeclarationDesc[0].frequency = RHI::VertexFrequency::PerVertex;
    vertexDeclarationDesc[0].buffer = 0;
    vertexDeclarationDesc[0].offset = offsetof(Vertex, pos);
    vertexDeclarationDesc[0].stride = sizeof(Vertex);
    auto vertexDeclaration = device.CreateVertexDeclaration(vertexDeclarationDesc);

    RHI::VertexBuffer::Desc vertexBufferDesc{};
    vertexBufferDesc.size = verticesCount * sizeof(Vertex);
    vertexBufferDesc.bufferUsage = RHI::BufferUsage::Static;
    auto vertexBuffer = device.CreateVertexBuffer(vertexBufferDesc);
    auto vertexBufferData = (RefCounted<ReadOnlyMemoryBuffer>) MemoryBufferGeneric<>::Create(verticesCount * sizeof(Vertex), vertices);;

    commands->UpdateVertexBuffer(vertexBuffer, 0,verticesCount * sizeof(Vertex), vertexBufferData);

    RHI::IndexBuffer::Desc indexBufferDesc{};
    indexBufferDesc.size = indicesCount * sizeof(uint32);
    indexBufferDesc.bufferUsage = RHI::BufferUsage::Static;
    auto indexBuffer = device.CreateIndexBuffer(indexBufferDesc);
    auto indexBufferData = (RefCounted<ReadOnlyMemoryBuffer>) SystemMemoryBuffer::Create(indicesCount * sizeof(uint32), indices);

    commands->UpdateIndexBuffer(indexBuffer, 0,indicesCount * sizeof(uint32), indexBufferData);

    RHI::UniformBuffer::Desc uniformBufferDesc{};
    uniformBufferDesc.size = sizeof(Transform);
    uniformBufferDesc.bufferUsage = RHI::BufferUsage::Dynamic;
    auto uniformBuffer = device.CreateUniformBuffer(uniformBufferDesc);
    auto transformBuffer = AllocateStruct(sizeof(Transform));

    RHI::Sampler::Desc samplerDesc;
    samplerDesc.minFilter = RHI::SamplerMinFilter::LinearMipmapLinear;
    samplerDesc.magFilter = RHI::SamplerMagFilter::Linear;
    samplerDesc.u = RHI::SamplerRepeatMode::ClampToEdge;
    samplerDesc.v = RHI::SamplerRepeatMode::ClampToEdge;
    samplerDesc.w = RHI::SamplerRepeatMode::ClampToEdge;
    auto sampler = device.CreateSampler(samplerDesc);

    LoadSkyBox(skyBoxFaces);

    RHI::Texture::Desc textureDesc;
    textureDesc.width = skyBoxFaces[0].GetWidth();
    textureDesc.height = skyBoxFaces[0].GetHeight();
    textureDesc.depth = 1;
    textureDesc.arraySlices = 0;
    textureDesc.mipsCount = PixelUtil::GetMaxMipsCount(textureDesc.width, textureDesc.height, textureDesc.depth);
    textureDesc.textureType = RHI::TextureType::TextureCube;
    textureDesc.textureFormat = RHI::TextureFormat::RGB8;
    textureDesc.textureUsage = { RHI::TextureUsage::Sampling };
    auto texture = device.CreateTexture(textureDesc);

    for (uint32 i = 0; i < skyBoxFaces.GetSize(); i++) {
        auto face = (RHI::TextureCubemapFace) i;
        auto& image = skyBoxFaces[i];
        auto region = Math::Rect2u{0,0,image.GetWidth(), image.GetHeight()};
        commands->UpdateTextureCube(texture, face, 0, region, AllocatePixelBuffer(Berserk::PixelDataFormat::RGB, image));
    }

    commands->GenerateMipMaps(texture);

    uint32 vertexShaderLength, fragmentShaderLength;
    const char *vertexShaderCode, *fragmentShaderCode;

    GetMainPassShaderVsGLSL410(vertexShaderCode, vertexShaderLength);
    GetMainPassShaderFsGLSL410(fragmentShaderCode, fragmentShaderLength);

    RHI::Program::Desc programDesc;
    programDesc.name = "Test Shader";
    programDesc.language = RHI::ShaderLanguage::GLSL410GL;
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

    while (!finish) {
        FixedUpdate();

        static float angle = 0.0f;
        float speed = 0.005f;

        angle += speed;

        Math::Vec3f eye(0, 0, 3);
        Math::Vec3f dir(Math::Utils::Sin(angle), Math::Utils::Sin(angle), Math::Utils::Cos(angle));
        Math::Vec3f up(0, 1, 0);
        float fov = Math::Utils::DegToRad(60.0f);
        float near = 0.1f, far = 10.0f;

        Math::Mat4x4f model = Math::Utils3d::IdentityMatrix();
        Math::Mat4x4f view = Math::Utils3d::LookAt(eye, dir, up).Submatrix<3, 3>(0, 0);
        Math::Mat4x4f proj = Math::Utils3d::Perspective(fov, window->GetAspectRatio(), near, far);
        Math::Mat4x4f projViewModel = proj * view * model;

        Transform t = { projViewModel.Transpose() };
        Memory::Copy(transformBuffer->GetData(), &t, sizeof(Transform));

        auto meta = program->GetProgramMeta();
        auto size = window->GetFramebufferSize();

        RHI::RenderPass renderPass{};
        renderPass.viewport.left = 0;
        renderPass.viewport.bottom = 0;
        renderPass.viewport.width = size.x();
        renderPass.viewport.height = size.y();
        renderPass.depthStencilAttachment.depthClear = 1.0f;
        renderPass.depthStencilAttachment.depthOption = RHI::RenderTargetOption::ClearStore;
        renderPass.depthStencilAttachment.stencilClear = 0;
        renderPass.depthStencilAttachment.stencilOption = RHI::RenderTargetOption::DiscardDiscard;
        renderPass.colorAttachments.Resize(1);
        renderPass.colorAttachments[0].clearColor = Color(0.2,0.15,0.3,1);
        renderPass.colorAttachments[0].option = RHI::RenderTargetOption::ClearStore;
        renderPass.presentation = true;

        RHI::PipelineState pipelineState{};
        pipelineState.primitivesType = RHI::PrimitivesType::Triangles;
        pipelineState.program = program;
        pipelineState.rasterState.frontFace = RHI::PolygonFrontFace::CounterClockwise;
        pipelineState.rasterState.cullMode = RHI::PolygonCullMode::Front;
        pipelineState.declaration = vertexDeclaration;
        pipelineState.depthStencilState.depthEnable = true;
        pipelineState.depthStencilState.depthWrite = false;
        pipelineState.depthStencilState.depthCompare = RHI::CompareFunction::LessEqual;
        pipelineState.blendState.attachments.Resize(1);

        commands->BeginScene();
        commands->UpdateUniformBuffer(uniformBuffer, 0, sizeof(Transform), (RefCounted<ReadOnlyMemoryBuffer>) transformBuffer);
        commands->BeginRenderPass(renderPass, window);
        commands->BindPipelineState(pipelineState);
        commands->BindUniformBuffer(uniformBuffer, meta->paramBlocks["Transform"].slot, 0, sizeof(Transform));
        commands->BindTexture(texture, meta->samplers["texBackground"].location);
        commands->BindSampler(sampler, meta->samplers["texBackground"].location);
        commands->BindVertexBuffers({vertexBuffer});
        commands->BindIndexBuffer({indexBuffer}, RHI::IndexType::Uint32);
        commands->DrawIndexed(indicesCount, 0, 1);
        commands->EndRenderPass();
        commands->EndScene();
        commands->Flush();
    }
}

BERSERK_GTEST_MAIN