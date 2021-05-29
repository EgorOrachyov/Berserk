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

struct VertexPCT {
    Math::Vec3f pos;
    Math::Vec3f color;
    Math::Vec2f texCoords;
};

struct VertexPT {
    Math::Vec2f pos;
    Math::Vec2f texCoords;
};

void GetQuadVertices(const VertexPCT* &vertices, size_t &count) {
    static VertexPCT vt[] = {
            { Math::Vec3f(-1, 1, 0),  Math::Vec3f(1, 0, 0), Math::Vec2f(0, 1) },
            { Math::Vec3f(-1, -1, 0), Math::Vec3f(0, 1, 0), Math::Vec2f(0, 0) },
            { Math::Vec3f(1, -1, 0),  Math::Vec3f(0, 0, 1), Math::Vec2f(1, 0) },
            { Math::Vec3f(1, 1, 0),   Math::Vec3f(1, 1, 1), Math::Vec2f(1, 1) }
    };

    vertices = vt;
    count = sizeof(vt) / sizeof(VertexPCT);
}

void GetScreenQuadVertices(const VertexPT* &vertices, size_t &count) {
    static VertexPT vt[] = {
            { Math::Vec2f(-1, 1),  Math::Vec2f(0, 1) },
            { Math::Vec2f(-1, -1), Math::Vec2f(0, 0) },
            { Math::Vec2f(1, -1),  Math::Vec2f(1, 0) },
            { Math::Vec2f(1, -1),  Math::Vec2f(1, 0) },
            { Math::Vec2f(1, 1),   Math::Vec2f(1, 1) },
            { Math::Vec2f(-1, 1),  Math::Vec2f(0, 1) },
    };

    vertices = vt;
    count = sizeof(vt) / sizeof(VertexPT);
}

void GetCubeIndices(const uint32* &indices, size_t &count) {
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

        layout (std140) uniform Params {
            mat4 MVP;
            vec2 tiling;
        };

        out vec3 fsColor;
        out vec2 fsTexCoords;

        void main() {
            fsColor = vsColor;
            fsTexCoords = tiling + vsTexCoords * tiling;
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
            vec3 color = fsColor * 0.2f;
            color += texture(texBackground, fsTexCoords).rgb;
            outColor = vec4(color, 0.5f);
        }
    )";

    code = sourceCode;
    length = StringUtils::Length(sourceCode);
}

void GetScreenPassShaderVsGLSL410(const char* &code, uint32 &length) {
    static const char sourceCode[] = R"(
        #version 410 core
        layout (location = 0) in vec2 vsPosition;
        layout (location = 1) in vec2 vsTexCoords;

        out vec2 fsTexCoords;

        void main() {
            fsTexCoords = vsTexCoords;
            gl_Position = vec4(vsPosition, 0.0f, 1.0f);
        }
    )";

    code = sourceCode;
    length = StringUtils::Length(sourceCode);
}

void GetScreenPassShaderFsGLSL410(const char* &code, uint32 &length) {
    static const char sourceCode[] = R"(
        #version 410 core
        layout (location = 0) out vec4 outColor;

        uniform sampler2D texColorBuffer;

        in vec2 fsTexCoords;

        const vec2 distortionFactor = vec2(4.0f);
        const float gamma = 2.2f;
        const float aspect = 1280.0f / 720.0f;
        const float aberrationFactor = 0.5f;

        vec3 correct(vec3 color) {
            return pow(color, vec3(1.0f / gamma));
        }

        void main() {
            vec2 uv = fsTexCoords;
            vec2 distFromCenter = uv - vec2(0.5f);
            distFromCenter.x *= aspect;

            vec2 aberration = aberrationFactor * pow(distFromCenter, vec2(3.0, 3.0));

            float r = texture(texColorBuffer, uv - aberration).r;
    	    float g = texture(texColorBuffer, uv).g;
    	    float b = texture(texColorBuffer, uv + aberration).b;

            outColor = vec4(correct(vec3(r, g, b)), 1.0f);
        }
    )";

    code = sourceCode;
    length = StringUtils::Length(sourceCode);
}

RefCounted<ReadOnlyMemoryBuffer> AllocateCode(const char* code, uint32 length) {
    return (RefCounted<ReadOnlyMemoryBuffer>) SystemMemoryBuffer::Create(length, code);
}

