/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Engine.h>
#include <LogMacro.h>
#include <Platform/ISystem.h>
#include <Console/ConsoleManager.h>
#include <ImageImporter/ImageImporter.h>

#include <Math/Vec3f.h>
#include <Math/Mat4x4f.h>
#include <RHI/RHIDevice.h>

#include <Rendering/RenderModule.h>
#include <Rendering/Shader.h>
#include <Rendering/GeometryGenerator.h>


using namespace Berserk;

BERSERK_TEST_SECTION(Render)
{
    BERSERK_TEST_COND(RenderCommon, true)
            {
                auto& system = ISystem::getSingleton();

                Engine engine;
                ConsoleManager consoleManager;
                ImageImporter imageImporter;
                RenderModule renderModule;
                engine.initialize("../../../Engine/", false);

                char vertex[] =
                        "#version 410 core\n"
                        "layout (location = 0) in vec3 vPosition;"
                        "layout (location = 1) in vec3 vNormal;"
                        "layout (location = 2) in vec2 vTexCoords;"
                        ""
                        "out vec3 fNormal;"
                        ""
                        "layout (std140) uniform Transform {"
                        "   mat4 Proj;"
                        "   mat4 View;"
                        "   mat4 Model;"
                        "};"
                        "void main() {"
                        "   fNormal = vec3(Model * vec4(vNormal,0.0f));"
                        "   gl_Position = Proj * View * Model * vec4(vPosition, 1.0f);"
                        "}";
                char fragment[] =
                        "#version 410 core\n"
                        "layout (location = 0) out vec4 fColor;"
                        ""
                        "in vec3 fNormal;"
                        ""
                        "void main() {"
                        "   fColor = vec4(fNormal * vec3(0.5f) + vec3(0.5f),1.0f);"
                        "}";

                TArray<uint8> vs; vs.add((uint8*)vertex, sizeof(vertex));
                TArray<uint8> fs; fs.add((uint8*)fragment, sizeof(fragment));

                auto& device = RHIDevice::getSingleton();
                auto& vertexPolicyFactory = renderModule.getVertexPolicyFactory();

                {
                    Shader shader("Flat_Normal_Shader", EShaderLanguage::GLSL, vs, fs);

                    auto vertexDeclaration = vertexPolicyFactory.getDeclaration(EVertexPolicy::PositionNormalTexCoords);
                    auto vertexPolicy = vertexPolicyFactory.getPolicy(EVertexPolicy::PositionNormalTexCoords);
                    TArray<uint8> vertdata;
                    TArray<uint32> indicesdata;
                    uint32 vertsCount;
                    uint32 indicesCount;

                    GeometryGenerator::generateSphere(0.5f, 16, 16, *vertexPolicy, vertsCount, vertdata, indicesdata);
                    indicesCount = indicesdata.size();

                    auto uniformBuffer = device.createUniformBuffer(shader.findUniformBlock("Transform")->getSize(), EMemoryType::Dynamic);
                    auto vertexBuffer = device.createVertexBuffer(vertdata.size(), EMemoryType::Dynamic);
                    vertexBuffer->update(vertdata.size(), 0, vertdata.data());
                    auto indexBuffer = device.createIndexBuffer(indicesdata.size() * sizeof(uint32), EMemoryType::Dynamic);
                    indexBuffer->update(indicesdata.size() * sizeof(uint32), 0, (uint8*) indicesdata.data());
                    auto arrayObject = device.createArrayObject({vertexBuffer},indexBuffer,vertexDeclaration);

                    TArray<RHIUniformBlockDesc> uniformBlocks;
                    {
                        auto block = shader.findUniformBlock("Transform");
                        auto& blockDesc = uniformBlocks.emplace();
                        blockDesc.binding = block->getBinding();
                        blockDesc.buffer = uniformBuffer;
                        blockDesc.offset = 0;
                        blockDesc.range = block->getSize();
                    }
                    auto uniformSet = device.createUniformSet(TArray<RHIUniformTextureDesc>(), uniformBlocks);

                    RHIGraphicsPipelineDesc pipelineDesc;
                    pipelineDesc.shader = shader.getShaderHandle();
                    pipelineDesc.depthTest = true;
                    pipelineDesc.depthWrite = true;
                    pipelineDesc.depthCompare = ECompareFunction::Less;
                    pipelineDesc.primitivesType = EPrimitivesType::Triangles;
                    pipelineDesc.polygonMode = EPolygonMode::Fill;
                    pipelineDesc.polygonCullMode = EPolygonCullMode::Back;
                    pipelineDesc.polygonFrontFace = EPolygonFrontFace::CounterClockwise;
                    pipelineDesc.framebufferFormat.useDepthStencil = true;
                    pipelineDesc.framebufferFormat.colorFormats.resize(1);
                    pipelineDesc.framebufferFormat.colorFormats[0] = EPixelFormat::R8G8B8A8;
                    pipelineDesc.blendState.attachments.resize(1);
                    pipelineDesc.blendState.attachments[0].enable = false;

                    auto pipeline = device.createGraphicsPipeline(pipelineDesc);

                    auto drawList = device.createDrawList();
                    {
                        Region2i area = { 0, 0, 1280, 720 };

                        drawList->begin();
                        drawList->bindWindow(ISystem::MAIN_WINDOW, area, {0,0,0,0}, 1.0f, 0);
                        drawList->bindPipeline(pipeline);
                        drawList->bindUniformSet(uniformSet);
                        drawList->bindArrayObject(arrayObject);
                        drawList->drawIndexed(EIndexType::Uint32, indicesCount);
                        drawList->end();
                    }

                    while (!system.shouldClose(ISystem::MAIN_WINDOW)) {
                        engine.update();

                        static Vec3f pos = Vec3f(0,0,1);
                        static float angle = 0.0f;
                        static float step = 0.005f;

                        //auto p = Vec3f(Mat4x4f::rotateY(angle) * Vec4f(pos, 1.0f));
                        auto p = pos;

                        auto Model = Mat4x4f::rotateY(angle * 3);
                        auto View = Mat4x4f::lookAt(p, -p, Vec3f::Y_AXIS);
                        auto Proj = Mat4x4f::perspective(Math::degToRad(100.0f), 1280.0f / 720.0f, 0.01, 10.0f);

                        angle += step;

                        Proj = Proj.transpose();
                        View = View.transpose();
                        auto block = shader.findUniformBlock("Transform");
                        auto memProj = block->findMember("Proj");
                        auto memView = block->findMember("View");
                        auto memModel = block->findMember("Model");
                        uniformBuffer->update(memProj->getSize(), memProj->getOffset(), (uint8*)Proj.data());
                        uniformBuffer->update(memView->getSize(), memView->getOffset(), (uint8*)View.data());
                        uniformBuffer->update(memModel->getSize(), memModel->getOffset(), (uint8*)Model.data());

                        device.beginRenderFrame();
                        device.submitDrawList(drawList);
                        device.endRenderFrame();
                    }
                }

                engine.finalize();
            };
}
