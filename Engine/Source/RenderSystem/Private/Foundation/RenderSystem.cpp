//
// Created by Egor Orachyov on 22.02.2019.
//

#include "Foundation/RenderSystem.h"
#include <Components/SpotLightComponent.h>
#include <Components/PointLightComponent.h>
#include <Components/DirectionalLightComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Foundation/PipelineScheduler.h>

#include <Pipeline/DebugDraw.h>
#include <Pipeline/ToneMapping.h>

#include <Info/ImageImporter.h>
#include <Info/VideoDriver.h>
#include <Info/FontImporter.h>

#ifdef USE_OPEN_GL
#include <Platform/GLRenderDriver.h>
#include <Managers/GLBufferManager.h>
#include <Managers/GLShaderManager.h>
#include <Managers/GLTextureManager.h>
#endif

#ifdef USE_FREE_IMAGE
#include <FreeImageImporter.h>
#endif

#ifdef USE_FREE_TYPE
#include <FreeTypeImporter.h>
#endif

namespace Berserk::Render
{

    RenderSystem::RenderSystem(const ISystemInitializer &systemInitializer)
            : IRenderSystem(systemInitializer),
              mGenAllocator(systemInitializer.getAllocator()),
              mPipelineStages(INITIAL_STAGES_COUNT, mGenAllocator)

    {
        auto allocator = systemInitializer.getAllocator();
        IWindow::WindowSetup setup;

        {
            // Setup platform dependent components, such as
            // graphics driver, image, font importers
            // Must be before any other init procedures

#ifdef USE_FREE_IMAGE
            mImageImporter = new (allocator->allocate(sizeof(Importers::FreeImageImporter))) FreeImageImporter();
#endif

#ifdef USE_FREE_TYPE
            mFontImporter = new (allocator->allocate(sizeof(Importers::FreeTypeImporter))) FreeTypeImporter();
#endif

#ifdef USE_OPEN_GL
            mRenderDriver   = new (allocator->allocate(sizeof(GLRenderDriver)))                GLRenderDriver(setup);
            mMainWindow     = mRenderDriver->getMainWindow();
            mBufferManager  = new(allocator->allocate(sizeof(Resources::GLBufferManager)))     GLBufferManager();
            mShaderManager  = new(allocator->allocate(sizeof(Resources::GLShaderManager)))     GLShaderManager("../Engine/Shaders");
            mTextureManager = new(allocator->allocate(sizeof(Resources::GLTextureManager)))    GLTextureManager(mImageImporter, "../Engine/Textures/");
#endif
        }


        {
            // Allocate and initialize all the default engine rendering system managers
            // Note: order of initialization and de-initialization is important

            mFontManager        = new (allocator->allocate(sizeof(FontManager)))       FontManager(mTextureManager, mFontImporter, mGenAllocator, "../Engine/Fonts");
            mMaterialManager    = new (allocator->allocate(sizeof(MaterialManager)))   MaterialManager(mTextureManager, "../Engine/Materials");
            mPipelineScheduler  = new (allocator->allocate(sizeof(PipelineScheduler))) PipelineScheduler(allocator);
            mRenderQueue        = new (allocator->allocate(sizeof(RenderQueue)))       RenderQueue(allocator);
            mDebugDrawManager   = new (allocator->allocate(sizeof(DebugDrawManager)))  DebugDrawManager(allocator);
            mRenderSystem       = this;
        }

        {
            // Initialize default display and frame buffers
            // Setup default helpers

            uint32 width, height;
            mMainWindow->getFrameBufferSize(width, height);
            mFrameBuffer = mBufferManager->createFrameBuffer("MainFrameBuffer");
            mFrameBuffer->createFrameBuffer(width, height, 1);
            mFrameBuffer->attachColorBuffer(IRenderDriver::RGBA16F, IRenderDriver::FILTER_NEAREST, IRenderDriver::WRAP_CLAMP_TO_EDGE);
            mFrameBuffer->attachDepthBuffer();
            mFrameBuffer->linkBuffers();
        }

        {
            // Setup default render pass properties
            // Get viewport and frame buffers

            uint32 width, height;
            mMainWindow->getFrameBufferSize(width, height);
            mRenderPass.mFrameBuffer = mFrameBuffer;
            mRenderPass.mDisplayViewPort = IRenderDriver::ViewPort(0, 0, width, height);
            mRenderPass.mFrameBufferViewPort = IRenderDriver::ViewPort(0, 0, width, height);
        }

        {
            // Generate common geometry and buffers for rendering
            // stages [screen plane]

            Vec3f v0(-1, -1, 0), v1(1, -1, 0),
                  v2(1, 1, 0),   v3(-1, 1, 0);

            Vec2f t0 = Vec2f(0,0), t1 = Vec2f(1,0),
                  t2 = Vec2f(1,1), t3 = Vec2f(0,1);

            const uint32 data_count = 4;
            VertPTf data[data_count] = { {v0,t0}, {v1,t1}, {v2,t2}, {v3,t3} };

            const uint32 indices_count = 6;
            uint16 indices[indices_count] = { 0, 1, 2, 2, 3, 0 };

            IGPUBuffer* screen = RenderBase::getBufferManager()->createGPUBuffer("ScreenPlane");
            screen->create(data_count, IGPUBuffer::eVT_VertexPT, data, indices_count, indices);
        }

        {
            // Setup all the pipeline stages
            // Allocate and store in scheduler

            IPipelineStage* stage = nullptr;

            stage = new (allocator->allocate(sizeof(DebugDraw))) DebugDraw("DebugDraw", allocator);
            mPipelineStages.add(stage);
            mPipelineScheduler->addStage(stage);

            stage = new (allocator->allocate(sizeof(ToneMapping))) ToneMapping("ToneMapping", allocator);
            mPipelineStages.add(stage);
            mPipelineScheduler->addStage(stage);
        }
    }

