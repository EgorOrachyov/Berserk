/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDERMODULE_H
#define BERSERK_RENDERMODULE_H

#include <IModule.h>
#include <Console/AutoVariable.h>
#include <Rendering/IRenderUpdate.h>
#include <Rendering/VertexPolicy.h>
#include <Rendering/RenderTargetScreen.h>

namespace Berserk {
    namespace Rendering {

        /**
         * @brief Rendering engine
         *
         * Rendering module processes all the rendering logic inside the engine.
         * This module allocates and tracks active rendering scenes, allocates and updates
         * render targets and render views, supports debug drawing and 2D drawing to canvas.
         *
         * This module main update is processed in the job system
         * in the separate thread. All unsafe requests to the rendering system
         * must be queued via special message queue.
         */
        class RenderModule : public IModule {
        public:

            RenderModule();

            ~RenderModule() override;

            /** @copydoc IModule::onPostInitialize() */
            void onPostInitialize() override;

            /** @copydoc IModule::onPostFinalize() */
            void onPostFinalize() override;

            /** @copydoc IModule::onPreUpdate() */
            void onPreUpdate() override;

            /** @copydoc IModule::onPostUpdate() */
            void onPostUpdate() override;

            /** Create render target to wrap platform window (called when ne window is created) */
            void createScreenTarget(ISystem::WINDOW_ID windowId);

            /** @return Screen target by window id (might be null) */
            const TPtrShared<RenderTargetScreen> getScreenTarget(ISystem::WINDOW_ID windowId) const;

            void addPreUpdateListener(IRenderModuleUpdateListener &listener);

            void removePreUpdateListener(IRenderModuleUpdateListener &listener);

            void addPostUpdateListener(IRenderModuleUpdateListener &listener);

            void removePostUpdateListener(IRenderModuleUpdateListener &listener);

            /** @return Default vertex policy factory */
            VertexPolicyFactory &getVertexPolicyFactory();

            /** @copydoc IModule::getModuleName() */
            const char *getModuleName() const override;

            /** @copydoc IModule::getModuleProjectName() */
            const char *getModuleProjectName() const override;

            /** @copydoc IModule::getModuleDescription() */
            const char *getModuleDescription() const override;

            /** @return Engine rendering module */
            static RenderModule &getSingleton();

        private:

            void initConsoleVars();

            void updateConsoleVars();

            TPtrShared<VertexPolicyFactory> mVertexPolicyFactory;

            /** Object to update on render module*/
            TArray<IRenderModuleUpdateListener *> mPreUpdateListeners;
            TArray<IRenderModuleUpdateListener *> mPostUpdateListeners;

            /** Screen targets for rendering to platform windows */
            TArray<TPtrShared<RenderTargetScreen>> mScreenTargets;

            /** Console variables exposed by rendering module */
            AutoConsoleVarFloat mCVarFramebufferScale;

            static Mutex mCVarAccessMutex;
            static RenderModule *gRenderModule;
        };

    }
}

#endif //BERSERK_RENDERMODULE_H