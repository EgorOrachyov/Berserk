//
// Created by Egor Orachyov on 22.02.2019.
//

#include "Foundation/RenderSystem.h"
#include <Components/SpotLightComponent.h>
#include <Components/PointLightComponent.h>
#include <Components/DirectionalLightComponent.h>
#include <Components/StaticMeshComponent.h>

#include <Info/ImageImporter.h>
#include <Info/VideoDriver.h>

#ifdef USE_OPEN_GL
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
        mBufferManager  = new (allocator->allocate(sizeof(Resources::GLBufferManager)))     GLBufferManager();
        mShaderManager  = new (allocator->allocate(sizeof(Resources::GLShaderManager)))     GLShaderManager(nullptr);
        mTextureManager = new (allocator->allocate(sizeof(Resources::GLTextureManager)))    GLTextureManager(mImageImporter, nullptr);
#endif

        mMaterialManager = new (allocator->allocate(sizeof(MaterialManager))) MaterialManager(mTextureManager, nullptr);
    }

    RenderSystem::~RenderSystem()
    {
        delete (mMaterialManager);
        delete (mTextureManager);
        delete (mShaderManager);
        delete (mBufferManager);
        delete (mImageImporter);

        mGenAllocator->free(mMaterialManager);
        mGenAllocator->free(mTextureManager);
        mGenAllocator->free(mShaderManager);
        mGenAllocator->free(mBufferManager);
        mGenAllocator->free(mImageImporter);
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
                if (mRegSpotLightComponent)
                {
                    item->mNext = mRegSpotLightComponent;
                    mRegSpotLightComponent->mPrev = item;
                }
                mRegSpotLightComponent = item;
                mTotalRegStaticMeshComponents += 1;
                break;
            }

            case LightSourceComponent::eLST_POINT_LIGHT:
            {
                auto item = (PointLightComponent*) component;
                if (mRegPointLightComponent)
                {
                    item->mNext = mRegPointLightComponent;
                    mRegPointLightComponent->mPrev = item;
                }
                mRegPointLightComponent = item;
                mTotalRegPointLightComponents += 1;
                break;
            }

            case LightSourceComponent::eLST_DIRECTIONAL_LIGHT:
            {
                auto item = (DirectionalLightComponent*) component;
                if (mRegDirectionalLightComponent)
                {
                    item->mNext = mRegDirectionalLightComponent;
                    mRegDirectionalLightComponent->mPrev = item;
                }
                mRegDirectionalLightComponent = item;
                mTotalRegDirectionalLightComponents += 1;
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
                if (mRegStaticMeshComponent)
                {
                    item->mNext = mRegStaticMeshComponent;
                    mRegStaticMeshComponent->mPrev = item;
                }
                mRegStaticMeshComponent = item;
                mTotalRegStaticMeshComponents += 1;
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