RefCounted<MemoryBuffer> AllocateStruct(size_t size) {
    return (RefCounted<MemoryBuffer>) SystemMemoryBuffer::Create(size, nullptr);
}

RefCounted<PixelData> AllocatePixelBuffer(const Image& image) {
    auto pixelData = Memory::Make<PixelData>(PixelDataFormat::RGBA, PixelDataType::UBYTE, image.GetBufferRef());
    return RefCounted<PixelData>(pixelData);
}

void printProgramInfo(const RefCounted<RHI::Program>& program) {
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
}

struct MainPass {
    RefCounted<RHI::VertexBuffer> vertexBuffer;
    RefCounted<RHI::IndexBuffer> indexBuffer;
    RefCounted<RHI::VertexDeclaration> vertexDeclaration;
    RefCounted<RHI::Sampler> sampler;
    RefCounted<RHI::Texture> texture;
    RefCounted<RHI::Program> program;
    RefCounted<RHI::UniformBuffer> uniformBuffer;
    RefCounted<RHI::Framebuffer> framebuffer;
    RefCounted<RHI::Texture> renderTarget;
};

struct ScreenPass {
    RefCounted<RHI::VertexBuffer> vertexBuffer;
    RefCounted<RHI::VertexDeclaration> vertexDeclaration;
    RefCounted<RHI::Sampler> sampler;
    RefCounted<RHI::Program> program;

};

