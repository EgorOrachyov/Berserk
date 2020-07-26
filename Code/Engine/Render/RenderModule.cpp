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
#include <ModuleManager.h>

namespace Berserk {
    namespace Render {

        Mutex RenderModule::mCVarAccessMutex;
        RenderModule *RenderModule::gRenderModule = nullptr;

        RenderModule::RenderModule() {
            BERSERK_COND_ERROR_RET(gRenderModule == nullptr, "Allowed only single instance of Render module");
            gRenderModule = this;

            ModuleManager::getSingleton().registerModule(*this);
        }

        RenderModule::~RenderModule() {
            gRenderModule = nullptr;
        }

        void RenderModule::onPostLoad() {
            // Note: at this step RHI device already initialized
            // Note: put your components setup here
            auto& engine = Engine::getSingleton();

            // Console vars for render module
            initConsoleVars();

            mDeclarationCache = TPtrUnique<VertexDeclarationCache>::make();
            mProgramCache = TPtrUnique<ShaderProgramCache>::make();

            BERSERK_LOG_INFO("Initialize RenderModule (Rendering engine)");
        }

        void RenderModule::onPreUnload() {
            // Note: release in reverse order
            // Note: RHI device will de destroyed after the post-finalize step

            mProgramCache.free();
            mDeclarationCache.free();

            BERSERK_LOG_INFO("Finalize RenderModule (Rendering engine)");
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