/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>
#include <Module.h>
#include <Math/Vec3f.h>
#include <Math/Mat4x4f.h>
#include <String/CStringBuilder.h>
#include <RHI/RHIDevice.h>
#include <Engine.h>
#include <Console/ConsoleManager.h>
#include <Platform/WindowManager.h>
#include <ShaderDefsMacro.h>

using namespace Berserk;
using namespace Render;

BERSERK_TEST_SECTION(TestOpenGLRHI)
{
    BERSERK_TEST(TestDeviceGL)
    {
        struct OffscreenPass {
            TPtrShared<RHIShader> shader;
            TPtrShared<RHIShaderMetaData> info;
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
            TPtrShared<RHIShaderMetaData> info;
        } presentPass;

        ConsoleManager console;

        int32 width = 1920;
        int32 height = 1280;
        
        auto& sys = System::getSingleton();
        sys.initialize("MAIN", "Test OpenGL Device", {width, height}, false, ERenderDeviceType::OpenGL, CString(),
                       false);
        auto window = WindowManager::getSingleton().find("MAIN");
        
        uint32 framebufferWidth = width / 2;
        uint32 framebufferHeight = height / 2;

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

        float vertices[] = {
            -0.4f,  0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            -0.4f, -0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
             0.4f, -0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
             0.4f,  0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

             0.4f,  0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
             0.4f, -0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
             0.4f, -0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
             0.4f,  0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

             0.4f,  0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
             0.4f, -0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            -0.4f, -0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            -0.4f,  0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

            -0.4f,  0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            -0.4f, -0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            -0.4f, -0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            -0.4f,  0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

            -0.4f,  0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            -0.4f,  0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
             0.4f,  0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
             0.4f,  0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

            -0.4f, -0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            -0.4f, -0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
             0.4f, -0.4f, -0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
             0.4f, -0.4f,  0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        };

        float positions[] = {
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

        auto Proj = Mat4x4f::perspective(Math::degToRad(90.0f), width / (float)height, 0.01, 100.0f);
        auto View = Mat4x4f();

        char vertexShader[] =   "layout (location = 0) in vec3 vPosition;"
                                "layout (location = 1) in vec3 vColor;"
                                "layout (location = 2) in vec2 vPosInstance;"
                                "layout (location = 3) in vec2 vTexCoords;"
                                "out vec3 fColor;"
                                "out vec2 fTexCoords;"
                                "layout (std140) uniform Transform {"
                                "   mat4 View;"
                                "   mat4 Proj;"
                                "\n#ifdef USE_BUFFER_ARRAYS\n"
                                "   float array1[7];"
                                "   mat3 array2[4];\n"
                                "\n#endif //USE_BUFFER_ARRAYS\n"
                                "};"
                                "void main() {"
                                "  fColor = vColor;"
                                "  fTexCoords = vTexCoords;"
                                "  gl_Position = Proj * View * vec4(vec3(vPosInstance, 0.0f) + vPosition, 1.0f);"
                                ""
                                "\n#ifdef USE_BUFFER_ARRAYS\n"
                                "  float a = array1[0];"
                                "  mat3 b = array2[0];"
                                "\n#endif //USE_BUFFER_ARRAYS\n"
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

        auto &device = RHIDevice::getSingleton();

        {
            offscreenPass.color0 = device.createTexture2D(framebufferWidth, framebufferHeight, EMemoryType::Dynamic, EPixelFormat::R8G8B8A8, false);
            offscreenPass.depth = device.createTexture2D(framebufferWidth, framebufferHeight, EMemoryType::Dynamic, EPixelFormat::D24S8, false);
            offscreenPass.framebuffer = device.createFramebuffer({offscreenPass.color0}, offscreenPass.depth);
            offscreenPass.framebuffer->setClearColor(0, background);
            offscreenPass.framebuffer->setClearOption(EClearOption::Color, true);
            offscreenPass.framebuffer->setClearOption(EClearOption::Depth, true);
            offscreenPass.framebuffer->setClearOption(EClearOption::Stencil, true);

            auto vertexBuffer = device.createVertexBuffer(sizeof(vertices), EMemoryType::Dynamic, nullptr);
            auto positionBuffer = device.createVertexBuffer(sizeof(positions), EMemoryType::Dynamic, nullptr);
            auto indexBuffer = device.createIndexBuffer(sizeof(indices), EMemoryType::Dynamic, nullptr);

            vertexBuffer->update(sizeof(vertices), 0, (uint8*) vertices);
            positionBuffer->update(sizeof(positions), 0, (uint8*) positions);
            indexBuffer->update(sizeof(indices), 0, (uint8*) indices);

            RHISamplerDesc samplerDesc{};
            offscreenPass.sampler = device.createSampler(samplerDesc);
            offscreenPass.texture = device.createTexture2D(EMemoryType::Static, true, image);

            ShaderDefsMacro preprocessor;
            CStringBuilder vertexShaderCode;
            vertexShaderCode.append("#version 410 core\n");
            preprocessor.setDefinitionPrefix("#define");
            preprocessor.addDefinition("USE_BUFFER_ARRAYS");
            preprocessor.writeDefs(vertexShaderCode);
            vertexShaderCode.append(vertexShader);
            vertexShaderCode.append('\0');

            RHIShaderDesc shaderDesc{};
            shaderDesc.resize(2);
            shaderDesc[0].type = EShaderType::Vertex;
            shaderDesc[0].code.add((uint8*) vertexShaderCode.data(), vertexShaderCode.length());
            shaderDesc[1].type = EShaderType::Fragment;
            shaderDesc[1].code.add((uint8*) fragmentShader, sizeof(fragmentShader));

            offscreenPass.shader = device.createShader(EShaderLanguage::GLSL, shaderDesc);
            offscreenPass.info = device.createShaderMeta(offscreenPass.shader);
            {
                auto& inputs = offscreenPass.info->getVertexShaderAttributes();

                for (auto& i: inputs) {
                    printf("Input: (name='%s',location=%i,type='%s')\n",
                            i.getName().data(), i.getLocation(), RHIShaderMetaData::getShaderDataName(i.getDataType()));
                }

                auto& params = offscreenPass.info->getParams();
                for (auto& p: params) {
                    printf("Param: (name='%s',location=%u)\n", p.getName().data(), p.getLocation());
                }

                auto& blocks = offscreenPass.info->getUniformBlocks();
                for (auto& b: blocks) {
                    printf("Block: (name='%s',binding=%u,size=%u)\n", b.getName().data(), b.getBinding(), b.getSize());

                    for (auto& m: b.getMembers()) {
                        printf(" Member: (name='%s',offset=%u,size=%u,elements=%u,stride=%u,matrixStride=%u,rowMajor=%u,type=%s)\n",
                                m.getName().data(), m.getOffset(), m.getSize(), m.getElements(),
                                m.getStride(), m.getMatrixStride(), m.getIsRowMajor(),
                               RHIShaderMetaData::getShaderDataName(m.getBaseType()));
                    }
                }
            }

            offscreenPass.uniformBuffer = device.createUniformBuffer(offscreenPass.info->getUniformBlock("Transform")->getSize(), EMemoryType::Dynamic, nullptr);

            TArray<RHIUniformTextureDesc> uniformTextures;
            {
                auto& desc = uniformTextures.emplace();
                desc.texture = offscreenPass.texture;
                desc.sampler = offscreenPass.sampler;
                desc.location = offscreenPass.info->getParam("Texture0")->getLocation();
            }
            TArray<RHIUniformBlockDesc> uniformBlocks;
            {
                auto& desc = uniformBlocks.emplace();
                auto& block = *offscreenPass.info->getUniformBlock("Transform");
                desc.binding = block.getBinding();
                desc.offset = 0;
                desc.range = block.getSize();
                desc.buffer = offscreenPass.uniformBuffer;
            }
            offscreenPass.uniformSet = device.createUniformSet(uniformTextures, uniformBlocks);

            RHIVertexDeclarationDesc vertDeclDesc;
            vertDeclDesc.resize(4);
            vertDeclDesc[0].buffer = 0;
            vertDeclDesc[0].iterating = EVertexIterating::PerVertex;
            vertDeclDesc[0].stride = 8 * sizeof(float);
            vertDeclDesc[0].location = 0;
            vertDeclDesc[0].offset = 0;
            vertDeclDesc[0].type = EVertexElementType::Float3;
            vertDeclDesc[1].buffer = 0;
            vertDeclDesc[1].iterating = EVertexIterating::PerVertex;
            vertDeclDesc[1].stride = 8 * sizeof(float);
            vertDeclDesc[1].location = 1;
            vertDeclDesc[1].offset = 3 * sizeof(float);
            vertDeclDesc[1].type = EVertexElementType::Float3;
            vertDeclDesc[2].buffer = 1;
            vertDeclDesc[2].iterating = EVertexIterating::PerInstance;
            vertDeclDesc[2].stride = 2 * sizeof(float);
            vertDeclDesc[2].location = 2;
            vertDeclDesc[2].offset = 0;
            vertDeclDesc[2].type = EVertexElementType::Float2;
            vertDeclDesc[3].buffer = 0;
            vertDeclDesc[3].iterating = EVertexIterating::PerVertex;
            vertDeclDesc[3].stride = 8 * sizeof(float);
            vertDeclDesc[3].location = 3;
            vertDeclDesc[3].offset = 6 * sizeof(float);
            vertDeclDesc[3].type = EVertexElementType::Float2;

            auto declaration = device.createVertexDeclaration(vertDeclDesc);
            offscreenPass.object = device.createArrayObject({ vertexBuffer, positionBuffer }, indexBuffer, declaration);

            RHIGraphicsPipelineDesc pipelineDesc{};
            pipelineDesc.forWindowRendering = false;
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
            offscreenPass.pipeline = device.createGraphicsPipeline(pipelineDesc);
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

            presentPass.shader = device.createShader(EShaderLanguage::GLSL, shaderDesc);
            presentPass.info = device.createShaderMeta(presentPass.shader);
            {
                auto& params = presentPass.info->getParams();
                for (auto& p: params) {
                    printf("Field: (name='%s',location=%u)\n", p.getName().data(), p.getLocation());
                }

                auto& blocks = presentPass.info->getUniformBlocks();
                for (auto& b: blocks) {
                    printf("Block: (name='%s',binding=%u,size=%u)\n", b.getName().data(), b.getBinding(), b.getSize());

                    for (auto& m: b.getMembers()) {
                        printf(" Member: (name='%s',offset=%u,size=%u,elements=%u,stride=%u,matrixStride=%u,rowMajor=%u,type=%s)\n",
                               m.getName().data(), m.getOffset(), m.getSize(), m.getElements(),
                               m.getStride(), m.getMatrixStride(), m.getIsRowMajor(),
                               RHIShaderMetaData::getShaderDataName(m.getBaseType()));                    }
                }
            }

            RHISamplerDesc samplerDesc{};
            samplerDesc.useMips = false;
            samplerDesc.mag = ESamplerFilter::Nearest;
            samplerDesc.u = ESamplerRepeatMode::ClampToBorder;
            samplerDesc.v = ESamplerRepeatMode::ClampToBorder;
            samplerDesc.w = ESamplerRepeatMode::ClampToBorder;
            samplerDesc.color = ESamplerBorderColor::White;
            presentPass.sampler = device.createSampler(samplerDesc);

            TArray<RHIUniformTextureDesc> uniformTextures;
            {
                auto& desc = uniformTextures.emplace();
                desc.texture = offscreenPass.color0;
                desc.sampler = presentPass.sampler;
                desc.location = presentPass.info->getParams()[0].getLocation();
            }
            presentPass.uniformSet = device.createUniformSet(uniformTextures, TArray<RHIUniformBlockDesc>());

            float screen[] = {
                -1.0f,  1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f,
                 1.0f, -1.0f, 1.0f, 0.0f,
                 1.0f, -1.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, 1.0f, 1.0f,
                -1.0f,  1.0f, 0.0f, 1.0f
            };

            auto vertexBuffer = device.createVertexBuffer(sizeof(screen), EMemoryType::Dynamic, nullptr);
            vertexBuffer->update(sizeof(screen), 0, (uint8*) screen);

            RHIVertexDeclarationDesc vertexDeclarationDesc;
            vertexDeclarationDesc.resize(2);
            vertexDeclarationDesc[0].buffer = 0;
            vertexDeclarationDesc[0].iterating = EVertexIterating::PerVertex;
            vertexDeclarationDesc[0].location = 0;
            vertexDeclarationDesc[0].offset = 0;
            vertexDeclarationDesc[0].stride = 4 * sizeof(float);
            vertexDeclarationDesc[0].type = EVertexElementType::Float2;
            vertexDeclarationDesc[1].buffer = 0;
            vertexDeclarationDesc[1].iterating = EVertexIterating::PerVertex;
            vertexDeclarationDesc[1].location = 1;
            vertexDeclarationDesc[1].offset = 2 * sizeof(float);
            vertexDeclarationDesc[1].stride = 4 * sizeof(float);
            vertexDeclarationDesc[1].type = EVertexElementType::Float2;

            auto vertexDeclaration = device.createVertexDeclaration(vertexDeclarationDesc);

            presentPass.object = device.createArrayObject({vertexBuffer}, nullptr, vertexDeclaration);

            RHIGraphicsPipelineDesc pipelineDesc{};
            pipelineDesc.forWindowRendering = true;
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

            presentPass.pipeline = device.createGraphicsPipeline(pipelineDesc);
        }

        auto list = device.createDrawList();
        {
            list->begin();
            list->bindFramebuffer(offscreenPass.framebuffer, Region2i(0,0,framebufferWidth,framebufferHeight));
            list->bindPipeline(offscreenPass.pipeline);
            list->bindArrayObject(offscreenPass.object);
            list->bindUniformSet(offscreenPass.uniformSet);
            list->drawIndexedInstanced(EIndexType::Uint32, 36, 4);
            list->bindWindow(window, Region2i(0,0,width,height), background);
            list->bindPipeline(presentPass.pipeline);
            list->bindArrayObject(presentPass.object);
            list->bindUniformSet(presentPass.uniformSet);
            list->draw(6);
            list->end();
        }

        auto v = Vec3f(Mat4x4f::rotateY(Math::degToRad(90.0f)) * Vec4f(0,0,1,1));

        auto t = TimeValue::now();
        auto d = TimeValue().setSeconds(1.0f / 60.f);
        auto frames = 0llu;
        auto first = t;

        while (!window->shouldClose()) {
            System::getSingleton().update();

            {
                device.beginRenderFrame();
                device.submitDrawList(list);
                device.endRenderFrame();
            }

            frames += 1;
            t = TimeValue::wait(t + d);

            {
                static Vec3f pos = Vec3f(0,0,1);
                static float angle = 0.0f;
                static float step = 0.005f;

                auto p = Vec3f(Mat4x4f::rotateY(angle) * Vec4f(pos, 1.0f));
                View = Mat4x4f::lookAt(p, -p, Vec3f::Y_AXIS);

                const auto& block = *offscreenPass.info->getUniformBlock("Transform");
                const auto& pProj = *block.getMember("Proj");
                const auto& pView = *block.getMember("View");
                auto tProj = Proj.transpose();
                auto tView = View.transpose();
                offscreenPass.uniformBuffer->update(pProj.getSize(), pProj.getOffset(), (uint8*) tProj.data());
                offscreenPass.uniformBuffer->update(pView.getSize(), pView.getOffset(), (uint8*) tView.data());

                angle += step;
            }
        }

        window->requestClose();

        auto averageFrame = (t - first).getSeconds() / (double)frames;
        auto averageFPS = 1.0f / averageFrame;
        System::getSingleton().getLog().logf(ELogVerbosity::Info, "Average FPS %f: Frames: %llu", averageFPS, frames);
        System::getSingleton().finalize();
        ErrorMacro::releaseAllErrors();
    };
}
