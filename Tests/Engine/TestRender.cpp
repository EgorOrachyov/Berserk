/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <IO/Ini.h>
#include <Engine.h>
#include <LogMacro.h>
#include <Math/Random.h>
#include <Platform/IInput.h>
#include <Platform/ISystem.h>
#include <Console/ConsoleManager.h>
#include <ImageImporter/ImageImporter.h>

#include <Math/Vec3f.h>
#include <Math/Mat4x4f.h>
#include <RHI/RHIDevice.h>

#include <Rendering/RenderModule.h>
#include <Rendering/Resources/Shader.h>
#include <Rendering/Resources/UniformBuffer.h>
#include <Rendering/Resources/IndexBufferUint32Static.h>
#include <Rendering/Aux/GeometryGenerator.h>
#include <Rendering/Shaders/ShaderProfile.h>
#include <Rendering/RenderCanvas.h>

using namespace Berserk;
using namespace Rendering;

BERSERK_TEST_SECTION(Render)
{
    BERSERK_TEST_COND(RenderCommon, true)
    {
        auto& system = ISystem::getSingleton();

        Random random;
        Engine engine;
        ConsoleManager consoleManager;
        ImageImporter imageImporter;
        RenderModule renderModule;
        engine.initialize("../../../Engine/", false);

        auto& input = IInput::getSingleton();
        auto& device = RHIDevice::getSingleton();
        auto& vertexPolicyFactory = renderModule.getVertexPolicyFactory();
        auto& shaderCache = renderModule.getShaderCache();
        auto& shaderManager = renderModule.getShaderManager();
        auto& config = renderModule.getConfig();
        auto& target = renderModule.getScreenTarget(ISystem::MAIN_WINDOW);

        {
            RenderCanvas renderCanvas((TPtrShared<IRenderTarget>) target);

            for (int i = 0; i < 100; i++) {
                float r = random.from(0.2f, 0.7f);
                Color4f c(random.from(0.1f, 1.0f), random.from(0.1f, 1.0f), random.from(0.1f, 1.0f));
                Vec3f pos(random.from(-8.0f, 8.0f), random.from(-1.0f, 1.0f), random.from(-8.0f, 0.0f));

                renderCanvas.drawSphere(pos, r, c);
            }

            auto shader = shaderManager.loadShader("FlatShader");
            shaderCache.cacheShader(*shader);
            shaderCache.setUpdateOnClose(true);

            auto vertexDeclaration = vertexPolicyFactory.getDeclaration(EVertexPolicy::PositionNormalTexCoords);
            auto vertexPolicy = vertexPolicyFactory.getPolicy(EVertexPolicy::PositionNormalTexCoords);
            TArray<uint8> vertdata;
            TArray<uint32> indicesdata;
            uint32 vertsCount;
            uint32 indicesCount;

            GeometryGenerator::generateSphere(0.5f, 16, 16, *vertexPolicy, vertsCount, vertdata, indicesdata);
            IndexBufferUint32Static indexBuffer(indicesdata);
            UniformBuffer uniformBuffer("TransformUbo", *shader->findUniformBlock("Transform"));

            indicesCount = indicesdata.size();

            auto vertexBuffer = device.createVertexBuffer(vertdata.size(), EMemoryType::Dynamic, vertdata.data());
            auto arrayObject = device.createArrayObject({vertexBuffer},indexBuffer.getIndexBufferRHI(),vertexDeclaration);

            TArray<RHIUniformBlockDesc> uniformBlocks;
            {
                auto block = shader->findUniformBlock("Transform");
                auto& blockDesc = uniformBlocks.emplace();
                blockDesc.binding = block->getBinding();
                blockDesc.buffer = uniformBuffer.getUniformBufferRHI();
                blockDesc.offset = 0;
                blockDesc.range = block->getSize();
            }
            auto uniformSet = device.createUniformSet(TArray<RHIUniformTextureDesc>(), uniformBlocks);

            RHIGraphicsPipelineDesc pipelineDesc;
            pipelineDesc.shader = shader->getShaderHandle();
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

            RHIGraphicsPipelineDesc pipelineDescWireframe;
            pipelineDesc.shader = shader->getShaderHandle();
            pipelineDesc.depthTest = true;
            pipelineDesc.depthWrite = true;
            pipelineDesc.depthCompare = ECompareFunction::Less;
            pipelineDesc.primitivesType = EPrimitivesType::Triangles;
            pipelineDesc.polygonMode = EPolygonMode::Line;
            pipelineDesc.polygonCullMode = EPolygonCullMode::Disabled;
            pipelineDesc.polygonFrontFace = EPolygonFrontFace::CounterClockwise;
            pipelineDesc.framebufferFormat.useDepthStencil = true;
            pipelineDesc.framebufferFormat.colorFormats.resize(1);
            pipelineDesc.framebufferFormat.colorFormats[0] = EPixelFormat::R8G8B8A8;
            pipelineDesc.blendState.attachments.resize(1);
            pipelineDesc.blendState.attachments[0].enable = false;

            auto pipelineWireframe = device.createGraphicsPipeline(pipelineDesc);
            auto currentPipeline = pipeline;

            for (auto current = IRenderResource::getResourcesList(); current.isNotNull(); current = current->getNextLink())
                printf("Render resource: %s\n", current->getFriendlyName().data());

            auto drawList = device.createDrawList();

            while (!system.shouldClose(ISystem::MAIN_WINDOW)) {
                engine.update();

                static Vec3f pos = Vec3f(0,0,4);
                static Vec3f dirZ = Vec3f(0,0,-1);
                static Vec3f dirX = Vec3f(1,0,0);
                static float delta = 0.1f;
                static float angle = 0.0f;
                static float step = 0.005f;

                //auto p = Vec3f(Mat4x4f::rotateY(angle) * Vec4f(pos, 1.0f));
                auto p = pos;

                auto Model = Mat4x4f::rotateY(angle * 3);
                auto View = Mat4x4f::lookAt(p, dirZ, Vec3f::Y_AXIS);
                auto Proj = Mat4x4f::perspective(Math::degToRad(100.0f), 1280.0f / 720.0f, 0.01, 100.0f);

                angle += step;

                static const CString model = "Model";
                static const CString view = "View";
                static const CString proj = "Proj";
                uniformBuffer.setMat4(model, Model);
                uniformBuffer.setMat4(view, View);
                uniformBuffer.setMat4(proj, Proj);
                uniformBuffer.updateUniformBufferDataGPU();

                device.beginRenderFrame();
                device.submitDrawList(drawList);
                device.endRenderFrame();

                if (input.isKeyPressed(EKeyboardKey::V)) {
                    currentPipeline = (currentPipeline == pipeline? pipelineWireframe: pipeline);
                }

                if (input.isKeyPressed(EKeyboardKey::W))
                    pos += dirZ;
                if (input.isKeyPressed(EKeyboardKey::S))
                    pos -= dirZ;
                if (input.isKeyPressed(EKeyboardKey::A))
                    pos -= dirX;
                if (input.isKeyPressed(EKeyboardKey::D))
                    pos += dirX;
                if (input.isKeyPressed(EKeyboardKey::E)) {
                    dirZ = Vec3f(Mat4x4f::rotateY(-0.05f) * Vec4f(dirZ, 0.0f));
                    dirX = Vec3f(Mat4x4f::rotateY(-0.05f) * Vec4f(dirX, 0.0f));
                }
                if (input.isKeyPressed(EKeyboardKey::Q)) {
                    dirZ = Vec3f(Mat4x4f::rotateY(0.05f) * Vec4f(dirZ, 0.0f));
                    dirX = Vec3f(Mat4x4f::rotateY(0.05f) * Vec4f(dirX, 0.0f));
                }

                {
                    Region2i area = { 0, 0, 1280, 720 };

                    drawList->begin();
                    drawList->bindWindow(ISystem::MAIN_WINDOW, area, {0,0,0,0}, 1.0f, 0);
                    drawList->bindPipeline(currentPipeline);
                    drawList->bindUniformSet(uniformSet);
                    drawList->bindArrayObject(arrayObject);
                    drawList->drawIndexed(EIndexType::Uint32, indicesCount);

                    renderCanvas.setView(View);
                    renderCanvas.setProjection(Proj);
                    renderCanvas.captureDrawCommands(*drawList);

                    drawList->end();
                }


            }
        }

        engine.finalize();
    };
}
