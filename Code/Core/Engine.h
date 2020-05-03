/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ENGINE_H
#define BERSERK_ENGINE_H

#include <IModule.h>
#include <TimeValue.h>

namespace Berserk {

    class Engine {
    public:

        Engine();

        /** Global Engine initialize step (must be called by application main) */
        void initialize(const CString &engineDirectory, bool editor = false);

        /** Global Engine update step (must be called by application main) */
        void update();

        /** Global Engine finalize step (must be called by application main) */
        void finalize();


        /** Register module in the engine system */
        void registerModule(IModule* module);

        /** Unregister module from the engine system */
        void unregisterModule(IModule* module);

        /** @return Module by name (null if not found) */
        TRef<IModule> getModule(const char* name);


        /** Set desired FPS count */
        void setTargetFPS(float FPS);

        /** @return Total frames count */
        uint64 getFramesCount() const { return mFramesCount; }

        /** @return Step between this frame and last frame */
        float getFrameTimeStep() const { return mFrameTimeStep; }

        /** @return How to scale real time step in the game time (default always 1.0 - no scale) */
        float getFrameTimeScale() const { return mFrameTimeScale; }

        /** @return In game delta time (for game logic) */
        float getFrameTimeDelta() const { return mFrameTimeDelta; }

        /** @return Current number of frames per second */
        float getFPS() const { return mFPS; }

        /** @return Desired number of frame per second */
        float getTargetFPS() const { return mTargetFPS; }

        /** @return True, whether engine run in editor */
        bool getIsEditor() const { return mIsEditor; }

        /** @return True, whether engine must abort on gpu errors (for debug) */
        bool getAbortOnGpuError() const { return mAbortGpuError; }

        /** @return Prefix to get the engine working directory (prepend to load engine resources) */
        const CString& getEngineDirectory() const { return mEngineDirectory; }

        /** @return Global engine instance */
        static Engine& getSingleton();

    private:

        /** Prefix to get the engine working directory */
        CString mEngineDirectory;
        /** Application name */
        CString mApplicationName;

        /** Current time to measure elapsed delta */
        TimeValue mCurrentTime;
        TimeValue mTargetFrameStep;

        uint64 mFramesCount;
        float mFrameTimeStep;
        float mFrameTimeScale;
        float mFrameTimeDelta;
        float mFPS;
        float mTargetFPS;

        double mExecutionTime;
        double mInGameTime;

        bool mIsEditor;
        bool mAbortGpuError;

        TSync<TArray<IModule*>> mModules;
    };

}

#endif //BERSERK_ENGINE_H