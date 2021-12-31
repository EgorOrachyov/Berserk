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

#ifndef BERSERK_TESTMESHSIMPLE_HPP
#define BERSERK_TESTMESHSIMPLE_HPP

#include <Berserk.hpp>

class TestMeshSimple final : public berserk::Application {
public:
    berserk::Engine *engine = nullptr;
    berserk::EventDispatcher *dispatcher = nullptr;
    berserk::RHIDevice *device = nullptr;
    berserk::FileSystem *fileSystem = nullptr;
    berserk::ResourceManager *resourceManager = nullptr;

    berserk::Ref<berserk::Window> window;

    berserk::Ref<berserk::RHIRenderPass> renderPass;
    berserk::Ref<berserk::RHIGraphicsPipeline> pipeline;
    berserk::Ref<berserk::RHICommandList> commandList;

    berserk::Ref<const berserk::Shader> shader;
    berserk::Ref<berserk::Material> material;
    berserk::Ref<berserk::Mesh> mesh;
    berserk::Ref<berserk::ResTexture> texture;

    float speed{};
    float angle{};
    float gamma{};

public:
    TestMeshSimple() = default;
    ~TestMeshSimple() override = default;

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
        resourceManager = &engine->GetResourceManager();
        device = &engine->GetRHIDevice();
        window = engine->GetWindowManager().GetPrimaryWindow();
        angle = 0.0f;
        speed = 0.25f;
        gamma = device->GetDriverType() == berserk::RHIType::Vulkan ? 2.2f : 1.0f;
        commandList = device->GetCoreCommandList();

        InitTexture();
        InitMaterial();
        InitMesh();
        InitRenderPass();
        InitPipeline();
    }

    void OnFinalize() override {
        window.Reset();
        texture.Reset();
        shader.Reset();
        material.Reset();
        mesh.Reset();
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

        Vec3f eye(0, 0, 6);
        Vec3f dir(0, 0, -1);
        Vec3f up(0, 1, 0);
        float fov = MathUtils::DegToRad(60.0f);
        float near = 0.1f, far = 10.0f;

        Mat4x4f model = Quatf(0.2f * angle, 0.5f * angle, 0.7f * angle).AsMatrix();
        Mat4x4f view = MathUtils3d::LookAt(eye, dir, up);
        Mat4x4f proj = MathUtils3d::Perspective(fov, window->GetAspectRatio(), near, far);
        Mat4x4f projViewModel = proj * view * model;

        Mat4x4f lightRotation = MathUtils3d::RotateY(-2.0f * angle);

        Vec3f lightPos = MathUtils3d::Multiply(lightRotation, Vec3f(0, 0, 5));

        static StringName ptDiffuse("ptDiffuse");
        static StringName pMVP("pMVP");
        static StringName pModel("pModel");
        static StringName pLightColor("pLightColor");
        static StringName pLightPos("pLightPos");
        static StringName pTime("pTime");

        Vec3f lightColor(1, 1, 1);

        material->SetTexture(ptDiffuse, texture->GetRHITexture(), texture->GetRHISampler());
        material->SetMat4(pMVP, projViewModel);
        material->SetMat3(pModel, model.SubMatrix<3, 3>(0, 0));
        material->SetFloat3(pLightColor, lightColor);
        material->SetFloat3(pLightPos, lightPos);
        material->SetFloat1(pTime, engine->GetTime());
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

        std::vector<Ref<RHIVertexBuffer>> vertexBuffers;
        if (mesh->HasVertexData()) vertexBuffers.push_back(mesh->GetVertexData());
        if (mesh->HasAttributeData()) vertexBuffers.push_back(mesh->GetAttributeData());
        if (mesh->HasSkinningData()) vertexBuffers.push_back(mesh->GetSkinningData());

        commandList->BeginRenderPass(renderPass, beginInfo);
        commandList->BindGraphicsPipeline(pipeline);
        commandList->BindResourceSet(material->GetPackedParams()->GetResourceSets()[0], 0);
        commandList->BindVertexBuffers(vertexBuffers);

        for (auto &subMesh : mesh->GetSubMeshes()) {
            commandList->BindIndexBuffer(subMesh->GetIndexBuffer(), subMesh->GetIndexType());
            commandList->DrawIndexed(subMesh->GetIndicesCount(), subMesh->GetBaseVertex(), 1);
        }

        commandList->EndRenderPass();
        commandList->SwapBuffers(window);
        commandList->Submit();
    }

