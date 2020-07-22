/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderModule.h>
#include <Engine.h>
#include <LogMacro.h>

namespace Berserk {
    namespace Rendering {

        Mutex RenderModule::mCVarAccessMutex;
        RenderModule *RenderModule::gRenderModule = nullptr;

        RenderModule::RenderModule() {
            BERSERK_COND_ERROR_RET(gRenderModule == nullptr, "Allowed only single instance of Render module");
            gRenderModule = this;
            registerModule();
        }

        RenderModule::~RenderModule() {
            unregisterModule();
            gRenderModule = nullptr;
        }

        void RenderModule::onPostInitialize() {
            // Note: at this step RHI device already initialized
            // Note: put your components setup here
            auto& engine = Engine::getSingleton();

            // Console vars for render module
            initConsoleVars();

            mConfig = TPtrShared<RenderModuleConfig>::make();
            mVertexPolicyFactory = TPtrShared<VertexPolicyFactory>::make();

            // Default target
            // createScreenTarget(System::MAIN_WINDOW);

            BERSERK_LOG_INFO("Initialize RenderModule (Rendering engine)");
        }

        void RenderModule::onPostFinalize() {
            // Note: release in reverse order
            // Note: RHI device will de destroyed after the post-finalize step

            mVertexPolicyFactory.free();
            mConfig.free();

            BERSERK_LOG_INFO("Finalize RenderModule (Rendering engine)");
        }

        void RenderModule::onPreUpdate() {
            // Single thread update of listeners
            // (Primary to track render targets resize)

            // Update console vars before any other update
            updateConsoleVars();

            for (auto preUpdateListener: mPreUpdateListeners) {
                preUpdateListener->onPreUpdate();
            }
        }

        void RenderModule::onPostUpdate() {
            for (auto postUpdateListener: mPostUpdateListeners) {
                postUpdateListener->onPostUpdate();
            }
        }

        void RenderModule::createScreenTarget(TPtrShared<Window> window) {
            auto target = getScreenTarget(window);
            BERSERK_COND_ERROR_RET(target.isNull(), "An attempt to recreate screen render target");
            target = TPtrShared<RenderTargetScreen>::make(window);
            addPreUpdateListener(*target);
            mScreenTargets.move(target);
        }

        const TPtrShared<RenderTargetScreen> RenderModule::getScreenTarget(TPtrShared<Window> window) const {
            for (const auto &target: mScreenTargets) {
                if (target->getWindow() == window)
                    return target;
            }
            return {};
        }

        void RenderModule::addPreUpdateListener(IRenderModuleUpdateListener &listener) {
            auto contains = mPreUpdateListeners.contains(&listener);
            BERSERK_COND_ERROR_RET(!contains, "An attempt to resubscribe pre-update listener");
            mPreUpdateListeners.add(&listener);
        }

        void RenderModule::removePreUpdateListener(IRenderModuleUpdateListener &listener) {
            mPreUpdateListeners.removeElement(&listener);
        }

        void RenderModule::addPostUpdateListener(IRenderModuleUpdateListener &listener) {
            auto contains = mPostUpdateListeners.contains(&listener);
            BERSERK_COND_ERROR_RET(!contains, "An attempt to resubscribe post-update listener");
            mPostUpdateListeners.add(&listener);
        }

        void RenderModule::removePostUpdateListener(IRenderModuleUpdateListener &listener) {
            mPostUpdateListeners.removeElement(&listener);
        }

        VertexPolicyFactory &RenderModule::getVertexPolicyFactory() {
            return *mVertexPolicyFactory;
        }

        RenderModuleConfig& RenderModule::getConfig() {
            return *mConfig;
        }

        const char *RenderModule::getModuleName() const {
            static const char moduleName[] = "RenderModule";
            return moduleName;
        }

        const char *RenderModule::getModuleProjectName() const {
            static const char moduleProjectName[] = "RenderModule";
            return moduleProjectName;
        }

        const char *RenderModule::getModuleDescription() const {
            static const char moduleDescription[] =
                    "Rendering module processes all the rendering logic inside the engine. "
                    "This module allocates and tracks active rendering scenes, allocates and updates "
                    "render targets and render views, supports debug drawing and 2D drawing to canvas. "
                    "This module main update is processed in the job system "
                    "in the separate thread. All unsafe requests to the rendering system "
                    "must be queued via special message queue.";
            return moduleDescription;
        }

        RenderModule &RenderModule::getSingleton() {
            return *gRenderModule;
        }

        void RenderModule::initConsoleVars() {
            mCVarFramebufferScale = AutoConsoleVarFloat(
                "r.FramebufferScale",
                0.5f,
                "How to scale offscreen framebuffer for better pixel shader performance.\n"
                "- 1.0f (no scale)\n"
                "- 0.5f (half size of the w and h)",
                {EConsoleFlag::ThreadSafe},
                mCVarAccessMutex
            );

            mCVarAbortOnGpuError = AutoConsoleVarInt(
                "r.AbortOnGpuError",
                1,
                "Abort engine execution on GPU error.\n"
                "- 1: abort\n"
                "- 0: ignore",
                { EConsoleFlag::ThreadSafe },
                mCVarAccessMutex
            );
        }

        void RenderModule::updateConsoleVars() {

        }


    }
}