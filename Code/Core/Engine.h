/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ENGINE_H
#define BERSERK_ENGINE_H

#include <Module.h>
#include <TimeValue.h>
#include <Variant.h>
#include <Console/AutoVariable.h>

namespace Berserk {

    class Engine {
    public:

        Engine();
        ~Engine();

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
        int32 getTargetFPS() const { return mTargetFPS; }

        /** @return True, whether engine run in editor */
        bool getIsEditor() const { return mIsEditor; }

        /** @return Global engine instance */
        static Engine& getSingleton();

    private:

        /** Global Engine initialize step (must be called by application main) */
        void initialize(bool editor = false);

        /** Global Engine update step (must be called by application main) */
        void update();

        /** Global Engine finalize step (must be called by application main) */
        void finalize();

        /** Configure engine console vars */
        void initializeConsoleVariables();

        /** Track changes of the console variables */
        void updateConsoleVariables();

        friend class Main;

    private:

        /** Current time to measure elapsed delta */
        TimeValue mCurrentTime;
        TimeValue mTargetFrameStep;
        uint64 mFramesCount;
        float mFrameTimeStep;
        float mMaxFrameTimeStep;
        float mFrameTimeScale;
        float mFrameTimeDelta;
        float mFPS;
        int32 mTargetFPS;
        int32 mMinFPS;
        double mExecutionTime;
        double mInGameTime;
        bool mIsEditor;
        bool mIsInitialized = false;
        bool mIsFinalized = false;

        AutoConsoleVarInt mCVarMinFps;
        AutoConsoleVarInt mCVarTargetFps;

        Variant mEngineConfig;

        /** Singleton reference */
        static Engine* gEngine;
    };

}

#endif //BERSERK_ENGINE_H