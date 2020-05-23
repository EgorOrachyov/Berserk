/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/RenderCanvas.h>
#include <Rendering/RenderModule.h>
#include <Rendering/Util/GeometryGenerator.h>
#include <Rendering/VertexPolicyBuilder.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Rendering {

        struct SphereInstanceData {
            Vec4f posRadius;
            Vec3f color;
        };

        RenderCanvas::RenderCanvas(TPtrShared<IRenderTarget> target) {
            mDrawTarget = std::move(target);

            createSphereData();
            createCommonData();
        }

        void RenderCanvas::drawSphere(const Vec3f &pos, float radius, const Color4f &color) {
            SphereDraw request;
            request.radius = Math::max(0.01f, radius);
            request.pos = pos;
            request.color = color;
            mSphere3dRequests.emplace(request);
        }

        void RenderCanvas::captureDrawCommands(RHIDrawList &drawList) {
            auto& device = RHIDevice::getSingleton();

            {
                auto projView = mPerspective * mView;
                auto orthoView = mOrthographic * mView;
                static CString ProjView = "ProjView";
                static CString OrthoView = "OrthoView";

                mCommonData->setMat4(ProjView, projView);
                mCommonData->setMat4(OrthoView, orthoView);
                mCommonData->updateUniformBufferDataGPU();
            }

            {
                uint32 requestCount = mSphere3dRequests.size();
                uint32 requiredBuffers = requestCount / mMaxSpheresInGPU + (requestCount % mMaxSpheresInGPU? 1: 0);

                while (mSphere3dBuffers.size() < requiredBuffers) {
                    auto& last = mSphere3dBuffers.emplace(sizeof(SphereInstanceData) * mMaxSpheresInGPU);
                    auto& object = mSphere3dObjects.emplace();
                    auto& base = mSphere3dObjects[0];

                    object = device.createArrayObject({base->getVertexBuffers()[0], last.getVertexBufferRHI()}, base->getIndexBuffer(), base->getVertexDeclaration());
                }

                for (uint32 i = 0; i < requestCount; i++) {
                    uint32 bidx = i / mMaxSpheresInGPU;
                    uint32 offset = (i % mMaxSpheresInGPU) * sizeof(SphereInstanceData);

                    auto& buffer = mSphere3dBuffers[bidx];
                    auto& request = mSphere3dRequests[i];

                    SphereInstanceData instanceData{Vec4f(request.pos, request.radius), Vec3f(request.color)};
                    buffer.write(sizeof(SphereInstanceData), offset, &instanceData);
                }

                for (auto& buffer: mSphere3dBuffers) {
                    buffer.updateBufferOnGpu();
                }
                
                drawList.bindPipeline(mSphere3dPipeline->getPipelineRHI());
                drawList.bindUniformSet(mCommonSet);

                uint32 rest = requestCount;
                for (auto& object: mSphere3dObjects) {
                    if (rest == 0) break;

                    uint32 toDraw = (mMaxSpheresInGPU > rest? rest : mMaxSpheresInGPU);
                    rest -= toDraw;

                    drawList.bindArrayObject(object);
                    drawList.drawIndexedInstances(EIndexType::Uint32, mSphere3dIndicesCount, toDraw);
                }
            }
        }

        void RenderCanvas::clearCanvas() {
            mSphere3dRequests.clear();
        }

        void RenderCanvas::createSphereData() {
            auto& device = RHIDevice::getSingleton();
            auto& module = RenderModule::getSingleton();
            auto& shaderManager = module.getShaderManager();
            auto& shaderCache = module.getShaderCache();

            VertexPolicyBuilder builder;
            builder
              .ensureElements(3)
              .addNamedBuffer({EVertexAttribute::Position})
              .addBufferInstanced({ EVertexElementType::Float4, EVertexElementType::Float3 });

            auto policy = std::move(builder.build());

            TArray<uint8> sphereVertexData;
            TArray<uint32> sphereIndexData;
            uint32 verticesCount;

            GeometryGenerator::generateSphere(1.0f, 6, 6, policy, verticesCount, sphereVertexData, sphereIndexData);

            auto vertexDeclaration = device.createVertexDeclaration(policy.getElementsDescsRHI());
            auto vertexBuffer = device.createVertexBuffer(sphereVertexData.size(), EMemoryType::Static, sphereVertexData.data());
            auto indexBuffer = device.createIndexBuffer(sphereIndexData.size() * sizeof(uint32), EMemoryType::Static, sphereIndexData.data());

            mSphere3dIndicesCount = sphereIndexData.size();
            mSphere3dBuffers.emplace(mMaxSpheresInGPU * sizeof(SphereInstanceData));
            mSphere3dObjects.emplace();

            mSphere3dObjects[0] = device.createArrayObject({ vertexBuffer, mSphere3dBuffers[0].getVertexBufferRHI() }, indexBuffer, vertexDeclaration);

            mSphere3dShader = shaderManager.loadShader("CanvasDrawSphere3d");
            shaderCache.cacheShader(*mSphere3dShader);

            mSphere3dPipeline = TPtrUnique<GraphicsPipeline>::make(mSphere3dShader->getShaderHandle(), true, false, EPrimitivesType::Triangles, EPolygonCullMode::Disabled, EPolygonMode::Line, *mDrawTarget);
            mSphere3dRequests.ensureToAdd(100);
        }

        void RenderCanvas::createCommonData() {
            auto& device = RHIDevice::getSingleton();
            auto block = mSphere3dShader->findUniformBlock("Transform");

            mCommonData = TPtrUnique<UniformBuffer>::make("CanvasTransform", *block);

            TArray<RHIUniformBlockDesc> uniformBlocks;
            {
                auto& desc = uniformBlocks.emplace();
                desc.binding = block->getBinding();
                desc.buffer = mCommonData->getUniformBufferRHI();
                desc.offset = 0;
                desc.range = block->getSize();
            }
            mCommonSet = device.createUniformSet(TArray<RHIUniformTextureDesc>(), uniformBlocks);
        }

    }
}