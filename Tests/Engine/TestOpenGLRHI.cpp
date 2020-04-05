/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>
#include <GLDevice.h>

///////////////////////// Debug
#include <GLTexture.h>
///////////////////////// Debug

using namespace Berserk;

BERSERK_TEST_SECTION(TestOpenGLRHI)
{
    BERSERK_TEST(TestDeviceGL)
    {
        ISystem::VideoMode videoMode{};
        videoMode.width = 1280;
        videoMode.height = 720;
        videoMode.forceVSync = true;
        videoMode.resizeable = false;
        ISystem::initialize("Test OpenGL Device", videoMode, ERenderDeviceType::OpenGL);

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
            -0.2f,  0.2f, 0.0f, 1.0f, 0.0, 0.0, 0.0, 1.0,
            -0.2f, -0.2f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 0.0,
             0.2f, -0.2f, 0.0f, 0.0f, 0.0, 1.0, 1.0, 0.0,
             0.2f,  0.2f, 0.0f, 1.0f, 1.0, 1.0, 1.0, 1.0,
        };

        float32 positions[] = {
            -0.5f,  0.5f,
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f
        };

        uint32 indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        char vertexShader[] =   "#version 410 core\n"
                                "layout (location = 0) in vec3 vPosition;"
                                "layout (location = 1) in vec3 vColor;"
                                "layout (location = 2) in vec2 vPosInstance;"
                                "layout (location = 3) in vec2 vTexCoords;"
                                "out vec3 fColor;"
                                "out vec2 fTexCoords;"
                                "void main() {"
                                "  fColor = vColor;"
                                "  fTexCoords = vTexCoords;"
                                "  gl_Position = vec4(vec3(vPosInstance, 0.0f) + vPosition, 1.0f);"
                                "}";

        char fragmentShader[] = "#version 410 core\n"
                                "layout (location = 0) out vec4 outColor;"
                                "uniform sampler2D Texture0;"
                                "in vec3 fColor;"
                                "in vec2 fTexCoords;"
                                "layout (std140) uniform Block {"
                                "   vec4 color1;"
                                "   vec4 color2;"
                                "   float r1;"
                                "   float r2;"
                                "   vec4 a[4];"
                                "   vec3 b[4];"
                                "   mat4 trnsf;"
                                "};"
                                "void main() {"
                                "  vec3 color = texture(Texture0, fTexCoords.xy).rrr * fColor;"
                                "  color = pow(color, vec3(1.0f/2.2f + r1 + r2)) + b[0];"
                                "  outColor = trnsf * vec4(color, 0.5f) + color1 + color2 + a[0];"
                                "  outColor = vec4(color, 0.5f);"
                                "}";

        auto device = TPtrShared<GLDevice>::make();
        auto vertexBuffer = device->createVertexBuffer(sizeof(vertices), EMemoryType::Dynamic);
        auto positionBuffer = device->createVertexBuffer(sizeof(positions), EMemoryType::Dynamic);
        auto indexBuffer = device->createIndexBuffer(sizeof(indices), EMemoryType::Dynamic);
        auto texture = device->createTexture2D(false, image);

        device->setWindowBindFunction(ISystem::getSingleton().getWindowBindFunction());
        vertexBuffer->update(sizeof(vertices), 0, (uint8*) vertices);
        positionBuffer->update(sizeof(positions), 0, (uint8*) positions);
        indexBuffer->update(sizeof(indices), 0, (uint8*) indices);

        RHIShaderDesc shaderDesc{};
        shaderDesc.resize(2);
        shaderDesc[0].type = EShaderTypeBit::Vertex;
        shaderDesc[0].code.add((uint8*) vertexShader, sizeof(vertexShader));
        shaderDesc[1].type = EShaderTypeBit::Fragment;
        shaderDesc[1].code.add((uint8*) fragmentShader, sizeof(fragmentShader));

        auto shader = device->createShader(EShaderLanguage::GLSL, shaderDesc);
        auto info = device->createShaderIntrospection(shader);

        for (const auto& block: info->etUniformBlocks()) {
            printf("Block name: %s, Binding: %i, Size: %i: Variables: ",
                    block.name.data(), block.binding, block.size);

            int i;

            for (i = 0; i < (int32)block.params.size() - 1; i++)
                printf("%s, ", block.params[i].data());
            for ( ; i < block.params.size(); i++)
                printf("%s", block.params[i].data());

            printf("\n");
        }

        for (const auto& params: info->getParams()) {
            const auto &name = params.first();
            const auto &data = params.second();
            printf("Var name: %s, Block: %i, Size: %i, Offset: %i, Location: %i, Array: %i, Stride: %i\n",
                   name.data(), data.block, data.size, data.offset, data.location, data.array, data.stride);
        }

        RHISamplerDesc samplerDesc{};
        auto sampler = device->createSampler(samplerDesc);

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
        auto array = device->createArrayObject({ vertexBuffer, positionBuffer }, indexBuffer, declaration);

        RHIGraphicsPipelineDesc pipelineDesc{};
        pipelineDesc.shader = shader;
        pipelineDesc.framebufferFormat.useDepthStencil = true;
        pipelineDesc.framebufferFormat.colorFormats.add(EPixelFormat::R8G8B8A8);
        auto& blend = pipelineDesc.blendState.attachments.emplace();
        blend.enable = true;
        blend.srcColorBlendFactor = EBlendFactor::SrcAlpha;
        blend.dstColorBlendFactor = EBlendFactor::OneMinusSrcAlpha;
        blend.srcAlphaBlendFactor = EBlendFactor::One;
        blend.dstAlphaBlendFactor = EBlendFactor::Zero;
        auto pipeline = device->createGraphicsPipeline(pipelineDesc);

        auto list = device->createDrawList();
        list->begin();
        list->bindWindow(ISystem::MAIN_WINDOW, Region2i(0,0,videoMode.width,videoMode.height), Color4f(0.9f,0.1f,0.1f, 1.0f));
        list->bindPipeline(pipeline);
        list->bindArrayObject(array);
        list->drawIndexedInstances(EIndexType::Uint32, 6, 4);
        list->end();

        auto t = TimeValue::now();
        auto d = TimeValue().setMilliseconds(30.0f);

        while (!ISystem::getSingleton().shouldClose(ISystem::MAIN_WINDOW)) {
            ISystem::update();

            // todo: remove
            {
                ((GLTexture&)*texture).bind(0); // todo: remove (generates gl error)
                device->beginRenderFrame();
                device->submitDrawList(list);
                device->endRenderFrame();
            }

            t = TimeValue::wait(t + d);
        }

        ISystem::finalize();
    };
}
