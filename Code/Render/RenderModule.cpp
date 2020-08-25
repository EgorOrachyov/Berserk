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
#include <ShaderManager.h>
#include <TextureManager.h>
#include <ShaderDefinitions.h>
#include <ShaderProgramCache.h>
#include <VertexDeclarationCache.h>
#include <FactoryRegistry.h>
#include <Platform/WindowManager.h>
#include <Graphics.h>
#include <RenderTargets/WindowTarget.h>
#include <Scene/Scene.h>
#include <Scene/SceneView.h>

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
            auto& device = RHIDevice::getSingleton();
            auto& windowManager = WindowManager::getSingleton();

            UpdateManager::getSingleton().subscribe(*this, EUpdateStage::Update);

            // Console vars for render module
            initConsoleVars();

            mTextureManager = TPtrUnique<TextureManager>::make();
            mDeclarationCache = TPtrUnique<VertexDeclarationCache>::make();
            mShaderDefinitions = TPtrUnique<ShaderDefinitions>::make();
            mProgramCache = TPtrUnique<ShaderProgramCache>::make();
            mShaderManager = TPtrUnique<ShaderManager>::make();
            mDefaultShaderFactories = TPtrUnique<FactoryRegistry>::make();

            auto window = windowManager.find(engine.getPrimaryWindowName());
            BERSERK_COND_ERROR_FAIL(window.isNotNull(), "Primary window not found");

            mPrimaryWindow = TPtrShared<WindowTarget>::make(window);
            mPrimaryGraphics = TPtrShared<Graphics>::make(window->getSize(),Region2i(0,0,window->getSize()),mPrimaryWindow.castTo<RenderTarget>());
            mDrawList = device.createDrawList();

            mPrimaryScene = TPtrShared<Scene>::make();
            mPrimaryScene->mBatch = TPtrShared<BatchedElements>::make();
            mPrimarySceneView = TPtrShared<SceneView>::make();
            mPrimarySceneView->scene = mPrimaryScene;
            mPrimarySceneView->target = mPrimaryWindow.castTo<RenderTarget>();
            mPrimarySceneView->batchRenderer = TPtrUnique<BatchedElementsRenderer>::make(*mPrimaryScene->mBatch);
            mPrimarySceneView->graphics = mPrimaryGraphics;
            mPrimarySceneView->camera = TPtrShared<Camera>::make(Math::degToRad(90.0f), 0.01f, 100.0f);

            BERSERK_LOG_INFO("Initialize RenderModule");
        }

        void RenderModule::onPreUnload() {
            // Note: release in reverse order
            // Note: RHI device will be destroyed after the post-finalize step
            UpdateManager::getSingleton().unsubscribeFromAll(*this);

            mPrimarySceneView.free();
            mPrimaryScene.free();
            mDrawList.free();
            mPrimaryGraphics.free();
            mPrimaryWindow.free();
            mDefaultShaderFactories.free();
            mShaderManager.free();
            mProgramCache.free();
            mShaderDefinitions.free();
            mDeclarationCache.free();
            mTextureManager.free();

            BERSERK_LOG_INFO("Finalize RenderModule");
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

        void RenderModule::onStageExec(EUpdateStage stage) {
            // todo: add task manager, run update in the render thread
            if (stage == EUpdateStage::Update) {
                auto& device = RHIDevice::getSingleton();

                mPrimaryWindow->update();
                mPrimaryGraphics->fitAreaToTarget();
                mPrimaryGraphics->fitRegionToTarget();

                if (mPrimarySceneView->camera->getIsAutoAspect()) {
                    mPrimarySceneView->camera->setAspect(mPrimarySceneView->target->getAreaAspect());
                }

                mPrimarySceneView->viewData.updateFromCamera(*mPrimarySceneView->camera);
                mPrimarySceneView->viewData.updateFromTarget(*mPrimarySceneView->target);

                mDrawList->begin();
                mPrimaryWindow->bind(*mDrawList, { EClearOption::Color, EClearOption::Depth });
                mPrimarySceneView->batchRenderer->draw(mPrimarySceneView->viewData, *mDrawList);
                mPrimarySceneView->graphics->draw(*mDrawList);
                mDrawList->end();

                device.beginRenderFrame();
                device.submitDrawList(mDrawList);
                device.endRenderFrame();

                mPrimaryGraphics->clear();
            }
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