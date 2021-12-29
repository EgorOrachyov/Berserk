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

#ifndef BERSERK_TESTMATERIALSHADER_HPP
#define BERSERK_TESTMATERIALSHADER_HPP

#include <Berserk.hpp>

class TestMaterialShader final : public berserk::Application {
public:
    struct Vertex {
        berserk::Vec3f pos;
        berserk::Vec3f norm;
        berserk::Vec3f color;
        berserk::Vec2f texCoords;
    };

    berserk::Engine *engine = nullptr;
    berserk::EventDispatcher *dispatcher = nullptr;
    berserk::RHIDevice *device = nullptr;
    berserk::FileSystem *fileSystem = nullptr;

    berserk::Ref<berserk::Window> window;

    std::vector<berserk::Ref<berserk::RHIVertexBuffer>> vertexBuffers;
    berserk::Ref<berserk::RHIIndexBuffer> indexBuffer;
    berserk::Ref<berserk::RHISampler> sampler;
    berserk::Ref<berserk::RHITexture> texture;
    berserk::Ref<berserk::RHIRenderPass> renderPass;
    berserk::Ref<berserk::RHIGraphicsPipeline> pipeline;
    berserk::Ref<berserk::RHICommandList> commandList;

    berserk::Ref<const berserk::Shader> shader;
    berserk::Ref<berserk::Material> material;

    float speed{};
    float angle{};
    float gamma{};

public:
    TestMaterialShader() = default;
    ~TestMaterialShader() override = default;

    void OnWindowCreate() override {
        BRK_INFO("Initialize game window");
        engine = &berserk::Engine::Instance();
        dispatcher = &engine->GetEventDispatcher();
        dispatcher->Subscribe(berserk::EventWindow::GetEventTypeStatic(), [](const berserk::Event &_event) {
            auto event = dynamic_cast<const berserk::EventWindow *>(&_event);
            if (event->GetType() == berserk::EventWindow::Type::CloseRequested) {
                BRK_INFO("Request engine close");
                berserk::Engine::Instance().RequestClose();
            }
            return false;
        });
    }

    void OnInitialize() override {
        fileSystem = &engine->GetFileSystem();
        device = &engine->GetRHIDevice();
        window = engine->GetWindowManager().GetPrimaryWindow();
        angle = 0.0f;
        speed = 0.5f;
        gamma = device->GetDriverType() == berserk::RHIType::Vulkan ? 2.2f : 1.0f;
        commandList = device->GetCoreCommandList();

        InitTexture();
        InitMaterial();
        InitBuffers();
        InitRenderPass();
        InitPipeline();
    }

    void OnFinalize() override {
        window.Reset();
        vertexBuffers.clear();
        indexBuffer.Reset();
        sampler.Reset();
        texture.Reset();
        shader.Reset();
        material.Reset();
        renderPass.Reset();
        pipeline.Reset();
        commandList.Reset();
    }

    void OnPostUpdate() override {
        Update();
        Draw();
    }

    void Update() {
        using namespace berserk;

        static auto clip = device->GetClipMatrix();

        angle += speed * engine->GetDeltaTime();
        angle = angle > berserk::MathUtils::PIf * 2 ? angle - berserk::MathUtils::PIf * 2 : angle;

        auto color1 = 0.5f + 0.5f * berserk::MathUtils::Sin(2 * angle);
        auto color2 = 0.5f + 0.5f * berserk::MathUtils::Sin(3 * angle);
        auto color3 = 0.5f + 0.5f * berserk::MathUtils::Sin(5 * angle);

        Vec3f eye(0, 0, 3);
        Vec3f dir(0, 0, -1);
        Vec3f up(0, 1, 0);
        float fov = MathUtils::DegToRad(60.0f);
        float near = 0.1f, far = 10.0f;

        Mat4x4f model = MathUtils3d::RotateY(angle);
        Mat4x4f view = MathUtils3d::LookAt(eye, dir, up);
        Mat4x4f proj = MathUtils3d::Perspective(fov, window->GetAspectRatio(), near, far);
        Mat4x4f projViewModel = proj * view * model;

        static StringName ptDiffuse("ptDiffuse");
        static StringName pMVP("pMVP");
        static StringName pModel("pModel");
        static StringName pLightColor("pLightColor");

        Vec3f lightColor(color1 * 0.8f + 2.0f, 1.0f - color2, color3);

        material->SetTexture(ptDiffuse, texture, sampler);
        material->SetMat4(pMVP, projViewModel);
        material->SetMat3(pModel, model.SubMatrix<3, 3>(0, 0));
        material->SetFloat3(pLightColor, lightColor);
        material->UpdatePack();
    }

    void Draw() const {
        using namespace berserk;

        RHIRenderPassBeginInfo beginInfo;
        beginInfo.viewport.left = 0;
        beginInfo.viewport.bottom = 0;
        beginInfo.viewport.width = window->GetSize().x();
        beginInfo.viewport.height = window->GetSize().y();
        beginInfo.clearColors.resize(1);
        beginInfo.depthClear = 1.0f;
        beginInfo.stencilClear = 0;
        beginInfo.clearColors[0] = Vec4f(0.198f, 0.092f, 0.121f, 1.0f).Pow(gamma);

        commandList->BeginRenderPass(renderPass, beginInfo);
        commandList->BindGraphicsPipeline(pipeline);
        commandList->BindResourceSet(material->GetPackedParams()->GetResourceSets()[0], 0);
        commandList->BindVertexBuffers(vertexBuffers);
        commandList->BindIndexBuffer(indexBuffer, RHIIndexType::Uint32);
        commandList->DrawIndexed(6, 0, 0, 1);
        commandList->EndRenderPass();
        commandList->SwapBuffers(window);
        commandList->Submit();
    }

protected:
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

    void InitMaterial() {
        auto &shaderManager = engine->GetRenderEngine().GetShaderManager();
        auto options = berserk::Ref<berserk::ShaderCompileOptions>(new berserk::ShaderCompileOptions);

        shader = shaderManager.Load("shaders/forward-lit.shader.xml", options);
        material = berserk::Ref<berserk::Material>(new berserk::Material(shader));

        material->SetName(berserk::StringName("mat-tiled-polygon"));
        material->SetDescription(BRK_TEXT("Tiled polygon with diffuse lightning"));
    }

    void InitBuffers() {
        using namespace berserk;

        // clang-format off
        static Vertex vt[] = {
                { Vec3f(-1, 1, 0),  Vec3f(0, 0, 1), Vec3f(1,1,1), Vec2f(0, 1), },
                { Vec3f(-1, -1, 0), Vec3f(0, 0, 1), Vec3f(1,1,1), Vec2f(0, 0), },
                { Vec3f(1, -1, 0),  Vec3f(0, 0, 1), Vec3f(1,1,1), Vec2f(1, 0), },
                { Vec3f(1, 1, 0),   Vec3f(0, 0, 1), Vec3f(1,1,1), Vec2f(1, 1) }
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
        pipelineDesc.shader = material->GetTechnique()->GetPass(0)->GetShader();
        pipelineDesc.declaration = material->GetShader()->GetVertexDeclaration();
        pipelineDesc.depthStencilState = RHIDepthStencilState::CreateDepthState(false);
        pipelineDesc.blendState.attachments.resize(1);
        pipelineDesc.renderPass = renderPass;
        pipeline = device->CreateGraphicsPipeline(pipelineDesc);
    }
};

#endif//BERSERK_TESTMATERIALSHADER_HPP
