/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Debug/ConsoleCommands.h>
#include <RenderModule.h>
#include <LogMacro.h>
#include <Engine.h>
#include <Scene/Scene.h>
#include <BatchedElements.h>

namespace Berserk {

    ConsoleCommands::ConsoleCommands() {
        mDebugStat = AutoCommand(
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

        mDebugLogInfo = AutoCommand(
                "d.log.info",
                [](const TArray<CString> &args, OutputDevice& device){
                    if (args.size() == 2) {
                        BERSERK_LOG_INFO("%s", args[1].data());
                    }
                },
                "Log info message to the System logger",
                { EConsoleFlag::MainThread }
        );

        mDebugLogWarning = AutoCommand(
                "d.log.warning",
                [](const TArray<CString> &args, OutputDevice& device){
                    if (args.size() == 2) {
                        BERSERK_LOG_WARNING("%s", args[1].data());
                    }
                },
                "Log warning message to the System logger",
                { EConsoleFlag::MainThread }
        );

        mDebugLogError = AutoCommand(
                "d.log.error",
                [](const TArray<CString> &args, OutputDevice& device){
                    if (args.size() == 2) {
                        BERSERK_LOG_ERROR("%s", args[1].data());
                    }
                },
                "Log error message to the System logger",
                { EConsoleFlag::MainThread }
        );

        mDebugClearBatch = AutoCommand(
                "d.batch.clear",
                [](const TArray<CString> &args, OutputDevice& device){
                    Render::BatchedElements& batch = *Render::RenderModule::getSingleton().getPrimaryScene()->mBatch;
                    batch.clear();
                },
                "Clear drawing batch",
                { EConsoleFlag::MainThread }
        );

        mDebugDrawBox = AutoCommand(
                "d.batch.drawBox",
                [](const TArray<CString> &args, OutputDevice& device){
                    if (args.size() == 8) {
                        Render::BatchedElements& batch = *Render::RenderModule::getSingleton().getPrimaryScene()->mBatch;

                        float x = args[1].toFloat();
                        float y = args[2].toFloat();
                        float z = args[3].toFloat();
                        float s = args[4].toFloat();
                        float r = args[5].toFloat();
                        float g = args[6].toFloat();
                        float b = args[7].toFloat();

                        batch.addBox({x,y,z}, {s,s,s}, Color4f(r,g,b), Quatf());
                    }
                },
                "Draw box with specified position, size and color",
                { EConsoleFlag::MainThread }
        );

        mDebugDrawSphere = AutoCommand(
                "d.batch.drawSphere",
                [](const TArray<CString> &args, OutputDevice& device){
                    if (args.size() == 8) {
                        Render::BatchedElements& batch = *Render::RenderModule::getSingleton().getPrimaryScene()->mBatch;

                        float x = args[1].toFloat();
                        float y = args[2].toFloat();
                        float z = args[3].toFloat();
                        float s = args[4].toFloat();
                        float r = args[5].toFloat();
                        float g = args[6].toFloat();
                        float b = args[7].toFloat();

                        batch.addSphere({x,y,z}, s, Color4f(r,g,b));
                    }
                },
                "Draw sphere with specified position, radius and color",
                { EConsoleFlag::MainThread }
        );

        mDebugDrawCylinder = AutoCommand(
                "d.batch.drawCylinder",
                [](const TArray<CString> &args, OutputDevice& device){
                    if (args.size() == 9) {
                        Render::BatchedElements& batch = *Render::RenderModule::getSingleton().getPrimaryScene()->mBatch;

                        float x = args[1].toFloat();
                        float y = args[2].toFloat();
                        float z = args[3].toFloat();
                        float s = args[4].toFloat();
                        float h = args[5].toFloat();
                        float r = args[6].toFloat();
                        float g = args[7].toFloat();
                        float b = args[8].toFloat();

                        batch.addCylinder({x,y,z}, {s,s}, h, Color4f(r,g,b), Quatf());
                    }
                },
                "Draw cylinder with specified position, size, height and color",
                { EConsoleFlag::MainThread }
        );
    }

    ConsoleCommands::~ConsoleCommands() {
        // No release for commands
    }

}