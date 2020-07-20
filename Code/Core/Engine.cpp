/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Engine.h>
#include <IO/Ini.h>
#include <LogMacro.h>
#include <Platform/System.h>
#include <BuildOptions.h>

namespace Berserk {

    Engine* Engine::gEngine = nullptr;

    Engine::Engine() {
        if (gEngine) {
            fprintf(stderr, "[BERSERK Core] Only single Engine could be set as singleton");
            return;
        }

        gEngine = this;
        mFramesCount = 0;
        mFrameTimeStep = 0.0f;
        mFrameTimeScale = 1.0f;
        mFrameTimeDelta = 0.0f;
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

    void Engine::initialize(const CString &engineDirectory, bool editor) {
        mEngineDirectory = engineDirectory;
        mApplicationName = "Berserk Engine";
        mIsEditor = editor;

        CString primaryWindowName = "Berserk Engine Window";

        int32 width = 640;
        int32 height = 480;
        bool forceVSync = false;

#ifdef BERSERK_WITH_OPENGL
        // Currently Rendering device defined in compile time
        ERenderDeviceType renderDeviceType = ERenderDeviceType::OpenGL;
#endif

        CString configPath = mEngineDirectory + "Config/Engine.ini";
        auto configFile = System::getSingleton().openFile(configPath, EFileMode::Read);

        if (configFile.isNotNull() && configFile->isOpen()) {
            BERSERK_LOG_INFO("Configure engine from INI '%s' file", configPath.data());

            Ini config = *configFile;

            if (config.isParsed()) {
                auto general = config.getSection("General");
                if (general.isNotNull()) {

                    auto cfgApplicationName = config.getValue(*general, "ApplicationName");
                    if (cfgApplicationName.isNotNull()) mApplicationName = *cfgApplicationName;

                    auto cfqPrimaryWindowName = config.getValue(*general, "PrimaryWindowName");
                    if (cfqPrimaryWindowName.isNotNull()) primaryWindowName = *cfqPrimaryWindowName;

                    auto cfgPrimaryWindowWidth = config.getValue(*general, "PrimaryWindowWidth");
                    if (cfgPrimaryWindowWidth.isNotNull()) width = cfgPrimaryWindowWidth->toUint32();

                    auto cfgPrimaryWindowHeight = config.getValue(*general, "PrimaryWindowHeight");
                    if (cfgPrimaryWindowHeight.isNotNull()) height = cfgPrimaryWindowHeight->toUint32();

                    auto cfgPrimaryWindowVsync = config.getValue(*general, "PrimaryWindowVsync");
                    if (cfgPrimaryWindowVsync.isNotNull()) forceVSync = cfgPrimaryWindowVsync->toBool();
                }
            }
        }

        // Engine console vars config (must be accessible for other modules)
        initializeConsoleVariables();

        // OS system window setup and input system
        System::getSingleton().initialize("MAIN_WINDOW", primaryWindowName, {width,height}, forceVSync, renderDeviceType);

        {
            TGuard<TArray<Module*>> guard(mModules);
            for (auto module: guard.get()) {
                module->onPostInitialize();
            }
        }
    }

    void Engine::update() {
        // Get current time, elapsed and in-game elapsed time with scale
        // Also wait for some time in order to sync to desired frame rate
        // (Currently simple while loop in order to wait)
        auto time = TimeValue::wait(mCurrentTime + mTargetFrameStep);
        auto elapsed = time - mCurrentTime;
        mCurrentTime = time;
        mFrameTimeStep = elapsed.getSeconds();
        mFrameTimeStep = (mFrameTimeStep > mMaxFrameTimeStep? mMaxFrameTimeStep : mFrameTimeStep);
        mFrameTimeDelta = mFrameTimeStep * mFrameTimeScale;
        mFPS = 1.0f / mFrameTimeStep;
        mFramesCount += 1;
        mExecutionTime += (double) mFrameTimeStep;
        mInGameTime += (double) mFrameTimeDelta;

        // The system is updated prior any other engine module
        // Since modules must have fresh input and window info
        System::getSingleton().update();

        // Update changes of the engine console vars
        updateConsoleVariables();

        {
            TGuard<TArray<Module*>> guard(mModules);
            for (auto module: guard.get()) {
                module->onPreUpdate();
            }
        }

        // todo: logic and scene update ?

        {
            TGuard<TArray<Module*>> guard(mModules);
            for (auto module: guard.get()) {
                module->onPostUpdate();
            }
        }
    }

    void Engine::finalize() {
        TGuard<TArray<Module*>> guard(mModules);
        for (auto module: guard.get()) {
            module->onPostFinalize();
        }

        System::getSingleton().finalize();
    }

    void Engine::registerModule(Module *module) {
        BERSERK_COND_ERROR_RET(module, "Module must be valid");

        TGuard<TArray<Module*>> guard(mModules);
        guard->add(module);
    }

    void Engine::unregisterModule(Module *module) {
        BERSERK_COND_ERROR_RET(module, "Module must be valid");

        TGuard<TArray<Module*>> guard(mModules);
        guard->removeElement(module);
    }

    TRef<Module> Engine::getModule(const char *name) {
        TGuard<TArray<Module*>> guard(mModules);
        for (auto module: guard.get()) {
            if (CStringUtility::compare(module->getModuleName(),name) == 0)
                return module;
        }

        return nullptr;
    }

    void Engine::initializeConsoleVariables() {
        mCVarMinFps = AutoConsoleVarInt(
            "e.MinFps",
            20,
            "Minimum frame rate, which means that max update time always <= 1.0f/MinFps.\n"
            "- 20 (default min value)",
            { EConsoleFlag::MainThread }
        );

        mCVarTargetFps = AutoConsoleVarInt(
            "e.Fps",
            30,
            "Desired frame rate of the application in frames per second.\n"
            "- 30 (about 33ms to process frame)\n"
            "- 60 (about 16ms to process frame)",
            { EConsoleFlag::MainThread }
        );
    }

    void Engine::updateConsoleVariables() {
        auto fps = mCVarTargetFps.get();
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