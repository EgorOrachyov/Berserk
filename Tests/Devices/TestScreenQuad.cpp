/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Platform/System.h>
#include <Platform/WindowManager.h>
#include <RHI/RHIDevice.h>
#include <Math/Vec3f.h>
#include <Main.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestScreenQuad)
{
    BERSERK_TEST_COND(ScreenQuad, true)
    {
        // Initialize engine systems
        Main main;
        main.initialize(0, nullptr);

        // References to common engine singletons
        auto& system = System::getSingleton();
        auto& device = RHIDevice::getSingleton();
        auto& windowManager = WindowManager::getSingleton();

        // Default window, created on engine start-up
        auto window = windowManager.find("MAIN_WINDOW");

        // Geometry to be rendered
        Vec3f position[] = { Vec3f{-0.8,0.8,0.0}, Vec3f{-0.8,-0.8,0.0}, Vec3f{0.8,-0.8,0.0}, Vec3f{0.8,0.8,0.0} };
        Vec3f color[] = { Vec3f{1,0,0}, Vec3f{0,1,0}, Vec3f{0,0,1}, Vec3f{1,1,1} };
        uint32 indices[] = { 0, 1, 2, 2, 3, 0 };

        // Track window resize
        Size2i windowSize = window->getSize();

        auto positionBuffer = device.createVertexBuffer(sizeof(position), EBufferUsage::Static, position);
        auto colorBuffer = device.createVertexBuffer(sizeof(color), EBufferUsage::Static, color);
        auto indexBuffer = device.createIndexBuffer(sizeof(indices), EBufferUsage::Static, indices);

        RHIVertexDeclarationDesc vertexDeclarationDesc;
        {
            vertexDeclarationDesc.resize(2);
            vertexDeclarationDesc[0].buffer = 0;
            vertexDeclarationDesc[0].iterating = EVertexIterating::PerVertex;
            vertexDeclarationDesc[0].location = 0;
            vertexDeclarationDesc[0].offset = 0;
            vertexDeclarationDesc[0].stride = sizeof(Vec3f);
            vertexDeclarationDesc[0].type = EVertexElementType::Float3;
            vertexDeclarationDesc[1].buffer = 1;
            vertexDeclarationDesc[1].iterating = EVertexIterating::PerVertex;
            vertexDeclarationDesc[1].location = 1;
            vertexDeclarationDesc[1].offset = 0;
            vertexDeclarationDesc[1].stride = sizeof(Vec3f);
            vertexDeclarationDesc[1].type = EVertexElementType::Float3;
        }

        auto declaration = device.createVertexDeclaration(vertexDeclarationDesc);
        auto array = device.createArrayObject({ positionBuffer, colorBuffer }, indexBuffer, declaration, EPrimitivesType::Triangles);

        // Vertex shader code in GLSL
        char vertexShader[] =
                "#version 410 core\n"
                "layout (location = 0) in vec3 position;"
                "layout (location = 1) in vec3 color;"
                "out vec3 fsColor;"
                "void main() {"
                " fsColor = color;"
                " gl_Position = vec4(position, 1.0f);"
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

        RHIGraphicsPipelineState pipelineState;
        {
            pipelineState.depthTest = false;
            pipelineState.depthWrite = false;
            pipelineState.shader = shader;
            pipelineState.vertexDeclaration = declaration;
        }

        RHIWindowPassOptions windowPass;
        {
            windowPass.clearColor = Color4f{ 0.0f, 0.0f, 0.0f, 0.0f };
            windowPass.viewport = Region2i{ 0, 0, windowSize[0], windowSize[1] };
            windowPass.clearMask = { EClearOption::Color };
        }

        // Create draw list (list of draw commands) and fill it for LATER execution
        auto drawList = device.createDrawList();
        {
            drawList->begin();
            drawList->bindWindow(window, windowPass);
            drawList->bindPipeline(pipelineState);
            drawList->bindArrayObject(array);
            drawList->drawIndexed(EIndexType::Uint32, sizeof(indices) / sizeof(indices[0]));
            drawList->end();
        }

        // If user press red button on window or exit
        while (!window->shouldClose()) {
            // Process all systems and OS events update
            main.execSingleIteration();

            // Mark frame and execute draw list. Control returns only when list is executed
            device.beginRenderFrame();
            device.submitDrawList(drawList);
            device.endRenderFrame();

            // If window resized
            if (window->getSize() != windowSize) {
                windowSize = window->getSize();
                windowPass.viewport = Region2i{ 0, 0, windowSize[0], windowSize[1] };

                drawList->begin();
                drawList->bindWindow(window, windowPass);
                drawList->bindPipeline(pipelineState);
                drawList->bindArrayObject(array);
                drawList->drawIndexed(EIndexType::Uint32, sizeof(indices) / sizeof(indices[0]));
                drawList->end();
            }
        }

        main.finalize();
    };
}