TEST_F(RHIFixture, ScreenEffects) {
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

    MainPass mp;
    ScreenPass sp;

    auto& device = RHI::Driver::GetDevice();
    auto commands = device.CreateCmdList();

    const VertexPCT* vertices;
    const uint32* indices;
    size_t verticesCount;
    size_t indicesCount;

    {
        RHI::VertexDeclaration::Desc vertexDeclarationDesc{};
        vertexDeclarationDesc.Resize(3);
        vertexDeclarationDesc[0].type = RHI::VertexElementType::Float3;
        vertexDeclarationDesc[0].frequency = RHI::VertexFrequency::PerVertex;
        vertexDeclarationDesc[0].buffer = 0;
        vertexDeclarationDesc[0].offset = offsetof(VertexPCT, pos);
        vertexDeclarationDesc[0].stride = sizeof(VertexPCT);
        vertexDeclarationDesc[1].type = RHI::VertexElementType::Float3;
        vertexDeclarationDesc[1].frequency = RHI::VertexFrequency::PerVertex;
        vertexDeclarationDesc[1].buffer = 0;
        vertexDeclarationDesc[1].offset = offsetof(VertexPCT, color);
        vertexDeclarationDesc[1].stride = sizeof(VertexPCT);
        vertexDeclarationDesc[2].type = RHI::VertexElementType::Float2;
        vertexDeclarationDesc[2].frequency = RHI::VertexFrequency::PerVertex;
        vertexDeclarationDesc[2].buffer = 0;
        vertexDeclarationDesc[2].offset = offsetof(VertexPCT, texCoords);
        vertexDeclarationDesc[2].stride = sizeof(VertexPCT);
        mp.vertexDeclaration = device.CreateVertexDeclaration(vertexDeclarationDesc);

        GetQuadVertices(vertices, verticesCount);
        GetCubeIndices(indices, indicesCount);

        RHI::VertexBuffer::Desc vertexBufferDesc{};
        vertexBufferDesc.size = verticesCount * sizeof(VertexPCT);
        vertexBufferDesc.bufferUsage = RHI::BufferUsage::Static;
        mp.vertexBuffer = device.CreateVertexBuffer(vertexBufferDesc);
        auto vertexBufferData = (RefCounted<ReadOnlyMemoryBuffer>) MemoryBufferGeneric<>::Create(verticesCount * sizeof(VertexPCT), vertices);;

        commands->UpdateVertexBuffer(mp.vertexBuffer, 0, verticesCount * sizeof(VertexPCT), vertexBufferData);

        RHI::IndexBuffer::Desc indexBufferDesc{};
        indexBufferDesc.size = indicesCount * sizeof(uint32);
        indexBufferDesc.bufferUsage = RHI::BufferUsage::Static;
        mp.indexBuffer = device.CreateIndexBuffer(indexBufferDesc);
        auto indexBufferData = (RefCounted<ReadOnlyMemoryBuffer>) SystemMemoryBuffer::Create(indicesCount * sizeof(uint32), indices);

        commands->UpdateIndexBuffer(mp.indexBuffer, 0, indicesCount * sizeof(uint32), indexBufferData);

        RHI::Sampler::Desc samplerDesc;
        samplerDesc.minFilter = RHI::SamplerMinFilter::LinearMipmapLinear;
        samplerDesc.magFilter = RHI::SamplerMagFilter::Linear;
        samplerDesc.u = RHI::SamplerRepeatMode::Repeat;
        samplerDesc.v = RHI::SamplerRepeatMode::Repeat;
        samplerDesc.w = RHI::SamplerRepeatMode::Repeat;
        mp.sampler = device.CreateSampler(samplerDesc);

        auto image = Image::Load(BERSERK_TEXT("../../Engine/Resources/Textures/background-64x4.png"), Image::Channels::RGBA);
        assert(!image.IsEmpty());

        RHI::Texture::Desc textureDesc;
        textureDesc.width = image.GetWidth();
        textureDesc.height = image.GetHeight();
        textureDesc.depth = 1;
        textureDesc.mipsCount = PixelUtil::GetMaxMipsCount(textureDesc.width, textureDesc.height, textureDesc.depth);
        textureDesc.textureType = RHI::TextureType::Texture2d;
        textureDesc.textureFormat = RHI::TextureFormat::RGBA8;
        textureDesc.textureUsage = {RHI::TextureUsage::Sampling};
        mp.texture = device.CreateTexture(textureDesc);

        commands->UpdateTexture2D(mp.texture, 0, {0, 0, image.GetWidth(), image.GetHeight()}, AllocatePixelBuffer(image));
        commands->GenerateMipMaps(mp.texture);

        uint32 vertexShaderLength, fragmentShaderLength;
        const char *vertexShaderCode, *fragmentShaderCode;

        GetMainPassShaderVsGLSL410(vertexShaderCode, vertexShaderLength);
        GetMainPassShaderFsGLSL410(fragmentShaderCode, fragmentShaderLength);

        RHI::Program::Desc programDesc;
        programDesc.name = "Main Pass Shader";
        programDesc.language = RHI::ShaderLanguage::GLSL410GL;
        programDesc.stages.Resize(2);
        programDesc.stages[0].type = RHI::ShaderType::Vertex;
        programDesc.stages[0].sourceCode = AllocateCode(vertexShaderCode, vertexShaderLength);
        programDesc.stages[1].type = RHI::ShaderType::Fragment;
        programDesc.stages[1].sourceCode = AllocateCode(fragmentShaderCode, fragmentShaderLength);
        mp.program = device.CreateProgram(programDesc);

        RHI::Texture::Desc renderTargetDesc;
        renderTargetDesc.width = windowDesc.size.x();
        renderTargetDesc.height = windowDesc.size.y();
        renderTargetDesc.depth = 1;
        renderTargetDesc.mipsCount = 1;
        renderTargetDesc.textureType = RHI::TextureType::Texture2d;
        renderTargetDesc.textureFormat = RHI::TextureFormat::RGBA16F;
        renderTargetDesc.textureUsage = { RHI::TextureUsage::ColorAttachment, RHI::TextureUsage::Sampling };
        mp.renderTarget = device.CreateTexture(renderTargetDesc);

        RHI::Framebuffer::Desc framebufferDesc;
        framebufferDesc.width = windowDesc.size.x();
        framebufferDesc.height = windowDesc.size.y();
        framebufferDesc.colorTargets.Resize(1);
        framebufferDesc.colorTargets[0].mipLevel = 0;
        framebufferDesc.colorTargets[0].arraySlice = 0;
        framebufferDesc.colorTargets[0].face = 0;
        framebufferDesc.colorTargets[0].target = mp.renderTarget;
        mp.framebuffer = device.CreateFramebuffer(framebufferDesc);
    }

    const VertexPT* quadVertices;
    size_t quadVerticesCount;

    {
        RHI::VertexDeclaration::Desc vertexDeclarationDesc{};
        vertexDeclarationDesc.Resize(2);
        vertexDeclarationDesc[0].type = RHI::VertexElementType::Float2;
        vertexDeclarationDesc[0].frequency = RHI::VertexFrequency::PerVertex;
        vertexDeclarationDesc[0].buffer = 0;
        vertexDeclarationDesc[0].offset = offsetof(VertexPT, pos);
        vertexDeclarationDesc[0].stride = sizeof(VertexPT);
        vertexDeclarationDesc[1].type = RHI::VertexElementType::Float2;
        vertexDeclarationDesc[1].frequency = RHI::VertexFrequency::PerVertex;
        vertexDeclarationDesc[1].buffer = 0;
        vertexDeclarationDesc[1].offset = offsetof(VertexPT, texCoords);
        vertexDeclarationDesc[1].stride = sizeof(VertexPT);
        sp.vertexDeclaration = device.CreateVertexDeclaration(vertexDeclarationDesc);

        GetScreenQuadVertices(quadVertices, quadVerticesCount);

        RHI::VertexBuffer::Desc vertexBufferDesc{};
        vertexBufferDesc.size = quadVerticesCount * sizeof(VertexPT);
        vertexBufferDesc.bufferUsage = RHI::BufferUsage::Static;
        sp.vertexBuffer = device.CreateVertexBuffer(vertexBufferDesc);
        auto vertexBufferData = (RefCounted<ReadOnlyMemoryBuffer>) MemoryBufferGeneric<>::Create(quadVerticesCount * sizeof(VertexPT), quadVertices);;

        commands->UpdateVertexBuffer(sp.vertexBuffer, 0, quadVerticesCount * sizeof(VertexPT), vertexBufferData);

        RHI::Sampler::Desc samplerDesc;
        samplerDesc.minFilter = RHI::SamplerMinFilter::Linear;
        samplerDesc.magFilter = RHI::SamplerMagFilter::Linear;
        samplerDesc.u = RHI::SamplerRepeatMode::ClampToEdge;
        samplerDesc.v = RHI::SamplerRepeatMode::ClampToEdge;
        samplerDesc.w = RHI::SamplerRepeatMode::ClampToEdge;
        sp.sampler = device.CreateSampler(samplerDesc);

        uint32 vertexShaderLength, fragmentShaderLength;
        const char *vertexShaderCode, *fragmentShaderCode;

        GetScreenPassShaderVsGLSL410(vertexShaderCode, vertexShaderLength);
        GetScreenPassShaderFsGLSL410(fragmentShaderCode, fragmentShaderLength);

        RHI::Program::Desc programDesc;
        programDesc.name = "Screen Pass Shader";
        programDesc.language = RHI::ShaderLanguage::GLSL410GL;
        programDesc.stages.Resize(2);
        programDesc.stages[0].type = RHI::ShaderType::Vertex;
        programDesc.stages[0].sourceCode = AllocateCode(vertexShaderCode, vertexShaderLength);
        programDesc.stages[1].type = RHI::ShaderType::Fragment;
        programDesc.stages[1].sourceCode = AllocateCode(fragmentShaderCode, fragmentShaderLength);
        sp.program = device.CreateProgram(programDesc);
    }

    while (!finish) {
        FixedUpdate();

        // Check errors program
        if (mp.program->GetCompilationStatus() == RHI::Program::Status::FailedCompile) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Failed to compile shader: {0}"), mp.program->GetShaderName());
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("{0}"), mp.program->GetCompilerMessage());
            return;
        }

        // If Ok, continue execution
        if (mp.program->GetCompilationStatus() == RHI::Program::Status::Compiled) {
            printProgramInfo(mp.program);
            break;
        }
    }

    while (!finish) {
        FixedUpdate();

        // Check errors program
        if (sp.program->GetCompilationStatus() == RHI::Program::Status::FailedCompile) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Failed to compile shader: {0}"), sp.program->GetShaderName());
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("{0}"), sp.program->GetCompilerMessage());
            return;
        }

        // If Ok, continue execution
        if (sp.program->GetCompilationStatus() == RHI::Program::Status::Compiled) {
            printProgramInfo(mp.program);
            break;
        }
    }

    auto paramsSize = mp.program->GetProgramMeta()->paramBlocks["Params"].size;;
    auto paramsData = AllocateStruct(paramsSize);
    auto paramMVP = mp.program->GetProgramMeta()->params["MVP"];
    auto paramTiling = mp.program->GetProgramMeta()->params["tiling"];

    RHI::UniformBuffer::Desc uniformBufferDesc{};
    uniformBufferDesc.size = paramsSize;
    uniformBufferDesc.bufferUsage = RHI::BufferUsage::Dynamic;
    auto uniformBuffer = device.CreateUniformBuffer(uniformBufferDesc);

    while (!finish) {
        FixedUpdate();

        static float angle = 0.0f;
        float speed = 0.01f;

        angle += speed;

        Math::Vec3f eye(0, 0, 4 + Math::Utils::Sin(angle));
        Math::Vec3f dir(0, 0, -1);
        Math::Vec3f up(0, 1, 0);
        float fov = Math::Utils::DegToRad(60.0f);
        float near = 0.1f, far = 10.0f;

        Math::Mat4x4f model = Math::Utils3d::RotateY(angle);
        Math::Mat4x4f view = Math::Utils3d::LookAt(eye, dir, up);
        Math::Mat4x4f proj = Math::Utils3d::Perspective(fov, window->GetAspectRatio(), near, far);
        Math::Mat4x4f projViewModel = proj * view * model;

        Math::Mat4x4f t = projViewModel.Transpose();
        Math::Vec2f tiling{2.0f + 0.5f * Math::Utils::Sin(angle), 2.0f + 0.5f * Math::Utils::Cos(angle),};
        Memory::Copy((uint8*) paramsData->GetData() + paramMVP.blockOffset, &t, sizeof(t));
        Memory::Copy((uint8*)paramsData->GetData() + paramTiling.blockOffset, &tiling, sizeof(tiling));

        auto size = window->GetFramebufferSize();

        commands->BeginScene(window);
        commands->UpdateUniformBuffer(uniformBuffer, 0, paramsSize, (RefCounted<ReadOnlyMemoryBuffer>) paramsData);

        RHI::RenderPass mainPass{};
        mainPass.viewport.left = 0;
        mainPass.viewport.bottom = 0;
        mainPass.viewport.width = mp.framebuffer->GetWidth();
        mainPass.viewport.height = mp.framebuffer->GetHeight();
        mainPass.colorAttachments.Resize(1);
        mainPass.colorAttachments[0].clearColor = Color(0.2, 0.15, 0.3, 1);
        mainPass.colorAttachments[0].option = RHI::RenderTargetOption::ClearStore;

        commands->BeginRenderPass(mainPass, mp.framebuffer);
        {
            RHI::PipelineState pipelineState{};
            pipelineState.primitivesType = RHI::PrimitivesType::Triangles;
            pipelineState.program = mp.program;
            pipelineState.declaration = mp.vertexDeclaration;
            pipelineState.blendState = RHI::PipelineState::BlendState::CreateBlendState(1);

            auto meta = mp.program->GetProgramMeta();

            commands->BindPipelineState(pipelineState);
            commands->BindUniformBuffer(uniformBuffer, meta->paramBlocks["Transform"].slot, 0, paramsSize);
            commands->BindTexture(mp.texture, meta->samplers["texBackground"].location);
            commands->BindSampler(mp.sampler, meta->samplers["texBackground"].location);
            commands->BindVertexBuffers({mp.vertexBuffer});
            commands->BindIndexBuffer({mp.indexBuffer}, RHI::IndexType::Uint32);
            commands->DrawIndexed(indicesCount, 0, 1);
        }
        commands->EndRenderPass();

        RHI::RenderPass screenPass{};
        mainPass.viewport.left = 0;
        mainPass.viewport.bottom = 0;
        mainPass.viewport.width = size.x();
        mainPass.viewport.height = size.y();
        mainPass.colorAttachments.Resize(1);
        mainPass.colorAttachments[0].option = RHI::RenderTargetOption::DiscardStore;

        commands->BeginRenderPass(mainPass);
        {
            RHI::PipelineState pipelineState{};
            pipelineState.primitivesType = RHI::PrimitivesType::Triangles;
            pipelineState.program = sp.program;
            pipelineState.declaration = sp.vertexDeclaration;
            pipelineState.blendState.attachments.Resize(1);

            auto meta = sp.program->GetProgramMeta();

            commands->BindPipelineState(pipelineState);
            commands->BindTexture(mp.renderTarget, meta->samplers["texColorBuffer"].location);
            commands->BindSampler(sp.sampler, meta->samplers["texColorBuffer"].location);
            commands->BindVertexBuffers({sp.vertexBuffer});
            commands->Draw(quadVerticesCount, 0, 1);
        }
        commands->EndRenderPass();

        commands->EndScene();
        commands->Flush();
    }
}

BERSERK_GTEST_MAIN