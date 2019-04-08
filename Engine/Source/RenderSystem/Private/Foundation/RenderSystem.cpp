//
// Created by Egor Orachyov on 22.02.2019.
//

#include "Foundation/RenderSystem.h"
#include <Components/SpotLightComponent.h>
#include <Components/PointLightComponent.h>
#include <Components/DirectionalLightComponent.h>
#include <Components/StaticMeshComponent.h>

namespace Berserk::Render
{

    RenderSystem::RenderSystem(const ISystemInitializer &systemInitializer)
            : IRenderSystem(systemInitializer)
    {

    }

    RenderSystem::~RenderSystem()
    {

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
                break;
            }

            default:
                FAIL(false, "An attempt to register unknown component [name: '%s']", component->getName());
        }
    }

    void RenderSystem::registerPrimitive(IPrimitiveComponent *component)
    {
        auto type = component->getPrimitiveType();

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