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
#include <Graphics.h>
#include <ConsoleRenderer.h>
#include <RenderTargets/WindowTarget.h>
#include <LogMacro.h>
#include <Input/TextInput.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestConsole)
{
    BERSERK_TEST_COND(TestConsole, true)
    {
        Main main;
        main.initialize(0, nullptr);
        main.enterMainLoop();
        main.finalize();
    };
}