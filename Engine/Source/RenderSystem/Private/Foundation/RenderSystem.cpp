//
// Created by Egor Orachyov on 22.02.2019.
//

#include "Foundation/RenderSystem.h"
#include <Components/SpotLightComponent.h>
#include <Components/PointLightComponent.h>
#include <Components/DirectionalLightComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Foundation/PipelineScheduler.h>
#include <Managers/MaterialManager.h>

#include <Info/ImageImporter.h>
#include <Info/VideoDriver.h>

#ifdef USE_OPEN_GL
#include <Platform/GLRenderDriver.h>
#include <Managers/GLBufferManager.h>
#include <Managers/GLShaderManager.h>
#include <Managers/GLTextureManager.h>
#endif

#ifdef USE_FREE_IMAGE
#include <FreeImageImporter.h>
#endif

namespace Berserk::Render
{

    RenderSystem::RenderSystem(const ISystemInitializer &systemInitializer)
            : IRenderSystem(systemInitializer),
              mGenAllocator(systemInitializer.getAllocator())
    {
        auto allocator = systemInitializer.getAllocator();

#ifdef USE_FREE_IMAGE
        mImageImporter = new (allocator->allocate(sizeof(Importers::FreeImageImporter))) FreeImageImporter();
#endif

        IWindow::WindowSetup setup;

#ifdef USE_OPEN_GL
        mRenderDriver   = new (allocator->allocate(sizeof(GLRenderDriver)))                 GLRenderDriver(setup);
        mMainWindow     = mRenderDriver->getMainWindow();
        mBufferManager  = new (allocator->allocate(sizeof(Resources::GLBufferManager)))     GLBufferManager();
        mShaderManager  = new (allocator->allocate(sizeof(Resources::GLShaderManager)))     GLShaderManager("../Engine/Shaders");
        mTextureManager = new (allocator->allocate(sizeof(Resources::GLTextureManager)))    GLTextureManager(mImageImporter, "../Engine/Textures/");
#endif

        mMaterialManager    = new (allocator->allocate(sizeof(MaterialManager))) MaterialManager(mTextureManager, "../Engine/Materials");
        mPipelineScheduler  = new (allocator->allocate(sizeof(PipelineScheduler))) PipelineScheduler(allocator);
    }

    RenderSystem::~RenderSystem()
    {
        delete (mPipelineScheduler);
        delete (mMaterialManager);
        delete (mTextureManager);
        delete (mShaderManager);
        delete (mBufferManager);
        delete (mImageImporter);
        delete (mRenderDriver);

        mGenAllocator->free(mPipelineScheduler);
        mGenAllocator->free(mMaterialManager);
        mGenAllocator->free(mTextureManager);
        mGenAllocator->free(mShaderManager);
        mGenAllocator->free(mBufferManager);
        mGenAllocator->free(mImageImporter);
        mGenAllocator->free(mRenderDriver);
    }

    void RenderSystem::initialize()
    {
        mIsInitialized = FIELD_ON;
    }

    void RenderSystem::preUpdate()
    {

    }

    void RenderSystem::update()
    {
        RenderPassInfo passInfo;

        mPipelineScheduler->execute(passInfo);

        // Swap buffers after all the rendering pipeline stages
        // are done. Update call -> calls main window update too
        mRenderDriver->swapBuffers();
        mRenderDriver->update();
    }

    void RenderSystem::postUpdate()
    {
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