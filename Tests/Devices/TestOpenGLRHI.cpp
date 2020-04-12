/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>
#include <GLDevice.h>
#include <Math/Vec3f.h>
#include <Math/Mat4x4f.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestOpenGLRHI)
{
    BERSERK_TEST(TestDeviceGL)
    {
        struct OffscreenPass {
            TPtrShared<RHIShader> shader;
            TPtrShared<RHIShaderIntrospection> info;
            TPtrShared<RHITexture> texture;
            TPtrShared<RHISampler> sampler;
            TPtrShared<RHITexture> color0;
            TPtrShared<RHITexture> depth;
            TPtrShared<RHIFramebuffer> framebuffer;
            TPtrShared<RHIUniformSet> uniformSet;
            TPtrShared<RHIArrayObject> object;
            TPtrShared<RHIUniformBuffer> uniformBuffer;
            TPtrShared<RHIGraphicsPipeline> pipeline;
        } offscreenPass;

        struct PresentPass {
            TPtrShared<RHISampler> sampler;
            TPtrShared<RHIArrayObject> object;
            TPtrShared<RHIUniformSet> uniformSet;
            TPtrShared<RHIGraphicsPipeline> pipeline;
            TPtrShared<RHIShader> shader;
            TPtrShared<RHIShaderIntrospection> info;
        } presentPass;

        ISystem::VideoMode videoMode{};
        videoMode.width = 1280;
        videoMode.height = 720;
        videoMode.forceVSync = true;
        videoMode.resizeable = false;
        ISystem::initialize("Test OpenGL Device", videoMode, ERenderDeviceType::OpenGL);

        uint32 framebufferWidth = videoMode.width;
        uint32 framebufferHeight = videoMode.height;

        Color4f background(0.905f, 0.815f, 0.901f);

        Image image;
        image.create(100, 100, EPixelFormat::R8, Color4f(0.0f));
        for (uint32 x = 0; x < 100; x++) {
            for (uint32 y = 0; y < 100; y++) {
                if (((x / 10) % 2) == 0 && ((y / 10) % 2) == 0) {
                    image.setPixel(x, y, Color4f::fromR8(0xff));
                }
                if (((x / 10) % 2) == 1 && ((y / 10) % 2) == 1) {
                    image.setPixel(x, y, Color4f::fromR8(0xff));
                }
            }
        }

        float32 vertices[] = {
            -0.4f,  0.4f,  0.4f, 1.0f, 1.0, 1.0, 0.0, 1.0,
            -0.4f, -0.4f,  0.4f, 1.0f, 1.0, 1.0, 0.0, 0.0,
             0.4f, -0.4f,  0.4f, 1.0f, 1.0, 1.0, 1.0, 0.0,
             0.4f,  0.4f,  0.4f, 1.0f, 1.0, 1.0, 1.0, 1.0,

             0.4f,  0.4f,  0.4f, 1.0f, 1.0, 1.0, 0.0, 1.0,
             0.4f, -0.4f,  0.4f, 1.0f, 1.0, 1.0, 0.0, 0.0,
             0.4f, -0.4f, -0.4f, 1.0f, 1.0, 1.0, 1.0, 0.0,
             0.4f,  0.4f, -0.4f, 1.0f, 1.0, 1.0, 1.0, 1.0,

             0.4f,  0.4f, -0.4f, 1.0f, 1.0, 1.0, 0.0, 1.0,
             0.4f, -0.4f, -0.4f, 1.0f, 1.0, 1.0, 0.0, 0.0,
            -0.4f, -0.4f, -0.4f, 1.0f, 1.0, 1.0, 1.0, 0.0,
            -0.4f,  0.4f, -0.4f, 1.0f, 1.0, 1.0, 1.0, 1.0,

            -0.4f,  0.4f, -0.4f, 1.0f, 1.0, 1.0, 0.0, 1.0,
            -0.4f, -0.4f, -0.4f, 1.0f, 1.0, 1.0, 0.0, 0.0,
            -0.4f, -0.4f,  0.4f, 1.0f, 1.0, 1.0, 1.0, 0.0,
            -0.4f,  0.4f,  0.4f, 1.0f, 1.0, 1.0, 1.0, 1.0,

            -0.4f,  0.4f, -0.4f, 1.0f, 1.0, 1.0, 0.0, 1.0,
            -0.4f,  0.4f,  0.4f, 1.0f, 1.0, 1.0, 0.0, 0.0,
             0.4f,  0.4f,  0.4f, 1.0f, 1.0, 1.0, 1.0, 0.0,
             0.4f,  0.4f, -0.4f, 1.0f, 1.0, 1.0, 1.0, 1.0,

            -0.4f, -0.4f,  0.4f, 1.0f, 1.0, 1.0, 0.0, 1.0,
            -0.4f, -0.4f, -0.4f, 1.0f, 1.0, 1.0, 0.0, 0.0,
             0.4f, -0.4f, -0.4f, 1.0f, 1.0, 1.0, 1.0, 0.0,
             0.4f, -0.4f,  0.4f, 1.0f, 1.0, 1.0, 1.0, 1.0,
        };

        float32 positions[] = {
            -0.5f,  0.5f,
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f
        };

        uint32 indices[] = {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            8, 9, 10,
            10,11,8,
            12,13,14,
            14,15,12,
            16,17,18,
            18,19,16,
            20,21,22,
            22,23,20
        };

        auto Proj = Mat4x4f::perspective(Math::degToRad(90.0f), videoMode.width / (float32)videoMode.height, 0.01, 10.0f);
        auto View = Mat4x4f();

        char vertexShader[] =   "#version 410 core\n"
                                "layout (location = 0) in vec3 vPosition;"
                                "layout (location = 1) in vec3 vColor;"
                                "layout (location = 2) in vec2 vPosInstance;"
                                "layout (location = 3) in vec2 vTexCoords;"
                                "out vec3 fColor;"
                                "out vec2 fTexCoords;"
                                "layout (std140) uniform Transform {"
                                "   mat4 View;"
                                "   mat4 Proj;"
                                "};"
                                "void main() {"
                                "  fColor = vColor;"
                                "  fTexCoords = vTexCoords;"
                                "  gl_Position = Proj * View * vec4(vec3(vPosInstance, 0.0f) + vPosition, 1.0f);"
                                "}";

        char fragmentShader[] = "#version 410 core\n"
                                "layout (location = 0) out vec4 outColor;"
                                "uniform sampler2D Texture0;"
                                "in vec3 fColor;"
                                "in vec2 fTexCoords;"
                                "void main() {"
                                "  vec3 color = texture(Texture0, fTexCoords.xy).rrr * fColor;"
                                "  color = pow(color, vec3(1.0f/2.2f));"
                                "  outColor = vec4(color, 0.5f);"
                                "}";

        auto device = TPtrShared<GLDevice>::make();
        device->setWindowBindFunction(ISystem::getSingleton().getWindowBindFunction());

        {
            offscreenPass.color0 = device->createTexture2D(framebufferWidth, framebufferHeight, EMemoryType::Dynamic, EPixelFormat::R8G8B8A8, false);
            offscreenPass.depth = device->createTexture2D(framebufferWidth, framebufferHeight, EMemoryType::Dynamic, EPixelFormat::D24_S8, false);
            offscreenPass.framebuffer = device->createFramebuffer({offscreenPass.color0}, offscreenPass.depth);
            offscreenPass.framebuffer->setClearColor(0, background);
            offscreenPass.framebuffer->setClearOption(EClearOption::Color, true);
            offscreenPass.framebuffer->setClearOption(EClearOption::Depth, true);
            offscreenPass.framebuffer->setClearOption(EClearOption::Stencil, true);

            auto vertexBuffer = device->createVertexBuffer(sizeof(vertices), EMemoryType::Dynamic);
            auto positionBuffer = device->createVertexBuffer(sizeof(positions), EMemoryType::Dynamic);
            auto indexBuffer = device->createIndexBuffer(sizeof(indices), EMemoryType::Dynamic);

            vertexBuffer->update(sizeof(vertices), 0, (uint8*) vertices);
            positionBuffer->update(sizeof(positions), 0, (uint8*) positions);
            indexBuffer->update(sizeof(indices), 0, (uint8*) indices);

            RHISamplerDesc samplerDesc{};
            offscreenPass.sampler = device->createSampler(samplerDesc);
            offscreenPass.texture = device->createTexture2D(EMemoryType::Static, true, image);

            RHIShaderDesc shaderDesc{};
            shaderDesc.resize(2);
            shaderDesc[0].type = EShaderType::Vertex;
            shaderDesc[0].code.add((uint8*) vertexShader, sizeof(vertexShader));
            shaderDesc[1].type = EShaderType::Fragment;
            shaderDesc[1].code.add((uint8*) fragmentShader, sizeof(fragmentShader));

            offscreenPass.shader = device->createShader(EShaderLanguage::GLSL, shaderDesc);
            offscreenPass.info = device->createShaderIntrospection(offscreenPass.shader);
            {
                for (const auto& block: offscreenPass.info->getUniformBlocks()) {
                    printf("Block name: %s, Binding: %i, Size: %i: Variables: ",
                           block.name.data(), block.binding, block.size);

                    int i;

                    for (i = 0; i < (int32)block.params.size() - 1; i++)
                        printf("%s, ", block.params[i].data());
                    for ( ; i < block.params.size(); i++)
                        printf("%s", block.params[i].data());

                    printf("\n");
                }

                for (const auto& params: offscreenPass.info->getParams()) {
                    const auto &name = params.first();
                    const auto &data = params.second();
                    printf("Var name: %s, Block: %i, Size: %i, Offset: %i, Location: %i, Array: %i, Stride: %i\n",
                           name.data(), data.block, data.size, data.offset, data.location, data.array, data.stride);
                }
            }

            offscreenPass.uniformBuffer = device->createUniformBuffer(offscreenPass.info->getUniformBlocks()[0].size, EMemoryType::Dynamic);

            TArray<RHIUniformTextureDesc> uniformTextures;
            {
                auto& desc = uniformTextures.emplace();
                desc.texture = offscreenPass.texture;
                desc.sampler = offscreenPass.sampler;
                desc.location = offscreenPass.info->getParams()["Texture0"].location;
            }
            TArray<RHIUniformBlockDesc> uniformBlocks;
            {
                auto& desc = uniformBlocks.emplace();
                auto& block = offscreenPass.info->getUniformBlocks()[0];
                desc.binding = block.binding;
                desc.offset = 0;
                desc.range = block.size;
                desc.buffer = offscreenPass.uniformBuffer;
            }
            offscreenPass.uniformSet = device->createUniformSet(uniformTextures, uniformBlocks);

            RHIVertexDeclarationDesc vertDeclDesc;
            vertDeclDesc.resize(4);
            vertDeclDesc[0].buffer = 0;
            vertDeclDesc[0].iterating = EVertexIterating::PerVertex;
            vertDeclDesc[0].stride = 8 * sizeof(float32);
            vertDeclDesc[0].location = 0;
            vertDeclDesc[0].offset = 0;
            vertDeclDesc[0].type = EVertexElementType::Float3;
            vertDeclDesc[1].buffer = 0;
            vertDeclDesc[1].iterating = EVertexIterating::PerVertex;
            vertDeclDesc[1].stride = 8 * sizeof(float32);
            vertDeclDesc[1].location = 1;
            vertDeclDesc[1].offset = 3 * sizeof(float32);
            vertDeclDesc[1].type = EVertexElementType::Float3;
            vertDeclDesc[2].buffer = 1;
            vertDeclDesc[2].iterating = EVertexIterating::PerInstance;
            vertDeclDesc[2].stride = 2 * sizeof(float32);
            vertDeclDesc[2].location = 2;
            vertDeclDesc[2].offset = 0;
            vertDeclDesc[2].type = EVertexElementType::Float2;
            vertDeclDesc[3].buffer = 0;
            vertDeclDesc[3].iterating = EVertexIterating::PerVertex;
            vertDeclDesc[3].stride = 8 * sizeof(float32);
            vertDeclDesc[3].location = 3;
            vertDeclDesc[3].offset = 6 * sizeof(float32);
            vertDeclDesc[3].type = EVertexElementType::Float2;

            auto declaration = device->createVertexDeclaration(vertDeclDesc);
            offscreenPass.object = device->createArrayObject({ vertexBuffer, positionBuffer }, indexBuffer, declaration);

            RHIGraphicsPipelineDesc pipelineDesc{};
            pipelineDesc.depthTest = false;
            pipelineDesc.depthWrite = false;
            pipelineDesc.depthCompare = ECompareFunction::Less;
            pipelineDesc.polygonMode = EPolygonMode::Fill;
            pipelineDesc.polygonCullMode = EPolygonCullMode::Disabled;
            pipelineDesc.shader = offscreenPass.shader;
            pipelineDesc.framebufferFormat.useDepthStencil = true;
            pipelineDesc.framebufferFormat.colorFormats.add(EPixelFormat::R8G8B8A8);
            auto& blend = pipelineDesc.blendState.attachments.emplace();
            blend.enable = true;
            blend.srcColorBlendFactor = EBlendFactor::SrcAlpha;
            blend.dstColorBlendFactor = EBlendFactor::OneMinusSrcAlpha;
            blend.srcAlphaBlendFactor = EBlendFactor::One;
            blend.dstAlphaBlendFactor = EBlendFactor::Zero;
            offscreenPass.pipeline = device->createGraphicsPipeline(pipelineDesc);
        }

        {
            char presentVertShader[] = "#version 410 core\n"
                                       "layout (location = 0) in vec2 vPosition;"
                                       "layout (location = 1) in vec2 vTexCoords;"
                                       "out vec2 fTexCoords;"
                                       "void main() {"
                                       "  fTexCoords = vTexCoords;"
                                       "  gl_Position = vec4(vPosition, 0.0f, 1.0f);"
                                       "}";

            char presentFragShader[] = "#version 410 core\n"
                                       "layout (location = 0) out vec4 fColor;"
                                       "in vec2 fTexCoords;"
                                       "uniform sampler2D Texture0;"
                                       "void main() {"
                                       "  vec2 coordsR = fTexCoords + vec2( 0.01f, 0.0f);"
                                       "  vec2 coordsG = fTexCoords;"
                                       "  vec2 coordsB = fTexCoords + vec2(-0.01f, 0.0f);"
                                       "  float r = texture(Texture0, coordsR).r;"
                                       "  float g = texture(Texture0, coordsG).g;"
                                       "  float b = texture(Texture0, coordsB).b;"
                                       "  fColor = vec4(r, g, b, 1.0f);"
                                       "}";

            RHIShaderDesc shaderDesc{};
            shaderDesc.resize(2);
            shaderDesc[0].type = EShaderType::Vertex;
            shaderDesc[0].code.add((uint8*) presentVertShader, sizeof(presentVertShader));
            shaderDesc[1].type = EShaderType::Fragment;
            shaderDesc[1].code.add((uint8*) presentFragShader, sizeof(presentFragShader));

            presentPass.shader = device->createShader(EShaderLanguage::GLSL, shaderDesc);
            presentPass.info = device->createShaderIntrospection(presentPass.shader);
            {
                for (const auto& block: presentPass.info->getUniformBlocks()) {
                    printf("Block name: %s, Binding: %i, Size: %i: Variables: ",
                           block.name.data(), block.binding, block.size);

                    int i;

                    for (i = 0; i < (int32)block.params.size() - 1; i++)
                        printf("%s, ", block.params[i].data());
                    for ( ; i < block.params.size(); i++)
                        printf("%s", block.params[i].data());

                    printf("\n");
                }

                for (const auto& params: presentPass.info->getParams()) {
                    const auto &name = params.first();
                    const auto &data = params.second();
                    printf("Var name: %s, Block: %i, Size: %i, Offset: %i, Location: %i, Array: %i, Stride: %i\n",
                           name.data(), data.block, data.size, data.offset, data.location, data.array, data.stride);
                }
            }

            RHISamplerDesc samplerDesc{};
            samplerDesc.useMips = false;
            samplerDesc.u = ESamplerRepeatMode::ClampToBorder;
            samplerDesc.v = ESamplerRepeatMode::ClampToBorder;
            samplerDesc.w = ESamplerRepeatMode::ClampToBorder;
            samplerDesc.color = ESamplerBorderColor::White;
            presentPass.sampler = device->createSampler(samplerDesc);

            TArray<RHIUniformTextureDesc> uniformTextures;
            {
                auto& desc = uniformTextures.emplace();
                desc.texture = offscreenPass.color0;
                desc.sampler = presentPass.sampler;
                desc.location = presentPass.info->getParams()["Texture0"].location;
            }
            presentPass.uniformSet = device->createUniformSet(uniformTextures, TArray<RHIUniformBlockDesc>());

            float32 screen[] = {
                -1.0,  1.0f, 0.0, 1.0,
                -1.0, -1.0f, 0.0, 0.0,
                 1.0, -1.0f, 1.0, 0.0,
                 1.0, -1.0f, 1.0, 0.0,
                 1.0,  1.0f, 1.0, 1.0,
                -1.0,  1.0f, 0.0, 1.0
            };

            auto vertexBuffer = device->createVertexBuffer(sizeof(screen), EMemoryType::Dynamic);
            vertexBuffer->update(sizeof(screen), 0, (uint8*) screen);

            RHIVertexDeclarationDesc vertexDeclarationDesc;
            vertexDeclarationDesc.resize(2);
            vertexDeclarationDesc[0].buffer = 0;
            vertexDeclarationDesc[0].iterating = EVertexIterating::PerVertex;
            vertexDeclarationDesc[0].location = 0;
            vertexDeclarationDesc[0].offset = 0;
            vertexDeclarationDesc[0].stride = 4 * sizeof(float32);
            vertexDeclarationDesc[0].type = EVertexElementType::Float2;
            vertexDeclarationDesc[1].buffer = 0;
            vertexDeclarationDesc[1].iterating = EVertexIterating::PerVertex;
            vertexDeclarationDesc[1].location = 1;
            vertexDeclarationDesc[1].offset = 2 * sizeof(float32);
            vertexDeclarationDesc[1].stride = 4 * sizeof(float32);
            vertexDeclarationDesc[1].type = EVertexElementType::Float2;

            auto vertexDeclaration = device->createVertexDeclaration(vertexDeclarationDesc);

            presentPass.object = device->createArrayObject({vertexBuffer}, nullptr, vertexDeclaration);

            RHIGraphicsPipelineDesc pipelineDesc{};
            pipelineDesc.shader = presentPass.shader;
            pipelineDesc.depthTest = false;
            pipelineDesc.depthWrite = false;
            pipelineDesc.depthCompare = ECompareFunction::Less;
            pipelineDesc.polygonMode = EPolygonMode::Fill;
            pipelineDesc.polygonCullMode = EPolygonCullMode::Disabled;
            pipelineDesc.framebufferFormat.useDepthStencil = true;
            pipelineDesc.framebufferFormat.colorFormats.add(EPixelFormat::R8G8B8A8);
            auto& blend = pipelineDesc.blendState.attachments.emplace();
            blend.enable = false;

            presentPass.pipeline = device->createGraphicsPipeline(pipelineDesc);
        }

        auto list = device->createDrawList();
        {
            list->begin();
            list->bindFramebuffer(offscreenPass.framebuffer, Region2i(0,0,framebufferWidth,framebufferHeight));
            list->bindPipeline(offscreenPass.pipeline);
            list->bindArrayObject(offscreenPass.object);
            list->bindUniformSet(offscreenPass.uniformSet);
            list->drawIndexedInstances(EIndexType::Uint32, 36, 4);
            list->bindWindow(ISystem::MAIN_WINDOW, Region2i(0,0,videoMode.width,videoMode.height), background);
            list->bindPipeline(presentPass.pipeline);
            list->bindArrayObject(presentPass.object);
            list->bindUniformSet(presentPass.uniformSet);
            list->draw(6);
            list->end();
        }

        auto v = Vec3f(Mat4x4f::rotateY(Math::degToRad(90.0f)) * Vec4f(0,0,1,1));

        auto t = TimeValue::now();
        auto d = TimeValue().setMilliseconds(30.0f);

        while (!ISystem::getSingleton().shouldClose(ISystem::MAIN_WINDOW)) {
            ISystem::update();

            {
                device->beginRenderFrame();
                device->submitDrawList(list);
                device->endRenderFrame();
            }

            t = TimeValue::wait(t + d);

            {
                static Vec3f pos = Vec3f(0,0,1);
                static float32 angle = 0.0f;
                static float32 step = 0.005f;

                auto p = Vec3f(Mat4x4f::rotateY(angle) * Vec4f(pos, 1.0f));
                View = Mat4x4f::lookAt(p, -p, Vec3f::Y_AXIS);

                const auto& pProj = offscreenPass.info->getParams()["Proj"];
                const auto& pView = offscreenPass.info->getParams()["View"];
                auto tProj = Proj.transpose();
                auto tView = View.transpose();
                offscreenPass.uniformBuffer->update(pProj.size, pProj.offset, (uint8*) tProj.data());
                offscreenPass.uniformBuffer->update(pView.size, pView.offset, (uint8*) tView.data());

                angle += step;
            }
        }

        ISystem::finalize();
    };
}
