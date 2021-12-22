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

#ifndef BERSERK_TESTRHISIMPLEQUAD_HPP
#define BERSERK_TESTRHISIMPLEQUAD_HPP

#include <Berserk.hpp>

class TestRHISimpleQuad {
public:
    struct Vertex {
        berserk::Vec3f pos;
        berserk::Vec3f color;
        berserk::Vec2f texCoords;
    };

    struct Transform {
        berserk::Mat4x4f projViewModel;
    };

    berserk::Engine *engine = nullptr;
    berserk::RHIDevice *device = nullptr;

    berserk::Ref<berserk::Window> window;

    std::vector<berserk::Ref<berserk::RHIVertexBuffer>> vertexBuffers;
    berserk::Ref<berserk::RHIIndexBuffer> indexBuffer;
    berserk::Ref<berserk::RHIUniformBuffer> uniformBuffer;
    berserk::Ref<berserk::RHIVertexDeclaration> vertexDeclaration;
    berserk::Ref<berserk::RHISampler> sampler;
    berserk::Ref<berserk::RHITexture> texture;
    berserk::Ref<berserk::RHIResourceSet> resourceSet;
    berserk::Ref<berserk::RHIShader> shader;
    berserk::Ref<berserk::RHIRenderPass> renderPass;
    berserk::Ref<berserk::RHIGraphicsPipeline> pipeline;

    berserk::Ref<berserk::RHICommandList> commandList;
    berserk::Ref<berserk::Data> transform = berserk::Data::Make(sizeof(Transform));

    float speed{};
    float angle{};
    float gamma{};

public:
    TestRHISimpleQuad() {
        engine = &berserk::Engine::Instance();
        device = &engine->GetRHIDevice();
        window = engine->GetWindowManager().GetPrimaryWindow();
        angle = 0.0f;
        speed = 0.5f;
        gamma = device->GetDriverType() == berserk::RHIType::Vulkan ? 2.2f : 1.0f;
        commandList = device->GetCoreCommandList();

        InitShader();
        InitTexture();
        InitVertexDeclaration();
        InitBuffers();
        InitResourceSet();
        InitRenderPass();
        InitPipeline();
    }

    void Update() {
        using namespace berserk;

        static auto clip = device->GetClipMatrix();

        angle += speed * engine->GetDeltaTime();

        Vec3f eye(0, 0, 3);
        Vec3f dir(0, 0, -1);
        Vec3f up(0, 1, 0);
        float fov = MathUtils::DegToRad(60.0f);
        float near = 0.1f, far = 10.0f;

        Mat4x4f model = MathUtils3d::RotateY(angle);
        Mat4x4f view = MathUtils3d::LookAt(eye, dir, up);
        Mat4x4f proj = MathUtils3d::Perspective(fov, window->GetAspectRatio(), near, far);
        Mat4x4f projViewModel = proj * view * model;

        Transform t = {(clip * projViewModel).Transpose()};
        Memory::Copy(transform->GetDataWrite(), &t, sizeof(Transform));

        commandList->UpdateUniformBuffer(uniformBuffer, 0, sizeof(Transform), transform);
    }

