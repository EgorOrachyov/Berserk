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
#include <Platform/ISystem.h>
#include <BuildOptions.h>

namespace Berserk {

    static Engine* gEngineInstance;

    Engine::Engine() {
        BERSERK_COND_ERROR_RET(gEngineInstance == nullptr, "Engine instance must be unique");
        gEngineInstance = this;
        mFramesCount = 0;
        mFrameTimeStep = 0.0f;
        mFrameTimeScale = 1.0f;
        mFrameTimeDelta = 0.0f;
        mFPS = 30.0f;
        mTargetFPS = 30.0f;
        mIsEditor = false;
        mAbortGpuError = true;
        mCurrentTime = TimeValue::now();
        mTargetFrameStep = TimeValue::asSeconds(1.0f / mTargetFPS);
        mExecutionTime = 0.0;
        mInGameTime = 0.0;
    }

    void Engine::initialize(const CString &engineDirectory, bool editor) {
        mEngineDirectory = engineDirectory;
        mApplicationName = "Berserk Engine";
        mIsEditor = editor;

        CString primaryWindowName = "Berserk Engine Window";

        ISystem::VideoMode videoMode;
        videoMode.forceVSync = false;
        videoMode.width = 1920;
        videoMode.height = 1280;
        videoMode.maximised = false;
        videoMode.resizeable = false;

#ifdef BERSERK_WITH_OPENGL
        // Currently Rendering device defined in compile time
        ERenderDeviceType renderDeviceType = ERenderDeviceType::OpenGL;
#endif

        CString configPath = mEngineDirectory + "Config/Engine.ini";
        auto configFile = ISystem::getSingleton().openFile(configPath, EFileMode::Read);

        if (configFile.isNotNull() && configFile->isOpen()) {
            BERSERK_LOG_INFO("Configure engine from INI Config/Engine.ini file");

            Ini config = *configFile;

            if (config.isParsed()) {
                auto general = config.getSection("General");
                if (general.isNotNull()) {

                    auto cfgApplicationName = config.getValue(*general, "ApplicationName");
                    if (cfgApplicationName.isNotNull()) mApplicationName = *cfgApplicationName;

                    auto cfqPrimaryWindowName = config.getValue(*general, "PrimaryWindowName");
                    if (cfqPrimaryWindowName.isNotNull()) primaryWindowName = *cfqPrimaryWindowName;

                    auto cfgPrimaryWindowWidth = config.getValue(*general, "PrimaryWindowWidth");
                    if (cfgPrimaryWindowWidth.isNotNull()) videoMode.width = cfgPrimaryWindowWidth->toUint32();

                    auto cfgPrimaryWindowHeight = config.getValue(*general, "PrimaryWindowHeight");
                    if (cfgPrimaryWindowHeight.isNotNull()) videoMode.height = cfgPrimaryWindowHeight->toUint32();

                    auto cfgPrimaryWindowVsync = config.getValue(*general, "PrimaryWindowVsync");
                    if (cfgPrimaryWindowVsync.isNotNull()) videoMode.forceVSync = cfgPrimaryWindowVsync->toBool();

                    auto cfgTargetFPS = config.getValue(*general, "TargetFPS");
                    if (cfgTargetFPS.isNotNull()) setTargetFPS(cfgTargetFPS->toFloat());

                    auto cfgAbortOnGpuError = config.getValue(*general, "AbortOnGpuError");
                    if (cfgAbortOnGpuError.isNotNull()) mAbortGpuError = cfgAbortOnGpuError->toBool();
                }
            }
        }

        ISystem::getSingleton().initialize(primaryWindowName, videoMode, renderDeviceType);

        {
            TGuard<TArray<IModule*>> guard(mModules);
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
        mFrameTimeDelta = mFrameTimeStep * mFrameTimeScale;
        mFPS = 1.0f / mFrameTimeStep;
        mFramesCount += 1;
        mExecutionTime += (double) mFrameTimeStep;
        mInGameTime += (double) mFrameTimeDelta;

        // The system is updated prior any other engine module
        // Since modules must have fresh input and window info
        ISystem::getSingleton().update();

        {
            TGuard<TArray<IModule*>> guard(mModules);
            for (auto module: guard.get()) {
                module->onPreUpdate();
            }
        }

        // todo: logic and scene update ?

        {
            TGuard<TArray<IModule*>> guard(mModules);
            for (auto module: guard.get()) {
                module->onPostUpdate();
            }
        }

        BERSERK_LOG_INFO("Frames: %llu FPS: %f", mFramesCount, mFPS);
    }

    void Engine::finalize() {
        TGuard<TArray<IModule*>> guard(mModules);
        for (auto module: guard.get()) {
            module->onPostFinalize();
        }

        ISystem::getSingleton().finalize();
    }

    void Engine::registerModule(IModule *module) {
        BERSERK_COND_ERROR_RET(module, "Module must be valid");

        TGuard<TArray<IModule*>> guard(mModules);
        guard->add(module);
    }

    void Engine::unregisterModule(IModule *module) {
        BERSERK_COND_ERROR_RET(module, "Module must be valid");

        TGuard<TArray<IModule*>> guard(mModules);
        guard->removeElement(module);
    }

    TRef<IModule> Engine::getModule(const char *name) {
        TGuard<TArray<IModule*>> guard(mModules);
        for (auto module: guard.get()) {
            if (CStringUtility::compare(module->getModuleName(),name) == 0)
                return module;
        }

        return nullptr;
    }

    void Engine::setTargetFPS(float FPS) {
        if (FPS > 0.0f) {
            mTargetFPS = FPS;
            mTargetFrameStep = TimeValue::asSeconds(1.0f / mTargetFPS);
        }
    }

    Engine& Engine::getSingleton() {
        return *gEngineInstance;
    }

}