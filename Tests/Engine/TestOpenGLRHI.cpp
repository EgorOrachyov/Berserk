/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>
#include <GLDevice.h>
#include <GLShader.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestOpenGLRHI)
{
    BERSERK_TEST(TestDeviceGL)
    {
        ISystem::VideoMode videoMode{};
        ISystem::initialize("Test OpenGL Device", videoMode, ERenderDeviceType::OpenGL);

        float32 vertices[] = {
            -0.2f,  0.2f, 0.0f, 1.0f, 0.0, 0.0,
            -0.2f, -0.2f, 0.0f, 0.0f, 1.0, 0.0,
             0.2f, -0.2f, 0.0f, 0.0f, 0.0, 1.0,
             0.2f,  0.2f, 0.0f, 1.0f, 1.0, 1.0,
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
                                "out vec3 fColor;"
                                "void main() {"
                                "  fColor = vColor;"
                                "  gl_Position = vec4(vec3(vPosInstance, 0.0f) + vPosition, 1.0f);"
                                "}";
        char fragmentShader[] = "#version 410 core\n"
                                "layout (location = 0) out vec4 outColor;"
                                "in vec3 fColor;"
                                "void main() {"
                                "  outColor = vec4(fColor, 1.0f);"
                                "}";

        auto device = TPtrShared<GLDevice>::make();
        auto vertexBuffer = device->createVertexBuffer(sizeof(vertices), EMemoryType::Dynamic);
        auto positionBuffer = device->createVertexBuffer(sizeof(positions), EMemoryType::Dynamic);
        auto indexBuffer = device->createIndexBuffer(sizeof(indices), EMemoryType::Dynamic);

        device->setWindowBindFunction(ISystem::getSingleton().getWindowBindFunction());
        vertexBuffer->update(sizeof(vertices), 0, (uint8*) vertices);
        positionBuffer->update(sizeof(positions), 0, (uint8*) positions);
        indexBuffer->update(sizeof(indices), 0, (uint8*) indices);

        RHIShaderDesc shaderDesc;
        shaderDesc.resize(2);
        shaderDesc[0].type = EShaderType::Vertex;
        shaderDesc[0].code.add((uint8*) vertexShader, sizeof(vertexShader));
        shaderDesc[1].type = EShaderType::Fragment;
        shaderDesc[1].code.add((uint8*) fragmentShader, sizeof(fragmentShader));

        auto shader = device->createShader(EShaderLanguage::GLSL, shaderDesc);

        RHIVertexDeclarationDesc vertDeclDesc;
        vertDeclDesc.resize(3);
        vertDeclDesc[0].buffer = 0;
        vertDeclDesc[0].iterating = EVertexIterating::PerVertex;
        vertDeclDesc[0].stride = 6 * sizeof(float32);
        vertDeclDesc[0].location = 0;
        vertDeclDesc[0].offset = 0;
        vertDeclDesc[0].type = EVertexElementType::Float3;
        vertDeclDesc[1].buffer = 0;
        vertDeclDesc[1].iterating = EVertexIterating::PerVertex;
        vertDeclDesc[1].stride = 6 * sizeof(float32);
        vertDeclDesc[1].location = 1;
        vertDeclDesc[1].offset = 3 * sizeof(float32);
        vertDeclDesc[1].type = EVertexElementType::Float3;
        vertDeclDesc[2].buffer = 1;
        vertDeclDesc[2].iterating = EVertexIterating::PerInstance;
        vertDeclDesc[2].stride = 2 * sizeof(float32);
        vertDeclDesc[2].location = 2;
        vertDeclDesc[2].offset = 0;
        vertDeclDesc[2].type = EVertexElementType::Float2;

        auto declaration = device->createVertexDeclaration(vertDeclDesc);
        auto array = device->createArrayObject({ vertexBuffer, positionBuffer }, indexBuffer, declaration);

        auto list = device->createDrawList();
        list->begin();
        list->bindWindow(ISystem::MAIN_WINDOW, Region2i(0,0,640,480), Color4f(0.0f,0.0f,0.0f));
        list->bindArrayObject(array);
        list->drawIndexedInstances(EIndexType::Uint32, 6, 4);
        list->end();

        auto t = TimeValue::now();
        auto d = TimeValue().setMilliseconds(30.0f);

        while (!ISystem::getSingleton().shouldClose(ISystem::MAIN_WINDOW)) {
            ISystem::update();

            // todo: remove
            {
                ((GLShader&)*shader).use();
                device->submitDrawList(list);
            }

            t = TimeValue::wait(t + d);
        }

        ISystem::finalize();
    };
}
