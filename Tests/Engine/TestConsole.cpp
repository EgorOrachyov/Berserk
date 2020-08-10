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
#include <UpdateManager.h>
#include <Console/AutoCommand.h>
#include <LogMacro.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestConsole)
{
    BERSERK_TEST_COND(TestConsole, true)
    {
        Main main;
        main.initialize(0, nullptr);
        {
            // Test command to show debug statistic
            AutoCommand frameStat(
                    "d.stat",
                    [](const TArray<CString> &args, OutputDevice& device){
                        auto& engine = Engine::getSingleton();
                        auto frameTimePerf = engine.getFrameTimePerformance() * 1000.f;
                        auto frameTime = engine.getFrameTime() * 1000.f;
                        auto fps = engine.getFPS();
                        auto alloc = Memory::getAllocCalls();
                        auto free = Memory::getFreeCalls();

                        device.printf(EOutputType::Text, "Frame time (CPU): %f\n"
                                                         "Sync time (CPU): %f (FPS: %f)\n"
                                                         "Memory: alloc: %llu free: %llu",
                                                         frameTimePerf,
                                                         frameTime, fps,
                                                         alloc, free);
                    },
                    "Show brief engine statistic",
                    { EConsoleFlag::MainThread }
            );

            AutoCommand logError(
                    "d.log.error",
                    [](const TArray<CString> &args, OutputDevice& device){
                        if (args.size() == 2) {
                            BERSERK_LOG_ERROR("%s", args[1].data());
                        }
                    },
                    "Log error message to the System logger",
                    { EConsoleFlag::MainThread }
            );

            AutoCommand logWarning(
                    "d.log.warning",
                    [](const TArray<CString> &args, OutputDevice& device){
                        if (args.size() == 2) {
                            BERSERK_LOG_WARNING("%s", args[1].data());
                        }
                    },
                    "Log warning message to the System logger",
                    { EConsoleFlag::MainThread }
            );

            AutoCommand logInfo(
                    "d.log.info",
                    [](const TArray<CString> &args, OutputDevice& device){
                        if (args.size() == 2) {
                            BERSERK_LOG_INFO("%s", args[1].data());
                        }
                    },
                    "Log info message to the System logger",
                    { EConsoleFlag::MainThread }
            );

            main.enterMainLoop();
        }
        main.finalize();
    };
}