    RenderSystem::~RenderSystem()
    {
        for (uint32 i = 0; i < mPipelineStages.getSize(); i++)
        {
            delete(mPipelineStages[i]);
            mGenAllocator->free(mPipelineStages[i]);
        }

        delete (mDebugDrawManager);
        delete (mPipelineScheduler);
        delete (mRenderQueue);
        delete (mFontManager);
        delete (mMaterialManager);
        delete (mTextureManager);
        delete (mShaderManager);
        delete (mBufferManager);
        delete (mImageImporter);
        delete (mFontImporter);
        delete (mRenderDriver);

        mGenAllocator->free(mDebugDrawManager);
        mGenAllocator->free(mPipelineScheduler);
        mGenAllocator->free(mRenderQueue);
        mGenAllocator->free(mFontManager);
        mGenAllocator->free(mMaterialManager);
        mGenAllocator->free(mTextureManager);
        mGenAllocator->free(mShaderManager);
        mGenAllocator->free(mBufferManager);
        mGenAllocator->free(mImageImporter);
        mGenAllocator->free(mFontImporter);
        mGenAllocator->free(mRenderDriver);
    }

    void RenderSystem::initialize()
    {
        mIsInitialized = FIELD_ON;
    }

    void RenderSystem::preUpdate()
    {
        // Everything, what should be done in single-thread mode
    }

    void RenderSystem::update()
    {
        // Clear main frame buffer
        mFrameBuffer->bindFrameBuffer();
        mRenderDriver->clear(true, true, false);

        // All rendering operations here (sequent execution of the stages)
        mPipelineScheduler->execute(mRenderPass);

        // Swap buffers after all the rendering pipeline stages are done
        mRenderDriver->swapBuffers();

        // Update main window and process window OS events
        mRenderDriver->update();

        // Swap render queue and submit queue
        mDebugDrawManager->update();
    }

    void RenderSystem::postUpdate()
    {
        // Everything, what should be done in single-thread mode

        // Increase frame number (in the end of the updates)
        mCurrentFrameNumber += 1;
    }

    void RenderSystem::destroy()
    {
        mIsDestroyed = FIELD_ON;
    }

    void RenderSystem::registerComponent(SpotLightComponent *component)
    {
        if (component->mPrev || component->mNext)
        {
            FAIL(false, "An attempt to pass registered component [name: '%s']", component->getName());
            return;
        }

        if (mSpotLightSources)
        {
            component->mNext = mSpotLightSources;
            mSpotLightSources->mPrev = component;
        }

        mSpotLightSources = component;
        mSpotLightSourcesCount += 1;
    }

    void RenderSystem::registerComponent(PointLightComponent *component)
    {
        if (component->mPrev || component->mNext)
        {
            FAIL(false, "An attempt to pass registered component [name: '%s']", component->getName());
            return;
        }

        if (mPointLightSources)
        {
            component->mNext = mPointLightSources;
            mPointLightSources->mPrev = component;
        }

        mPointLightSources = component;
        mPointLightSourcesCount += 1;
    }

    void RenderSystem::registerComponent(DirectionalLightComponent *component)
    {
        if (component->mPrev || component->mNext)
        {
            FAIL(false, "An attempt to pass registered component [name: '%s']", component->getName());
            return;
        }

        if (mDirLightSources)
        {
            component->mNext = mDirLightSources;
            mDirLightSources->mPrev = component;
        }

        mDirLightSources = component;
        mDirLightSourcesCount += 1;
    }

    void RenderSystem::registerComponent(StaticMeshComponent *component)
    {
        if (component->mPrev || component->mNext)
        {
            FAIL(false, "An attempt to pass registered component [name: '%s']", component->getName());
            return;
        }

        if (mStaticMeshes)
        {
            component->mNext = mStaticMeshes;
            mStaticMeshes->mPrev = component;
        }

        mStaticMeshes = component;
        mStaticMeshesCount += 1;
    }

} // namespace Berserk::Render