/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Engine.h>
#include <RenderModule.h>
#include <VertexArrayData.h>
#include <RHI/RHIDevice.h>
#include <String/CStringBuilder.h>
#include <Console/ConsoleManager.h>
#include <ShaderCore/ShaderFile.h>
#include <ShaderCore/ShaderProgramCompiler.h>
#include <ShaderCore/ShaderProgramCache.h>
#include <RenderResources/GraphicsPipeline.h>
#include <RenderResources/GraphicsPipelineBuilder.h>
#include <RenderResources/VertexDeclaration.h>
#include <RenderResources/VertexDeclarationBuilder.h>
#include <RenderResources/VertexArray.h>
#include <RenderResources/VertexArrayBuilder.h>
#include <RenderResources/VertexArrayUpdate.h>
#include <RenderTargets/WindowTarget.h>
#include <Platform/WindowManager.h>
#include <Main.h>

using namespace Berserk;
using namespace Render;

BERSERK_TEST_SECTION(TestRenderCore)
{
    BERSERK_TEST_COND(ShaderFile, false)
    {
        auto printStat = [](ShaderFile& shaderFile){
            if (shaderFile.isFileParsed()) {
                printf("Info about %s:\n", shaderFile.getShaderName().data());
                printf("Version: %s\n", shaderFile.getStringVersion().data());
                printf("Author: %s\n", shaderFile.getAuthorName().data());
                printf("Description: %s\n", shaderFile.getShaderDescription().data());
                printf("Type: %s\n", ShaderFile::getShaderFileTypeStringFromEnum(shaderFile.getShaderType()));

                static CString OpenGL = "OpenGL";

                if (shaderFile.supportsDevice(OpenGL)) {
                    printf("Supports OpenGL\n");

                    auto types = shaderFile.getShaderTypesForDevice(OpenGL);
                    auto names = shaderFile.getShaderNamesForDevice(OpenGL);

                    for (uint32 i = 0; i < types.size(); i++) {
                        printf("Shader %s [type: %s]\n", names[i].data(), RHIDefinitionsUtil::getShaderTypeStringFromEnum(types[i]));
                    }
                }
            }
        };

        ShaderFile file1("/Shaders/TestShader.json", EPathType::Engine);
        printStat(file1);

        ShaderFile file2("/Shaders/TestDependency.json", EPathType::Engine);
        printStat(file2);
    };

    BERSERK_TEST_COND(ShaderProgramCompiler, false)
    {
        Main main;
        main.initialize(0, nullptr);

        ShaderProgramCompiler compiler("Engine/Shaders/TestShader.json", EPathType::Root);

        if (compiler.canCompile()) {

            compiler.compile();

            if (!compiler.isCompiled())
                printf("Compiler message: %s\n", compiler.getInfoMessage().data());

            if (!compiler.canCreateProgram())
                printf("Unable to create program\n");

            auto program = compiler.createProgram();

            if (program.isNull())
                printf("Program is not created\n");
        }

        main.finalize();
    };

    BERSERK_TEST_COND(ShaderProgramCache, false)
    {
        Main main;
        main.initialize(0, nullptr);

        auto& cache = ShaderProgramCache::getSingleton();
        auto program = cache.load("Engine/Shaders/TestShader.json", EPathType::Root);

        if (program.isNull())
            printf("Program is not created\n");

        program->getMetaData()->showDebugInfo();
        cache.showDebugInfo();

        main.finalize();
    };

    BERSERK_TEST_COND(GraphicsPipeline, false)
    {
        Main main;
        main.initialize(0, nullptr);

        auto& cache = ShaderProgramCache::getSingleton();
        auto& sys = System::getSingleton();
        auto& winMan = WindowManager::getSingleton();
        auto window = winMan.find("MAIN_WINDOW");

        WindowTarget target(window);
        auto program = cache.load("Engine/Shaders/TestShader.json", EPathType::Root);

        GraphicsPipelineBuilder builder;

        auto pipeline = builder.setTarget(target)
                               .setShader(program)
                               .primitivesType(EPrimitivesType::Triangles)
                               .depthTest(false)
                               .depthWrite(false)
                               .stencilTest(false)
                               .blend(false)
                               .buildShared();

        if (pipeline.isNotNull())
            printf("Pipeline created!\n");


        main.enterMainLoop();
        main.finalize();
    };

    BERSERK_TEST_COND(VertexDeclaration, false)
    {
        Main main;
        main.initialize(0, nullptr);

        auto& cache = ShaderProgramCache::getSingleton();
        auto& sys = System::getSingleton();
        auto& winMan = WindowManager::getSingleton();
        auto window = winMan.find("MAIN_WINDOW");

        VertexDeclarationBuilder builder;

        auto declaration = builder.setName("Pos.Norm.Tang.Bitang.TexCoord|Color")
                                  .addBuffer("Pos.Norm.Tang.Bitang.TexCoord")
                                   .addElement("Pos", EVertexElementType::Float3)
                                   .addElement("Norm", EVertexElementType::Float3)
                                   .addElement("Tang", EVertexElementType::Float3)
                                   .addElement("Bitang", EVertexElementType::Float3)
                                   .addElement("TexCoord", EVertexElementType::Float2)
                                  .addBuffer("Color", EVertexIterating::PerInstance)
                                   .addElement("Color", EVertexElementType::Float3)
                                  .buildShared();

        declaration->showDebugInfo();

        main.enterMainLoop();
        main.finalize();
    };

    BERSERK_TEST_COND(VertexArray, true)
    {
        Main main;
        main.initialize(0, nullptr);

        auto& system = System::getSingleton();
        auto& device = RHIDevice::getSingleton();

        auto& winMan = WindowManager::getSingleton();
        auto  window = winMan.find("MAIN_WINDOW");
        WindowTarget target(window);

        auto& cache = ShaderProgramCache::getSingleton();
        auto program = cache.load("Engine/Shaders/TestShader.json", EPathType::Root);

        VertexDeclarationBuilder declarationBuilder;
        auto declaration = declarationBuilder
                .setName("vsPosition.vsColor")
                .addBuffer("vsPosition.vsColor")
                .addElement("vsPosition", EVertexElementType::Float3)
                .addElement("vsColor", EVertexElementType::Float2)
                .buildShared();

        GraphicsPipelineBuilder pipelineBuilder;
        auto pipeline = pipelineBuilder
                .setTarget(target)
                .setShader(program)
                .setDeclaration(declaration)
                .primitivesType(EPrimitivesType::Triangles)
                .polygonFrontFace(EPolygonFrontFace::CounterClockwise)
                .depthTest(false)
                .depthWrite(false)
                .stencilTest(false)
                .blend(false)
                .buildShared();

        VertexArrayData data;
        data.setDeclaration(declaration).useIndices(EIndexType::Uint32);

        auto position = data.getStreamFor("vsPosition");
        auto color = data.getStreamFor("vsColor");
        auto indices = data.getIndexStream();

        position << Vec3f(-0.8,0.8,0.0)
                 << Vec3f(-0.8,-0.8,0.0)
                 << Vec3f(0.8,-0.8,0.0)
                 << Vec3f(0.8,0.8,0.0);

        color    << Vec2f(0.0,1.0)
                 << Vec2f(0.0,0.0)
                 << Vec2f(1.0,0.0)
                 << Vec2f(1.0,1.0);

        indices  << 0u << 1u << 2u
                 << 2u << 3u << 0u;

        data.evaluate();

        VertexArrayBuilder arrayBuilder;
        auto array = arrayBuilder
                .setName("ScreenQuad")
                .configureFromData(data)
                .allocateBuffers()
                .setDataFrom(data)
                .buildShared();

        auto drawList = device.createDrawList();
        {
            drawList->begin();
            target.bind(*drawList);
            pipeline->bind(*drawList);
            array->draw(*drawList);
            drawList->end();
        }

        while (!system.isCloseRequested()) {
            main.execSingleIteration();

            device.beginRenderFrame();
            device.submitDrawList(drawList);
            device.endRenderFrame();
        }

        main.finalize();
    };

}