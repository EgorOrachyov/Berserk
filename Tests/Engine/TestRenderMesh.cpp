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

using namespace Berserk;

BERSERK_TEST_SECTION(TestRenderMesh)
{
    BERSERK_TEST_COND(RenderMesh, true)
    {
        Main main;
        main.initialize(0, nullptr);
        {
            // References to common engine singletons
            auto& system = System::getSingleton();
            auto& input = Input::getSingleton();
            auto& device = RHIDevice::getSingleton();
            auto& windowManager = WindowManager::getSingleton();
            auto& importers = ResourceImporters::getSingleton();

            // Default window, created on engine start-up
            auto window = windowManager.find("MAIN_WINDOW");

            MeshImportOptions options;
            options.setFormat({EMeshAttribute::Position,EMeshAttribute::Color,EMeshAttribute::TexCoord});
            auto path = Paths::getFullPathFor("Assets/Models/geometry.obj", EPathType::Engine);
            auto importer = importers.findImporterFromPath(path);

            if (importer.isNotNull()) {
                TPtrShared<Resource> res;
                importer->import(res, path, options);
            }

            while (!window->shouldClose()) {
                main.execSingleIteration();
            }
        }
        main.finalize();
    };
}