    void Draw() {
        using namespace berserk;

        RHIRenderPassBeginInfo beginInfo;
        beginInfo.viewport.left = 0;
        beginInfo.viewport.bottom = 0;
        beginInfo.viewport.width = window->GetSize().x();
        beginInfo.viewport.height = window->GetSize().y();
        beginInfo.clearColors.resize(1);
        beginInfo.depthClear = 1.0f;
        beginInfo.stencilClear = 0;
        beginInfo.clearColors[0] = Vec4f(0.298f, 0.0f, 0.321f, 1.0f).Pow(gamma);

        commandList->BeginRenderPass(renderPass, beginInfo);
        commandList->BindGraphicsPipeline(pipeline);
        commandList->BindResourceSet(resourceSet, 0);
        commandList->BindVertexBuffers(vertexBuffers);
        commandList->BindIndexBuffer(indexBuffer, RHIIndexType::Uint32);
        commandList->DrawIndexed(6, 0, 0, 1);
        commandList->EndRenderPass();
        commandList->SwapBuffers(window);
    }

protected:
    void InitShader() {
        static const char vs[] = R"(
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

        static const char fs[] = R"(
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

        using namespace berserk;

        RHIShaderDesc shaderDesc;
        shaderDesc.name = StringName("test-shader");
        shaderDesc.language = RHIShaderLanguage::GLSL410GL;
        shaderDesc.stages.resize(2);
        shaderDesc.stages[0].type = RHIShaderType::Vertex;
        shaderDesc.stages[0].sourceCode = Data::Make(vs);
        shaderDesc.stages[1].type = RHIShaderType::Fragment;
        shaderDesc.stages[1].sourceCode = Data::Make(fs);
        shader = device->CreateShader(shaderDesc);

        assert(shader->GetCompilationStatus() == RHIShader::Status::Compiled);
    }

    void InitTexture() {
        using namespace berserk;

        String imagePath = BRK_TEXT("../engine/resources/textures/background-32x8.png");
        Image image = Image::LoadRgba(imagePath, 4);

        assert(!image.Empty());

        RHITextureDesc textureDesc;
        textureDesc.name = StringName(imagePath);
        textureDesc.width = image.GetWidth();
        textureDesc.height = image.GetHeight();
        textureDesc.depth = 1;
        textureDesc.arraySlices = 1;
        textureDesc.mipsCount = ImageUtil::GetMaxMipsCount(textureDesc.width, textureDesc.height, textureDesc.depth);
        textureDesc.textureType = RHITextureType::Texture2d;
        textureDesc.textureFormat = RHITextureFormat::RGBA8;
        textureDesc.textureUsage = {RHITextureUsage::Sampling};
        texture = device->CreateTexture(textureDesc);

        commandList->UpdateTexture2D(texture, 0, {0, 0, image.GetWidth(), image.GetHeight()}, image.GetPixelData());
        commandList->GenerateMipMaps(texture);

        RHISamplerDesc samplerDesc;
        samplerDesc.minFilter = RHISamplerMinFilter::LinearMipmapLinear;
        samplerDesc.magFilter = RHISamplerMagFilter::Linear;
        samplerDesc.maxAnisotropy = 16.0f;
        samplerDesc.useAnisotropy = true;
        samplerDesc.u = RHISamplerRepeatMode::ClampToEdge;
        samplerDesc.v = RHISamplerRepeatMode::ClampToEdge;
        samplerDesc.w = RHISamplerRepeatMode::ClampToEdge;
        sampler = device->CreateSampler(samplerDesc);
    }

    void InitVertexDeclaration() {
        using namespace berserk;

        RHIVertexDeclarationDesc vertexDeclarationDesc;
        vertexDeclarationDesc.resize(3);
        vertexDeclarationDesc[0].type = RHIVertexElementType::Float3;
        vertexDeclarationDesc[0].frequency = RHIVertexFrequency::PerVertex;
        vertexDeclarationDesc[0].buffer = 0;
        vertexDeclarationDesc[0].offset = offsetof(Vertex, pos);
        vertexDeclarationDesc[0].stride = sizeof(Vertex);
        vertexDeclarationDesc[1].type = RHIVertexElementType::Float3;
        vertexDeclarationDesc[1].frequency = RHIVertexFrequency::PerVertex;
        vertexDeclarationDesc[1].buffer = 0;
        vertexDeclarationDesc[1].offset = offsetof(Vertex, color);
        vertexDeclarationDesc[1].stride = sizeof(Vertex);
        vertexDeclarationDesc[2].type = RHIVertexElementType::Float2;
        vertexDeclarationDesc[2].frequency = RHIVertexFrequency::PerVertex;
        vertexDeclarationDesc[2].buffer = 0;
        vertexDeclarationDesc[2].offset = offsetof(Vertex, texCoords);
        vertexDeclarationDesc[2].stride = sizeof(Vertex);
        vertexDeclaration = device->CreateVertexDeclaration(vertexDeclarationDesc);
    }

