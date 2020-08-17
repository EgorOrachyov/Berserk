/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Main.h>
#include <Engine.h>
#include <RHI/RHIDevice.h>
#include <Platform/Input.h>
#include <Platform/WindowManager.h>
#include <Platform/System.h>
#include <RenderTargets/WindowTarget.h>
#include <Paths.h>
#include <ResourceImporters.h>
#include <Math/Vec3f.h>
#include <Resources/Mesh.h>
#include <Resources/MeshImportOptions.h>
#include <Math/Mat4x4f.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestRenderMesh)
{
    BERSERK_TEST_COND(RenderMesh, true)
    {
        Main main;
        main.initialize(0, nullptr);
        {
            // References to common engine singletons
            auto& system = System::getSingleton();
            auto& input = Input::getSingleton();
            auto& device = RHIDevice::getSingleton();
            auto& windowManager = WindowManager::getSingleton();
            auto& importers = ResourceImporters::getSingleton();

            // Default window, created on engine start-up
            auto window = windowManager.find("MAIN_WINDOW");

            MeshImportOptions options;
            TPtrShared<Mesh> mesh;
            options.setFormat({EMeshAttribute::Position, EMeshAttribute::Normal});
            options.setTransform(Transformf().scale(Vec3f(0.9f)));
            auto path = Paths::getFullPathFor("Assets/Models/suzanne.obj", EPathType::Engine);
            auto importer = importers.findImporterFromPath(path);

            if (importer.isNotNull()) {
                TPtrShared<Resource> res;
                importer->import(res, path, options);
                mesh = res.castTo<Mesh>();
            }

            auto positionBuffer = device.createVertexBuffer(mesh->getVertexData().size(), EBufferUsage::Static, mesh->getVertexData().data());
            auto indexBuffer = device.createIndexBuffer(mesh->getIndexData().size(), EBufferUsage::Static, mesh->getIndexData().data());

            RHIVertexDeclarationDesc vertexDeclarationDesc;
            {
                vertexDeclarationDesc.resize(2);
                vertexDeclarationDesc[0].buffer = 0;
                vertexDeclarationDesc[0].iterating = EVertexIterating::PerVertex;
                vertexDeclarationDesc[0].location = 0;
                vertexDeclarationDesc[0].offset = 0;
                vertexDeclarationDesc[0].stride = sizeof(Vec3f) + sizeof(Vec3f);
                vertexDeclarationDesc[0].type = EVertexElementType::Float3;
                vertexDeclarationDesc[1].buffer = 0;
                vertexDeclarationDesc[1].iterating = EVertexIterating::PerVertex;
                vertexDeclarationDesc[1].location = 1;
                vertexDeclarationDesc[1].offset = sizeof(Vec3f);
                vertexDeclarationDesc[1].stride = sizeof(Vec3f) + sizeof(Vec3f);
                vertexDeclarationDesc[1].type = EVertexElementType::Float3;
            }

            auto declaration = device.createVertexDeclaration(vertexDeclarationDesc);
            auto array = device.createArrayObject({ positionBuffer }, indexBuffer, declaration, mesh->getPrimitivesType());

            // Vertex shader code in GLSL
            char vertexShader[] =
                    "#version 410 core\n"
                    "layout (location = 0) in vec3 position;"
                    "layout (location = 1) in vec3 normal;"
                    "out vec3 fsColor;"
                    "layout (std140) uniform Transform {"
                    "  mat4 proj;"
                    "  mat4 view;"
                    "  mat4 model;"
                    "};"
                    "void main() {"
                    " fsColor = vec3(0.5f) + vec3(0.5f) * normal;"
                    " gl_Position = proj * view * model * vec4(position, 1.0f);"
                    "}";

            // Fragment shader code in GLSL
            char fragmentShader[] =
                    "#version 410 core\n"
                    "layout (location = 0) out vec4 outColor;"
                    "in vec3 fsColor;"
                    "void main() {"
                    " outColor = vec4(fsColor, 1.0f);"
                    "}";

            RHIShaderDesc shaderDesc;
            {
                shaderDesc.resize(2);
                shaderDesc[0].type = EShaderType::Vertex;
                shaderDesc[0].code.add((uint8*) vertexShader, CStringUtility::length(vertexShader));
                shaderDesc[1].type = EShaderType::Fragment;
                shaderDesc[1].code.add((uint8*) fragmentShader, CStringUtility::length(fragmentShader));
            }

            auto shader = device.createShader(EShaderLanguage::GLSL, shaderDesc);
            auto meta = device.createShaderMeta(shader);

            RHIGraphicsPipelineState pipelineState;
            {
                pipelineState.polygonMode = EPolygonMode::Fill;
                pipelineState.polygonFrontFace = EPolygonFrontFace::CounterClockwise;
                pipelineState.polygonCullMode = EPolygonCullMode::Back;
                pipelineState.depthTest = true;
                pipelineState.depthWrite = true;
                pipelineState.depthCompare = ECompareFunction::Less;
                pipelineState.shader = shader;
                pipelineState.vertexDeclaration = declaration;
            }

            auto pTransform = meta->getUniformBlock("Transform");
            auto pProj = pTransform->getMember("proj");
            auto pView = pTransform->getMember("view");
            auto pModel = pTransform->getMember("model");

            auto uniformBuffer = device.createUniformBuffer(pTransform->getSize(), EBufferUsage::Dynamic, nullptr);

            RHIUniformBlockDesc blockDesc;
            {
                blockDesc.binding = pTransform->getBinding();
                blockDesc.buffer = uniformBuffer;
                blockDesc.offset = 0;
                blockDesc.range = pTransform->getSize();
            }
            auto uniformSet = device.createUniformSet({}, {blockDesc});

            // Create draw list (list of draw commands) and fill it for LATER execution
            auto drawList = device.createDrawList();
            auto writeDrawList = [&](){
                RHIWindowPassOptions windowPass;
                {
                    windowPass.clearColor = Color4f{ 0.0f, 0.0f, 0.0f, 0.0f };
                    windowPass.viewport = Region2i{ 0, 0, window->getSize() };
                    windowPass.clearMask = { EClearOption::Color, EClearOption::Depth };
                }

                drawList->begin();
                drawList->bindWindow(window, windowPass);
                drawList->bindPipeline(pipelineState);
                drawList->bindUniformSet(uniformSet);
                drawList->bindArrayObject(array);
                drawList->drawIndexed(mesh->getIndexType(), mesh->getIndicesCount());
                drawList->end();
            };

            while (!window->shouldClose()) {
                main.execSingleIteration();

                writeDrawList();

                static float tX = 0;
                static float tY = 0;
                static float tZ = 0;
                static float dtX = 10.0f;
                static float dtY = 10.0f;
                static float dtZ = 10.0f;

                auto proj = Mat4x4f::perspective(Math::degToRad(90.f), window->getAspectRatio(), 0.01, 100.0f);
                auto view = Mat4x4f::lookAt({0,0,0}, {0,0,1}, {0,1,0});
                auto model = Mat4x4f::translate({tX,tY,tZ});

                auto tproj = proj.transpose();
                auto tview = view.transpose();
                auto tmodel = model.transpose();

                uniformBuffer->update(pProj->getSize(), pProj->getOffset(), &tproj);
                uniformBuffer->update(pView->getSize(), pView->getOffset(), &tview);
                uniformBuffer->update(pModel->getSize(), pModel->getOffset(), &tmodel);

                device.beginRenderFrame();
                device.submitDrawList(drawList);
                device.endRenderFrame();

                if (input.isKeyPressed(EKeyboardKey::Right))
                    tX -= dtX;
                if (input.isKeyPressed(EKeyboardKey::Left))
                    tX += dtX;
                if (input.isKeyPressed(EKeyboardKey::Up))
                    tZ += dtX;
                if (input.isKeyPressed(EKeyboardKey::Down))
                    tZ -= dtX;
            }
        }
        main.finalize();
    };
}