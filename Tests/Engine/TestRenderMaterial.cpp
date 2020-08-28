/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Main.h>
#include <Engine.h>
#include <RHI/RHIDevice.h>
#include <Platform/Input.h>
#include <Platform/WindowManager.h>
#include <Platform/System.h>
#include <RenderTargets/WindowTarget.h>
#include <Paths.h>
#include <ResourceImporters.h>
#include <Resources/Mesh.h>
#include <Resources/MeshImportOptions.h>
#include <ShaderManager.h>
#include <Material.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestRenderMaterial)
{
    BERSERK_TEST_COND(RenderMaterial, true)
    {
        Main main;
        main.initialize(0, nullptr);
        {
            // References to common engine singletons
            auto& system = System::getSingleton();
            auto& engine = Engine::getSingleton();
            auto& input = Input::getSingleton();
            auto& device = RHIDevice::getSingleton();
            auto& shaderManager = Render::ShaderManager::getSingleton();
            auto& windowManager = WindowManager::getSingleton();
            auto& importers = ResourceImporters::getSingleton();

            // Default window, created on engine start-up
            auto window = windowManager.find("MAIN_WINDOW");

            MeshImportOptions options;
            TPtrShared<Mesh> mesh;
            options.setFormat({EMeshAttribute::Position, EMeshAttribute::Normal, EMeshAttribute::Tangent, EMeshAttribute::TexCoord });
            options.setTransform(Transformf().scale(Vec3f(8.0f)));
            auto path = Paths::getFullPathFor("Assets/Models/sphere.obj", EPathType::Engine);
            auto importer = importers.findImporterFromPath(path);

            if (importer.isNotNull()) {
                TPtrShared<Resource> resource;
                importer->import(resource, path, options);
                mesh = resource.castTo<Mesh>();
            }

            while (!window->shouldClose()) {
                main.execSingleIteration();
            }
        }
        main.finalize();
    };
}