    void InitBuffers() {
        using namespace berserk;

        // clang-format off
        static Vertex vt[] = {
            { Vec3f(-1, 1, 0),  Vec3f(1, 0, 0), Vec2f(0, 1), },
            { Vec3f(-1, -1, 0), Vec3f(0, 1, 0), Vec2f(0, 0), },
            { Vec3f(1, -1, 0),  Vec3f(0, 0, 1), Vec2f(1, 0), },
            { Vec3f(1, 1, 0),   Vec3f(1, 1, 1), Vec2f(1, 1) }
        };
        // clang-format on
        static uint32 id[] = {0, 1, 2, 2, 3, 0};

        RHIBufferDesc vertexBufferDesc{};
        vertexBufferDesc.size = sizeof(vt);
        vertexBufferDesc.bufferUsage = RHIBufferUsage::Static;
        vertexBuffers.resize(1);
        vertexBuffers[0] = device->CreateVertexBuffer(vertexBufferDesc);

        commandList->UpdateVertexBuffer(vertexBuffers[0], 0, sizeof(vt), Data::Make(vt, sizeof(vt)));

        RHIBufferDesc indexBufferDesc{};
        indexBufferDesc.size = sizeof(id);
        indexBufferDesc.bufferUsage = RHIBufferUsage::Static;
        indexBuffer = device->CreateIndexBuffer(indexBufferDesc);

        commandList->UpdateIndexBuffer(indexBuffer, 0, sizeof(id), Data::Make(id, sizeof(id)));

        RHIBufferDesc uniformBufferDesc{};
        uniformBufferDesc.size = sizeof(Transform);
        uniformBufferDesc.bufferUsage = RHIBufferUsage::Dynamic;
        uniformBuffer = device->CreateUniformBuffer(uniformBufferDesc);
    }

    void InitResourceSet() {
        using namespace berserk;

        auto meta = shader->GetShaderMeta();
        StringName pTexBackground("texBackground");
        StringName pTransform("Transform");

        RHIResourceSetDesc resourceSetDesc;
        resourceSetDesc.SetTexture(texture, meta->GetSamplerLocation(pTexBackground));
        resourceSetDesc.SetSampler(sampler, meta->GetSamplerLocation(pTexBackground));
        resourceSetDesc.SetBuffer(uniformBuffer, meta->GetParamBlockSlot(pTransform), 0, sizeof(Transform));
        resourceSet = device->CreateResourceSet(resourceSetDesc);
    }

    void InitRenderPass() {
        using namespace berserk;

        RHIRenderPassDesc renderPassDesc;
        renderPassDesc.name = StringName("draw-rect-pass");
        renderPassDesc.depthStencilAttachment.depthOption = RHIRenderTargetOption::ClearStore;
        renderPassDesc.depthStencilAttachment.stencilOption = RHIRenderTargetOption::DiscardDiscard;
        renderPassDesc.colorAttachments.resize(1);
        renderPassDesc.colorAttachments[0].option = RHIRenderTargetOption::ClearStore;
        renderPassDesc.window = window;
        renderPass = device->CreateRenderPass(renderPassDesc);
    }

    void InitPipeline() {
        using namespace berserk;

        RHIGraphicsPipelineDesc pipelineDesc{};
        pipelineDesc.primitivesType = RHIPrimitivesType::Triangles;
        pipelineDesc.shader = shader;
        pipelineDesc.declaration = vertexDeclaration;
        pipelineDesc.depthStencilState = RHIDepthStencilState::CreateDepthState(false);
        pipelineDesc.blendState.attachments.resize(1);
        pipelineDesc.renderPass = renderPass;
        pipeline = device->CreateGraphicsPipeline(pipelineDesc);
    }
};

#endif//BERSERK_TESTRHISIMPLEQUAD_HPP
