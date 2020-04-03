/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>
#include <GLDevice.h>
#include <glad/glad.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestOpenGLRHI)
{
    BERSERK_TEST(TestDeviceGL)
    {
        ISystem::VideoMode videoMode{};
        ISystem::getSingleton().initialize("Test OpenGL Device", videoMode, ERenderDeviceType::OpenGL);

        float32 vertices[] = {
            -0.5f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f
        };

        uint32 indices[] = {
            0, 1, 2,
            2, 3, 4
        };

        char vertexShader[] =   "#version 410 core\n"
                                "layout (location = 0) in vec3 vPosition;"
                                "out vec2 fScreenPos;"
                                "void main() {"
                                "  fScreenPos = vPosition.xy;"
                                "  gl_Position = vec4(vPosition, 1.0f);"
                                "}";
        char fragmentShader[] = "#version 410 core\n"
                                "layout (location = 0) out vec4 fColor;"
                                "in vec2 fScreenPos;"
                                "void main() {"
                                "  fColor = vec4(fScreenPos.xy, 0.0f, 1.0f);"
                                "}";

        auto device = (TPtrShared<RHIDevice>) TPtrShared<GLDevice>::make();
        auto vertexBuffer = device->createVertexBuffer(sizeof(vertices), EMemoryType::Dynamic);
        auto indexBuffer = device->createIndexBuffer(sizeof(indices), EMemoryType::Dynamic);

        vertexBuffer->update(sizeof(vertices), 0, (uint8*) vertices);
        indexBuffer->update(sizeof(indices), 0, (uint8*) indices);

        RHIShaderDesc shaderDesc;
        shaderDesc.resize(2);
        shaderDesc[0].type = EShaderType::Vertex;
        shaderDesc[0].code.add((uint8*) vertexShader, sizeof(vertexShader));
        shaderDesc[1].type = EShaderType::Fragment;
        shaderDesc[1].code.add((uint8*) fragmentShader, sizeof(fragmentShader));

        auto shader = device->createShader(EShaderLanguage::GLSL, shaderDesc);

        RHIVertexDeclarationDesc vertDeclDesc;
        vertDeclDesc.resize(1);
        vertDeclDesc[0].buffer = 0;
        vertDeclDesc[0].iterating = EVertexIterating::PerVertex;
        vertDeclDesc[0].stride = 3 * sizeof(float32);
        vertDeclDesc[0].location = 0;
        vertDeclDesc[0].offset = 0;
        vertDeclDesc[0].type = EVertexElementType::Float3;

        auto declaration = device->createVertexDeclaration(vertDeclDesc);

        auto t = TimeValue::now();
        auto d = TimeValue().setMilliseconds(30.0f);

        while (!ISystem::getSingleton().shouldClose(ISystem::MAIN_WINDOW)) {
            ISystem::getSingleton().update();

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            t = TimeValue::wait(t + d);
        }

        ISystem::getSingleton().finalize();
    };
}
