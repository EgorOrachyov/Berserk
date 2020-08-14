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
#include <TPtrUnique.h>
#include <TPtrShared.h>
#include <UpdateManager.h>
#include <Console/AutoVariable.h>
#include <RHI/RHIDevice.h>

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
        class RenderModule : public Module, public UpdateStageListener {
        public:

            RenderModule();
            ~RenderModule() override;

            /** @copydoc Module::onPostInitialize() */
            void onPostLoad() override;

            /** @copydoc Module::onPostFinalize() */
            void onPreUnload() override;

            /** @copydoc Module::getModuleName() */
            const char *getModuleName() const override;

            /** @copydoc Module::getModuleProjectName() */
            const char *getModuleProjectName() const override;

            /** @copydoc Module::getModuleDescription() */
            const char *getModuleDescription() const override;

            void onStageExec(EUpdateStage stage) override;

            /** @return Primary default window target created on engine start-up */
            const TPtrShared<class WindowTarget>& getPrimaryWindow() const { return mPrimaryWindow; }

            /** @return Primary default graphics for on-screen rendering */
            const TPtrShared<class Graphics>& getPrimaryGraphics() const { return mPrimaryGraphics; }

            /** @return Primary default scene for rendering */
            const TPtrShared<class Scene>& getPrimaryScene() const { return mPrimaryScene; }

            /** @return Primary default scene view for rendering */
            const TPtrShared<class SceneView>& getPrimarySceneView() const { return mPrimarySceneView; }

            /** @return Engine rendering module */
            static RenderModule &getSingleton();

        private:

            void initConsoleVars();
            void updateConsoleVars();

            TPtrUnique<class TextureManager> mTextureManager;
            TPtrUnique<class VertexDeclarationCache> mDeclarationCache;
            TPtrUnique<class ShaderProgramCache> mProgramCache;
            TPtrUnique<class ShaderManager> mShaderManager;
            TPtrUnique<class FactoryRegistry> mDefaultShaderFactories;

            TPtrShared<class WindowTarget> mPrimaryWindow;
            TPtrShared<class Graphics> mPrimaryGraphics;

            // todo: remove it
            TPtrShared<RHIDrawList> mDrawList;
            TPtrShared<class Scene> mPrimaryScene;
            TPtrShared<class SceneView> mPrimarySceneView;

            /** Scene view to be rendered */
            TArray<TPtrShared<class SceneView>> mRenderedView;

            /** Console variables exposed by rendering module */
            AutoConsoleVarFloat mCVarFramebufferScale;
            AutoConsoleVarInt mCVarAbortOnGpuError;

            static Mutex mCVarAccessMutex;
            static RenderModule *gRenderModule;
        };

    }
}

#endif //BERSERK_RENDERMODULE_H