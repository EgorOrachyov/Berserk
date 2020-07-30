/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Engine.h>
#include <Math/Random.h>
#include <RenderModule.h>
#include <RenderResources/VertexArrayData.h>
#include <GeometryGenerator.h>
#include <RHI/RHIDevice.h>
#include <String/CStringBuilder.h>
#include <Console/ConsoleManager.h>
#include <ShaderCore/ShaderFile.h>
#include <ShaderCore/ShaderProgramCache.h>
#include <ShaderCore/ShaderProgramCompiler.h>
#include <ShaderCore/ShaderBindings.h>
#include <ShaderCore/ShaderManager.h>
#include <Shaders/TestGeometryShader.h>
#include <RenderResources/GraphicsPipelineBuilder.h>
#include <RenderResources/VertexDeclaration.h>
#include <RenderResources/VertexDeclarationBuilder.h>
#include <RenderResources/VertexArray.h>
#include <RenderResources/VertexArrayBuilder.h>
#include <RenderResources/VertexArrayUpdate.h>
#include <RenderResources/Texture2D.h>
#include <RenderTargets/WindowTarget.h>
#include <Platform/WindowManager.h>
#include <Platform/Input.h>
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

        auto pipeline = builder.setShader(program)
                               .depthTest(false)
                               .depthWrite(false)
                               .stencilTest(false)
                               .blend(false)
                               .build();

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

    BERSERK_TEST_COND(VertexArray, false)
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
                .setShader(program)
                .setDeclaration(declaration)
                .polygonFrontFace(EPolygonFrontFace::CounterClockwise)
                .depthTest(false)
                .depthWrite(false)
                .stencilTest(false)
                .blend(false)
                .build();

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
                .setPrimitivesType(EPrimitivesType::Triangles)
                .configureFromData(data)
                .allocateBuffers()
                .setDataFrom(data)
                .buildShared();

        auto drawList = device.createDrawList();
        {
            drawList->begin();
            target.bind(*drawList);
            drawList->bindPipeline(pipeline);
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

    BERSERK_TEST_COND(GeometryGenerator, true)
    {
        Main main;
        main.initialize(0, nullptr);

        auto& system = System::getSingleton();
        auto& input = Input::getSingleton();
        auto& device = RHIDevice::getSingleton();
        auto& winMan = WindowManager::getSingleton();
        auto& importers = ResourceImporters::getSingleton();
        auto& shaderMan = ShaderManager::getSingleton();

        auto  window = winMan.find("MAIN_WINDOW");
        WindowTarget target(window);

        auto shader = shaderMan.load("TestGeometry");
        auto declaration = shader->getDeclaration();
        auto program = shader->getProgram();

        VertexArrayData data;
        data.setDeclaration(declaration).useIndices(EIndexType::Uint32);

        auto position = data.getStreamFor("vsPosition");
        auto texcoords = data.getStreamFor("vsTexCoords");
        auto indices = data.getIndexStream();

        GeometryGenerator::generateSphere(1.0f, 100, 100, position, texcoords, indices);

        auto instancesP2 = 10;
        auto instances = instancesP2 * instancesP2;
        auto colors = data.getStreamFor("vsInstanceColor");
        auto positions = data.getStreamFor("vsInstancePosition");
        auto radius = data.getStreamFor("vsInstanceRadius");
        colors.ensureToAdd(instances);
        positions.ensureToAdd(instances);
        radius.ensureToAdd(instances);

        Random random;

        for (uint32 i = 0; i < instancesP2; i++) {
            for (uint32 j = 0; j < instancesP2; j++) {
                colors << Vec3f(random.from(0.5f, 1.0f),random.from(0.5f, 1.0f),random.from(0.5f, 1.0f));
                positions << Vec3f(random.from(-0.5f, 0.5f) + (float)i, random.from(-0.5f, 0.5f), random.from(-0.5f, 0.5f) + (float)j);
                radius << random.from(0.2f, 0.5f);
            }
        }

        data.evaluate();

        VertexArrayBuilder arrayBuilder;
        auto array = arrayBuilder
                .setName("ScreenSpheres")
                .configureFromData(data)
                .allocateBuffers()
                .setDataFrom(data)
                .buildShared();

        ShaderBindings bindings(program->getMetaData());
        bindings.associateUniformBuffers();

        auto pProj = bindings.findParam("Transform", "Proj");
        auto pView = bindings.findParam("Transform", "View");
        auto pTextureColor = bindings.findTexture2D("fsTextureColor");

        Image whiteImage;
        whiteImage.create(1,1,EPixelFormat::R8G8B8A8,Color4f(0.5f));
        auto whiteTexture = TPtrShared<Texture2D>::make("ColorTexture", whiteImage);
        bindings.setTexture2D(pTextureColor, whiteTexture);

        bindings.updateSetGPU();

        GraphicsPipelineBuilder pipelineBuilder;
        auto pipeline = pipelineBuilder
                .setShader(program)
                .setDeclaration(declaration)
                .polygonFrontFace(EPolygonFrontFace::CounterClockwise)
                .polygonCullMode(EPolygonCullMode::Back)
                .polygonMode(EPolygonMode::Fill)
                .depthTest(true)
                .depthWrite(true)
                .build();

        auto drawList = device.createDrawList();
        auto updateList = [&]()
        {
            drawList->begin();
            target.bind(*drawList);
            drawList->bindPipeline(pipeline);
            bindings.bind(*drawList);
            array->draw(*drawList);
            drawList->end();
        };

        while (!system.isCloseRequested()) {
            main.execSingleIteration();

            static Region2i area;
            static Vec3f pos = Vec3f(instancesP2 / 2, 0, instancesP2 / 2);
            static Vec3f dirZ = Vec3f(0,0,-0.05);
            static Vec3f dirL = Vec3f(0,0,-0.05);
            static Vec3f dirX = Vec3f(0.05,0,0);
            static Vec3f dirY = Vec3f(0.0,0.5,0);
            static float rotationSpeed = 1.0f / 60.0f;
            static float angle = 0.0f;
            static float step = 0.005f;

            area = Region2i(0, 0, window->getSize());

            auto p = pos;

            auto View = Mat4x4f::lookAt(p, dirL, Vec3f::Y_AXIS);
            auto Proj = Mat4x4f::perspective(Math::degToRad(100.0f), (float)area.getW() / (float)area.getH(), 0.01, 200.0f);

            angle += step;

            bindings.setMat4(pView, View);
            bindings.setMat4(pProj, Proj);
            bindings.updateGPU();
            updateList();

            if (input.isKeyPressed(EKeyboardKey::Escape))
                window->explicitClose();
            if (input.isKeyRepeated(EKeyboardKey::W))
                pos += dirZ;
            if (input.isKeyRepeated(EKeyboardKey::S))
                pos -= dirZ;
            if (input.isKeyRepeated(EKeyboardKey::D))
                pos += dirX;
            if (input.isKeyRepeated(EKeyboardKey::A))
                pos -= dirX;
            if (input.isKeyRepeated(EKeyboardKey::R))
                pos += dirY;
            if (input.isKeyRepeated(EKeyboardKey::F))
                pos -= dirY;
            if (input.isKeyRepeated(EKeyboardKey::Up)) {
                dirL = Vec3f(Mat4x4f::rotateX(rotationSpeed) * Vec4f(dirL, 0.0f));
            }
            if (input.isKeyRepeated(EKeyboardKey::Down)) {
                dirL = Vec3f(Mat4x4f::rotateX(-rotationSpeed) * Vec4f(dirL, 0.0f));
            }
            if (input.isKeyRepeated(EKeyboardKey::Left)) {
                dirZ = Vec3f(Mat4x4f::rotateY(rotationSpeed) * Vec4f(dirZ, 0.0f));
                dirL = Vec3f(Mat4x4f::rotateY(rotationSpeed) * Vec4f(dirL, 0.0f));
                dirX = Vec3f(Mat4x4f::rotateY(rotationSpeed) * Vec4f(dirX, 0.0f));
            }
            if (input.isKeyRepeated(EKeyboardKey::Right)) {
                dirZ = Vec3f(Mat4x4f::rotateY(-rotationSpeed) * Vec4f(dirZ, 0.0f));
                dirL = Vec3f(Mat4x4f::rotateY(-rotationSpeed) * Vec4f(dirL, 0.0f));
                dirX = Vec3f(Mat4x4f::rotateY(-rotationSpeed) * Vec4f(dirX, 0.0f));
            }

            device.beginRenderFrame();
            device.submitDrawList(drawList);
            device.endRenderFrame();

            target.update();

            {
                // Update input: check drop texture
                if (input.hasDropInput()) {
                    TArray<CString> names;
                    input.getDropInput(names);
                    auto& path = names[0];

                    auto importer = importers.findImporterFromPath(path);
                    if (importer.isNotNull()) {
                        TPtrShared<Resource> image;
                        auto options = TPtrShared<ImageImportOptions>::make();
                        options->setFromSRGB(true);
                        auto result = importer->import(image, path, (TPtrShared<ResourceImportOptions>) options);

                        if (result == EError::OK) {
                            auto& imageData = (Image&) *image;
                            auto imageTexture = TPtrShared<Texture2D>::make("ColorTexture", imageData);
                            bindings.setTexture2D(pTextureColor, imageTexture);
                        }
                    }
                }
            }
        }

        main.finalize();
    };

}