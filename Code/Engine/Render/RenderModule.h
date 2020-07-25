/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDERMODULE_H
#define BERSERK_RENDERMODULE_H

#include <Module.h>
#include <Console/AutoVariable.h>
#include <ShaderCore/ShaderProgramCache.h>

namespace Berserk {
    namespace Render {

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
        class RenderModule : public Module {
        public:

            RenderModule();
            ~RenderModule() override;

            /** @copydoc IModule::onPostInitialize() */
            void onPostLoad() override;

            /** @copydoc IModule::onPostFinalize() */
            void onPreUnload() override;

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

            ShaderProgramCache mProgramCache;

            /** Console variables exposed by rendering module */
            AutoConsoleVarFloat mCVarFramebufferScale;
            AutoConsoleVarInt mCVarAbortOnGpuError;

            static Mutex mCVarAccessMutex;
            static RenderModule *gRenderModule;
        };

    }
}

#endif //BERSERK_RENDERMODULE_H