protected:
    void InitTexture() {
        using namespace berserk;

        String texturePath = BRK_TEXT("resources/textures/background-32x8.png");
        Ref<ResTextureImportOptions> importOptions(new ResTextureImportOptions);
        importOptions->width = 64;
        importOptions->height = 64;
        importOptions->cacheCPU = false;
        importOptions->mipmaps = true;
        importOptions->channels = 4;

        texture = resourceManager->Import(texturePath, importOptions.As<ResourceImportOptions>()).Cast<ResTexture>();
    }

    void InitMaterial() {
        auto &shaderManager = engine->GetRenderEngine().GetShaderManager();
        auto options = berserk::Ref<berserk::ShaderCompileOptions>(new berserk::ShaderCompileOptions);

#ifdef BERSERK_DEBUG
        options->Set(berserk::StringName("D_DEBUG_COLOR"));
#endif
        options->Set(berserk::StringName("D_ANIMATE"));

        shader = shaderManager.Load("shaders/forward-lit.shader.xml", options);
        material = berserk::Ref<berserk::Material>(new berserk::Material(shader));

        material->SetName(berserk::StringName("mat-tiled-diffuse"));
        material->SetDescription(BRK_TEXT("Tiled polygon with diffuse lightning"));
    }

    void InitMesh() {
        using namespace berserk;

        //     v4---v7
        //    /|   / |
        //   / v5-/-v6
        //  v0---v3 /
        //  | /  | /
        //  v1---v2

        float positions[] = {
                -1, 1, 1, -1, -1, 1, 1, -1, 1, 1, 1, 1,
                1, 1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1,
                1, 1, -1, 1, -1, -1, -1, -1, -1, -1, 1, -1,
                -1, 1, -1, -1, -1, -1, -1, -1, 1, -1, 1, 1,
                -1, 1, -1, -1, 1, 1, 1, 1, 1, 1, 1, -1,
                1, -1, -1, 1, -1, 1, -1, -1, 1, -1, -1, -1};
        float normals[] = {
                0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
                1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
                0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1,
                -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0,
                0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
                0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0};
        float colors[] = {
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        float uvs[] = {
                0, 1, 0, 0, 1, 0, 1, 1,
                0, 1, 0, 0, 1, 0, 1, 1,
                0, 1, 0, 0, 1, 0, 1, 1,
                0, 1, 0, 0, 1, 0, 1, 1,
                0, 1, 0, 0, 1, 0, 1, 1,
                0, 1, 0, 0, 1, 0, 1, 1};
        uint32 indices[] = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4,
                8, 9, 10, 10, 11, 8,
                12, 13, 14, 14, 15, 12,
                16, 17, 18, 18, 19, 16,
                20, 21, 22, 22, 23, 20};

        MeshFormat format = {MeshAttribute::Position, MeshAttribute::Normal, MeshAttribute::Color, MeshAttribute::UV};
        MeshArrays arrays;
        arrays.positions = positions;
        arrays.normals = normals;
        arrays.colors = colors;
        arrays.uvs = uvs;

        mesh = Ref<Mesh>(new Mesh(format, 24, arrays));
        mesh->SetAabb(Aabbf(Vec3f(0, 0, 0), 1));
        mesh->AddSubMesh(StringName("box"), berserk::RHIPrimitivesType::Triangles, Aabbf(Vec3f(0, 0, 0), 1), 0, berserk::RHIIndexType::Uint32, 6 * 6, Data::Make(indices, sizeof(indices)));
        mesh->AddMaterial(material);
        mesh->SetSubMeshMaterial(StringName("box"), 0);
    }

    void InitRenderPass() {
        using namespace berserk;

        RHIRenderPassDesc renderPassDesc;
        renderPassDesc.name = StringName("draw-mesh-pass");
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
        pipelineDesc.rasterState.cullMode = berserk::RHIPolygonCullMode::Back;
        pipelineDesc.rasterState.frontFace = berserk::RHIPolygonFrontFace::CounterClockwise;
        pipelineDesc.rasterState.mode = berserk::RHIPolygonMode::Fill;
        pipelineDesc.primitivesType = RHIPrimitivesType::Triangles;
        pipelineDesc.shader = material->GetTechnique()->GetPass(0)->GetShader();
        pipelineDesc.declaration = mesh->GetDeclaration();
        pipelineDesc.depthStencilState = RHIDepthStencilState::CreateDepthState(true);
        pipelineDesc.blendState.attachments.resize(1);
        pipelineDesc.renderPass = renderPass;
        pipeline = device->CreateGraphicsPipeline(pipelineDesc);
    }
};

#endif//BERSERK_TESTMESHSIMPLE_HPP
