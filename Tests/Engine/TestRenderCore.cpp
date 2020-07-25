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
#include <String/CStringBuilder.h>
#include <Console/ConsoleManagerImpl.h>
#include <ShaderCore/ShaderFile.h>
#include <ShaderCore/ShaderProgramCompiler.h>
#include <ShaderCore/ShaderProgramCache.h>
#include <RenderResources/GraphicsPipeline.h>
#include <RenderResources/GraphicsPipelineBuilder.h>
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
        Engine engine;
        ConsoleManagerImpl consoleManager;

        engine.initialize(false);

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
    };

    BERSERK_TEST_COND(ShaderProgramCache, false)
    {
        Engine engine;
        RenderModule renderModule;
        ConsoleManagerImpl consoleManager;

        engine.initialize(false);
        renderModule.onPostInitialize();

        auto& cache = ShaderProgramCache::getSingleton();
        auto program = cache.load("Engine/Shaders/TestShader.json", EPathType::Root);

        if (program.isNull())
            printf("Program is not created\n");

        cache.showEntriesInfo();
    };

    BERSERK_TEST_COND(GraphicsPipeline, true)
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

        auto pipeline = builder.forTarget(target)
                               .forShader(program)
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

}