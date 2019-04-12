//
// Created by Egor Orachyov on 22.02.2019.
//

#include "Foundation/RenderSystem.h"
#include <Components/SpotLightComponent.h>
#include <Components/PointLightComponent.h>
#include <Components/DirectionalLightComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Foundation/PipelineScheduler.h>

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

#ifdef USE_OPEN_GL
        IWindow::WindowSetup setup;

        mRenderDriver   = new (allocator->allocate(sizeof(GLRenderDriver)))                 GLRenderDriver(setup);
        mMainWindow     = mRenderDriver->getMainWindow();
        mBufferManager  = new (allocator->allocate(sizeof(Resources::GLBufferManager)))     GLBufferManager();
        mShaderManager  = new (allocator->allocate(sizeof(Resources::GLShaderManager)))     GLShaderManager(nullptr);
        mTextureManager = new (allocator->allocate(sizeof(Resources::GLTextureManager)))    GLTextureManager(mImageImporter, nullptr);
#endif

        mMaterialManager    = new (allocator->allocate(sizeof(MaterialManager))) MaterialManager(mTextureManager, nullptr);
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

    }

    void RenderSystem::postUpdate()
    {

    }

    void RenderSystem::destroy()
    {

        mIsDestroyed = FIELD_ON;
    }

    void RenderSystem::registerLightSource(LightSourceComponent *component)
    {
        auto type = component->getLightSourceType();

        if (component->mPrev || component->mNext)
        {
            FAIL(false, "An attempt to pass registered component [name: '%s']", component->getName());
            return;
        }

        switch (type)
        {
            case LightSourceComponent::eLST_SPOT_LIGHT:
            {
                auto item = (SpotLightComponent*) component;
                if (mSpotLightSources)
                {
                    item->mNext = mSpotLightSources;
                    mSpotLightSources->mPrev = item;
                }
                mSpotLightSources = item;
                mStaticMeshesCount += 1;
                break;
            }

            case LightSourceComponent::eLST_POINT_LIGHT:
            {
                auto item = (PointLightComponent*) component;
                if (mPointLightSources)
                {
                    item->mNext = mPointLightSources;
                    mPointLightSources->mPrev = item;
                }
                mPointLightSources = item;
                mPointLightSourcesCount += 1;
                break;
            }

            case LightSourceComponent::eLST_DIRECTIONAL_LIGHT:
            {
                auto item = (DirectionalLightComponent*) component;
                if (mDirLightSources)
                {
                    item->mNext = mDirLightSources;
                    mDirLightSources->mPrev = item;
                }
                mDirLightSources = item;
                mDirLightSourcesCount += 1;
                break;
            }

            default:
                FAIL(false, "An attempt to register unknown component [name: '%s']", component->getName());
        }
    }

    void RenderSystem::registerPrimitive(IPrimitiveComponent *component)
    {
        auto type = component->getPrimitiveType();

        if (component->mPrev || component->mNext)
        {
            FAIL(false, "An attempt to pass registered component [name: '%s']", component->getName());
            return;
        }

        switch (type)
        {
            case IPrimitiveComponent::ePT_STATIC_MESH:
            {
                auto item = (StaticMeshComponent*) component;
                if (mStaticMeshes)
                {
                    item->mNext = mStaticMeshes;
                    mStaticMeshes->mPrev = item;
                }
                mStaticMeshes = item;
                mStaticMeshesCount += 1;
                break;
            }

            default:
                FAIL(false, "An attempt to register unknown component [name: '%s']", component->getName());
        }
    }

    void RenderSystem::unregisterLightSource(LightSourceComponent *component)
    {

    }

    void RenderSystem::unregisterPrimitive(IPrimitiveComponent *component)
    {

    }

    LightSourceComponent* RenderSystem::findLightSource(const char *name)
    {

    }

    IPrimitiveComponent* RenderSystem::findPrimitive(const char *name)
    {

    }

} // namespace Berserk::Render