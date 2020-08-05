/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <BuildOptions.h>
#include <Engine.h>
#include <Paths.h>
#include <LogMacro.h>
#include <IO/Config.h>
#include <Platform/System.h>

namespace Berserk {

    Engine* Engine::gEngine = nullptr;

    void checkAndGet(CString& var, const CString& name, IniDocument::Section& section) {
        if (section.contains(name))
            var = section[name].getString();
    }

    void checkAndGet(int32& var, const CString& name, IniDocument::Section& section) {
        if (section.contains(name))
            var = section[name].getInt();
    }

    void checkAndGet(bool& var, const CString& name, IniDocument::Section& section) {
        if (section.contains(name))
            var = section[name].getBool();
    }

    Engine::Engine() {
        if (gEngine) {
            fprintf(stderr, "[BERSERK Core] Only single Engine could be set as singleton");
            return;
        }

        gEngine = this;

        mFramesCount = 0;
        mFrameTime = 0.0f;
        mFrameTimePerformance = 0.0f;
        mFrameTimeScale = 1.0f;
        mFrameTimeStepVirtual = 0.0f;
        mFPS = 30.0f;
        mTargetFPS = 30;
        mMinFPS = 20;
        mMaxFrameTimeStep = 1.0f / (float) mMinFPS;
        mIsEditor = false;
        mCurrentTime = TimeValue::now();
        mTargetFrameStep = TimeValue::asSeconds(1.0f / (float)mTargetFPS);
        mExecutionTime = 0.0;
        mInGameTime = 0.0;
    }

    Engine::~Engine() {
        // do nothing
    }

    void Engine::initialize(bool editor) {
        mIsEditor = editor;

        auto& system = System::getSingleton();
        auto configPath = Paths::getFullPathFor("Config/Engine.ini", EPathType::Engine);
        auto file = system.openFile(configPath, EFileMode::Read);

        CString applicationName = "Berserk";
        CString primaryWindow = "MAIN_WINDOW";
        CString primaryWindowCaption = "Berserk Window";
        Size2i primaryWindowSize = { 1280, 720 };
        ERenderDeviceType renderDeviceType = ERenderDeviceType::OpenGL;
        bool vsync = false;
        bool logToFile = false;
        CString logPath = Paths::getFullPathFor("Logs", EPathType::Engine);

        bool fileFound = false;
        bool configParsed = false;

        if (file.isNotNull() && file->isOpen()) {
            Config config(*file);

            if (config.isParsed()) {
                auto& data = config.getContent();

                if (data.contains("General")) {
                    auto& general = data["General"];

                    checkAndGet(applicationName, "ApplicationName", general);
                    checkAndGet(primaryWindowCaption, "PrimaryWindowCaption", general);
                    checkAndGet(primaryWindowSize[0], "PrimaryWindowWidth", general);
                    checkAndGet(primaryWindowSize[1], "PrimaryWindowHeight", general);
                    checkAndGet(vsync, "Vsync", general);
                    checkAndGet(logToFile, "LogToFile", general);
                }

                configParsed = true;
            }

            fileFound = true;
        }

        system.initialize(primaryWindow, primaryWindowCaption, primaryWindowSize, vsync, renderDeviceType, logPath, logToFile);

        // Error here after system setup
        BERSERK_COND_ERROR(fileFound, "Failed to find config file: %s", configPath.data());
        BERSERK_COND_ERROR(configParsed, "Failed to parse config file: %s", configPath.data());

        mIsInitialized = true;
    }

    void Engine::update() {
        static System& system = System::getSingleton();

        // Frame time without FPS sync
        mFrameTimePerformance = (TimeValue::now() - mCurrentTime).getSeconds();

        // Get current time, elapsed and in-game elapsed time with scale
        // Also wait for some time in order to sync to desired frame rate
        // (Currently simple while loop in order to wait)
        auto time = TimeValue::wait(mCurrentTime + mTargetFrameStep);
        auto elapsed = time - mCurrentTime;
        mCurrentTime = time;
        mFrameTime = elapsed.getSeconds();
        mFrameTime = (mFrameTime > mMaxFrameTimeStep? mMaxFrameTimeStep : mFrameTime);
        mFrameTimeStepVirtual = mFrameTime * mFrameTimeScale;
        mFPS = 1.0f / mFrameTime;
        mFramesCount += 1;
        mExecutionTime += (double) mFrameTime;
        mInGameTime += (double) mFrameTimeStepVirtual;

        // Update changes of the engine console vars
        updateConsoleVariables();

        // Update system, window api and input.
        // System must be updated prior any other action is done.
        system.update();
    }

    void Engine::finalize() {
        BERSERK_COND_ERROR_RET(mIsInitialized, "Engine must be initialized");
        BERSERK_COND_ERROR_RET(!mIsFinalized, "Engine must not be finalized");

        // Finalize platform windows and resources
        System& system = System::getSingleton();
        system.finalize();

        // Release all error, since they stay in memory by default
        ErrorMacro::releaseAllErrors();

        mIsFinalized = true;
    }

    void Engine::initializeConsoleVariables() {
        mCVarMinFps = AutoConsoleVarInt(
            "e.MinFps",
            20,
            "Minimum frame rate, which means that max update time always <= 1.0f/MinFps.\n"
            "- 20 (default min value)",
            { EConsoleFlag::MainThread }
        );

        mCVarFps = AutoConsoleVarInt(
            "e.Fps",
            30,
            "Desired frame rate of the application in frames per second.\n"
            "- 30 (about 33ms to process frame)\n"
            "- 60 (about 16ms to process frame)",
            { EConsoleFlag::MainThread }
        );
    }

    void Engine::initializeConsoleVariablesFromConfig() {
        auto path = Paths::getFullPathFor("Config/CVars.ini", EPathType::Engine);
        auto file = System::getSingleton().openFile(path, EFileMode::Read);

        if (file.isNotNull() && file->isOpen()) {
            Config cvars = *file;

            if (cvars.isParsed()) {
                auto& startup = cvars.getContent()["StartUp"];
                auto& manager = ConsoleManager::getSingleton();

                for (auto& var: startup) {
                    auto& name = var.first();
                    auto  val = var.second().toString();

                    auto ref = manager.findVariable(name);

                    if (ref.isNotNull()) {
                        ref->set(val, EConsoleMod::ByConfig);
                    }
                }
            }
        }
    }

    void Engine::updateConsoleVariables() {
        auto fps = mCVarFps.get();
        if (fps != mTargetFPS) {
            mTargetFPS = Math::max(mMinFPS, fps);
            mTargetFrameStep = TimeValue::asSeconds(1.0 / (float) mTargetFPS);
        }

        auto minFps = mCVarMinFps.get();
        if (minFps != mMinFPS) {
            mMinFPS = Math::max(1, minFps);
            mMaxFrameTimeStep = 1.0f / (float) mMinFPS;
        }
    }

    Engine& Engine::getSingleton() {
        return *gEngine;
    }

}