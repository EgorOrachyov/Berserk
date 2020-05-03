/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Engine.h>
#include <Platform/ISystem.h>
#include <Rendering/RenderModule.h>
#include <ImageImporter/ImageImporter.h>

using namespace Berserk;

BERSERK_TEST_SECTION(Engine)
{
    BERSERK_TEST_COND(EngineStartUp, true)
    {
        Engine engine;
        ImageImporter imageImporter;
        RenderModule renderModule;

        auto& system = ISystem::getSingleton();

        engine.initialize("../../../Engine/", false);

        while (!system.shouldClose(ISystem::MAIN_WINDOW)) {
            engine.update();
        }

        engine.finalize();
    };
}
